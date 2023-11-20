#include "serialization.h"
#include <variant>

namespace serialize {
    void Serialize(const Transport::Catalogue& catalogue
            , const renderer::MapSettings&  map_settings
            , const transport_router::TransportRouter& router
            , std::ostream &output)
    {
        catalogue_serialize::FullBase proto_base;

        SerializeCatalogue(proto_base, catalogue);
        SerializeMap(proto_base, map_settings);
        RouterSaveTo( proto_base, router);

        proto_base.SerializeToOstream(&output);
    }

    std::tuple<Transport::Catalogue, renderer::MapSettings, transport_router::TransportRouter, graph::DirectedWeightedGraph<double>>
        Deserialize(Transport::Catalogue& catalogue, std::istream& input)
    {
        catalogue_serialize::FullBase base_proto;
        base_proto.ParseFromIstream(&input);

        renderer::MapSettings settings = DeserializeMap(base_proto.render_settings());
        Transport::Catalogue des_catalogue = DeserializeCatalogue(base_proto);
        catalogue = des_catalogue;
        transport_router::TransportRouter router = DeserializeRoute(base_proto, catalogue);
        graph::DirectedWeightedGraph<double> graph = DeserializeGraph(base_proto.router().graph());

        return {std::move(des_catalogue), std::move(settings), std::move(router), std::move(graph)};
    }

