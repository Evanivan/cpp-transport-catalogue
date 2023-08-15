#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <cmath>
#include <numeric>

namespace json {

    class Node;
// Сохраните объявления Dict и Array без изменения
    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;

// Эта ошибка должна выбрасываться при ошибках парсинга JSON
    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

//    bool operator==(const Node& lhs, const Node& rhs);
//    bool operator!=(const Node& lhs, const Node& rhs);

    using Number = std::variant<int, double>;

    Number LoadNumber(std::istream& input);
    std::string LoadString(std::istream& input);
    nullptr_t LoadNull(std::istream& input);

    struct PrintContext {
        std::ostream& out;
        int indent_step = 4;
        int indent = 0;

        void PrintIndent() const {
            for (int i = 0; i < indent; ++i) {
                out.put(' ');
            }
        }
        // Возвращает новый контекст вывода с увеличенным смещением
        PrintContext Indented() const {
            return {out, indent_step, indent_step + indent};
        }
    };

    class Node {
    public:
        using Value = std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>;

        Node() = default;

        template<typename T>
        Node(T value)
                : value_(std::move(value)){
        }
        bool operator==(const Node& rhs) const {
            return GetValue() == rhs.GetValue();
        }

        const Value& GetValue() const;
        bool IsInt() const;

        //Возвращает true, если в Node хранится int либо double.
        bool IsDouble() const;
//        bool IsFloat() const;
        bool IsPureDouble() const; //Возвращает true, если в Node хранится double.
        bool IsBool() const;
        bool IsString() const;
        bool IsNull() const;
        bool IsArray() const;
        bool IsMap() const;

        //Если внутри содержится значение другого типа,
        // должно выбрасываться исключение std::logic_error.

        const Array& AsArray() const;
        bool AsBool() const;
        const Dict& AsMap() const;
        double AsDouble() const; //Возвращает значение типа double,
        // если внутри хранится double либо int. В последнем случае возвращается приведённое в double значение.
//        float AsFloat() const;
        int AsInt() const;
        const std::string& AsString() const;

    private:
        Value value_{};
    };

    class Document {
    public:

        Document() = default;
        explicit Document(Node root);

        const Node& GetRoot() const;

    private:
        Node root_;
    };

    Document Load(std::istream& input);
    bool operator==(const Document& lhs, const Document& rhs);
    bool operator!=(const Document& lhs, const Document& rhs);

    template <typename Value>
    void PrintValue(const Value& value, std::ostream& out) {
        out << value;
    }
    void PrintValue(std::nullptr_t, std::ostream& out);
    void PrintValue(const std::string& value, std::ostream& out);
    void PrintValue(bool b, std::ostream& out);
    void PrintValue(const Array& value, std::ostream& out);
    void PrintValue(const Dict& dict, std::ostream& out);
    void PrintNode(const Node& node, std::ostream& out);

    void Print(const Document& doc, std::ostream& output);
}  // namespace json