#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <set>

#include "transport_catalogue.h"
#include "geo.h"
#include "input_reader.h"

namespace ReadStat {
    using namespace Transport;
    using namespace Input;

    enum class TypeInput {
        Bus,
        Stop
    };

    struct Input {
        TypeInput type_;
        std::string req_name_;
    };

    std::string FindName(std::string_view vector_word);

    int ReadLineWithNumber();

    std::vector<Input> ReadData();

    void FindAllStopsRequired(Catalogue& catalogue, const std::vector<const Stop*>& route);

    int DistanceRouteCycle(const Catalogue& catalogue, const Bus& bus);

    int DistanceRouteSeq(const Catalogue& catalogue, const Bus& bus);

    void StatOut(Catalogue& catalogue, const std::unordered_map<std::string, Path>& paths);
}