    void SerializeCatalogue(catalogue_serialize::FullBase &base_proto, const Transport::Catalogue& catalogue) {
        catalogue_serialize::TransportCatalogue transport_catalogue;

        for (const auto& stop : catalogue.GetDequeStp()) {
            catalogue_serialize::Stop* pb_stop = transport_catalogue.add_stops();
            pb_stop->set_stop_name(stop.stop_name);
            pb_stop->set_latitude(stop.latitude);
            pb_stop->set_longitude(stop.longitude);
        }


        for(const auto& [stops, distance] : catalogue.GetAllDist()) {
            catalogue_serialize::Distances* proto_distance = transport_catalogue.add_distance();
            proto_distance->set_from(stops.first->stop_name);
            proto_distance->set_to(stops.second->stop_name);
            proto_distance->set_dist(distance);
        }

        for (const auto& bus : catalogue.GetDequeBus()) {
            catalogue_serialize::Bus* pb_bus = transport_catalogue.add_buses();
            pb_bus->set_bus_name(bus.bus_name_);
            pb_bus->set_is_route_round(bus.is_route_round);

            for (const auto& stop : bus.bus_route_) {
                pb_bus->add_route(stop->stop_name);
            }
        }

        *base_proto.mutable_transport_catalogue() = std::move(transport_catalogue);
    }
    void SerializeMap(catalogue_serialize::FullBase &base_proto, const renderer::MapSettings& render_map) {
        map_render_serialize::MapSettings map_settings_proto;

        map_settings_proto.set_width(render_map.width);
        map_settings_proto.set_height(render_map.height);
        map_settings_proto.set_padding(render_map.padding);
        map_settings_proto.set_stop_radius(render_map.stop_radius);
        map_settings_proto.set_line_width(render_map.line_width);
        map_settings_proto.set_bus_label_font_size(render_map.bus_label_font_size);
        map_render_serialize::Pair* bus_label_offset_proto = map_settings_proto.mutable_bus_label_offset();
        bus_label_offset_proto->set_first(render_map.bus_label_offset.first);
        bus_label_offset_proto->set_second(render_map.bus_label_offset.second);

        map_settings_proto.set_stop_label_font_size(render_map.stop_label_font_size);

        map_render_serialize::Pair* stop_label_offset_proto = map_settings_proto.mutable_stop_label_offset();
        stop_label_offset_proto->set_first(render_map.stop_label_offset.first);
        stop_label_offset_proto->set_second(render_map.stop_label_offset.second);

        if (std::holds_alternative<std::monostate>(render_map.underlayer_color)) {
            map_settings_proto.mutable_underlayer_color()->set_empty("");
        } else if (std::holds_alternative<std::string>(render_map.underlayer_color)) {
            map_settings_proto.mutable_underlayer_color()->set_empty(std::get<std::string>(render_map.underlayer_color));
        } else if (std::holds_alternative<svg::Rgb>(render_map.underlayer_color)) {
            const auto& rgb = std::get<svg::Rgb>(render_map.underlayer_color);
            proto_svg::Rgb* underlayer_color_proto = map_settings_proto.mutable_underlayer_color()->mutable_rgb();
            underlayer_color_proto->set_red(rgb.red);
            underlayer_color_proto->set_green(rgb.green);
            underlayer_color_proto->set_blue(rgb.blue);
        } else if (std::holds_alternative<svg::Rgba>(render_map.underlayer_color)) {
            const auto& rgba = std::get<svg::Rgba>(render_map.underlayer_color);
            proto_svg::Rgba* underlayer_color_proto = map_settings_proto.mutable_underlayer_color()->mutable_rgba();
            underlayer_color_proto->set_red(rgba.red);
            underlayer_color_proto->set_green(rgba.green);
            underlayer_color_proto->set_blue(rgba.blue);
            underlayer_color_proto->set_opacity(rgba.opacity);
        }

        map_settings_proto.set_underlayer_width(render_map.underlayer_width);

        for (const auto& color : render_map.color_palette) {
            proto_svg::Color* color_proto = map_settings_proto.add_color_palette();

            if (std::holds_alternative<std::monostate>(color)) {
                color_proto->set_empty("");
            } else if (std::holds_alternative<std::string>(color)) {
                color_proto->set_empty(std::get<std::string>(color));
            } else if (std::holds_alternative<svg::Rgb>(color)) {
                const svg::Rgb& rgb = std::get<svg::Rgb>(color);
                proto_svg::Rgb* color_rgb_proto = color_proto->mutable_rgb();
                color_rgb_proto->set_red(rgb.red);
                color_rgb_proto->set_green(rgb.green);
                color_rgb_proto->set_blue(rgb.blue);
            } else if (std::holds_alternative<svg::Rgba>(color)) {
                const svg::Rgba& rgba = std::get<svg::Rgba>(color);
                proto_svg::Rgba* color_rgba_proto = color_proto->mutable_rgba();
                color_rgba_proto->set_red(rgba.red);
                color_rgba_proto->set_green(rgba.green);
                color_rgba_proto->set_blue(rgba.blue);
                color_rgba_proto->set_opacity(rgba.opacity);
            }
        }

        *base_proto.mutable_render_settings() = std::move(map_settings_proto);
    }
    router_proto::ResponsRouteData SerializeEdgeInfo(const domain::ResponsRoute &edge_info)
    {
        router_proto::ResponsRouteData response_data_proto;
        const auto edge_type = edge_info.type;

        response_data_proto.set_name(edge_info.name_);
        response_data_proto.set_type(edge_type == domain::ResponsType::Wait ? router_proto::TYPE_WAIT : router_proto::TYPE_BUS);

        if (edge_info.span_count_.has_value()) {
            response_data_proto.set_span_count(edge_info.span_count_.value());
        }

        response_data_proto.set_time(edge_info.time);

        return std::move(response_data_proto);
    }
    graph_proto::DirectedWeightedGraph SerializeGraph(const graph::DirectedWeightedGraph<double> &graph)
    {
        graph_proto::DirectedWeightedGraph serial_graph;

        for(size_t i = 0; i < graph.GetEdgeCount(); ++i) {
            const auto edge = graph.GetEdge(i);
            graph_proto::Edge serial_edge;
            serial_edge.set_from(edge.from);
            serial_edge.set_to(edge.to);

            serial_edge.set_weight(edge.weight);
            *serial_graph.add_edge() = serial_edge;
        }

        for(size_t i = 0; i < graph.GetVertexCount(); ++i) {
            graph_proto::IncidenceList list;
            for(const auto& edge_id : graph.GetIncidentEdges(i)) {
                list.add_edge_id(edge_id);
            }
            *serial_graph.add_incidence_list() = list;
        }

        return std::move(serial_graph);
    }
    catalogue_serialize::Stop SerializeStop(const domain::Stop& stop) {
        catalogue_serialize::Stop stop_proto;
        stop_proto.set_stop_name(stop.stop_name);
        stop_proto.set_latitude(stop.latitude);
        stop_proto.set_longitude(stop.longitude);

        return std::move(stop_proto);
    }
    void RouterSaveTo(catalogue_serialize::FullBase &base_proto, const transport_router::TransportRouter& router)
    {
        router_proto::RouteData route_data;

        // Serialize the graph
        *route_data.mutable_graph() = SerializeGraph(router.GetGraph());

        // Serialize stops_to_id_transit
        for (const auto& entry : router.GetIds()) {
            router_proto::StopsToId stp_to_id;
            stp_to_id.set_stp_bus(entry.first->stop_name);
            stp_to_id.set_stop_id(entry.second);
            *route_data.add_stops_to_id_transit() = stp_to_id;
        }

        // Serialize edge_to_info
        const auto& edge_to_info = router.GetEdgeToInfo();
        for (const auto& entry : edge_to_info) {
            router_proto::ResponsResult proto_respons;
            proto_respons.set_r_id(entry.first);
            *proto_respons.mutable_data() = SerializeEdgeInfo(entry.second);
            *route_data.add_edge_to_info() = std::move(proto_respons);
        }

        *base_proto.mutable_router() = std::move(route_data);
    }

