#include "json_reader.h"


namespace json_reader {
    using namespace json;
    using namespace std::literals;
    using namespace req_handler;

    int Stops_Uniq(const std::vector<const domain::Stop *>& stops) {
        std::set<std::string> stops_unique;
        for (auto el : stops) {
            stops_unique.insert(el->stop_name);
        }
        return static_cast<int>(stops_unique.size());
    }

    int DistanceRouteCycle(const Catalogue& catalogue, const domain::Bus& bus) {
        int sum = 0;
        for (size_t i = 0; i != bus.bus_route_.size() - 1; ++i) {
            if (catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name) == 0) {
                sum += catalogue.GetDistance(bus.bus_route_[i + 1]->stop_name, bus.bus_route_[i]->stop_name);
                continue;
            }
            sum += catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name);
        }
        return sum;
    }

    int DistanceRouteSeq(const Catalogue& catalogue, const domain::Bus& bus) {
        std::vector<const domain::Stop*> route_reverse(bus.bus_route_.size());
        std::reverse_copy(bus.bus_route_.begin(), bus.bus_route_.end(), route_reverse.begin());
        int sum = 0;
        for (size_t i = 0; i != bus.bus_route_.size() - 1; ++i) {
            if (catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name) == 0) {
                sum += catalogue.GetDistance(bus.bus_route_[i + 1]->stop_name, bus.bus_route_[i]->stop_name);
                continue;
            }
            sum += catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name);
        }

        for (size_t i = 0; i != route_reverse.size() - 1; ++i) {
            if (catalogue.GetDistance(route_reverse[i]->stop_name, route_reverse[i + 1]->stop_name) == 0) {
                sum += catalogue.GetDistance(route_reverse[i + 1]->stop_name, route_reverse[i]->stop_name);
                continue;
            }
            sum += catalogue.GetDistance(route_reverse[i]->stop_name, route_reverse[i + 1]->stop_name);
        }

        return sum;
    }

    double CurvationCalc(const Transport::Catalogue& catalogue, const domain::Bus& bus) {
        double sum = 0.0;
        const auto& all_stops = catalogue.GetBusInfo(bus.bus_name_);
        if (bus.is_route_round) {
            for (size_t i = 0; i < (*all_stops).size() + 1; ++i) {
                if (i == (*all_stops).size() - 1) {
                    break;
                }
                sum += geo::ComputeDistance({(*all_stops)[i]->latitude, (*all_stops)[i]->longitude},
                                       {(*all_stops)[i + 1]->latitude, (*all_stops)[i + 1]->longitude});
            }
        } else {
            for (size_t i = 0; i < (*all_stops).size() + 1; ++i) {
                if (i == (*all_stops).size() - 1) {
                    break;
                }
                sum += 2 * geo::ComputeDistance({(*all_stops)[i]->latitude, (*all_stops)[i]->longitude},
                                           {(*all_stops)[i + 1]->latitude, (*all_stops)[i + 1]->longitude});
            }
        }
        return sum;
    }

    json::Document LoadJSON(const std::string &s) {
        std::istringstream strm(s);
        return json::Load(strm);
    }

    std::string Print(const Node &node) {
        std::ostringstream out;
        Print(Document{node}, out);
        return out.str();
    }

    svg::Color ParseColor(const json::Node& type) {
        svg::Color tmp_color = {};
        if (type.IsString()) {
            tmp_color = type.AsString();
        }
        if (type.IsArray()) {
            if (type.AsArray().size() == 4) {
                tmp_color = svg::Rgba(
                        type.AsArray()[0].AsInt(),
                        type.AsArray()[1].AsInt(),
                        type.AsArray()[2].AsInt(),
                        type.AsArray()[3].AsDouble()
                );
            } else if (type.AsArray().size() == 3) {
                tmp_color = svg::Rgb(
                        type.AsArray()[0].AsInt(),
                        type.AsArray()[1].AsInt(),
                        type.AsArray()[2].AsInt()
                );
            }
        }
        return tmp_color;
    }

    StopsNBuses ReadBase(const json::Array& base_input) {
        std::deque<BaseRequestTypeStop> deque_of_stops;
        std::deque<BaseRequestTypeBus> deque_of_buses;

        for (const auto& el : base_input) {
            if (el.AsDict().at("type"s) == "Stop"s) {
                BaseRequestTypeStop stop{};
                std::stringstream strm{};
                std::stringstream strm_2{};

                stop.name = el.AsDict().at("name"s).AsString();
                auto lat = el.AsDict().at("latitude"s).AsDouble();
                auto longt = el.AsDict().at("longitude"s).AsDouble();
                stop.latitude = lat;
                stop.longitude = longt;

                for (const auto &[stp_name, distance]: el.AsDict().at("road_distances"s).AsDict()) {
                    stop.road_dist[stp_name] = distance.AsInt();
                }
                deque_of_stops.emplace_back(std::move(stop));
            } else {
                continue;
            }
        }

        for (const auto& el : base_input) {
            if (el.AsDict().at("type"s) == "Bus"s) {
                BaseRequestTypeBus bus;
                bus.type = RequestType::Bus;
                bus.name = el.AsDict().at("name"s).AsString();
                for (const auto& stp : el.AsDict().at("stops"s).AsArray()) {
                    bus.stops_.push_back(stp.AsString());
                }
                bus.is_roundtrip = el.AsDict().at("is_roundtrip"s).AsBool();
                deque_of_buses.emplace_back(std::move(bus));
            } else {
                continue;
            }
        }
        return StopsNBuses{deque_of_stops, deque_of_buses};
    }

    std::vector<StatReqs> ReadStat(const json::Array& reqs) {
        std::vector<StatReqs> processed_reqs;
        for (const auto& el : reqs) {
            StatReqs req = {};
            try {
                if (el.AsDict().at("id").IsInt()) {
                    req.id = el.AsDict().at("id").AsInt();
                }
                if (el.AsDict().at("type").AsString() == "Map") {
                    req.type = RequestType::Map;
                } else if (el.AsDict().at("id").IsInt() && el.AsDict().at("name").IsString()) {
                    req.name = el.AsDict().at("name").AsString();
                }
                if (el.AsDict().at("type").AsString() == "Stop") {
                    req.type = RequestType::Stop;
                } else if (el.AsDict().at("type").AsString() == "Bus") {
                    req.type = RequestType::Bus;
                }
                processed_reqs.push_back(std::move(req));
            } catch (...) {
                continue;
            }
        }
        return processed_reqs;
    }

    renderer::MapSettings ReadMapSettings(const json::Dict & req) {
        renderer::MapSettings map_set;

        for (const auto& type : req) {
            if (type.first == "width"s) {
                map_set.width = type.second.AsDouble();
            } else if (type.first == "height"s) {
                map_set.height = type.second.AsDouble();
            } else if (type.first == "padding"s) {
                map_set.padding = type.second.AsDouble();
            } else if (type.first == "stop_radius"s) {
                map_set.stop_radius = type.second.AsDouble();
            } else if (type.first == "line_width"s) {
                map_set.line_width = type.second.AsDouble();
            } else if (type.first == "bus_label_font_size"s) {
                map_set.bus_label_font_size = type.second.AsInt();
            } else if (type.first == "bus_label_offset"s) {
                map_set.bus_label_offset.first = type.second.AsArray()[0].AsDouble();
                map_set.bus_label_offset.second = type.second.AsArray()[1].AsDouble();
            } else if (type.first == "stop_label_font_size"s) {
                map_set.stop_label_font_size = type.second.AsInt();
            } else if (type.first == "stop_label_offset"s) {
                map_set.stop_label_offset.first = type.second.AsArray()[0].AsDouble();
                map_set.stop_label_offset.second = type.second.AsArray()[1].AsDouble();
            } else if (type.first == "underlayer_color"s) {
                map_set.underlayer_color = ParseColor(type.second);
            } else if (type.first == "underlayer_width"s) {
                map_set.underlayer_width = type.second.AsDouble();
            } else if (type.first == "color_palette"s) {
                for (const auto& el : type.second.AsArray()) {
                    map_set.color_palette.emplace_back(ParseColor(el));
                }
            }
        }

        return map_set;
    }

    void GetJson::ReadJSON() {
        auto input_json = json::Load(std::cin).GetRoot().AsDict();
        try {
            const auto base_input = input_json.at("base_requests").AsArray();
            const auto stat_reqs = input_json.at("stat_requests").AsArray();
            const auto map_settings = input_json.at("render_settings").AsDict();
            stp_n_buses = ReadBase(base_input);
            stats = ReadStat(stat_reqs);
            map_settings_ = ReadMapSettings(map_settings);
        } catch (...) {
        }
    }

    const StopsNBuses& GetJson::GetStopsNBuses() const {
        return stp_n_buses;
    }

    const std::vector<StatReqs>& GetJson::GetStats() const {
        return stats;
    }

    const renderer::MapSettings& GetJson::GetMapSettings() const {
        return map_settings_;
    }

    Transport::Catalogue& BuildBase::GetCatalogue() const {
        return catalogue_;
    }

    void BuildBase::BuildStops() {
        for (const auto& el : stp_n_base_stat_.deque_of_stops) {
            catalogue_.AddStop(el.name, el.latitude, el.longitude);
        }

        for (const auto& el : stp_n_base_stat_.deque_of_stops) {
            for (const auto& [stop_name, dist]  : el.road_dist) {
                catalogue_.AddDistance(*catalogue_.FindStop(el.name),
                                       *catalogue_.FindStop(stop_name),
                                       dist);
            }
        }
    }

    void BuildBase::BuildBuses() {
        for (const auto& bus : stp_n_base_stat_.deque_of_buses) {
            std::vector<const domain::Stop*> route;
            for (const auto& stop_ptr : bus.stops_) {
                route.emplace_back(catalogue_.FindStop(stop_ptr));
            }
            catalogue_.AddBus(bus.name, route, bus.is_roundtrip);
        }
    }

    std::set<std::string> BusesInRoute(const std::unordered_set<const domain::Bus*>& buses_struct) {
        std::set<std::string> buses;
        for (auto el : buses_struct) {
            auto el_ptr = *el;
            buses.emplace(std::move(el_ptr.bus_name_));
        }
        return buses;
    }

    RouteInfo CollectStatRoute(const Transport::Catalogue& catalogue, const std::optional<domain::Bus>& bus, int id) {
        double curvation = 0.0;
        int dist = 0;
        int unique_stps = 0;
        int count_stops = 0;
        if (bus.has_value()) {
            unique_stps = Stops_Uniq(bus->bus_route_);
            curvation = CurvationCalc(catalogue, bus.value());
            if (bus->is_route_round) {
                dist = DistanceRouteCycle(catalogue, bus.value());
                count_stops = static_cast<int>(bus.value().bus_route_.size());
            } else if (!bus->is_route_round) {
                dist = DistanceRouteSeq(catalogue, bus.value());
                count_stops = static_cast<int>(bus.value().bus_route_.size() * 2 - 1);
            }
        }
        return {curvation, id, dist, count_stops, unique_stps};
    }

    //implement new builder
    Array BuildJSON(BuildBase base, const std::vector<StatReqs>& json_stats, req_handler::RequestHandler handler){
//        json::Dict dict;
//        json::Array arr;
        json::Builder builder;
        builder.StartArray();

        for (const auto& el : json_stats) {
            std::set<std::string> buses_set;
            json::Array arr_busses;

            if (el.type == RequestType::Stop) {
                try {
                    auto route = handler.GetBusesByStop(el.name);
                    if (route.empty()) {
                        builder.StartDict()
                            .Key("buses"s).Value(arr_busses)
                            .Key("request_id").Value(el.id).EndDict();

//                        arr.emplace_back(Dict{{"buses"s, arr_busses},
//                                              {"request_id", el.id}});
                        continue;
                    }
                    const std::set<std::string>& buses = BusesInRoute(route);
                    for (const auto& bus : buses) {
                        buses_set.emplace(bus);
                    }
                } catch (...) {
                    std::string str = "not found"s;
                    builder.StartDict()
                            .Key("request_id").Value(el.id)
                            .Key("error_message").Value(str).EndDict();
//                    arr.emplace_back(Dict{{"request_id", el.id}, {"error_message", str}});
                    continue;
                }

                for (const auto& el : buses_set) {
                    arr_busses.emplace_back(el);
                }
                builder.StartDict()
                        .Key("buses"s).Value(arr_busses)
                        .Key("request_id").Value(el.id)
                        .EndDict();
//                arr.emplace_back(Dict{{"buses"s, arr_busses}, {"request_id", el.id}});
                continue;
            }
            if (el.type == RequestType::Bus) {
                try {
                    auto route = handler.GetBusStat(el.name);
                    if (route.value().bus_name_.empty() || !route.has_value()) {
                        std::string str = "not found"s;
                        builder.StartDict()
                                .Key("request_id").Value(el.id)
                                .Key("error_message").Value(str).EndDict();
//                        arr.emplace_back(Dict{{"request_id",    el.id},
//                                              {"error_message", str}});
                        continue;
                    }
                    const auto& buse_info = CollectStatRoute(base.GetCatalogue(), route, el.id);
                    builder.StartDict()
                            .Key("curvature").Value(buse_info.route_length / buse_info.curvature)
                            .Key("request_id").Value(buse_info.request_id)
                            .Key("route_length").Value(buse_info.route_length)
                            .Key("stop_count").Value(buse_info.stop_count)
                            .Key("unique_stop_count").Value(buse_info.unique_stop_count).EndDict();

//                    arr.emplace_back(Dict{{"curvature", buse_info.route_length / buse_info.curvature},
//                                          {"request_id", buse_info.request_id},
//                                          {"route_length",buse_info.route_length},
//                                          {"stop_count", buse_info.stop_count},
//                                          {"unique_stop_count", buse_info.unique_stop_count}});
                } catch (...) {
                    std::string str = "not found"s;
                    builder.StartDict()
                            .Key("request_id").Value(el.id)
                            .Key("error_message").Value(str).EndDict();
//                    arr.emplace_back(Dict{{"request_id",    el.id},
//                                          {"error_message", str}});
                    continue;
                }
                continue;
            }

            if (el.type == RequestType::Map) {
                std::ostringstream map_os;
                const auto& doc = handler.RenderMap();

                doc.Render(map_os);
                builder.StartDict()
                        .Key("request_id").Value(el.id)
                        .Key("map").Value(map_os.str()).EndDict();
//                arr.emplace_back(Dict{{"request_id", el.id},
//                                      {"map", map_os.str()}});
                continue;
            }
        }
        builder.EndArray();
        return builder.Build().AsArray();
    }

    std::ostream& operator<<(std::ostream& out, const BaseRequestTypeBus& bus_request) {
        out << std::endl << "Type: "s << "BUS" << std::endl;
        out << "Name: "s << bus_request.name << std::endl;
        out << "Stops:"s;
        for (const auto& el :bus_request.stops_) {
            out << " "s << el << ",";
        }
        out << std::endl << "Is_roundtrip: " << std::boolalpha << bus_request.is_roundtrip << std::endl;

        return out;
    }
    std::ostream& operator<<(std::ostream& out, const BaseRequestTypeStop& stop_request) {
        out << std::endl << "Type: "s << "STOP" << std::endl;
        out << "Name: "s << stop_request.name <<std:: endl;
        out << "latitude: "s << stop_request.latitude << std::endl;
        out << "longitude: "s << stop_request.longitude << std::endl;

        out << "road_distances:"s;
        for (const auto& el : stop_request.road_dist) {
            out << " "s << "{"s <<  el.first << "\": "s << el.second << "}"s;
        }
        out << std::endl;

        return out;
    }
}