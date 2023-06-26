#include "json.h"

using namespace std;

namespace json {

    namespace {
        Node LoadNode(istream& input);

        Node LoadNull(std::istream& input) {
            std::string result;

            for (char c; input >> c;) {
                if (result == "null") {
                    break;
                }
                result.push_back(c);
            }

            if (result != "null") {
                throw ParsingError("Not null"s);
            }
            return {nullptr};
        }

        Node LoadBool(std::istream& input) {
            std::string result;

            for (char c; input >> c;) {
                result.push_back(c);
                if (result == "true"sv) return {true};
                if (result == "false"sv) return {false};
            }
            if (result != "true"sv || result != "false"sv) throw ParsingError("Not a bool"s);

            return {};
        }

        Node LoadArray(istream& input) {
            Array result;
            char ch{};

            for (char c; input >> c;) {
                if (c == ']') {
                    ch = ']';
                    break;
                }
                if (c != ',') {
                    input.putback(c);
                }
                result.push_back(LoadNode(input));
            }
            if (ch != ']') throw ParsingError("Not enough symbols in array"s);

            return {std::move(result)};
        }

        Number LoadNumber(istream& input) {
            using namespace std::literals;

            std::string parsed_num;

            // Считывает в parsed_num очередной символ из input
            auto read_char = [&parsed_num, &input] {
                parsed_num += static_cast<char>(input.get());
                if (!input) {
                    throw ParsingError("Failed to read number from stream"s);
                }
            };

            // Считывает одну или более цифр в parsed_num из input
            auto read_digits = [&input, read_char] {
                if (!std::isdigit(input.peek())) {
                    throw ParsingError("A digit is expected"s);
                }
                while (std::isdigit(input.peek())) {
                    read_char();
                }
            };

            if (input.peek() == '-') {
                read_char();
            }
            // Парсим целую часть числа
            if (input.peek() == '0') {
                read_char();
                // После 0 в JSON не могут идти другие цифры
            } else {
                read_digits();
            }

            bool is_int = true;
            // Парсим дробную часть числа
            if (input.peek() == '.') {
                read_char();
                read_digits();
                is_int = false;
            }

            // Парсим экспоненциальную часть числа
            if (int ch = input.peek(); ch == 'e' || ch == 'E') {
                read_char();
                if (ch = input.peek(); ch == '+' || ch == '-') {
                    read_char();
                }
                read_digits();
                is_int = false;
            }

            try {
                if (is_int) {
                    // Сначала пробуем преобразовать строку в int
                    try {
                        return std::stoi(parsed_num);
                    } catch (...) {
                        // В случае неудачи, например, при переполнении,
                        // код ниже попробует преобразовать строку в double
                    }
                }
                return std::stod(parsed_num);
            } catch (...) {
                throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
            }
        }

        std::string LoadString(istream& input) {
            using namespace std::literals;

            auto it = std::istreambuf_iterator<char>(input);
            auto end = std::istreambuf_iterator<char>();
            std::string s;
            while (true) {
                if (it == end) {
                    // Поток закончился до того, как встретили закрывающую кавычку?
                    throw ParsingError("String parsing error");
                }
                const char ch = *it;
                if (ch == '"') {
                    // Встретили закрывающую кавычку
                    ++it;
                    break;
                } else if (ch == '\\') {
                    // Встретили начало escape-последовательности
                    ++it;
                    if (it == end) {
                        // Поток завершился сразу после символа обратной косой черты
                        throw ParsingError("String parsing error");
                    }
                    const char escaped_char = *(it);
                    // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
                    switch (escaped_char) {
                        case 'n':
                            s.push_back('\n');
                            break;
                        case 't':
                            s.push_back('\t');
                            break;
                        case 'r':
                            s.push_back('\r');
                            break;
                        case '"':
                            s.push_back('"');
                            break;
                        case '\\':
                            s.push_back('\\');
                            break;
                        default:
                            // Встретили неизвестную escape-последовательность
                            throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
                    }
                } else if (ch == '\n' || ch == '\r') {
                    // Строковый литерал внутри- JSON не может прерываться символами \r или \n
                    throw ParsingError("Unexpected end of line"s);
                } else {
                    // Просто считываем очередной символ и помещаем его в результирующую строку
                    s.push_back(ch);
                }
                ++it;
            }
//            std::cout << s << endl;

            return s;
        }

