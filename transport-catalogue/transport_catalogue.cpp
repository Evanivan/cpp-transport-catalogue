#include "transport_catalogue.h"

using namespace std::string_literals;

namespace Transport {
    void Catalogue::AddStop(std::string_view stop, double latitude, double longitude) {
        domain::Stop stp = {std::string(stop), latitude, longitude};
        stops_.push_back(std::move(stp));
        stopname_to_stop_[stops_.back().stop_name] = &stops_.back();
        buses_to_stop[std::string(stop)] = {};
    }

    const domain::Stop *Catalogue::FindStop(std::string_view stop) const {
        if (stopname_to_stop_.count(stop) != 0) {
            return stopname_to_stop_.at(stop);
        }
        return nullptr;
    }

    void Catalogue::AddBus(std::string_view bus, std::vector<const domain::Stop *> route, bool path) {
        domain::Bus bus_add = {std::string(bus), std::move(route), path};
        buses_.push_back(std::move(bus_add));
        if (!bus.empty()) {
            busname_to_bus_[buses_.back().bus_name_] = {&buses_.back(), path};
        }
    }

    const domain::Bus *Catalogue::FindBus(std::string_view bus) const {
        if (busname_to_bus_.count(bus) != 0) return busname_to_bus_.at(bus).first;
        return nullptr;
    }

    void Catalogue::AddDistance(const domain::Stop &stop1, const domain::Stop &stop2, int dist) {
        std::pair<const domain::Stop *, const domain::Stop *> pair_of_stops = {&stop1, &stop2};
        distances_[pair_of_stops] = dist;
    }

    int Catalogue::GetDistance(std::string_view stop_from, std::string stop_to) const {
        const domain::Stop* stp_p1 = FindStop(stop_from);
        const domain::Stop* stp_p2 = FindStop(stop_to);
        std::pair<const domain::Stop*, const domain::Stop*> from_stop_to_stop = {stp_p1, stp_p2};
        if (distances_.count(from_stop_to_stop) != 0) {
            return distances_.at(from_stop_to_stop);
        }

        if (distances_.count(from_stop_to_stop)) {
            return distances_.at(from_stop_to_stop);
        }
        std::pair<const domain::Stop*, const domain::Stop*> reversed_pair;
        reversed_pair = std::make_pair(from_stop_to_stop.second, from_stop_to_stop.first);
        if (distances_.count(reversed_pair)) {
            return distances_.at(reversed_pair);
        }

        return 0;
    }

    const std::vector<const domain::Stop *> *Catalogue::GetBusInfo(std::string_view bus) const {
        if (busname_to_bus_.count(bus) != 0) return &busname_to_bus_.at(bus).first->bus_route_;
        return {};
    }

    std::set<std::string> Catalogue::GetStopInfo(std::string_view stop) {
        std::set<std::string> buss;

        for (const auto &bus: buses_) {
            if (std::count_if(bus.bus_route_.begin(), bus.bus_route_.end(),
                              [&](const domain::Stop *stp) { return stop == stp->stop_name; }) > 0) {
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
        for (const auto &[name, pair_of_type]: busname_to_bus_) {
            map_of_types[std::string(name)] = pair_of_type.second;
        }
        return map_of_types;
    }
}