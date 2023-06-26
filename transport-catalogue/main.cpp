#include <iostream>

#include "json_reader.h"
#include "map_renderer.h"
#include "request_handler.h"

using namespace json_reader;

int main() {
    /*
     * Примерная структура программы:
     *
     * Считать JSON из stdin
     * Построить на его основе JSON базу данных транспортного справочника
     * Выполнить запросы к справочнику, находящиеся в массиве "stat_requests", построив JSON-массив
     * с ответами.
     * Вывести в stdout ответы в виде JSON
     */

    using namespace renderer::shapes;
    using namespace std;

    json_reader::GetJson json;
    json.ReadJSON();

    renderer::MapSettings settings = json.GetMapSettings();
    renderer::MapRenderer renderer(settings);

    Transport::Catalogue catalogue;
    //строим базу справочника
    json_reader::BuildBase base(catalogue, json.GetStopsNBuses());

    base.BuildStops();
    base.BuildBuses();

    req_handler::RequestHandler render_request(base.GetCatalogue(), renderer);
    render_request.GetAllBuses();

    auto arr = BuildJSON(base, json.GetStats(), render_request);

    std::stringstream strm;
    json::Print(json::Document{arr}, strm);
    const auto doc = json::Load(strm);

    std::cout << json_reader::Print(doc.GetRoot());
    return 0;
}