        Node LoadDict(istream& input) {
            Dict result;
            char c = {};

            for (; input >> c;) {
                if (c == '}') {
                    break;
                }
                if (c == ',') {
                    input >> c;
                }

                if (c == 's') {
                    input.putback(c);
                }
                string key = LoadString(input);
                input >> c;
                result.insert({std::move(key), LoadNode(input)});
            }
            if (c != '}' && result.empty()) throw ParsingError("Not enough symbols in dictionary"s);

            return Node(std::move(result));
        }

        Node LoadNode(istream& input) {
            char c;
            input >> c;

            if (c == '[') {
                return LoadArray(input);
            } else if (c == '{') {
                return LoadDict(input);
            } else if (c == '"') {
                return LoadString(input);
            } else if (c == 'n'){
                input.putback(c);
                return LoadNull(input);
            } else if (c == 't' || c == 'f'){
                input.putback(c);
                return LoadBool(input);
            } else if ((c >= '0' && c <= '9') || c == '-') {
                input.putback(c);
                auto value = LoadNumber(input);
                if (holds_alternative<int>(value)) {
                    return {get<int>(value)};
                }
//                auto res = std::trunc(get<double>(value) * 1000000) / 1000000;
                return {get<double>(value)};
//                return {res};
            }
            throw ParsingError("Not a node");
        }
    }  // namespace
    bool operator==(const Node& lhs, const Node& rhs) {
        bool res = true;
        if (lhs.GetValue().index() != rhs.GetValue().index()) {
            res = false;
        } else {
            switch (lhs.GetValue().index()) {
                case 0:
                    return true;
                case 1:
                    return get<Array>(lhs.GetValue()) == get<Array>(rhs.GetValue());
                case 2:
                    return get<Dict>(lhs.GetValue()) == get<Dict>(rhs.GetValue());
                case 3:
                    return get<bool>(lhs.GetValue()) == get<bool>(rhs.GetValue());
                case 4:
                    return get<int>(lhs.GetValue()) == get<int>(rhs.GetValue());
                case 5:
                    return get<double>(lhs.GetValue()) == get<double>(rhs.GetValue());
                case 6:
                    return get<std::string>(lhs.GetValue()) == get<std::string>(rhs.GetValue());
            }
        }
        return res;
    }

    bool operator!=(const Node& lhs, const Node& rhs) {
        return !(lhs == rhs);
    }

    bool operator==(const Document& lhs, const Document& rhs) {
        bool res = true;
        if (lhs.GetRoot() != rhs.GetRoot()) {
            res = false;
        }
        return res;
    }

    bool operator!=(const Document& lhs, const Document& rhs) {
        return !(lhs == rhs);
    }

    const Node::Value& Node::GetValue() const {
        return value_;
    }

    Node::Node(nullptr_t)
            : value_(std::nullptr_t()) {
    }

    Node::Node(Array array)
            : value_(std::move(array)) {
    }

    Node::Node(Dict map)
            : value_(std::move(map)) {
    }

    Node::Node(int value)
            : value_(value) {
    }

    Node::Node(string value)
            : value_(std::move(value)) {
    }

    Node::Node(double value)
            : value_(value) {
    }

    Node::Node(bool value)
            : value_(value) {
    }

    bool Node::IsInt() const {
        return holds_alternative<int>(value_);
    }
    bool Node::IsDouble() const {
        bool res = false;
        if (holds_alternative<int>(value_) || holds_alternative<double>(value_)) {
            res = true;
        }
        return res;
    }

