#include "transport_router.h"

#include <utility>

namespace transport_router {
    using namespace std::string_literals;

    TransportRouter::TransportRouter(Transport::Catalogue catalogue, const domain::RouteSettings settings,
                                     const StopsNB &buses)
            : db_(std::move(catalogue)),
              route_settings_(settings),
              stp_n_base_stat_(buses) {
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
        graph::DirectedWeightedGraph<double> tmp_graph(stp_n_base_stat_.deque_of_stops.size() * 2);
        graph_ = std::move(tmp_graph);

        size_t i = 0;
        for (const auto &stp: stp_n_base_stat_.deque_of_stops) {
            auto stop = db_.FindStop(stp.name);
            stop_to_id_transit[stop] = (int) (i);

            id_to_stop_transit.reserve(stp_n_base_stat_.deque_of_stops.size() * 2);
            id_to_stop_transit[i].stop_ = stop;
            auto id_edge = graph_.AddEdge({i, i + 1, route_settings_.bus_wait_time, {}});

            domain::ResponsRoute edge_info;
            edge_info.type = domain::ResponsType::Wait;
            edge_info.name_ = stop->stop_name;
            edge_to_info_[id_edge] = std::move(edge_info);
            i += 2;
        }

        for (const auto &bus: stp_n_base_stat_.deque_of_buses) {
            const domain::Stop *prev_stop_from;
            for (size_t j = 0; j < bus.stops_.size(); ++j) {
                int dist = 0;
                int span_c = 0;
                auto stop_from = db_.FindStop(bus.stops_[j]);

                size_t id_from_first = stop_to_id_transit[stop_from];

                id_to_stop_transit[id_from_first].bus_ = db_.FindBus(bus.name);

                for (size_t k = j + 1; k < bus.stops_.size(); ++k) {
                    ++span_c;
                    domain::ResponsRoute edge_info;
                    edge_info.type = domain::ResponsType::Bus;
                    edge_info.name_ = bus.name;
                    edge_info.span_count_ = span_c;

                    auto stop_to = db_.FindStop(bus.stops_[k]);
                    prev_stop_from = db_.FindStop(bus.stops_[k - 1]);
                    size_t id_to = stop_to_id_transit.at(stop_to);
                    id_to_stop_transit[id_to].bus_ = db_.FindBus(bus.name);

                    int dist_current_range = 0;

                    graph::EdgeId edge_id;
                    if (k == 1) {
                        dist = db_.GetDistance(stop_from->stop_name, stop_to->stop_name);
                        edge_id = graph_.AddEdge({id_from_first + 1, id_to,
                                                  ((dist / route_settings_.bus_velocity) / 60), bus.name});
                    } else {
                        dist_current_range = db_.GetDistance(prev_stop_from->stop_name, stop_to->stop_name);
                        if (dist_current_range == 0) {
                            dist_current_range = db_.GetDistance(stop_to->stop_name, prev_stop_from->stop_name);
                        }
                        dist += dist_current_range;
                        edge_id = graph_.AddEdge({id_from_first + 1, id_to,
                                                  ((dist / route_settings_.bus_velocity) / 60), bus.name});
                    }
                    edge_to_info_[edge_id] = std::move(edge_info);
                }
            }
            if (!bus.is_roundtrip) {
                for (size_t j = bus.stops_.size() - 1; j > 0; --j) {
                    int dist = 0;
                    int span_c = 0;
                    auto stop_from = db_.FindStop(bus.stops_[j]);
                    size_t id_from_first = stop_to_id_transit[stop_from];

                    for (size_t k = j - 1; k < bus.stops_.size(); --k) {
                        ++span_c;
                        domain::ResponsRoute edge_info;
                        edge_info.type = domain::ResponsType::Bus;
                        edge_info.name_ = bus.name;
                        edge_info.span_count_ = span_c;

                        auto stop_to = db_.FindStop(bus.stops_[k]);
                        prev_stop_from = db_.FindStop(bus.stops_[k + 1]);
                        size_t id_to = stop_to_id_transit.at(stop_to);
                        int dist_current_range = 0;

                        graph::EdgeId edge_id;
                        dist_current_range = db_.GetDistance(prev_stop_from->stop_name, stop_to->stop_name);
                        if (dist_current_range == 0) {
                            dist_current_range = db_.GetDistance(stop_to->stop_name, prev_stop_from->stop_name);
                        }
                        dist += dist_current_range;
                        edge_id = graph_.AddEdge({id_from_first + 1, id_to,
                                                  ((dist / route_settings_.bus_velocity) / 60), bus.name});
                        edge_to_info_[edge_id] = std::move(edge_info);
                    }
                }
            }
        }
    }

    std::optional<graph::Router<double>::RouteInfo> TransportRouter::BuildRoute(const domain::StatReqs &request) const {
        auto find_stp_from = db_.FindStop(request.from.value());
        auto find_stp_to = db_.FindStop(request.to.value());

        return router_->BuildRoute(stop_to_id_transit.at(find_stp_from), stop_to_id_transit.at(find_stp_to));
    }

    graph::DirectedWeightedGraph<double> &TransportRouter::GetGraph() {
        return graph_;
    }

    const std::unordered_map<const domain::Stop *, int> &TransportRouter::GetIds() const {
        return stop_to_id_transit;
    }

    const std::unordered_map<size_t, domain::StopInBus> &TransportRouter::GetStopsToId() const {
        return id_to_stop_transit;
    }

    std::pair<const graph::Edge<double> &, const domain::ResponsRoute &>
    TransportRouter::GetFullEdgeInfo(graph::EdgeId edge_id) const {
        return {graph_.GetEdge(edge_id), edge_to_info_.at(edge_id)};
    }

    void TransportRouter::InitRoute() {
        router_ = std::make_unique<graph::Router<double>>(graph_);
    }

    const Transport::Catalogue &TransportRouter::GetCatalogue() const {
        return db_;
    }
}