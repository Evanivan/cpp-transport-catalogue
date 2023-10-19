#pragma once

#include <map>
#include <memory>

#include "router.h"
#include "transport_catalogue.h"
#include "json.h"
//#include "json_reader.h"

namespace transport_router {
    using StopsNB = domain::StopsNBuses;
    class TransportRouter {
    public:
        TransportRouter(Transport::Catalogue catalogue, const domain::RouteSettings settings,
                        const StopsNB& buses);

        graph::DirectedWeightedGraph<double>& GetGraph();
        [[nodiscard]] const std::unordered_map<const domain::Stop *, int> &GetIds() const;
        [[nodiscard]] const std::unordered_map<size_t, domain::StopInBus> &GetStopsToId() const;
        [[nodiscard]] std::pair<const graph::Edge<double>&, const domain::ResponsRoute&> GetFullEdgeInfo(graph::EdgeId edge_id) const;
        [[nodiscard]] std::optional<graph::Router<double>::RouteInfo> BuildRoute(const domain::StatReqs& request) const;
        void InitRoute();
        [[nodiscard]] const Transport::Catalogue& GetCatalogue() const;
    private:

        void BuildGraph();
        Transport::Catalogue db_{};
        domain::RouteSettings route_settings_{};
        graph::DirectedWeightedGraph<double> graph_;
        std::unique_ptr<graph::Router<double>> router_;
        std::unordered_map<const domain::Stop*, int> stop_to_id_transit;
        std::unordered_map<size_t, domain::StopInBus> id_to_stop_transit;
        std::map<graph::EdgeId, domain::ResponsRoute> edge_to_info_;
        const StopsNB& stp_n_base_stat_{};
    };
}