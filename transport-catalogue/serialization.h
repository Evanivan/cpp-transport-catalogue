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
    void Serialize(const Transport::Catalogue& catalogue, const renderer::MapSettings& map_settings, const transport_router::TransportRouter &router, std::ostream& output);
    std::tuple<Transport::Catalogue, renderer::MapSettings, transport_router::TransportRouter, graph::DirectedWeightedGraph<double>>
        Deserialize(Transport::Catalogue& catalogue, std::istream& input);

    void SerializeCatalogue(catalogue_serialize::FullBase &base_proto, const Transport::Catalogue& catalogue);
    void SerializeMap(catalogue_serialize::FullBase &base_proto, const renderer::MapSettings& map_settings_);

    renderer::MapSettings DeserializeMap(const map_render_serialize::MapSettings& map_settings_proto);
    Transport::Catalogue DeserializeCatalogue(const catalogue_serialize::FullBase& base_proto);

    void RouterSaveTo(catalogue_serialize::FullBase &base_proto, const transport_router::TransportRouter& router);
    graph::DirectedWeightedGraph<double> DeserializeGraph(graph_proto::DirectedWeightedGraph proto_graph);
    transport_router::TransportRouter DeserializeRoute(const catalogue_serialize::FullBase& base_proto, const Transport::Catalogue& catalogue);
}