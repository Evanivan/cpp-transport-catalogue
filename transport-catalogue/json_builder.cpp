#include "json_builder.h"

namespace json {
    Builder::KeyContext Builder::Key(const std::string &key) {
        if (nodes_stack_.empty() && root_.has_value()) {
            throw std::logic_error("Pure value");
        }
        KeyContext key_cont(*this);
        auto last = nodes_stack_.back();
        if (!last->IsDict() || curr_str.has_value()) {
            throw std::logic_error("Cant End non Array object");
        }
        curr_str = key;
        Node node;
        std::get<Dict>(nodes_stack_.back()->GetValue()).emplace(std::make_pair(curr_str.value(), node));
        return key_cont;
    }

    void Builder::EmplaceNode(const Node& item_node) {
        if (nodes_stack_.empty() && root_.has_value()) {
            throw std::logic_error("Pure value in StartArr");
        }

        if (!root_.has_value()) {
            root_ = item_node;
            nodes_stack_.emplace_back(&(root_.value()));
            return ;
        }
        auto last = nodes_stack_.back();

        if (last->IsArray()) {
            std::get<Array>(nodes_stack_.back()->GetValue()).emplace_back(item_node);
            nodes_stack_.emplace_back(&std::get<Array>(nodes_stack_.back()->GetValue()).back());
        } else if (last->IsDict()) {
            if (!curr_str.has_value()) {
                throw std::logic_error("Starting Dict before closing prev");
            }
            std::get<Dict>(nodes_stack_.back()->GetValue()).at(curr_str.value()) = item_node;
            nodes_stack_.emplace_back(&std::get<Dict>(nodes_stack_.back()->GetValue()).at(curr_str.value()));
        } else {
            throw std::logic_error("Can't start an Array");
        }
        curr_str.reset();
    }

    Builder::ArrayItemContext Builder::StartArray() {
        Node node({Array()});
        EmplaceNode(node);
        ArrayItemContext arr(*this);
        return arr;
    }

    Builder& Builder::EndArray() {
        if (nodes_stack_.empty() && root_.has_value()) {
            throw std::logic_error("Pure value");
        }
        if (nodes_stack_.empty()) throw std::logic_error("Cant End non-Array object");
        auto last = nodes_stack_.back();
        if (!last->IsArray()) {
            throw std::logic_error("Cant End non Array object");
        }
        nodes_stack_.pop_back();
        return *this;
    }

    Builder::DictItemContext Builder::StartDict() {
        Node node({Dict()});
        EmplaceNode(node);
        DictItemContext dict(*this);
        return dict;
    }

    Builder& Builder::EndDict() {
        if (nodes_stack_.empty() && root_.has_value()) throw std::logic_error("Pure value in EndDIct");
        if (nodes_stack_.empty()) throw std::logic_error("Cant End non-dict object");
        auto last = nodes_stack_.back();
        if (!last->IsDict()) {
            throw std::logic_error("Cant End non dict object");
        }
        nodes_stack_.pop_back();
        return *this;
    }

    Builder::BaseContext Builder::Value(const Node& value) {
        if (nodes_stack_.empty()) {
            if (root_.has_value()) {
                throw std::logic_error("Inappropriate Value");
            }
            root_ = value;
            BaseContext val_arr(*this);
            return val_arr;
        }
        auto last = nodes_stack_.back();

        if (last->IsArray()) {
            std::get<Array>(nodes_stack_.back()->GetValue()).emplace_back(value);
        } else if (last->IsDict()) {
            std::get<Dict>(nodes_stack_.back()->GetValue()).at(curr_str.value()) = value;
            curr_str.reset();
        } else {
            throw std::logic_error("Inappropriate Value");
        }
        BaseContext val_arr(*this);
        return val_arr;
    }

    Node Builder::Build() const{
        if (root_ == nullptr) throw std::logic_error("Was not build");
        if (!nodes_stack_.empty()) throw std::logic_error("Stack is not empty");
        return root_.value();
    }

    Builder::ArrayItemContext Builder::BaseContext::StartArray() {
        return builder_.StartArray();
    }

    Builder::DictItemContext Builder::BaseContext::StartDict() {
        return builder_.StartDict();
    }

    Builder::BaseContext& Builder::BaseContext::Value(const Node &value) {
        builder_.Value(value);
        return *this;
    }

    Builder::BaseContext& Builder::BaseContext::EndArray() {
        builder_.EndArray();
        return *this;
    }

    Builder::KeyContext Builder::BaseContext::Key(const std::string &key) {
        return builder_.Key(key);
    }

    Builder::BaseContext& Builder::BaseContext::EndDict() {
        builder_.EndDict();
        return *this;
    }

    Node Builder::BaseContext::Build() const {
        return builder_.Build();
    }

    Builder::DictItemContext Builder::KeyContext::Value(const Node &value) {
        return base_.Value(value);
    }

    Builder::DictItemContext Builder::KeyContext::StartDict() {
        return base_.StartDict();
    }

    Builder::ArrayItemContext Builder::KeyContext::StartArray() {
        return base_.StartArray();
    }

    Builder::KeyContext Builder::DictItemContext::Key(const std::string &key) {
        return base_.Key(key);
    }

    Builder::BaseContext& Builder::DictItemContext::EndDict() {
        return base_.EndDict();
    }

    Builder::ArrayItemContext Builder::ArrayItemContext::StartArray() {
        return BaseContext::StartArray();
    }

    Builder::DictItemContext Builder::ArrayItemContext::StartDict() {
        return BaseContext::StartDict();
    }

    Builder::ArrayItemContext Builder::ArrayItemContext::Value(const Node &value) {
        return BaseContext::Value(value);
    }

    Builder::BaseContext& Builder::ArrayItemContext::EndArray() {
        return BaseContext::EndArray();
    }
}