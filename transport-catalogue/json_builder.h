#pragma once

#include <utility>
#include <optional>

#include "json.h"

namespace json {
    class Builder {
        std::optional<Node> root_ = {};
        std::vector<Node*> nodes_stack_;
        std::optional<std::string> curr_str;

        void EmplaceNode(const Node& item_node);
    public:
        class KeyContext;
        class DictItemContext;
        class ArrayItemContext;
        class BaseContext;
        Builder() = default;

        KeyContext Key(const std::string& key);
        ArrayItemContext StartArray();
        Builder& EndArray();
        DictItemContext StartDict();
        Builder& EndDict();
        BaseContext Value(const Node& value);

        Node Build() const;
    };

    class Builder::BaseContext {
    public:
        BaseContext(Builder& builder)
                : builder_(builder){
        }
        ArrayItemContext StartArray();
        DictItemContext StartDict();
        BaseContext& Value(const Node& value);
        BaseContext& EndArray();
        KeyContext Key(const std::string& key);
        BaseContext& EndDict();

        Node Build() const;
    private:
        Builder& builder_;
    };

    class Builder::KeyContext final : public BaseContext{
    public:
        KeyContext() = delete;
        KeyContext(BaseContext base)
                : BaseContext(std::move(base)), base_(base) {
        }

        Builder::DictItemContext Value(const Node& value);
        Builder::DictItemContext StartDict();
        Builder::ArrayItemContext StartArray();

        Builder& EndArray() = delete;
        BaseContext& EndDict() = delete;
        Node Build() const = delete;
        KeyContext Key(const std::string& key) = delete;
    private:
        BaseContext base_;
    };

    class Builder::DictItemContext : public BaseContext {
    public:
        DictItemContext() = delete;
        DictItemContext(BaseContext base)
                : BaseContext(std::move(base)), base_(base){}

        KeyContext Key(const std::string& key);
        BaseContext& EndDict();

        Builder::ArrayItemContext StartArray() = delete;
        DictItemContext StartDict() = delete;
        DictItemContext Value(const Node& value) = delete;
        Builder& EndArray() = delete;
        Node Build() const = delete;
    private:
        BaseContext base_;
    };

    class Builder::ArrayItemContext final : public BaseContext {
    public:
        ArrayItemContext() = default;
        ArrayItemContext(BaseContext base)
                : BaseContext(std::move(base)) {
        }

        ArrayItemContext StartArray();
        DictItemContext StartDict();
        ArrayItemContext Value(const Node& value);
        BaseContext& EndArray();

        KeyContext Key(const std::string& key) = delete;
        BaseContext& EndDict() = delete;
        Node Build() const = delete;
    };
}