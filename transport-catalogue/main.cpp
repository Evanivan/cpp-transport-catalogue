#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <iostream>

using namespace std::literals::string_literals;
using namespace Catalogue;

int main() {

    Transport::Catalogue catalogue;

    const auto paths = Reader::ParseRequestStrings(catalogue);

    Stat::StatOut(catalogue, paths);

    return 0;
}