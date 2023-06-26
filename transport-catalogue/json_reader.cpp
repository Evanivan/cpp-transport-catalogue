#include "json_reader.h"


namespace json_reader {
    using namespace json;
    using namespace std;
    using namespace req_handler;

    json::Document LoadJSON(const std::string &s) {
        std::istringstream strm(s);
        return json::Load(strm);
    }

    std::string Print(const Node &node) {
        std::ostringstream out;
        Print(Document{node}, out);
        return out.str();
    }

    StopsNBuses ReadBase(const json::Array& base_input) {
        std::deque<BaseRequestTypeStop> deque_of_stops;
        std::deque<BaseRequestTypeBus> deque_of_buses;

        for (const auto& el : base_input) {
            if (el.AsMap().at("type"s) == "Stop"s) {
                BaseRequestTypeStop stop{};
                stringstream strm{};
                stringstream strm_2{};

                stop.name = el.AsMap().at("name"s).AsString();
                auto lat = el.AsMap().at("latitude"s).AsDouble();
                auto longt = el.AsMap().at("longitude"s).AsDouble();
                stop.latitude = lat;
                stop.longitude = longt;

                for (const auto &[stp_name, distance]: el.AsMap().at("road_distances"s).AsMap()) {
                    stop.road_dist[stp_name] = distance.AsInt();
                }
                deque_of_stops.emplace_back(std::move(stop));
            } else {
                continue;
            }
        }

        for (const auto& el : base_input) {
            if (el.AsMap().at("type"s) == "Bus"s) {
                BaseRequestTypeBus bus;
                bus.type = RequestType::Bus;
                bus.name = el.AsMap().at("name"s).AsString();
                for (const auto& stp : el.AsMap().at("stops"s).AsArray()) {
                    bus.stops_.push_back(stp.AsString());
                }
                bus.is_roundtrip = el.AsMap().at("is_roundtrip"s).AsBool();
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
                if (el.AsMap().at("id").IsInt()) {
                    req.id = el.AsMap().at("id").AsInt();
                }
                if (el.AsMap().at("type").AsString() == "Map") {
                    req.type = RequestType::Map;
                } else if (el.AsMap().at("id").IsInt() && el.AsMap().at("name").IsString()) {
                    req.name = el.AsMap().at("name").AsString();
                }
                if (el.AsMap().at("type").AsString() == "Stop") {
                    req.type = RequestType::Stop;
                } else if (el.AsMap().at("type").AsString() == "Bus") {
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
                if (type.second.IsString()) {
                    map_set.underlayer_color = type.second.AsString();
                }
                if (type.second.IsArray()) {
                    if (type.second.AsArray().size() == 4) {
                        map_set.underlayer_color = svg::Rgba(
                                type.second.AsArray()[0].AsInt(),
                                type.second.AsArray()[1].AsInt(),
                                type.second.AsArray()[2].AsInt(),
                                type.second.AsArray()[3].AsDouble()
                        );
                    } else if (type.second.AsArray().size() == 3) {
                        map_set.underlayer_color = svg::Rgb(
                                type.second.AsArray()[0].AsInt(),
                                type.second.AsArray()[1].AsInt(),
                                type.second.AsArray()[2].AsInt()
                        );
                    }
                }
            } else if (type.first == "underlayer_width"s) {
                map_set.underlayer_width = type.second.AsDouble();
            } else if (type.first == "color_palette"s) {
                for (const auto& el : type.second.AsArray()) {
                    if (el.IsString()) {
                        map_set.color_palette.emplace_back(el.AsString());
                    }
                    if (el.IsArray()) {
                        if (el.AsArray().size() == 4) {
                            map_set.color_palette.emplace_back(svg::Rgba (
                                    el.AsArray()[0].AsInt(),
                                    el.AsArray()[1].AsInt(),
                                    el.AsArray()[2].AsInt(),
                                    el.AsArray()[3].AsDouble()
                            ));
                        } else if (el.AsArray().size() == 3) {
                            map_set.color_palette.emplace_back(svg::Rgb (
                                    el.AsArray()[0].AsInt(),
                                    el.AsArray()[1].AsInt(),
                                    el.AsArray()[2].AsInt()
                            ));
                        }
                    }
                }
            }
        }

        return map_set;
    }

    void GetJson::ReadJSON() {
        auto input_json = json::Load(std::cin).GetRoot().AsMap();
        try {
            const auto base_input = input_json.at("base_requests").AsArray();
            const auto stat_reqs = input_json.at("stat_requests").AsArray();
            const auto map_settings = input_json.at("render_settings").AsMap();
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
            std::vector<const Stop*> route;
            for (const auto& stop_ptr : bus.stops_) {
                route.emplace_back(catalogue_.FindStop(stop_ptr));
            }
            catalogue_.AddBus(bus.name, route, bus.is_roundtrip);
        }
    }

    set<std::string> BusesInRoute(const unordered_set<const Bus*>& buses_struct) {
        set<std::string> buses;
        for (auto el : buses_struct) {
            auto el_ptr = *el;
            buses.emplace(std::move(el_ptr.bus_name_));
        }
        return buses;
    }

    RouteInfo CollectStatRoute(const Transport::Catalogue& catalogue, const std::optional<Bus>& bus, int id) {
        double curvation = 0.0;
        int dist = 0;
        int unique_stps = 0;
        int count_stops = 0;
        if (bus.has_value()) {
            unique_stps = domain_json::Stops_Uniq(bus->bus_route_);
            curvation = domain_json::CurvationCalc(catalogue, bus.value());
            if (bus->is_route_round) {
                dist = domain_json::DistanceRouteCycle(catalogue, bus.value());
                count_stops = static_cast<int>(bus.value().bus_route_.size());
            } else if (!bus->is_route_round) {
                dist = domain_json::DistanceRouteSeq(catalogue, bus.value());
                count_stops = static_cast<int>(bus.value().bus_route_.size() * 2 - 1);
            }
        }
        return {curvation, id, dist, count_stops, unique_stps};
    }

    Array BuildJSON(BuildBase base, const std::vector<StatReqs>& json_stats, req_handler::RequestHandler handler){
        json::Dict dict;
        json::Array arr;

        for (const auto& el : json_stats) {
            std::set<std::string> buses_set;
            json::Array arr_busses;

            if (el.type == RequestType::Stop) {
                try {
                    auto route = handler.GetBusesByStop(el.name);
                    if (route.empty()) {
                        arr.emplace_back(Dict{{"buses"s, arr_busses},
                                              {"request_id", el.id}});
                        continue;
                    }
                    const set<std::string>& buses = BusesInRoute(route);
                    for (const auto& bus : buses) {
                        buses_set.emplace(bus);
                    }
                } catch (...) {
                    string str = "not found"s;
                    arr.emplace_back(Dict{{"request_id", el.id}, {"error_message", str}});
                    continue;
                }

                for (const auto& el : buses_set) {
                    arr_busses.emplace_back(el);
                }
                arr.emplace_back(Dict{{"buses"s, arr_busses}, {"request_id", el.id}});
                continue;
            }
            if (el.type == RequestType::Bus) {
                try {
                    auto route = handler.GetBusStat(el.name);
                    if (route.value().bus_name_.empty() || !route.has_value()) {
                        string str = "not found"s;
                        arr.emplace_back(Dict{{"request_id",    el.id},
                                              {"error_message", str}});
                        continue;
                    }
                    const auto& buse_info = CollectStatRoute(base.GetCatalogue(), route, el.id);
                    arr.emplace_back(Dict{{"curvature", buse_info.route_length / buse_info.curvature},
                                          {"request_id", buse_info.request_id},
                                          {"route_length",buse_info.route_length},
                                          {"stop_count", buse_info.stop_count},
                                          {"unique_stop_count", buse_info.unique_stop_count}});
                } catch (...) {
                    string str = "not found"s;
                    arr.emplace_back(Dict{{"request_id",    el.id},
                                          {"error_message", str}});
                    continue;
                }
                continue;
            }

            if (el.type == RequestType::Map) {
                std::ostringstream map_os;
                const auto& doc = handler.RenderMap();

                doc.Render(map_os);
                arr.emplace_back(Dict{{"request_id", el.id},
                                      {"map", map_os.str()}});
                continue;
            }
        }
        return arr;
    }

    ostream& operator<<(ostream& out, const BaseRequestTypeBus& bus_request) {
        out << endl << "Type: "s << "BUS" << endl;
        out << "Name: "s << bus_request.name << endl;
        out << "Stops:"s;
        for (const auto& el :bus_request.stops_) {
            out << " "s << el << ",";
        }
        out << endl << "Is_roundtrip: " << boolalpha << bus_request.is_roundtrip << endl;

        return out;
    }
    ostream& operator<<(ostream& out, const BaseRequestTypeStop& stop_request) {
        out << endl << "Type: "s << "STOP" << endl;
        out << "Name: "s << stop_request.name << endl;
        out << "latitude: "s << stop_request.latitude << endl;
        out << "longitude: "s << stop_request.longitude << endl;

        out << "road_distances:"s;
        for (const auto& el : stop_request.road_dist) {
            out << " "s << "{"s <<  el.first << "\": "s << el.second << "}"s;
        }
        out << endl;

        return out;
    }
}