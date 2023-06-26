#include "domain.h"

using namespace std::string_literals;

namespace domain_json {
    using namespace Transport;
    using namespace geo;


    int Stops_Uniq(const std::vector<const Stop *>& stops) {
        std::set<std::string> stops_unique;
        for (auto el : stops) {
            stops_unique.insert(el->stop_name);
        }
        return static_cast<int>(stops_unique.size());
    }

    int DistanceRouteCycle(const Catalogue& catalogue, const Bus& bus) {
        int sum = 0;
        for (size_t i = 0; i != bus.bus_route_.size() - 1; ++i) {
            if (catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name) == 0) {
                sum += catalogue.GetDistance(bus.bus_route_[i + 1]->stop_name, bus.bus_route_[i]->stop_name);
                continue;
            }
            sum += catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name);
        }
        return sum;
    }

    int DistanceRouteSeq(const Catalogue& catalogue, const Bus& bus) {
        std::vector<const Stop*> route_reverse(bus.bus_route_.size());
        std::reverse_copy(bus.bus_route_.begin(), bus.bus_route_.end(), route_reverse.begin());
        int sum = 0;
        for (size_t i = 0; i != bus.bus_route_.size() - 1; ++i) {
            if (catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name) == 0) {
                sum += catalogue.GetDistance(bus.bus_route_[i + 1]->stop_name, bus.bus_route_[i]->stop_name);
                continue;
            }
            sum += catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name);
        }

        for (size_t i = 0; i != route_reverse.size() - 1; ++i) {
            if (catalogue.GetDistance(route_reverse[i]->stop_name, route_reverse[i + 1]->stop_name) == 0) {
                sum += catalogue.GetDistance(route_reverse[i + 1]->stop_name, route_reverse[i]->stop_name);
                continue;
            }
            sum += catalogue.GetDistance(route_reverse[i]->stop_name, route_reverse[i + 1]->stop_name);
        }

        return sum;
    }

    double CurvationCalc(const Transport::Catalogue& catalogue, const Transport::Bus& bus) {
        double sum = 0.0;
        const auto& all_stops = catalogue.GetBusInfo(bus.bus_name_);
        if (bus.is_route_round) {
            for (size_t i = 0; i < (*all_stops).size() + 1; ++i) {
                if (i == (*all_stops).size() - 1) {
                    break;
                }
                sum += ComputeDistance({(*all_stops)[i]->latitude, (*all_stops)[i]->longitude},
                                       {(*all_stops)[i + 1]->latitude, (*all_stops)[i + 1]->longitude});
            }
        } else {
            for (size_t i = 0; i < (*all_stops).size() + 1; ++i) {
                if (i == (*all_stops).size() - 1) {
                    break;
                }
                sum += 2 * ComputeDistance({(*all_stops)[i]->latitude, (*all_stops)[i]->longitude},
                                           {(*all_stops)[i + 1]->latitude, (*all_stops)[i + 1]->longitude});
            }
        }
        return sum;
    }

}