#pragma once

#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <memory>

#include "transport_catalogue.h"
#include "map_renderer.h"
#include "domain.h"
#include "router.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * В качестве источника для идей предлагаем взглянуть на нашу версию обработчика запросов.
 * Вы можете реализовать обработку запросов способом, который удобнее вам.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

// Класс RequestHandler играет роль Фасада, упрощающего взаимодействие JSON reader-а
// с другими подсистемами приложения.
// См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)

namespace req_handler {
    using namespace Transport;

    struct BusCountInRoute {
        int count = 0;
        std::string bus_{};
    };

    int StopsInRange(const domain::Stop *start_, const domain::Stop *fin_, const domain::Bus *bus);

    class RequestHandler {
    public:
        // MapRenderer понадобится в следующей части итогового проекта
        RequestHandler(Transport::Catalogue &db, renderer::MapRenderer &renderer, graph::DirectedWeightedGraph<double>& graph)
                : db_(db),
                  renderer_(renderer),
                  graph_(graph){
        }

        // Возвращает информацию о маршруте (запрос Bus)
        std::optional<domain::Bus> GetBusStat(const std::string_view &bus_name) const;

        // Возвращает маршруты, проходящие через
        std::unordered_set<const domain::Bus *> GetBusesByStop(const std::string_view &stop_name);

        void GetAllBuses();
        svg::Document RenderMap() const;

        std::optional<graph::Router<double>::RouteInfo> BuildRoute(const std::unordered_map<const domain::Stop*, int>& ids,
                                                                   const domain::StatReqs& request) const {
            auto find_stp_from = db_.FindStop(request.from.value());
            auto find_stp_to = db_.FindStop(request.to.value());

            return router_.get()->BuildRoute(ids.at(find_stp_from), ids.at(find_stp_to));
        }

        void InitRoute(const graph::DirectedWeightedGraph<double>& graph) {
            router_ = std::make_unique<graph::Router<double>>(graph);
        }

        std::vector<domain::ResponsRoute> BuildingRoute(const graph::DirectedWeightedGraph<double>& graph,
                                                        const domain::StatReqs& request,
                                                        const std::unordered_map<const domain::Stop*, int>& ids,
                                                        const std::unordered_map<size_t, domain::StopInBus>& stop_by_id) {
            graph::Router router(graph);
            std::vector<domain::ResponsRoute> vector_responses;

            auto find_stp_from = db_.FindStop(request.from.value());
            auto find_stp_to = db_.FindStop(request.to.value());
            const auto router_res = router.BuildRoute(ids.at(find_stp_from), ids.at(find_stp_to));

            int span_c = 0;
            domain::ResponsRoute res_r;

            for (const auto& edge : router_res.value().edges) {
                auto current_edge = graph.GetEdge(edge);

                if (current_edge.to - current_edge.from == 1 && current_edge.from % 2 == 0) {
                    if (res_r.type == domain::ResponsType::Bus && span_c > 0) {
                        res_r.span_count_ = span_c;
                        vector_responses.emplace_back(res_r);
                        res_r = {};
                    }
                    auto stop_name = stop_by_id.at(current_edge.from).stop_;

                    res_r.type = domain::ResponsType::Wait;
                    res_r.time = current_edge.weight;
                    res_r.name_ = stop_name->stop_name;
                    span_c = 0;
                } else {
                    res_r.type = domain::ResponsType::Bus;
                    res_r.time = current_edge.weight;
                    res_r.name_ = current_edge.bus_name_;

                    int range_size = 0;
                    size_t tmp_id_from = 0.0;
                    size_t tmp_id_to = 0.0;
                    const domain::Stop* stop_from;
                    const domain::Stop* stop_to;

                    if (current_edge.from % 2 != 0) {
                        tmp_id_from = current_edge.from - 1;
                        stop_from = stop_by_id.at(tmp_id_from).stop_;
                    } else {
                        stop_from = stop_by_id.at(current_edge.from).stop_;
                    }
                    if (current_edge.to % 2 != 0) {
                        tmp_id_to = current_edge.to - 1;
                        stop_to = stop_by_id.at(tmp_id_to).stop_;
                    } else {
                        stop_to = stop_by_id.at(current_edge.to).stop_;
                    }
                    auto bus_ptr = db_.FindBus(current_edge.bus_name_);
                    range_size = abs(StopsInRange(stop_from, stop_to, bus_ptr));

                    if (range_size == 0) {
                        ++span_c;
                    } else {
                        span_c = range_size;
                    }
                    continue;
                }
                vector_responses.emplace_back(res_r);
                res_r = {};
            }
            if (res_r.type == domain::ResponsType::Bus) {
                res_r.span_count_ = span_c;
            }
            vector_responses.emplace_back(res_r);
            return vector_responses;
        }
    private:
        // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
        Transport::Catalogue &db_;
        std::unordered_set<const domain::Bus *> buses;
        renderer::MapRenderer &renderer_;
        graph::DirectedWeightedGraph<double>& graph_;
        std::unique_ptr<graph::Router<double>> router_;
    };
}