    renderer::MapSettings DeserializeMap(const map_render_serialize::MapSettings& map_settings_proto) {
        renderer::MapSettings map_settings;

        map_settings.width = map_settings_proto.width();
        map_settings.height = map_settings_proto.height();
        map_settings.padding = map_settings_proto.padding();
        map_settings.stop_radius = map_settings_proto.stop_radius();
        map_settings.line_width = map_settings_proto.line_width();
        map_settings.bus_label_font_size = map_settings_proto.bus_label_font_size();
        map_settings.bus_label_offset.first = map_settings_proto.bus_label_offset().first();
        map_settings.bus_label_offset.second = map_settings_proto.bus_label_offset().second();
        map_settings.stop_label_font_size = map_settings_proto.stop_label_font_size();
        map_settings.stop_label_offset.first = map_settings_proto.stop_label_offset().first();
        map_settings.stop_label_offset.second = map_settings_proto.stop_label_offset().second();

        svg::Color underlayer_color;
        if (map_settings_proto.has_underlayer_color()) {
            const auto& color_proto = map_settings_proto.underlayer_color();
            if (color_proto.color_case() == proto_svg::Color::ColorCase::kEmpty) {
                underlayer_color = std::move(svg::Color(color_proto.empty()));
            } else if (color_proto.color_case() == proto_svg::Color::ColorCase::kRgb) {
                const auto& rgb_proto = std::move(color_proto.rgb());
                svg::Rgb rgb(rgb_proto.red(), rgb_proto.green(), rgb_proto.blue());
                underlayer_color = std::move(svg::Color(rgb));
            } else if (color_proto.color_case() == proto_svg::Color::ColorCase::kRgba) {
                const auto& rgba_proto = std::move(color_proto.rgba());
                svg::Rgba rgba(rgba_proto.red(), rgba_proto.green(), rgba_proto.blue(), rgba_proto.opacity());
                underlayer_color = std::move(svg::Color(rgba));
            }
        }
        map_settings.underlayer_color = std::move(underlayer_color);

        map_settings.underlayer_width = map_settings_proto.underlayer_width();

        for (const auto& color_proto : map_settings_proto.color_palette()) {
            svg::Color color;
            if (color_proto.color_case() == proto_svg::Color::ColorCase::kEmpty) {
                color = std::move(svg::Color(color_proto.empty()));
            } else if (color_proto.color_case() == proto_svg::Color::ColorCase::kRgb) {
                const auto& rgb_proto = std::move(color_proto.rgb());
                svg::Rgb rgb(rgb_proto.red(), rgb_proto.green(), rgb_proto.blue());
                color = std::move(svg::Color(rgb));
            } else if (color_proto.color_case() == proto_svg::Color::ColorCase::kRgba) {
                const auto& rgba_proto = std::move(color_proto.rgba());
                svg::Rgba rgba(rgba_proto.red(), rgba_proto.green(), rgba_proto.blue(), rgba_proto.opacity());
                color = std::move(svg::Color(rgba));
            } else {
                std::cerr << "Invalid color message" << std::endl;
                continue;
            }
            map_settings.color_palette.push_back(std::move(color));
        }

        return std::move(map_settings);
    }

