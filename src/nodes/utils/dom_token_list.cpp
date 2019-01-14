//
// Created by prem on 12/12/2018.
//

#include "nodes/utils/dom_token_list.h"

using namespace feather::dom;

const feather::DOMString &DOMTokenList::getValue() const {
    if (isValid()) return cached;
    cached = "";
    UInt reserve = 0;
    for (UInt i = 0; i < cached.size(); reserve += source[i++].length());
    cached.reserve(reserve);
    for (UInt i = 0; i < cached.size() - 1; (cached += source[i++]) += " ");
    cached += source.back();
    validate();
    return cached;
}

void DOMTokenList::setValue(feather::DOMString value) {
    cached = value;
    validate();
    source.clear();
    DOMString val;
    UInt count = 0;
    bool space = false;
    for (auto c : value) {
        if (isspace(c)) space = true;
        else if (space) {
            space = false;
            count++;
        }
    }
    count++;
    source.reserve(count);
    space = false;
    for (auto c : value) {
        if (isspace(c)) space = true;
        else if (space) {
            space = false;
            source.push_back(val);
            val = c;
        } else val += c;
    }
    source.push_back(val);
}

void DOMTokenList::add(feather::DOMString value) {
    if (std::find(source.begin(), source.end(), value) == source.end()) source.push_back(value);
}

void DOMTokenList::remove(const feather::DOMString &value) {
    auto idx = std::find(source.begin(), source.end(), value);
    if (idx != source.end()) source.erase(idx);
}

bool DOMTokenList::toggle(feather::DOMString value) {
    auto idx = std::find(source.begin(), source.end(), value);
    if (idx == source.end()) {
        source.push_back(value);
        return true;
    } else {
        source.erase(idx);
        return false;
    }
}

bool DOMTokenList::toggle(feather::DOMString value, bool force) {
    auto idx = std::find(source.begin(), source.end(), value);
    if (idx != source.end()) {
        if (!force) {
            source.erase(idx);
            return false;
        }
        return true;
    } else {
        if (force) {
            source.push_back(value);
            return true;
        }
        return false;
    }
}

bool DOMTokenList::replace(const feather::DOMString &old, feather::DOMString replace) {
    auto valid = std::find(source.begin(), source.end(), replace);
    if (valid == source.end()) {
        auto idx = std::find(source.begin(), source.end(), old);
        if (idx != source.end()) {
            *idx = std::move(replace);
            return true;
        }
    }
    return false;
}