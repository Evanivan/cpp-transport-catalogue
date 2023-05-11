#include "input_reader.h"


namespace Input {
    using namespace std::string_literals;
    using namespace Transport;

    int ReadLineWithNumber() {
        int result;
        std::cin >> result;
        return result;
    }

    bool IsValidStopName(std::string_view str) {
        bool out = false;
        for (const char &c: str) {
            if (std::isspace(c) || std::isalnum(c)) out = true;
        }
        return out;
    }

    std::vector<std::string> SplitIntoWordsStop(std::string_view text) {
        std::vector<std::string> words;
        text.remove_prefix(std::min(text.size(), text.find_first_of(":"s) + 2));
        const int64_t pos_end = text.npos;

        while (!text.empty()) {
            int64_t space = text.find(' ');
            words.push_back(
                    space == pos_end ? std::string(text.substr(0, text.npos)) : std::string(text.substr(0, space)));
            text.remove_prefix(std::min(text.find_first_not_of(" "s, space), text.size()));
        }

        return words;
    }


    std::vector<std::string> SplitIntoWords(std::string_view text) {
        std::vector<std::string> words;
        text.remove_prefix(std::min(text.size(), text.find_first_of(":"s) + 2));
        const int64_t pos_end = text.npos;

        while (!text.empty()) {
            text.remove_prefix(text.find_first_of(" "s) + 1);
            int64_t space = text.find(' ');
            words.push_back(space == pos_end ? std::string(text.substr(0, text.npos)) : std::string(
                    text.substr(0, text.find_first_of(" "s))));
            text.remove_prefix(std::min(text.find_first_of(" "s), text.size()));
        }

        return words;
    }

    std::pair<Path, std::vector<std::string>> SplitIntoWordsRoute(std::string_view text) {
        std::pair<Path, std::vector<std::string>> paths_splitted;
        std::vector<std::string> words;
        text.remove_prefix(std::min(text.size(), text.find_first_of(":"s) + 2));
        const int64_t pos_end = text.npos;

        if (text.find('-') != text.npos) {
            paths_splitted.first = Path::Seq;
            while (!text.empty()) {
                text.remove_prefix(std::min(text.size(), text.find_first_not_of(" "s)));
                int64_t symbol = text.find('-');
                words.push_back(symbol == pos_end ? std::string(text.substr(0, text.npos)) : std::string(
                        text.substr(0, symbol - 1)));
                text.remove_prefix(std::min(text.find_first_not_of("-"s, symbol), text.size()));
            }
        } else if (text.find('>') != text.npos) {
            paths_splitted.first = Path::Cycle;
            while (!text.empty()) {
                text.remove_prefix(std::min(text.size(), text.find_first_not_of(" "s)));
                int64_t symbol = text.find('>');
                words.push_back(
                        symbol == pos_end ? std::string(text.substr(0, text.npos)) : std::string(
                                text.substr(0, symbol - 1)));
                text.remove_prefix(std::min(text.find_first_not_of(">"s, symbol), text.size()));
            }
        }
        paths_splitted.second = words;

        return paths_splitted;
    }

    std::string FindName(std::string_view vector_word) {
        vector_word.remove_prefix(std::min(vector_word.size(), vector_word.find_first_of(" "s) + 1));

        return std::string(vector_word.substr(0, vector_word.find_first_of(':')));
    }

    std::pair<double, double> ParseCoords(const std::vector<std::string> &vector_word) {
        std::pair<double, double> coords;

        std::string s = vector_word[0];
        s.pop_back();
        coords.first = std::stod(s);

        std::string s_second = vector_word[1];
        s_second.pop_back();
        coords.second = std::stod(vector_word[1]);

        return coords;
    }

    std::vector<Request> ReadData() {
        int count_input = ReadLineWithNumber();
        std::vector<Request> all_requests;
        std::string line;
        std::vector<std::string> splitted;

        while (count_input + 1 > 0) {
            getline(std::cin, line);

            if (line[0] == 'B') all_requests.push_back({Type::Bus, line});
            if (line[0] == 'S') all_requests.push_back({Type::Stop, line});

            --count_input;
        }

        return all_requests;
    }

    std::vector<const Stop *> ParseRoute(Catalogue &catalogue, const std::vector<std::string> &request) {
        std::vector<const Stop *> route;

        for (const auto &stop_: request) {
            if (IsValidStopName(stop_)) {
                route.push_back(&catalogue.FindStop(stop_));
            }
        }

        return route;
    }

    std::unordered_map<std::string, int> ParseDistance(std::string_view request) {
        std::unordered_map<std::string, int> distances;
        std::vector<std::string> request_vector = SplitIntoWords(request);
        std::vector<std::string> reverse_vector(request_vector.size() - 1);

        std::move(request_vector.begin() + 1, request_vector.end(), reverse_vector.begin());

        std::string name_stop = ""s;
        int dist = 0;
        while (!reverse_vector.empty()) {
            if (std::count(reverse_vector[0].begin(), reverse_vector[0].end(), ',') == 1) {
                reverse_vector[0].pop_back();
                name_stop += reverse_vector[0];
                distances[name_stop] = dist;
                dist = 0;
                name_stop.clear();
                reverse_vector.erase(reverse_vector.begin());
                continue;
            } else if (std::next(reverse_vector.begin()) == reverse_vector.end()) {
                name_stop += reverse_vector[0];
                distances[name_stop] = dist;
                reverse_vector.clear();
                break;
            } else if (*std::next(reverse_vector.begin()) == "to"s) {
                reverse_vector[0].pop_back();
                dist = std::stoi(*reverse_vector.begin());
                reverse_vector.erase(reverse_vector.begin(), reverse_vector.begin() + 2);
                continue;
            } else {
                name_stop += *reverse_vector.begin() + " "s;
                reverse_vector.erase(reverse_vector.begin());
            }
        }

        return distances;
    }

    std::unordered_map<std::string, Path> ParseRequestStrings(Catalogue &catalogue) {
        const auto requests = ReadData();
        std::unordered_map<std::string, Path> paths;

        for (auto &req: requests) {
            if (req.type == Type::Stop) {
                const std::string name = FindName(req.request);
                const auto reqs_ = SplitIntoWordsStop(req.request);
                auto coords = ParseCoords(reqs_);

                if (IsValidStopName(name)) catalogue.AddStop(name, coords.first, coords.second);
            }
        }

        for (auto &req: requests) {
            if (req.type == Type::Stop) {
                const std::string name = FindName(req.request);
                const auto reqs_ = ParseDistance(req.request);
                for (const auto &[stp, distance]: reqs_) {
                    catalogue.AddDistance(catalogue.FindStop(name),
                                          catalogue.FindStop(stp),
                                          distance);
                }
            }
        }

        for (int i = 0; i < requests.size(); ++i) {
            if (requests[i].type == Type::Bus) {
                const std::string name = FindName(requests[i].request);

                const auto reqs_ = SplitIntoWordsRoute(requests[i].request);
                const auto route = ParseRoute(catalogue, reqs_.second);

                paths[name] = (reqs_.first);
                catalogue.AddBus(name, route);
            }
        }

        return paths;
    }

    std::ostream &operator<<(std::ostream &os, const Type &tp) {
        using namespace std;

        if (tp == Type::Bus) os << "Bus"s;
        if (tp == Type::Stop) os << "Stop"s;

        return os;
    }
}