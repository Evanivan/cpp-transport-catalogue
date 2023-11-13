#include <iostream>
#include <fstream>
#include <string_view>
#include <fstream>

#include "json_reader.h"
#include "map_renderer.h"
#include "request_handler.h"
#include "transport_router.h"
#include "serialization.h"

using namespace std::literals;

void PrintUsage(std::ostream& stream = std::cerr) {
    stream << "Usage: transport_catalogue [make_base|process_requests]\n"sv;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        PrintUsage();
        return 1;
    }

    const std::string_view mode(argv[1]);

    //строим базу справочника

    if (mode == "make_base"sv) {
        json_reader::GetJson json;
        Transport::Catalogue catalogue;
        json.ReadJSON();

        json_reader::BuildBase base(catalogue, json.GetStopsNBuses());

        base.BuildStops();
        base.BuildBuses();

        transport_router::TransportRouter router(catalogue, json.GetRouteSettings());

        std::ofstream output(json.GetFileName(), std::ios::out | std::ios::binary);
        if(output.is_open()) {
            serialize::Serialize(base.GetCatalogue(), json.GetMapSettings(), json.GetStopsNBuses(), router, output);
        }
    } else if (mode == "process_requests"sv) {
        // process requests here
        json_reader::GetJson json;
        json.ReadJSON();

        Transport::Catalogue catalogue;

        std::ifstream input_f(json.GetFileName(), std::ios::in | std::ios::binary);
        if(input_f) {
            auto [deserialized_catalogue, deserialized_map, deserialized_route, deserialized_graph] = serialize::Deserialize(catalogue, input_f);
            renderer::MapRenderer renderer(deserialized_map);

            req_handler::RequestHandler render_request(catalogue, renderer);
            render_request.GetAllBuses();

            deserialized_route.SetGraph(deserialized_graph);
            auto arr = json_reader::BuildJSON(json.GetStats(), std::move(render_request), deserialized_route);
            //вывод JSON в поток
            std::stringstream strm;
            json::Print(json::Document{arr}, strm);

            const auto doc = json::Load(strm);

            std::cout << json_reader::Print(doc.GetRoot());
        }

    } else {
        PrintUsage();
        return 1;
    }

    return 0;
}