    bool Node::IsPureDouble() const {
        return holds_alternative<double>(value_);
    }
    bool Node::IsBool() const {
        return holds_alternative<bool>(value_);
    }
    bool Node::IsString() const {
        return holds_alternative<std::string>(value_);
    }
    bool Node::IsNull() const {
        return holds_alternative<nullptr_t>(value_);
    }
    bool Node::IsArray() const {
        return holds_alternative<Array>(value_);
    }
    bool Node::IsMap() const {
        return holds_alternative<Dict>(value_);
    }

    const Array& Node::AsArray() const {
        if (!(this->IsArray())) throw std::logic_error("");
        return get<Array>(value_);
    }

    bool Node::AsBool() const {
        if (!(this->IsBool())) throw std::logic_error("");
        return get<bool>(value_);
    }

    double Node::AsDouble() const {
        if (!(this->IsDouble())) throw std::logic_error("");
        if (this->IsInt()) return static_cast<double>(get<int>(value_));
        auto v = get<double>(value_);
        return v;
    }

    const Dict& Node::AsMap() const {
        if (!(this->IsMap())) throw std::logic_error("");
        return get<Dict>(value_);
    }

    int Node::AsInt() const {
        if (!(this->IsInt())) throw std::logic_error("");
        int i = get<int>(value_);
        return i;
    }

    const string& Node::AsString() const {
        if (!(this->IsString())) throw std::logic_error("");
        return get<std::string>(value_);
    }

    Document::Document(Node root)
            : root_(move(root)) {
    }

    const Node& Document::GetRoot() const {
        return root_;
    }

    Document Load(istream& input) {
        return Document{LoadNode(input)};
    }

// Перегрузка функции PrintValue для вывода значений null
    void PrintValue(std::nullptr_t, std::ostream& out) {
        out << "null"sv;
    }

    void PrintValue(const std::string& value, std::ostream& out) {
        out << "\""sv;

        for (char c : value) {
            switch (c) {
                case '"':
                    out << "\\\""sv;
                    break;
                case '\n':
                    out << "\\n"sv;
                    break;
                case '\t':
                    out << "\t"sv;
                    break;
                case '\r':
                    out << "\\r"sv;
                    break;
                case '\\':
                    out << "\\\\"sv;
                    break;
                default:
                    out << c;
                    break;
            }
        }
        out << "\""sv;
    }
// Другие перегрузки функции PrintValue пишутся аналогично

    void PrintValue(bool b, std::ostream& out) {
        out << boolalpha << b;
    }

    void PrintNode(const Node& node, std::ostream& out) {
        std::visit(
                [&out](const auto& value){ PrintValue(value, out); },
                node.GetValue());
    }

    void PrintValue(const Array& value, std::ostream& out) {

        out << "["sv;
        for (size_t i = 0; i < value.size(); ++i) {
            if (i == value.size() - 1) {
                PrintNode(value[i], out);
                break;
            }
            PrintNode(value[i], out);
            out << ", ";
        }
        out << "]"sv;
    }

    void PrintValue(const Dict& dict, std::ostream& out) {
        out << "{"sv;
        size_t i = 0;
        for (const auto& [str, node] : dict) {
            if (i > 0 && i < dict.size()) {
                out << ", "sv;
            }
            out << "\""sv << str << "\": "sv;
            ++i;
            PrintNode(node, out);
        }
        out << "}"sv;
    }

    void Print(const Document& doc, std::ostream& output) {
        const auto& doc_type = doc.GetRoot();

        if (doc_type.IsInt()) {
            output << doc_type.AsInt();
        } else if (doc_type.IsDouble()) {
            output << std::defaultfloat << doc_type.AsDouble();
        } else {
            PrintNode(doc_type, output);
        }
    }

}  // namespace json