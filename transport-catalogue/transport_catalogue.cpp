#include "transport_catalogue.h"

using namespace std::string_literals;

namespace Transport {
    void Catalogue::AddStop(std::string_view stop, double latitude, double longitude) {
        Stop stp = {std::string(stop), latitude, longitude};
        stops_.push_back(std::move(stp));
        stopname_to_stop_[stops_.back().stop_name] = &stops_.back();
        buses_to_stop[std::string(stop)] = {};
    }

    const Stop *Catalogue::FindStop(std::string_view stop) const {
        if (stopname_to_stop_.count(stop) != 0) {
            return stopname_to_stop_.at(stop);
        }
        return nullptr;
    }

    void Catalogue::AddBus(std::string_view bus, std::vector<const Stop *> route, bool path) {
        Bus bus_add = {std::string(bus), std::move(route), path};
        buses_.push_back(std::move(bus_add));
        if (!bus.empty()) {
            busname_to_bus_[buses_.back().bus_name_] = {&buses_.back(), path};
        }
    }

    const Bus* Catalogue::FindBus(std::string_view bus) const {
        if (busname_to_bus_.count(bus) != 0) return busname_to_bus_.at(bus).first;
        return nullptr;
    }

    void Catalogue::AddDistance(const Stop &stop1, const Stop &stop2, int dist) {
        std::pair<const Stop *, const Stop *> pair_of_stops = {&stop1, &stop2};
        distances_[pair_of_stops] = dist;
    }

    int Catalogue::GetDistance(std::string_view stop1, std::string stop2) const {
        const Stop &stp_p1 = *FindStop(stop1);
        const Stop &stp_p2 = *FindStop(stop2);
        if (distances_.count({&stp_p1, &stp_p2}) != 0) {
            return distances_.at({&stp_p1, &stp_p2});
        }

        return 0;
    }

    const std::vector<const Stop *> *Catalogue::GetBusInfo(std::string_view bus) const {
        if (busname_to_bus_.count(bus) != 0) return &busname_to_bus_.at(bus).first->bus_route_;
        return {};
    }

    std::set<std::string> Catalogue::GetStopInfo(std::string_view stop) {
        std::set<std::string> buss;

        for (const auto &bus: buses_) {
            if (std::count_if(bus.bus_route_.begin(), bus.bus_route_.end(),
                              [&](const Stop *stp) { return stop == stp->stop_name; }) > 0) {
                buss.insert(bus.bus_name_);
            }
        }
        buses_to_stop.at(std::string(stop)) = buss;

        if (buses_to_stop.count(std::string(stop)) != 0) {
            return buses_to_stop.at(std::string(stop));
        }

        return buss;
    }

    std::unordered_map<std::string, bool> Catalogue::GetRouteType() const {
        std::unordered_map<std::string, bool> map_of_types;
        for (const auto& [name, pair_of_type]  : busname_to_bus_) {
            map_of_types[std::string(name)] = pair_of_type.second;
        }
        return map_of_types;
    }
}