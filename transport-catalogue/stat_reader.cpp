#include "stat_reader.h"

using namespace std::string_literals;

namespace Catalogue {
    namespace Stat {
        using namespace Catalogue::Reader;

        std::string FindNameStat(std::string_view vector_word) {
            vector_word.remove_prefix(std::min(vector_word.size(), vector_word.find_first_of(" "s) + 1));

            return std::string(vector_word.substr(0, vector_word.find_first_of('\n')));
        }

        std::vector<Input> ReadDataInput() {
            int count_input = ReadLineWithNumber();
            std::vector<Input> result;
            Input el;
            std::string line;

            while (count_input + 1 > 0) {
                getline(std::cin, line);

                if (!line.empty()) {
                    if (line[0] == 'B') {
                        result.push_back({TypeInput::Bus, FindNameStat(line)});
                    }
                    if (line[0] == 'S') {
                        result.push_back({TypeInput::Stop, FindNameStat(line)});
                    }
                }
                --count_input;
            }

            return result;
        }

        void FindAllStopsRequired(Transport::Catalogue &catalogue, const std::string_view stop) {
            std::cout << "Stop "s << stop << ":";
            try {
                const auto el_to_prirnt = catalogue.GetStopInfo(std::string(stop));
                if (el_to_prirnt.empty()) {
                    std::cout << " no buses\n"s;
                    return;
                }
                std::cout << " buses"s;
                for (const std::string &srt: el_to_prirnt) {
                    std::cout << " " << srt;
                }
                std::cout << std::endl;
            } catch (std::out_of_range &) {
                std::cout << " not found\n"s;
            }
        }

        int DistanceRouteCycle(const Transport::Catalogue &catalogue, const Transport::Bus &bus) {
            int sum = 0;
            for (int i = 0; i != bus.bus_route_.size() - 1; ++i) {
                if (catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name) == 0) {
                    sum += catalogue.GetDistance(bus.bus_route_[i + 1]->stop_name, bus.bus_route_[i]->stop_name);
                    continue;
                }
                sum += catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name);
            }
            return sum;
        }

        int DistanceRouteSeq(const Transport::Catalogue &catalogue, const Transport::Bus &bus) {
            std::vector<const Transport::Stop *> route_reverse(bus.bus_route_.size());
            std::reverse_copy(bus.bus_route_.begin(), bus.bus_route_.end(), route_reverse.begin());
            int sum = 0;
            for (int i = 0; i != bus.bus_route_.size() - 1; ++i) {
                if (catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name) == 0) {
                    sum += catalogue.GetDistance(bus.bus_route_[i + 1]->stop_name, bus.bus_route_[i]->stop_name);
                    continue;
                }
                sum += catalogue.GetDistance(bus.bus_route_[i]->stop_name, bus.bus_route_[i + 1]->stop_name);
            }

            for (int i = 0; i != route_reverse.size() - 1; ++i) {
                if (catalogue.GetDistance(route_reverse[i]->stop_name, route_reverse[i + 1]->stop_name) == 0) {
                    sum += catalogue.GetDistance(route_reverse[i + 1]->stop_name, route_reverse[i]->stop_name);
                    continue;
                }
                sum += catalogue.GetDistance(route_reverse[i]->stop_name, route_reverse[i + 1]->stop_name);
            }

            return sum;
        }

        void StatOut(Transport::Catalogue &catalogue, const std::unordered_map<std::string, Path> &paths) {
            const auto requests = ReadDataInput();

            for (int j = 0; j < requests.size(); ++j) {
                double sum = 0;
                double dist = 0;
                try {
                    const auto &all_stops = *catalogue.GetBusInfo(requests[j].req_name_);
                    const auto &bus_name = catalogue.FindBus(requests[j].req_name_);

                    std::set<std::string> stops_unique;
                    if (requests[j].type_ == TypeInput::Stop) {
                        FindAllStopsRequired(catalogue, requests[j].req_name_);
                    } else {
                        if (bus_name) {
                            if (paths.at(bus_name->bus_name_) == Path::Seq)
                                dist += DistanceRouteSeq(catalogue, *bus_name);
                            if (paths.at(bus_name->bus_name_) == Path::Cycle)
                                dist += DistanceRouteCycle(catalogue, *bus_name);
                            for (int i = 0; i < all_stops.size() + 1; ++i) {
                                if (paths.at(bus_name->bus_name_) == Path::Seq) {
                                    stops_unique.insert(all_stops[i]->stop_name);
                                    if (i == all_stops.size() - 1) {
                                        stops_unique.insert(all_stops[all_stops.size() - 1]->stop_name);
                                        std::cout << "Bus "s << requests[j].req_name_ << ": "
                                                  << (all_stops.size() * 2) - 1;
                                        break;
                                    }
                                    sum += 2 * ComputeDistance({all_stops[i]->latitude, all_stops[i]->longitude},
                                                               {all_stops[i + 1]->latitude,
                                                                all_stops[i + 1]->longitude});
                                } else if (paths.at(bus_name->bus_name_) == Path::Cycle) {
                                    stops_unique.insert(all_stops[i]->stop_name);
                                    if (i == all_stops.size() - 1) {
                                        stops_unique.insert(all_stops[all_stops.size() - 1]->stop_name);
                                        std::cout << "Bus "s << requests[j].req_name_ << ": " << all_stops.size();
                                        break;
                                    }
                                    sum += ComputeDistance({all_stops[i]->latitude, all_stops[i]->longitude},
                                                           {all_stops[i + 1]->latitude, all_stops[i + 1]->longitude});
                                }
                            }
                            std::cout << " stops on route, "s << stops_unique.size() << " unique stops, "s
                                      << std::defaultfloat << dist;
                            std::cout << " route length, "s << std::defaultfloat << dist / sum << " curvature\n"s;
                        } else {
                            throw std::out_of_range("");
                        }
                    }
                } catch (std::out_of_range &) {
                    std::cout << "Bus "s << requests[j].req_name_ << ": not found\n"s;
                }
            }
        }
    }
}