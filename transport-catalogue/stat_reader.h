#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <set>

#include "transport_catalogue.h"
#include "geo.h"
#include "input_reader.h"

namespace ReadStat {
    using Input::Path;
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
    void FindAllStopsRequired(Transport::Catalogue& catalogue, const std::vector<const Transport::Stop*>& route);
    int DistanceRouteCycle(const Transport::Catalogue& catalogue, const Transport::Bus& bus);
    int DistanceRouteSeq(const Transport::Catalogue& catalogue, const Transport::Bus& bus);
    void StatOut(Transport::Catalogue& catalogue, const std::unordered_map<std::string, Path>& paths);
}
