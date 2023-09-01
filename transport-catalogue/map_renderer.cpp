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

    void MapRenderer::MakePathPicture(const std::vector<svg::Point>& tmp_vector_coords, size_t j) {
        picture_.emplace_back(std::make_unique<shapes::RoutePath>(
                tmp_vector_coords, settings_.color_palette[j], settings_
        ));
    }

    void MapRenderer::MakePointsPicture(const std::vector<svg::Point>& stp_coords) {
        picture_.emplace_back(std::make_unique<shapes::BusPoints>(
                stp_coords, settings_));
    }

    void MapRenderer::RenDrawPicture(svg::Document& doc_) {
        DrawPicture(picture_, doc_);
        picture_.clear();
    }

    void MapRenderer::RenderRoute(svg::Document& doc, const std::string& bus, size_t num_route, size_t color_num, const std::unordered_map<std::string, bool>& type_bus, const std::vector<const domain::Stop *>* route) {
        if (type_bus.at(bus)) {
            const svg::Text base_text =  //
                    svg::Text()
                            .SetPosition({points_[num_route].x, points_[num_route].y})
                            .SetOffset({settings_.bus_label_offset.first, settings_.bus_label_offset.second})
                            .SetFontSize(settings_.bus_label_font_size)
                            .SetFontFamily("Verdana")
                            .SetFontWeight("bold")
                            .SetData(bus);
            doc.Add(svg::Text{base_text}
                            .SetStrokeColor(settings_.underlayer_color)
                            .SetFillColor(settings_.underlayer_color)
                            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                            .SetStrokeWidth(settings_.underlayer_width));
            doc.Add(svg::Text{base_text}.SetFillColor(settings_.color_palette[color_num]));
        } else {
            const svg::Text base_text =  //first stop
                    svg::Text()
                            .SetPosition({points_[num_route].x, points_[num_route].y})
                            .SetOffset({settings_.bus_label_offset.first, settings_.bus_label_offset.second})
                            .SetFontSize(settings_.bus_label_font_size)
                            .SetFontFamily("Verdana")
                            .SetFontWeight("bold")
                            .SetData(bus);
            doc.Add(svg::Text{base_text}
                            .SetStrokeColor(settings_.underlayer_color)
                            .SetFillColor(settings_.underlayer_color)
                            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                            .SetStrokeWidth(settings_.underlayer_width));
            doc.Add(svg::Text{base_text}.SetFillColor(settings_.color_palette[color_num]));

            if ((*route)[0]->stop_name != (*route)[route->size() - 1]->stop_name) {
                const svg::Text base_text_last =  //last stop
                        svg::Text()
                                .SetPosition({points_[num_route + (route->size() - 1)].x,
                                              points_[num_route + (route->size() - 1)].y})
                                .SetOffset({settings_.bus_label_offset.first, settings_.bus_label_offset.second})
                                .SetFontSize(settings_.bus_label_font_size)
                                .SetFontFamily("Verdana")
                                .SetFontWeight("bold")
                                .SetData(bus);

                doc.Add(svg::Text{base_text_last}
                                .SetStrokeColor(settings_.underlayer_color)
                                .SetFillColor(settings_.underlayer_color)
                                .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                                .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                                .SetStrokeWidth(settings_.underlayer_width));
                doc.Add(svg::Text{base_text_last}.SetFillColor(settings_.color_palette[color_num]));
            }
        }
    }

    void MapRenderer::RenderBusNames(const svg::Point& p, const std::string& stop, svg::Document& doc) {
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
    }
}