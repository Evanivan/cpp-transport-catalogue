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
        using namespace std::literals;

        svg::Document doc;
        std::vector<geo::Coordinates> points;
        auto colors = renderer_.GetColorPallet();
        auto settings = renderer_.GetSettings();
        std::set<std::string> set_of_buses;

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
        size_t j = 0;
        int k = 0;

        std::map<std::string, svg::Point> stp_to_proj_coords;
        for (const auto& bus : set_of_buses) {
            auto route = db_.GetBusInfo(bus);
            if (route->empty()) continue;
            if (j == colors.size()) {
                j = 0;
            }
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
            renderer_.MakePathPicture(tmp_vector_coords, j);
            ++j;
        }
        renderer_.RenDrawPicture(doc);

        size_t num_route = 0;
        size_t color_num = 0;
        for (const auto& bus : set_of_buses) { //отрисовка названий маршрута
            auto route = db_.GetBusInfo(bus);
            if (route->empty()) continue;
            if (color_num == colors.size()) {
                color_num = 0;
            }
            const auto type_bus = db_.GetRouteType();

            renderer_.RenderRoute(doc, bus, num_route, color_num, type_bus, route);
            num_route += route->size();
            ++color_num;
        }

        std::vector<svg::Point> stp_coords;

        for (const auto& [name, p] : stp_to_proj_coords) {
            stp_coords.emplace_back(p);
        }

        renderer_.MakePointsPicture(stp_coords);
        renderer_.RenDrawPicture(doc);

        for (const auto& [stop, p] : stp_to_proj_coords) { //отрисовка названий остановок
            auto route = db_.GetStopInfo(stop);
            if (route.empty()) continue;
            renderer_.RenderBusNames(p, stop, doc);
        }
        return doc;
    }
}