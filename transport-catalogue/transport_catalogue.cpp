#include "transport_catalogue.h"

using namespace std::string_literals;

namespace Transport {
    const Stop CONST_STOPNAME = {};
    const Bus CONST_BUSNAME ={};
    const std::vector<const Stop*> CONST_BUS_ROUTE = {};
    const std::set<std::string> CONST_BUSES_ = {};


    void Catalogue::AddStop(const std::string &stop, double latitude, double longitude) {
        Stop stp = {stop, latitude, longitude};
        stops_.push_back(std::move(stp));
        stopname_to_stop_[stops_.back().stop_] = &stops_.back();
        buses_to_stop[stop] = {};
    }

    const Stop& Catalogue::FindStop(const std::string &stop) const {
        if (stopname_to_stop_.count(stop) != 0) return *stopname_to_stop_.at(stop);
        return CONST_STOPNAME;
    }

    void Catalogue::AddBus(const std::string &bus, std::vector<const Stop *> route) {
        Bus bus_add = {bus, std::move(route)};
        buses_.push_back(std::move(bus_add));
        busname_to_bus_[buses_.back().bus_name_] = &buses_.back();
    }

    const Bus& Catalogue::FindBus(const std::string &bus) const {
        if (busname_to_bus_.count(bus) != 0) return *busname_to_bus_.at(bus);
        return CONST_BUSNAME;
    }

    void Catalogue::AddDistance(const Stop &stop1, const Stop &stop2, int dist) {
        std::pair<const Stop *, const Stop *> pair_of_stops = {&stop1, &stop2};
        distances_[pair_of_stops] = dist;
    }

    int Catalogue::GetDistance(const std::string &stop1, const std::string &stop2) const {
        const Stop &stp_p1 = FindStop(stop1);
        const Stop &stp_p2 = FindStop(stop2);
        if (distances_.count({&stp_p1, &stp_p2}) != 0) {
            return distances_.at({&stp_p1, &stp_p2});
        }

        return 0;
    }

    const std::vector<const Stop *>& Catalogue::GetBusInfo(const std::string &bus) const {
        if (busname_to_bus_.count(bus) != 0) return busname_to_bus_.at(bus)->bus_route_;
        return CONST_BUS_ROUTE;
    }

    std::set<std::string> Catalogue::GetStopInfo(const std::string &stop) {
        std::set<std::string> buss;

        for (const auto &bus: buses_) {
            if (std::count_if(bus.bus_route_.begin(), bus.bus_route_.end(),
                              [&](const Stop *stp) { return stop == stp->stop_; }) > 0) {
                buss.insert(bus.bus_name_);
            }
        }
        buses_to_stop.at(stop) = buss;

        if (buses_to_stop.count(stop) != 0) {
            return buses_to_stop.at(stop);
        }

        return CONST_BUSES_;
    }
}