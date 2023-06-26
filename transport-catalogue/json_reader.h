#pragma once

#include <sstream>
#include <map>
#include <deque>
#include <algorithm>
#include <optional>
#include <set>
#include <unordered_set>
#include <cmath>
#include <numeric>

#include "json.h"
#include "request_handler.h"
#include "domain.h"
#include "map_renderer.h"

namespace json_reader {
    enum class RequestType {
        Bus,
        Stop,
        Map
    };

    struct BaseRequestTypeBus {
        RequestType type = RequestType::Bus;
        std::string name;
        std::vector<std::string> stops_;
        bool is_roundtrip{};
    };

    struct BaseRequestTypeStop {
        RequestType type = RequestType::Stop;
        std::string name;
        double latitude = 0.0;
        double longitude = 0.0;
        std::map<std::string, int> road_dist;
    };

    struct StatReqs {
        int id;
        RequestType type;
        std::string name;
    };

    struct StopsNBuses {
        std::deque<BaseRequestTypeStop> deque_of_stops;
        std::deque<BaseRequestTypeBus> deque_of_buses;
    };

    struct RouteInfo {
        double curvature = 0.0;
        int request_id = 1;
        int route_length = 0.0;
        int stop_count = 0;
        int unique_stop_count = 0;
    };

    std::ostream& operator<<(std::ostream& out, const BaseRequestTypeBus& bus_request);
    std::ostream& operator<<(std::ostream& out, const BaseRequestTypeStop& stop_request);

    json::Document LoadJSON(const std::string &s);
    std::string Print(const json::Node &node);
    StopsNBuses ReadBase(const json::Array& arr);
    std::vector<StatReqs> ReadStat(const json::Array& req);
    renderer::MapSettings ReadMapSettings(const json::Dict & req);

    class GetJson {
    public:
        GetJson() = default;

        void ReadJSON();
        [[nodiscard]] const StopsNBuses& GetStopsNBuses() const;
        [[nodiscard]] const std::vector<StatReqs>& GetStats() const;
        [[nodiscard]] const renderer::MapSettings& GetMapSettings() const;
    private:
        StopsNBuses stp_n_buses;
        std::vector<StatReqs> stats;
        renderer::MapSettings map_settings_;
    };

    class BuildBase {
    public:
        BuildBase(Transport::Catalogue& catalogue, const StopsNBuses& stats)
                :   catalogue_(catalogue),
                    stp_n_base_stat_(stats){
        }

        void BuildStops();
        void BuildBuses();
        [[nodiscard]] Transport::Catalogue& GetCatalogue() const;

    private:
        Transport::Catalogue& catalogue_;
        const StopsNBuses& stp_n_base_stat_;
    };

    json::Array BuildJSON(BuildBase base, const std::vector<StatReqs>& stats, req_handler::RequestHandler handler);
}
