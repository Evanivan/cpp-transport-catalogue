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

namespace Catalogue {
    namespace Transport {
        struct Stop {
            std::string stop_name;
            double latitude;
            double longitude;
        };

        struct Bus {
            std::string bus_name_;
            std::vector<const Stop *> bus_route_;
        };

        enum class Path{
            Cycle,
            Seq
        };

        namespace detail {
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
        }

        class Catalogue {
        public:
            Catalogue() = default;

            void AddStop(std::string_view stop, double latitude, double longitude);

            const Stop *FindStop(std::string_view stop) const;

            void AddBus(std::string_view bus, std::vector<const Stop *> route);

            const Bus *FindBus(std::string_view bus) const;

            void AddDistance(const Stop &stop1, const Stop &stop2, int dist);

            int GetDistance(std::string_view stop1, std::string stop2) const;

            const std::vector<const Stop *> *GetBusInfo(std::string_view bus) const;

            std::set<std::string> GetStopInfo(std::string_view str);

            void SetRouteType(std::string_view str, Path path);

            const std::unordered_map<std::string, Path>& GetRouteType() const;

        private:
            std::deque<Stop> stops_;
            std::unordered_map<std::string_view, const Stop *, detail::StopBusHash> stopname_to_stop_;
            std::deque<Bus> buses_;
            std::unordered_map<std::string_view, const Bus *, detail::StopBusHash> busname_to_bus_;
            std::unordered_map<std::string_view, std::set<std::string>, detail::StopBusHash> buses_to_stop;
            std::unordered_map<std::pair<const Stop *, const Stop *>, int, detail::DistanceHash> distances_;
            std::unordered_map<std::string, Path> route_type;
        };
    }
}