#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string>

#include "transport_catalogue.h"

namespace Catalogue {
    namespace Reader {
        enum class Type {
            Bus,
            Stop
        };

        enum class Path {
            Cycle,
            Seq
        };

        struct Request {
            Type type;
            std::string request;
        };

        std::vector<std::string> SplitIntoWords(std::string_view text);

        std::pair<Path, std::vector<std::string>> SplitIntoWordsRoute(std::string_view text);

        std::vector<std::string> SplitIntoWordsStop(std::string_view text);

        std::string FindName(std::string_view vector_word);

        int ReadLineWithNumber();

        std::vector<Request> ReadData();

        std::unordered_map<std::string, Path> ParseRequestStrings(Transport::Catalogue &catalogue);

        std::vector<const Transport::Stop *>
        ParseRoute(Transport::Catalogue &catalogue, const std::vector<std::string> &request);

        std::unordered_map<std::string, int> ParseDistance(std::string_view request);

        std::ostream &operator<<(std::ostream &os, const Type &tp);

    }
}