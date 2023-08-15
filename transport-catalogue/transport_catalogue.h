#pragma once

#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <algorithm>
#include <optional>

#include "domain.h"

namespace Transport {
    namespace detail {
        struct DistanceHash {
            size_t operator()(std::pair<const domain::Stop *, const domain::Stop *> stop) const {
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
    }

    class Catalogue {
    public:
        Catalogue() = default;

        void AddStop(std::string_view stop, double latitude, double longitude);
        const domain::Stop *FindStop(std::string_view stop) const;
        void AddBus(std::string_view bus, std::vector<const domain::Stop *> route, bool path);
        const domain::Bus *FindBus(std::string_view bus) const;
        void AddDistance(const domain::Stop &stop1, const domain::Stop &stop2, int dist);
        int GetDistance(std::string_view stop1, std::string stop2) const;
        const std::vector<const domain::Stop *> *GetBusInfo(std::string_view bus) const;
        std::set<std::string> GetStopInfo(std::string_view str);
        std::unordered_map<std::string, bool> GetRouteType() const;

    private:
        std::deque<domain::Stop> stops_;
        std::unordered_map<std::string_view, const domain::Stop *, detail::StopBusHash> stopname_to_stop_;
        std::deque<domain::Bus> buses_;
        std::unordered_map<std::string_view, std::pair<const domain::Bus *, bool>, detail::StopBusHash> busname_to_bus_;
        std::unordered_map<std::string, std::set<std::string>, detail::StopBusHash> buses_to_stop;
        std::unordered_map<std::pair<const domain::Stop *, const domain::Stop *>, int, detail::DistanceHash> distances_;
    };
}