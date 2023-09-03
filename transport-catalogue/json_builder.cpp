#include "json_builder.h"

namespace json {
    KeyContext Builder::Key(const std::string &k) {
        KeyContext key_cont(*this);
        if (value_.size() >= 2) throw std::logic_error("Cant End non Array object");
        if (pure_value) {
            throw std::logic_error("Pure value");
        }
        auto last = nodes_stack_.back();
        if (!last->IsDict() || !tmp_str.empty()) {
            throw std::logic_error("Cant End non Array object");
        }
        curr_str = k;
        tmp_str = k;
        Node node;
        std::get<Dict>(nodes_stack_.back()->GetValue()).emplace(std::make_pair(curr_str, node));
        value_.clear();
        return key_cont;
    }

    ArrayItemContext Builder::StartArray() {
        if (pure_value) {
            throw std::logic_error("Pure value");
        }
        Node n({Array()});
        if (root_ == nullptr) {
            root_ = n;
            nodes_stack_.emplace_back(&root_);
        } else if (nodes_stack_.back()->IsArray()) {
            std::get<Array>(nodes_stack_.back()->GetValue()).emplace_back(n);
            nodes_stack_.emplace_back(&std::get<Array>(nodes_stack_.back()->GetValue()).back());
        } else if (root_.IsDict()) {
            if (tmp_str.empty()) {
                throw std::logic_error("Starting Dict before closing prev");
            }
            std::get<Dict>(nodes_stack_.back()->GetValue()).at(curr_str) = n;
            nodes_stack_.emplace_back(&std::get<Dict>(nodes_stack_.back()->GetValue()).at(curr_str));
            tmp_str.clear();
        } else {
            throw std::logic_error("Can't start an Array");
        }
        ArrayItemContext arr(*this);

        return arr;
    }

    Builder& Builder::EndArray() {
        if (pure_value) {
            throw std::logic_error("Pure value");
        }
        if (nodes_stack_.empty()) throw std::logic_error("Cant End non-Array object");
        auto last = nodes_stack_.back();
        if (!last->IsArray()) {
            throw std::logic_error("Cant End non Array object");
        }
        nodes_stack_.pop_back();
        value_.clear();
        return *this;
    }

    DictItemContext Builder::StartDict() {
        if (pure_value) {
            throw std::logic_error("Pure value");
        }

        if (!value_.empty()) {
            throw std::logic_error("Cant start a Dict");
        }
        Node n({Dict()});
        DictItemContext dict(*this);

        if (root_ == nullptr) {
            root_ = n;
            nodes_stack_.emplace_back(&root_);
            return dict;
        }
        auto last = nodes_stack_.back();

        if (last->IsArray()) {
            std::get<Array>(nodes_stack_.back()->GetValue()).emplace_back(n);
            nodes_stack_.emplace_back(&std::get<Array>(nodes_stack_.back()->GetValue()).back());
        } else if (last->IsDict()) {
            std::get<Dict>(nodes_stack_.back()->GetValue()).at(curr_str) = n;
            nodes_stack_.emplace_back(&std::get<Dict>(nodes_stack_.back()->GetValue()).at(curr_str));
        }  else {
            throw std::logic_error("Can't start a Dict");
        }
        tmp_str.clear();
        return dict;
    }

    Builder& Builder::EndDict() {
        if (pure_value) throw std::logic_error("Cant End non-dict object");
        if (nodes_stack_.empty()) throw std::logic_error("Cant End non-dict object");
        auto last = nodes_stack_.back();
        if (!last->IsDict()) {
            throw std::logic_error("Cant End non dict object");
        }
        nodes_stack_.pop_back();
        value_.clear();
        return *this;
    }

    BaseContext Builder::Value(const Node& value) {
        if (nodes_stack_.empty()) {
            root_ = value;
            pure_value = true;
            value_.emplace_back(value);
            if (value_.size() >= 2) {
                throw std::logic_error("Inappropriate Value");
            }
            BaseContext val_arr(*this);
            return val_arr;
        }
        auto last = nodes_stack_.back();

        if (last->IsArray()) {
            std::get<Array>(nodes_stack_.back()->GetValue()).emplace_back(value);

//            json::Print(
//                    json::Document{
//                        *nodes_stack_.back()
//                    },
//                    std::cout
//            );
        } else if (last->IsDict()) {
            value_.emplace_back(value);
            if (value_.size() >= 2) {
                throw std::logic_error("Inappropriate Value");
            }
            std::get<Dict>(nodes_stack_.back()->GetValue()).at(curr_str) = value;
            tmp_str.clear();
        } else {
            throw std::logic_error("Inappropriate Value");
        }
        BaseContext val_arr(*this);
        return val_arr;
    }

    Node Builder::Build() const{
        if (root_ == nullptr) throw std::logic_error("Was not build");
        if (!nodes_stack_.empty()) throw std::logic_error("Stack is not empty");
        return root_;
    }

    ArrayItemContext BaseContext::StartArray() {
        return builder_.StartArray();
    }

    DictItemContext BaseContext::StartDict() {
        return builder_.StartDict();
    }

    BaseContext& BaseContext::Value(const Node &value) {
        builder_.Value(value);
        return *this;
    }

    BaseContext& BaseContext::EndArray() {
        builder_.EndArray();
        return *this;
    }

    KeyContext BaseContext::Key(const std::string &k) {
        return builder_.Key(k);
    }

    BaseContext& BaseContext::EndDict() {
        builder_.EndDict();
        return *this;
    }

    Node BaseContext::Build() const {
        return builder_.Build();
    }

    DictValueContext KeyContext::Value(const Node &value) {
        return base_.Value(value);
    }

    DictItemContext KeyContext::StartDict() {
        return base_.StartDict();
    }

    ArrayItemContext KeyContext::StartArray() {
        return base_.StartArray();
    }

    KeyContext DictItemContext::Key(const std::string &k) {
        return base_.Key(k);
    }

    BaseContext& DictItemContext::EndDict() {
        return base_.EndDict();
    }

    ArrayItemContext ArrayItemContext::StartArray() {
        return BaseContext::StartArray();
    }

    DictItemContext ArrayItemContext::StartDict() {
        return BaseContext::StartDict();
    }

    ArrayItemContext ArrayItemContext::Value(const Node &value) {
        return BaseContext::Value(value);
    }

    BaseContext& ArrayItemContext::EndArray() {
        return BaseContext::EndArray();
    }
}