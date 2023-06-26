#include "map_renderer.h"

/*
 * В этом файле вы можете разместить код, отвечающий за визуализацию карты маршрутов в формате SVG.
 * Визуализация маршрутов вам понадобится во второй части итогового проекта.
 * Пока можете оставить файл пустым.
 */

namespace renderer {
    svg::Polyline CreateTrajectory(const std::vector<svg::Point>& points) {
        using namespace svg;
        Polyline polyline;
        std::vector<svg::Point> tmp_points;

        for (const auto& p : points) {
            polyline.AddPoint(p);
        }

        return polyline;
    }

    bool IsZero(double value) {
        return std::abs(value) < EPSILON;
    }

    void MapRenderer::SetProjCoords(std::vector<geo::Coordinates> points) {
        std::vector<svg::Point> tmp_points;
        points_.clear();
        const SphereProjector proj{
                points.begin(), points.end(),
                settings_.width,
                settings_.height,
                settings_.padding};

        // Проецируем и выводим координаты
        for (const auto geo_coord: points) {
            const svg::Point screen_coord = proj(geo_coord);
            tmp_points.emplace_back(screen_coord);
        }
        points_ = tmp_points;
    }

    std::vector<svg::Color> MapRenderer::GetColorPallet() const {
        return settings_.color_palette;
    }

    const MapSettings& MapRenderer::GetSettings() const {
        return settings_;
    }

    const std::vector<svg::Point>& MapRenderer::GetProjectedCoords() const {
        return points_;
    }
}