    Transport::Catalogue DeserializeCatalogue(const catalogue_serialize::FullBase& base) {
        catalogue_serialize::TransportCatalogue transport_catalogue_proto = base.transport_catalogue();
        Transport::Catalogue catalogue;

        for (const auto& pb_stop : transport_catalogue_proto.stops()) {
            json_reader::BaseRequestTypeStop stop;
            stop.name = std::move(pb_stop.stop_name());
            stop.latitude = pb_stop.latitude();
            stop.longitude = pb_stop.longitude();

            // формируем остановки каталога
            catalogue.AddStop(stop.name, stop.latitude, stop.longitude);
        }
        // формируем остановки каталога

        for (const auto& pb_stop : transport_catalogue_proto.distance()) {
            auto tmp_from = catalogue.FindStop(pb_stop.from());
            auto tmp_to = catalogue.FindStop(pb_stop.to());

            catalogue.AddDistance(*tmp_from, *tmp_to, pb_stop.dist()); // формируем расстояния
        }

        for (const auto& pb_bus : transport_catalogue_proto.buses()) {
            std::vector<const domain::Stop *> route;

            for (const auto& stop : pb_bus.route()) {
                route.emplace_back(catalogue.FindStop(stop));
            }
            catalogue.AddBus(pb_bus.bus_name(), route, pb_bus.is_route_round()); //формируем маршруты каталога
        }
        return std::move(catalogue);
    }

    graph::Edge<double> DeserializeEdge(const graph_proto::Edge &proto_edge)
    {
        graph::Edge<double> edge;
        edge.from = proto_edge.from();
        edge.to = proto_edge.to();
        edge.weight = std::move(proto_edge.weight());

        return std::move(edge);
    }
    std::vector<graph::EdgeId> DeserializeIncidenceList(const graph_proto::IncidenceList &incidence_list_serialized)
    {
        std::vector<graph::EdgeId> list;
        list.reserve(incidence_list_serialized.edge_id_size());

        for(size_t i = 0; i < incidence_list_serialized.edge_id_size(); ++i) {
            list.push_back(incidence_list_serialized.edge_id(i));
        }

        return std::move(list);
    }
    graph::DirectedWeightedGraph<double> DeserializeGraph(graph_proto::DirectedWeightedGraph serial_graph)
    {
        std::vector<std::vector<graph::EdgeId>> incidence_lists;
        incidence_lists.reserve(serial_graph.incidence_list_size());
        for(size_t i = 0; i < serial_graph.incidence_list_size(); ++i) {
            incidence_lists.emplace_back(std::move(DeserializeIncidenceList(serial_graph.incidence_list(i))));
        }

        std::vector<graph::Edge<double>> edges;
        edges.reserve(serial_graph.edge_size());
        for(size_t i = 0; i < serial_graph.edge_size(); ++i) {
            edges.emplace_back(std::move(DeserializeEdge(serial_graph.edge(i))));
        }

        return graph::DirectedWeightedGraph<double>(edges, incidence_lists);
    }

    transport_router::TransportRouter DeserializeRoute(const catalogue_serialize::FullBase& base_proto, const Transport::Catalogue& catalogue) {
        transport_router::TransportRouter router(catalogue);
        router_proto::RouteData routeData_proto = base_proto.router();

        std::unordered_map<const domain::Stop*, int> stop_to_id_transit_;
        for (const router_proto::StopsToId& stopsToId : routeData_proto.stops_to_id_transit()) {
            const domain::Stop* stp_ptr = catalogue.FindStop(stopsToId.stp_bus());
            stop_to_id_transit_[stp_ptr] = stopsToId.stop_id();
        }

        std::map<graph::EdgeId, domain::ResponsRoute> edge_to_info_;
        for (const router_proto::ResponsResult& result : routeData_proto.edge_to_info()) {
            domain::ResponsRoute responsRoute;
            auto type = result.data().type();
            if (router_proto::TYPE_WAIT == type) {
                responsRoute.type = domain::ResponsType::Wait;
            } else if (router_proto::TYPE_BUS == type) {
                responsRoute.type = domain::ResponsType::Bus;
            }
            responsRoute.name_ = std::move(static_cast<std::string>(result.data().name()));
            if (result.data().span_count() > 0) {
                responsRoute.span_count_ = result.data().span_count();
            } else {
                responsRoute.span_count_ = {};
            }
            responsRoute.time = result.data().time();

            edge_to_info_[result.r_id()] = std::move(responsRoute);
        }

        router.SetEdgeIdToInfo(edge_to_info_);
        router.SetStopToId(stop_to_id_transit_);

        return std::move(router);
    }
}