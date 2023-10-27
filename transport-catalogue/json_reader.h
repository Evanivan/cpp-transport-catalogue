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

#include "request_handler.h"
#include "json_builder.h"
#include "domain.h"
#include "map_renderer.h"
#include "router.h"
#include "transport_router.h"


namespace json_reader {
//    using domain::StopsNBuses;

    struct RouteInfo {
        double curvature = 0.0;
        int request_id = 1;
        int route_length = 0.0;
        int stop_count = 0;
        int unique_stop_count = 0;
    };

    struct BaseRequestTypeBus {
        domain::RequestType type = domain::RequestType::Bus;
        std::string name;
        std::vector<std::string> stops_;
        bool is_roundtrip{};
    };

    struct BaseRequestTypeStop {
        domain::RequestType type = domain::RequestType::Stop;
        std::string name;
        double latitude = 0.0;
        double longitude = 0.0;
        std::map<std::string, int> road_dist;
    };

    struct StopsNBuses {
        std::deque<BaseRequestTypeStop> deque_of_stops;
        std::deque<BaseRequestTypeBus> deque_of_buses;
    };

    std::ostream& operator<<(std::ostream& out, const BaseRequestTypeBus& bus_request);
    std::ostream& operator<<(std::ostream& out, const BaseRequestTypeStop& stop_request);

    int Stops_Uniq(const std::vector<const domain::Stop *>& stops);
    int DistanceRouteCycle(const Transport::Catalogue& catalogue, const domain::Bus& bus);
    int DistanceRouteSeq(const Transport::Catalogue& catalogue, const domain::Bus& bus);
    double CurvationCalc(const Transport::Catalogue& catalogue, const domain::Bus& bus);

    json::Document LoadJSON(const std::string &s);
    std::string Print(const json::Node &node);
    StopsNBuses ReadBase(const json::Array& arr);
    std::vector<domain::StatReqs> ReadStat(const json::Array& req) ;
    renderer::MapSettings ReadMapSettings(const json::Dict & req);
    svg::Color ParseColor(const json::Node& type);

    class GetJson {
    public:
        GetJson() = default;

        void ReadJSON();
        [[nodiscard]] const StopsNBuses& GetStopsNBuses() const;
        [[nodiscard]] const std::vector<domain::StatReqs>& GetStats() const;
        [[nodiscard]] const renderer::MapSettings& GetMapSettings() const;
        [[nodiscard]] const domain::RouteSettings& GetRouteSettings() const;
    private:
        StopsNBuses stp_n_buses;
        std::vector<domain::StatReqs> stats;
        renderer::MapSettings map_settings_;
        domain::RouteSettings route_settings_;
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

    json::Array BuildJSON(const std::vector<domain::StatReqs>& json_stats, req_handler::RequestHandler handler, transport_router::TransportRouter& transport_router);
}
