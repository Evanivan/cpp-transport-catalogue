#include "request_handler.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

namespace req_handler {
    std::optional<domain::Bus> RequestHandler::GetBusStat(const std::string_view &bus_name) const {
        auto found_bus = db_.FindBus(bus_name);
        if (found_bus) {
            return *found_bus;
        }
        return std::nullopt;
    }

// Возвращает маршруты, проходящие через
    std::unordered_set<const domain::Bus *> RequestHandler::GetBusesByStop(const std::string_view &stop_name) {

        std::unordered_set<const domain::Bus *> bss;
        try {
            const std::set<std::string> routes = db_.GetStopInfo(stop_name);

            for (auto &el: routes) {
                bss.emplace(db_.FindBus(el));
//                buses.emplace(db_.FindBus(el));
            }
        } catch (...) {
            throw ("smth wrong");
        }

        return bss;
    }

    const Transport::Catalogue& RequestHandler::GetCatalogue() const {
        return db_;
    }

    svg::Document RequestHandler::RenderMap() const {
        std::vector<geo::Coordinates> points;
        std::vector<std::vector<svg::Point>> vector_of_path_picture;
        std::vector<renderer::RouteForRend> route_picture;
        std::vector<std::pair<const svg::Point&, const std::string&>> render_busnames;

        auto deque_of_buses = const_cast<std::deque<domain::Bus>&>(db_.GetDequeBus());
        std::sort(deque_of_buses.begin(), deque_of_buses.end());

        for (const auto& bus : deque_of_buses) {
            auto route = db_.GetBusInfo(bus.bus_name_);
            if (route->empty()) continue;
            for (auto stp: *route) {
                if (stp) {
                    geo::Coordinates coords = {stp->latitude, stp->longitude};
                    points.emplace_back(coords);
                }
            }
        }
        renderer_.SetProjCoords(points);

        const auto coord = renderer_.GetProjectedCoords();
        int k = 0;
        std::map<std::string, svg::Point> stp_to_proj_coords;

        for (const auto& bus : deque_of_buses) {
            auto route = db_.GetBusInfo(bus.bus_name_);
            if (route->empty()) continue;
            std::vector<svg::Point> tmp_vector_coords;
            const auto& type_bus = db_.GetRouteType();

            for (size_t i = 0; i < route->size(); ++i) {
                tmp_vector_coords.emplace_back(coord[k]);
                stp_to_proj_coords[(*route)[i]->stop_name] = coord[k];
                ++k;
            }
            if (!type_bus.at(bus.bus_name_)) {
                for (size_t i = 0; i < route->size() - 1; ++i) {
                    tmp_vector_coords.emplace_back(coord[(k - 1) - (i + 1)]);
                }
            }
            vector_of_path_picture.emplace_back(std::move(tmp_vector_coords));
        }
        for (const auto& bus : deque_of_buses) { //отрисовка названий маршрута
            auto route = db_.GetBusInfo(bus.bus_name_);
            const auto& type_bus = db_.GetRouteType();
            renderer::RouteForRend r_f_r({bus.bus_name_, type_bus, route});
            route_picture.emplace_back(r_f_r);
        }
        std::vector<svg::Point> stp_coords;

        for (const auto& [name, p] : stp_to_proj_coords) {
            stp_coords.emplace_back(p);
        }

        for (const auto& [stop, p] : stp_to_proj_coords) { //отрисовка названий остановок
            auto route = db_.GetStopInfo(stop);
            if (route.empty()) continue;
            render_busnames.emplace_back(p, stop);
        }
        return renderer_.RenderMap(vector_of_path_picture, route_picture, stp_coords, render_busnames);
    }
}