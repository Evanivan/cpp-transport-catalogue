#pragma once

#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <algorithm>

namespace Transport {

    struct Stop {
        std::string stop_;
        double lat_de_;
        double long_de_;
    };

    struct Bus {
        std::string bus_name_;
        std::vector<const Stop *> bus_route_;
    };

    struct DistanceHash {
        size_t operator()(std::pair<const Stop *, const Stop *> stop) const {
            size_t h_first = pointer_hasher(stop.first);
            size_t h_second = pointer2_hasher(stop.second);

            return h_first * 37 + h_second * (37 * 37 * 37 * 37);
        }

    private:
        std::hash<const void *> pointer_hasher;
        std::hash<const void *> pointer2_hasher;
    };

    struct StopBusHash {
        size_t operator()(std::string_view name) const {
            size_t name_h = s_hasher(std::string(name));
            return name_h * 37;
        }

    private:
        std::hash<std::string> s_hasher;
    };

    class Catalogue {
    public:
        Catalogue() = default;

        void AddStop(const std::string &stop, double latitude, double longitude);

        const Stop &FindStop(const std::string &stop) const;

        void AddBus(const std::string &bus, std::vector<const Stop *> route);

        const Bus &FindBus(const std::string &bus) const;

        void AddDistance(const Stop &stop1, const Stop &stop2, int dist);

        int GetDistance(const std::string &stop1, const std::string &stop2) const;

        const std::vector<const Stop *> &GetBusInfo(const std::string &bus) const;

        std::set<std::string> GetStopInfo(const std::string &str);

    private:
        std::deque<Stop> stops_;
        std::unordered_map<std::string_view, const Stop *, StopBusHash> stopname_to_stop_;
        std::deque<Bus> buses_;
        std::unordered_map<std::string_view, const Bus *, StopBusHash> busname_to_bus_;
        std::unordered_map<std::string, std::set<std::string>, StopBusHash> buses_to_stop;
        std::unordered_map<std::pair<const Stop *, const Stop *>, int, DistanceHash> distances_;
    };
}