#pragma once

#include <utility>
#include <vector>
#include <fstream>

#include <transport_catalogue.pb.h>
#include <map_renderer.pb.h>
#include <transport_router.pb.h>
#include <graph.pb.h>
#include <svg.pb.h>


#include "domain.h"
#include "json_reader.h"
#include "transport_router.h"

namespace serialize {
    void Serialize(Transport::Catalogue& catalogue, const renderer::MapSettings& map_settings, const json_reader::StopsNBuses&  stp_n_b, const transport_router::TransportRouter &router, std::ostream& output);
    std::tuple<json_reader::StopsNBuses, renderer::MapSettings, transport_router::TransportRouter, graph::DirectedWeightedGraph<double>> Deserialize(Transport::Catalogue& catalogue, std::istream& input);

    void SerializeCatalogue(catalogue_serialize::FullBase &base_proto, const json_reader::StopsNBuses& stp_n_b);
    void SerializeMap(const renderer::MapSettings& map_settings_, catalogue_serialize::FullBase &base_proto);

    renderer::MapSettings DeserializeMap(const map_render_serialize::MapSettings& map_settings_proto);
    json_reader::StopsNBuses DeserializeCatalogue(Transport::Catalogue& catalogue, const catalogue_serialize::FullBase& base_proto);

    void RouterSaveTo(catalogue_serialize::FullBase &base_proto, const transport_router::TransportRouter& router);
    graph::DirectedWeightedGraph<double> DeserializeGraph(graph_proto::DirectedWeightedGraph proto_graph);
//    graph::DirectedWeightedGraph<double> DeserializeGraphFromInput(const catalogue_serialize::FullBase& base_proto);
    transport_router::TransportRouter DeserializeRoute(const Transport::Catalogue& catalogue, const catalogue_serialize::FullBase& base_proto);
}