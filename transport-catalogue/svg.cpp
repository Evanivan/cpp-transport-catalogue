#include "svg.h"

#include <utility>

namespace svg {

    using namespace std::literals;

    void Object::Render(const RenderContext& context) const {
        context.RenderIndent();

        // Делегируем вывод тега своим подклассам
        RenderObject(context);

        context.out << std::endl;
    }

    std::ostream& operator<<(std::ostream &out, const StrokeLineCap& cap) {
        using namespace std::literals;

        switch (cap) {
            case StrokeLineCap::BUTT:
                return out << "butt"sv;
            case StrokeLineCap::ROUND:
                return out << "round"sv;
            case StrokeLineCap::SQUARE:
                return out << "square"sv;
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const StrokeLineJoin& join) {
        using namespace std::literals;

        if (join == StrokeLineJoin::ROUND) {
            return out << "round"sv;
        }
        if (join == StrokeLineJoin::ARCS) {
            return out << "arcs"sv;
        }
        if (join == StrokeLineJoin::BEVEL) {
            return out << "bevel"sv;
        }
        if (join == StrokeLineJoin::MITER) {
            return out << "miter"sv;
        }
        if (join == StrokeLineJoin::MITER_CLIP) {
            return out << "miter-clip"sv;
        }
        return out;
    }

    void PrintType(std::ostream& out, std::monostate) {
        out << "none"sv;
    }

    void PrintType(std::ostream& out, const std::string& color) {
        out << color;
    }

    void PrintType(std::ostream& out, const Rgb& rgb_type) {
        out << "rgb("sv << static_cast<int>(rgb_type.red) << ","sv << static_cast<int>(rgb_type.green)
            << ","sv << static_cast<int>(rgb_type.blue) << ")"sv;
    }

    void PrintType(std::ostream& out, const Rgba& rgba_type) {
//        std::string str_d = std::to_string(rgba_type.opacity);
//        str_d.erase(str_d.find_last_not_of('0') + 1, std::string::npos);

        out << "rgba("sv << static_cast<int>(rgba_type.red) << ","sv << static_cast<int>(rgba_type.green)
            << ","sv << static_cast<int>(rgba_type.blue) << ","sv << rgba_type.opacity << ")"sv;
    }

    std::ostream& operator<<(std::ostream& out, const Color& color) {
        using namespace std::literals;
        std::ostringstream strm;

        visit(
                [&strm](auto value) {
                    PrintType(strm, value);
                }, color);

        return out << strm.str();
    }

// ---------- Circle ------------------

    Circle& Circle::SetCenter(Point center)  {
        center_ = center;
        return *this;
    }

    Circle& Circle::SetRadius(double radius)  {
        radius_ = radius;
        return *this;
    }

    void Circle::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
        out << "r=\""sv << radius_ << "\""sv;

        RenderAttrs(out);
        RenderStrokeCap(out);
        RenderStrokeJoin(out);

        out << "/>"sv;
    }
// ---------- Polyline ------------------

    Polyline& Polyline::AddPoint(Point point) {
        points.emplace_back(point);
        return *this;
    }

    void Polyline::RenderObject(const RenderContext &context) const {
        auto& out = context.out;
        out << "<polyline points=\""sv;
        for (size_t i = 0; i < points.size(); ++i) {
            if (i == points.size() - 1) {
                out << points[i].x << ","sv << points[i].y;
                break;
            }
            out << points[i].x << ","sv << points[i].y << " ";
        }
        out << "\""sv;

        RenderAttrs(out);
        RenderStrokeCap(out);
        RenderStrokeJoin(out);

        out << "/>"sv;
    }

// ---------- Text ------------------

    Text& Text::SetPosition(Point pos) {
        pos_ = pos;
        return *this;
    }

    Text& Text::SetOffset(Point offset) {
        offset_ = offset;
        return *this;
    }

    Text& Text::SetFontSize(uint32_t size) {
        size_ = size;
        return *this;
    }

    Text& Text::SetFontFamily(std::string font_family) {
        font_family_ = std::move(font_family);
        return *this;
    }

    Text& Text::SetFontWeight(std::string font_weight) {
        font_weight_ = std::move(font_weight);
        return *this;
    }

    Text& Text::SetData(std::string data) {
        data_ = std::move(data);
        return *this;
    }

    void Text::RenderObject(const RenderContext& context) const {
        auto& out = context.out;

        out << "<text"sv;

        RenderAttrs(out);
        RenderStrokeCap(out);
        RenderStrokeJoin(out);

        out << " x=\""sv << pos_.x << "\" y=\""sv << pos_.y << "\""sv;
        out << " dx=\""sv << offset_.x << "\" dy=\""sv << offset_.y << "\""sv;
        out << " font-size=\""sv << size_ << "\""sv;
        if (!font_family_.empty()) {
            out << " font-family=\""sv << font_family_ << "\""sv;
        }
        if (!font_weight_.empty()) {
            out << " font-weight=\""sv << font_weight_ << "\""sv;
        }
        out << ">"sv;
        if (!data_.empty()) {
            for (const auto &el: data_) {
                switch (el) {
                    case '"':
                        out << "&quot;"sv;
                        continue;
                    case '\'':
                        out << "&apos;"sv;
                        continue;
                    case '<':
                        out << "&lt;"sv;
                        continue;
                    case '>':
                        out << "&gt;"sv;
                        continue;
                    case '&':
                        out << "&amp;"sv;
                        continue;
                    default:
                        out << el;
                }
            }
        }
        out << "</text>"sv;
    }

    void Document::AddPtr(std::unique_ptr<Object>&& obj) {
        objects_.emplace_back(std::move(obj));
    }

    void Document::Render(std::ostream& out) const {
        RenderContext context(out, 2);
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"sv;
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n"sv;

        for (const auto& pointer_odj : objects_) {
            pointer_odj->Render(context.Indented());
        }

        out << "</svg>"sv;
    }
}  // namespace svg