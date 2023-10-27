#pragma once

#include <map>
#include <memory>

#include "router.h"
#include "transport_catalogue.h"
#include "json.h"

namespace transport_router {
    class TransportRouter {
    public:
        TransportRouter(const Transport::Catalogue& catalogue, const domain::RouteSettings settings);

        graph::DirectedWeightedGraph<double>& GetGraph();
        [[nodiscard]] const std::unordered_map<const domain::Stop *, int> &GetIds() const;
        [[nodiscard]] const std::unordered_map<size_t, domain::StopInBus> &GetStopsToId() const;
        [[nodiscard]] std::pair<const graph::Edge<double>&, const domain::ResponsRoute&> GetFullEdgeInfo(graph::EdgeId edge_id) const;
        [[nodiscard]] std::optional<graph::Router<double>::RouteInfo> BuildRoute(std::optional<std::string> from, std::optional<std::string> to)const;
        void InitRoute();
        [[nodiscard]] const Transport::Catalogue& GetCatalogue() const;
    private:

        void BuildGraph();
        const Transport::Catalogue& db_;
        domain::RouteSettings route_settings_{};
        graph::DirectedWeightedGraph<double> graph_;
        std::unique_ptr<graph::Router<double>> router_;
        std::unordered_map<const domain::Stop*, int> stop_to_id_transit;
        std::unordered_map<size_t, domain::StopInBus> id_to_stop_transit;
        std::map<graph::EdgeId, domain::ResponsRoute> edge_to_info_;
    };
}