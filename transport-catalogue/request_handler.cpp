#include "request_handler.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

namespace req_handler {
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
                buses.emplace(db_.FindBus(el));
            }
        } catch (...) {
            throw ("smth wrong");
        }

        return bss;
    }

    void RequestHandler::GetAllBuses() {
        auto routes = db_.GetRouteType();

        for (auto& [name, type]: routes) {
            buses.emplace(db_.FindBus(name));
        }
    }

    svg::Document RequestHandler::RenderMap() const {
        std::vector<geo::Coordinates> points;
        std::set<std::string> set_of_buses;
        std::vector<std::vector<svg::Point>> vector_of_path_picture;
        std::vector<renderer::RouteForRend> route_picture;
        std::vector<std::pair<const svg::Point&, const std::string&>> render_busnames;

        for (auto bus : buses) {
            set_of_buses.insert(bus->bus_name_);
        }

        for (const auto& bus : set_of_buses) {
            auto route = db_.GetBusInfo(bus);
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

        for (const auto& bus : set_of_buses) {
            auto route = db_.GetBusInfo(bus);
            if (route->empty()) continue;
            std::vector<svg::Point> tmp_vector_coords;
            const auto type_bus = db_.GetRouteType();

            for (size_t i = 0; i < route->size(); ++i) {
                tmp_vector_coords.emplace_back(coord[k]);
                stp_to_proj_coords[(*route)[i]->stop_name] = coord[k];
                ++k;
            }
            if (!type_bus.at(bus)) {
                for (size_t i = 0; i < route->size() - 1; ++i) {
                    tmp_vector_coords.emplace_back(coord[(k - 1) - (i + 1)]);
                }
            }
            vector_of_path_picture.emplace_back(std::move(tmp_vector_coords));
        }
        for (const auto& bus : set_of_buses) { //отрисовка названий маршрута
            auto route = db_.GetBusInfo(bus);
            const auto type_bus = db_.GetRouteType();
            renderer::RouteForRend r_f_r({bus, type_bus, route});
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