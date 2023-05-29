#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <iostream>

using namespace std::literals::string_literals;
using namespace Transport;
using namespace Stat;

int main() {

    Catalogue catalogue;

    const auto paths = ParseRequestStrings(catalogue);

    StatOut(catalogue, paths);

    return 0;
}