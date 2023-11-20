#pragma once

#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <memory>

#include "transport_catalogue.h"
#include "map_renderer.h"
#include "domain.h"
#include "router.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * В качестве источника для идей предлагаем взглянуть на нашу версию обработчика запросов.
 * Вы можете реализовать обработку запросов способом, который удобнее вам.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

// Класс RequestHandler играет роль Фасада, упрощающего взаимодействие JSON reader-а
// с другими подсистемами приложения.
// См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)

namespace req_handler {
    struct BusCountInRoute {
        int count = 0;
        std::string bus_{};
    };

    int StopsInRange(const domain::Stop *start_, const domain::Stop *fin_, const domain::Bus *bus);

    class RequestHandler {
    public:
        // MapRenderer понадобится в следующей части итогового проекта
        RequestHandler(Transport::Catalogue &db, renderer::MapRenderer &renderer)
                : db_(db),
                  renderer_(renderer){
        }

        // Возвращает информацию о маршруте (запрос Bus)
        std::optional<domain::Bus> GetBusStat(const std::string_view &bus_name) const;

        // Возвращает маршруты, проходящие через
        std::unordered_set<const domain::Bus *> GetBusesByStop(const std::string_view &stop_name);

        svg::Document RenderMap() const;
        const Transport::Catalogue& GetCatalogue() const;
    private:
        // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
        Transport::Catalogue& db_;
        renderer::MapRenderer &renderer_;
    };
}