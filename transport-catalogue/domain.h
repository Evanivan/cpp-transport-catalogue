#pragma once

#include <string>
#include <vector>
#include "geo.h"

namespace domain{
    struct Stop {
        std::string stop_name;
        double latitude;
        double longitude;
    };

    struct Bus {
        std::string bus_name_;
        std::vector<const Stop *> bus_route_;
        bool is_route_round{};
    };
}
