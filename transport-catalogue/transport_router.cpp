#include "transport_router.h"

#include <utility>
#include <iostream>

namespace transport_router {
    using namespace std::string_literals;

    TransportRouter::TransportRouter(const Transport::Catalogue& catalogue, const domain::RouteSettings& settings)
            : db_(catalogue),
              route_settings_(settings) {
        BuildGraph();
    }

    int StopsInRange(const domain::Stop *start_, const domain::Stop *fin_, const domain::Bus *bus) {
        auto r = bus->bus_route_;
        auto start = std::find(r.begin(), r.end(), start_);
        auto finish = std::find(r.begin(), r.end(), fin_);
        auto tmp_end = r.end();

        int dst = abs(std::distance(start, finish));

        if (bus->is_route_round && fin_->stop_name == (*(bus->bus_route_.end() - 1))->stop_name) {
            dst = abs(std::distance(start, tmp_end - 1));
            return dst;
        }
        if (bus->is_route_round && start_->stop_name == (*(bus->bus_route_.begin() + 1))->stop_name
            && fin_->stop_name == (*(bus->bus_route_.begin()))->stop_name && dst == 1) {
            dst = r.size() - 2;
        }

        return dst;
    }

    void TransportRouter::BuildGraph() {
        graph::DirectedWeightedGraph<double> tmp_graph(db_.GetDequeStp().size() * 2);
        graph_ = std::move(tmp_graph);

        size_t i = 0;
        for (const auto &stp: db_.GetDequeStp()) {
            auto stop = db_.FindStop(stp.stop_name);
            stop_to_id_transit[stop] = (int) (i);

            auto id_edge = graph_.AddEdge({i, i + 1, route_settings_.bus_wait_time, {}});

            domain::ResponsRoute edge_info;
            edge_info.type = domain::ResponsType::Wait;
            edge_info.name_ = stop->stop_name;
            edge_to_info_[id_edge] = std::move(edge_info);
            i += 2;
        }

        for (const auto &bus: db_.GetDequeBus()) {
            const domain::Stop *prev_stop_from;
            for (size_t j = 0; j < bus.bus_route_.size(); ++j) {
                int dist = 0;
                int span_c = 0;
                auto stop_from = db_.FindStop(bus.bus_route_[j]->stop_name);

                size_t id_from_first = stop_to_id_transit[stop_from];
                for (size_t k = j + 1; k < bus.bus_route_.size(); ++k) {
                    ++span_c;
                    domain::ResponsRoute edge_info;
                    edge_info.type = domain::ResponsType::Bus;
                    edge_info.name_ = bus.bus_name_;
                    edge_info.span_count_ = span_c;

                    auto stop_to = db_.FindStop(bus.bus_route_[k]->stop_name);
                    prev_stop_from = db_.FindStop(bus.bus_route_[k - 1]->stop_name);
                    size_t id_to = stop_to_id_transit.at(stop_to);

                    int dist_current_range = 0;

                    graph::EdgeId edge_id;
                    if (k == 1) {
                        dist = db_.GetDistance(stop_from->stop_name, stop_to->stop_name);
                        edge_id = graph_.AddEdge({id_from_first + 1, id_to,
                                                  ((dist / route_settings_.bus_velocity) / 60), bus.bus_name_});
                    } else {
                        dist_current_range = db_.GetDistance(prev_stop_from->stop_name, stop_to->stop_name);
                        if (dist_current_range == 0) {
                            dist_current_range = db_.GetDistance(stop_to->stop_name, prev_stop_from->stop_name);
                        }
                        dist += dist_current_range;
                        edge_id = graph_.AddEdge({id_from_first + 1, id_to,
                                                  ((dist / route_settings_.bus_velocity) / 60), bus.bus_name_});
                    }
                    edge_to_info_[edge_id] = std::move(edge_info);
                }
            }
            if (!bus.is_route_round) {
                for (size_t j = bus.bus_route_.size() - 1; j > 0; --j) {
                    int dist = 0;
                    int span_c = 0;
                    auto stop_from = db_.FindStop(bus.bus_route_[j]->stop_name);
                    size_t id_from_first = stop_to_id_transit[stop_from];

                    for (size_t k = j - 1; k < bus.bus_route_.size(); --k) {
                        ++span_c;
                        domain::ResponsRoute edge_info;
                        edge_info.type = domain::ResponsType::Bus;
                        edge_info.name_ = bus.bus_name_;
                        edge_info.span_count_ = span_c;

                        auto stop_to = db_.FindStop(bus.bus_route_[k]->stop_name);
                        prev_stop_from = db_.FindStop(bus.bus_route_[k + 1]->stop_name);
                        size_t id_to = stop_to_id_transit.at(stop_to);
                        int dist_current_range = 0;

                        graph::EdgeId edge_id;
                        dist_current_range = db_.GetDistance(prev_stop_from->stop_name, stop_to->stop_name);
                        if (dist_current_range == 0) {
                            dist_current_range = db_.GetDistance(stop_to->stop_name, prev_stop_from->stop_name);
                        }
                        dist += dist_current_range;
                        edge_id = graph_.AddEdge({id_from_first + 1, id_to,
                                                  ((dist / route_settings_.bus_velocity) / 60), bus.bus_name_});
                        edge_to_info_[edge_id] = std::move(edge_info);
                    }
                }
            }
        }
    }

    std::optional<graph::Router<double>::RouteInfo> TransportRouter::BuildRoute(std::optional<std::string> from, std::optional<std::string> to) const {
        if (from.has_value() && to.has_value()) {
            auto find_stp_from = db_.FindStop(from.value());
            auto find_stp_to = db_.FindStop(to.value());

            return router_->BuildRoute(stop_to_id_transit.at(find_stp_from), stop_to_id_transit.at(find_stp_to));
        }
        return {};
    }

    const graph::DirectedWeightedGraph<double> &TransportRouter::GetGraph() const{
        return graph_;
    }

    const std::unordered_map<const domain::Stop *, int> &TransportRouter::GetIds() const {
        return stop_to_id_transit;
    }

    std::pair<const graph::Edge<double> &, const domain::ResponsRoute &>
    TransportRouter::GetFullEdgeInfo(graph::EdgeId edge_id) const {
        return {graph_.GetEdge(edge_id), edge_to_info_.at(edge_id)};
    }

    const std::map<graph::EdgeId, domain::ResponsRoute>& TransportRouter::GetEdgeToInfo() const{
        return edge_to_info_;
    }

//    void TransportRouter::InitRoute() {
//        router_ = std::make_unique<graph::Router<double>>(graph_);
//    }

    const Transport::Catalogue &TransportRouter::GetCatalogue() const {
        return db_;
    }

    void TransportRouter::SetGraph(graph::DirectedWeightedGraph<double> graph) {
        graph_ = std::move(graph);
        router_ = std::make_unique<graph::Router<double>>(graph_);
    }

    void TransportRouter::SetStopToId(std::unordered_map<const domain::Stop *, int> stop_to_id) {
        stop_to_id_transit = std::move(stop_to_id);
    }

    void TransportRouter::SetEdgeIdToInfo(std::map<graph::EdgeId, domain::ResponsRoute> edge_to_info) {
        edge_to_info_ = std::move(edge_to_info);
    }
}