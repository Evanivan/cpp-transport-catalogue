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

    const std::vector<svg::Point>& MapRenderer::GetProjectedCoords() const {
        return points_;
    }

    svg::Document MapRenderer::RenderMap(const std::vector<std::vector<svg::Point>>& vector_of_path_picture, const std::vector<RouteForRend>& route_picture,
                                         const std::vector<svg::Point>& stp_coords,
                                         const std::vector<std::pair<const svg::Point &, const std::string &>>& render_busnames) {
        svg::Document doc;

        size_t j = 0;
        for (const auto& tmp_vector_coords : vector_of_path_picture) {
            if (j == settings_.color_palette.size()) {
                j = 0;
            }
            picture_.emplace_back(std::make_unique<shapes::RoutePath>(
                    tmp_vector_coords, settings_.color_palette[j], settings_
            ));
            ++j;
        }
        DrawPicture(picture_, doc);
        picture_.clear();

        size_t num_route_ = 0;
        size_t color_num_ = 0;
        for (const auto& r : route_picture) {
            if (r.route->empty()) continue;
            if (color_num_ == settings_.color_palette.size()) {
                color_num_ = 0;
            }
            if (r.type_bus.at(r.bus)) {
                const svg::Text base_text =  //
                        svg::Text()
                                .SetPosition({points_[num_route_].x, points_[num_route_].y})
                                .SetOffset({settings_.bus_label_offset.first, settings_.bus_label_offset.second})
                                .SetFontSize(settings_.bus_label_font_size)
                                .SetFontFamily("Verdana")
                                .SetFontWeight("bold")
                                .SetData(r.bus);
                doc.Add(svg::Text{base_text}
                                .SetStrokeColor(settings_.underlayer_color)
                                .SetFillColor(settings_.underlayer_color)
                                .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                                .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                                .SetStrokeWidth(settings_.underlayer_width));
                doc.Add(svg::Text{base_text}.SetFillColor(settings_.color_palette[color_num_]));
            } else {
                const svg::Text base_text =  //first stop
                        svg::Text()
                                .SetPosition({points_[num_route_].x, points_[num_route_].y})
                                .SetOffset({settings_.bus_label_offset.first, settings_.bus_label_offset.second})
                                .SetFontSize(settings_.bus_label_font_size)
                                .SetFontFamily("Verdana")
                                .SetFontWeight("bold")
                                .SetData(r.bus);
                doc.Add(svg::Text{base_text}
                                .SetStrokeColor(settings_.underlayer_color)
                                .SetFillColor(settings_.underlayer_color)
                                .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                                .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                                .SetStrokeWidth(settings_.underlayer_width));
                doc.Add(svg::Text{base_text}.SetFillColor(settings_.color_palette[color_num_]));

                if ((*r.route)[0]->stop_name != (*r.route)[r.route->size() - 1]->stop_name) {
                    const svg::Text base_text_last =  //last stop
                            svg::Text()
                                    .SetPosition({points_[num_route_ + (r.route->size() - 1)].x,
                                                  points_[num_route_ + (r.route->size() - 1)].y})
                                    .SetOffset({settings_.bus_label_offset.first, settings_.bus_label_offset.second})
                                    .SetFontSize(settings_.bus_label_font_size)
                                    .SetFontFamily("Verdana")
                                    .SetFontWeight("bold")
                                    .SetData(r.bus);

                    doc.Add(svg::Text{base_text_last}
                                    .SetStrokeColor(settings_.underlayer_color)
                                    .SetFillColor(settings_.underlayer_color)
                                    .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                                    .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                                    .SetStrokeWidth(settings_.underlayer_width));
                    doc.Add(svg::Text{base_text_last}.SetFillColor(settings_.color_palette[color_num_]));
                }
            }
            num_route_ += r.route->size();
            ++color_num_;
        }

        picture_.emplace_back(std::make_unique<shapes::BusPoints>(
                stp_coords, settings_));
        DrawPicture(picture_, doc);
        picture_.clear();

        for (const auto& [p, stop] : render_busnames) {
            const svg::Text base_text_2 =  //
                    svg::Text()
                            .SetPosition({p.x, p.y})
                            .SetOffset({settings_.stop_label_offset.first, settings_.stop_label_offset.second})
                            .SetFontSize(settings_.stop_label_font_size)
                            .SetFontFamily("Verdana")
                            .SetData(stop);
            doc.Add(svg::Text{base_text_2}
                            .SetStrokeColor(settings_.underlayer_color)
                            .SetFillColor(settings_.underlayer_color)
                            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                            .SetStrokeWidth(settings_.underlayer_width));
            doc.Add(svg::Text{base_text_2}.SetFillColor("black"));
            ++j;
        }

        return doc;
    }
}