#pragma once

#include <utility>

#include "json.h"

namespace json {
    class KeyContext;
    class DictItemContext;
    class ArrayItemContext;
    class BaseContext;
    class DictValueContext;

    class Builder {
        Node root_ = nullptr;
        std::vector<Node*> nodes_stack_;

        std::string curr_str;
        std::string tmp_str;
        std::vector<Node> value_ = {};
        bool pure_value = false;

    public:
        Builder() = default;

        KeyContext Key(const std::string& k);
        ArrayItemContext StartArray();
        Builder& EndArray();
        DictItemContext StartDict();
        Builder& EndDict();
        BaseContext Value(const Node& value);

        Node Build() const;
    };

    class BaseContext {
    public:
        BaseContext(Builder& builder)
            : builder_(builder){
        }
        ArrayItemContext StartArray();
        DictItemContext StartDict();
        BaseContext& Value(const Node& value);
        BaseContext& EndArray();
        KeyContext Key(const std::string& k);
        BaseContext& EndDict();

        Node Build() const;
    private:
        Builder& builder_;
    };

    class KeyContext final : public BaseContext{
    public:
        KeyContext() = delete;
        KeyContext(BaseContext base)
                : BaseContext(std::move(base)), base_(base) {
        }

        DictValueContext Value(const Node& value);
        DictItemContext StartDict();
        ArrayItemContext StartArray();

        Builder& EndArray() = delete;
        BaseContext& EndDict() = delete;
        Node Build() const = delete;
        KeyContext Key(const std::string& k) = delete;
    private:
        BaseContext base_;
    };

    class DictValueContext final : public BaseContext{
    public:
        DictValueContext() = delete;
        DictValueContext(BaseContext base)
                : BaseContext(std::move(base)) {
        }

        KeyContext Key(const std::string& k) { return BaseContext::Key(k); };
        BaseContext& EndDict() {return BaseContext::EndDict(); };

        DictValueContext Value(const Node& value) = delete;
        DictItemContext StartDict() = delete;
        ArrayItemContext StartArray() = delete;
        Builder& EndArray() = delete;
        Node Build() const = delete;
    };

    class DictItemContext : public BaseContext {
    public:
        DictItemContext() = delete;
        DictItemContext(BaseContext base)
            : BaseContext(std::move(base)), base_(base){}

        KeyContext Key(const std::string& k);
        BaseContext& EndDict();

        ArrayItemContext StartArray() = delete;
        DictItemContext StartDict() = delete;
        DictItemContext Value(const Node& value) = delete;
        Builder& EndArray() = delete;
        Node Build() const = delete;
    private:
        BaseContext base_;
    };

    class ArrayItemContext final : public BaseContext {
    public:
        ArrayItemContext() = default;
        ArrayItemContext(BaseContext base)
                : BaseContext(std::move(base)) {
        }

        ArrayItemContext StartArray();
        DictItemContext StartDict();
        ArrayItemContext Value(const Node& value);
        BaseContext& EndArray();

        KeyContext Key(const std::string& k) = delete;
        BaseContext& EndDict() = delete;
        Node Build() const = delete;
    };
}