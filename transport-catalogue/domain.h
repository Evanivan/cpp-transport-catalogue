#pragma once

#include <string>
#include <vector>
#include <optional>
#include <map>
#include <deque>

#include "geo.h"

namespace domain{
    enum class ResponsType{
        Wait,
        Bus
    };

    enum class RequestType {
        Bus,
        Stop,
        Map,
        Route
    };

    struct ResponsRoute {
        ResponsType type{};
        std::string name_;
        std::optional<int> span_count_;
        double time = 0.0;
    };

    struct StatReqs {
        int id{};
        RequestType type{};
        std::optional<std::string> name; // name of bus or stop
        std::optional<std::string> from;
        std::optional<std::string> to;
    };

    struct RouteSettings {
        double bus_wait_time = 0;
        double bus_velocity = 0;
    };

    struct Stop {
        std::string stop_name;
        double latitude;
        double longitude;
    };

    struct Bus {
        std::string bus_name_;
        std::vector<const Stop*> bus_route_;
        bool is_route_round{};
    };

    struct StopInBus {
        const domain::Stop* stop_;
        const domain::Bus* bus_;
    };
}
