#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

using namespace std::literals::string_literals;
using namespace Catalogue;

int main() {
    Transport::Catalogue catalogue;

    Reader::ParseRequestStrings(catalogue);
    Stat::StatOut(catalogue);

    return 0;
}