#pragma once

#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <set>

#include "transport_catalogue.h"
#include "geo.h"

namespace domain_json {
    int Stops_Uniq(const std::vector<const Transport::Stop *>& stops);
    int DistanceRouteCycle(const Transport::Catalogue& catalogue, const Transport::Bus& bus);
    int DistanceRouteSeq(const Transport::Catalogue& catalogue, const Transport::Bus& bus);
    double CurvationCalc(const Transport::Catalogue& catalogue, const Transport::Bus& bus);
}
