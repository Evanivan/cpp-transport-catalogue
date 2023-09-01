#pragma once

#include "svg.h"
#include "geo.h"
#include "domain.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>
#include <cmath>
#include <unordered_map>

/*
 * В этом файле вы можете разместить код, отвечающий за визуализацию карты маршрутов в формате SVG.
 * Визуализация маршрутов вам понадобится во второй части итогового проекта.
 * Пока можете оставить файл пустым.
 */

namespace renderer {
    template <typename DrawableIterator>
    void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
        for (auto it = begin; it != end; ++it) {
            (*it)->Draw(target);
        }
    }

    template <typename Container>
    void DrawPicture(const Container& container, svg::ObjectContainer& target) {
        using namespace std;
        DrawPicture(begin(container), end(container), target);
    }

    struct MapSettings {
        double width = 0.0;
        double height = 0.0;
        double padding = 0.0;
        double stop_radius = 0.0;
        double line_width = 0.0;
        int bus_label_font_size = 0;
        std::pair<double, double> bus_label_offset = {0.0, 0.0};
        int stop_label_font_size = 0;
        std::pair<double, double> stop_label_offset = {0.0, 0.0};
        svg::Color underlayer_color = {};
        double underlayer_width = 0.0;
        std::vector<svg::Color> color_palette = {};
    };

    inline const double EPSILON = 1e-6;

    bool IsZero(double value);

    class SphereProjector {
    public:
        // points_begin и points_end задают начало и конец интервала элементов geo::Coordinates
        template <typename PointInputIt>
        SphereProjector(PointInputIt points_begin, PointInputIt points_end,
                        double max_width, double max_height, double padding)
                : padding_(padding) //
        {
            // Если точки поверхности сферы не заданы, вычислять нечего
            if (points_begin == points_end) {
                return;
            }

            // Находим точки с минимальной и максимальной долготой
            const auto [left_it, right_it] = std::minmax_element(
                    points_begin, points_end,
                    [](auto lhs, auto rhs) { return lhs.lng < rhs.lng; });
            min_lon_ = left_it->lng;
            const double max_lon = right_it->lng;

            // Находим точки с минимальной и максимальной широтой
            const auto [bottom_it, top_it] = std::minmax_element(
                    points_begin, points_end,
                    [](auto lhs, auto rhs) { return lhs.lat < rhs.lat; });
            const double min_lat = bottom_it->lat;
            max_lat_ = top_it->lat;

            // Вычисляем коэффициент масштабирования вдоль координаты x
            std::optional<double> width_zoom;
            if (!IsZero(max_lon - min_lon_)) {
                width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
            }

            // Вычисляем коэффициент масштабирования вдоль координаты y
            std::optional<double> height_zoom;
            if (!IsZero(max_lat_ - min_lat)) {
                height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
            }

            if (width_zoom && height_zoom) {
                // Коэффициенты масштабирования по ширине и высоте ненулевые,
                // берём минимальный из них
                zoom_coeff_ = std::min(*width_zoom, *height_zoom);
            } else if (width_zoom) {
                // Коэффициент масштабирования по ширине ненулевой, используем его
                zoom_coeff_ = *width_zoom;
            } else if (height_zoom) {
                // Коэффициент масштабирования по высоте ненулевой, используем его
                zoom_coeff_ = *height_zoom;
            }
        }

        // Проецирует широту и долготу в координаты внутри SVG-изображения
        svg::Point operator()(geo::Coordinates coords) const {
            return {
                    (coords.lng - min_lon_) * zoom_coeff_ + padding_,
                    (max_lat_ - coords.lat) * zoom_coeff_ + padding_
            };
        }

    private:
        double padding_;
        double min_lon_ = 0;
        double max_lat_ = 0;
        double zoom_coeff_ = 0;
    };

    svg::Polyline CreateTrajectory(const std::vector<svg::Point>& points);

    namespace shapes {
        class RoutePath : public svg::Drawable {
        public:
            RoutePath() = default;
            RoutePath(const std::vector<svg::Point>& points, svg::Color color, MapSettings  settings)
                    : points_(points),
                      color_(std::move(color)),
                      settings_(std::move(settings)){
            }

            // Реализует метод Draw интерфейса svg::Drawable
            void Draw(svg::ObjectContainer &container) const override {
                auto lines = CreateTrajectory(points_);
                container.Add(lines.SetStrokeColor(color_)
                                      .SetFillColor("none")
                                      .SetStrokeWidth(settings_.line_width)
                                      .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                                      .SetStrokeLineCap(svg::StrokeLineCap::ROUND));
            }

        private:
            std::vector<svg::Point> points_;
            svg::Color color_;
            MapSettings settings_;
        };

        class BusPoints : public svg::Drawable {
        public:
            BusPoints(const std::vector<svg::Point>& points, MapSettings  settings)
                    : points_(points),
                      settings_(std::move(settings)){
            }

            // Реализует метод Draw интерфейса svg::Drawable
            void Draw(svg::ObjectContainer &container) const override {
                std::vector<svg::Point> tmp_vector = points_;

                for (const auto& p : points_) {
                    container.Add(svg::Circle()
                                          .SetCenter({p.x, p.y})
                                          .SetRadius(settings_.stop_radius)
                                          .SetFillColor("white"));

                }
            }

        private:
            std::vector<svg::Point> points_;
            MapSettings settings_;
        };
    }

    class MapRenderer {
    public:
        explicit MapRenderer(MapSettings& settings)
                : settings_(std::move(settings)) {
        }

        void SetProjCoords(std::vector<geo::Coordinates> points);
        [[nodiscard]] std::vector<svg::Color> GetColorPallet() const;
        [[nodiscard]] const MapSettings& GetSettings() const;
        [[nodiscard]] const std::vector<svg::Point>& GetProjectedCoords() const;
        void MakePathPicture(const std::vector<svg::Point>& tmp_vector_coords, size_t j);
        void MakePointsPicture(const std::vector<svg::Point>& stp_coords);
        void RenDrawPicture(svg::Document& doc_);
        void RenderRoute(svg::Document& doc, const std::string& bus, size_t num_route, size_t color_num, const std::unordered_map<std::string, bool>& type_bus, const std::vector<const domain::Stop *>* route);
        void RenderBusNames(const svg::Point& p, const std::string& stop, svg::Document& doc);

    private:
        MapSettings settings_;
        std::vector<svg::Point> points_;
        std::vector<std::unique_ptr<svg::Drawable>> picture_;
    };
}