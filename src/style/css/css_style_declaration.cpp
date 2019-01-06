//
// Created by binhe on 12/17/2018.
//

#include "style/css/rules/css_style_declaration.h"

using namespace feather::css;

namespace {
    template<typename K, typename V>
    auto find(const feather::Map<K, V> &map, const feather::DOMString &str) {
        return std::find(map.begin(), map.end(), str);
    }
}

feather::DOMString CSSStyleDeclaration::getPropertyValue(const feather::DOMString &property) const {
    auto it = find(indeces, property);
    return it == indeces.end() ? "" : props[it->second].val;
}

feather::DOMString CSSStyleDeclaration::getPropertyPriority(const feather::DOMString &property) const {
    auto it = find(indeces, property);
    return it == indeces.end() || !props[it->second].priority ? "" : "!important";
}

void CSSStyleDeclaration::setProperty(const feather::DOMString &property, feather::DOMString value, bool priority) {
    auto it = find(indeces, property);
    if (it != indeces.end()) {
        auto val = props[it->second];
        val.val = std::move(value);
        val.priority = priority;
    } else {
        indeces[property] = props.size();
        props.push_back({property, std::move(value), priority});
    }
    invalidate(RegularEnumSet<observable::InvEvent>() + observable::InvEvent::LOCAL_STYLE_PROPERTY_CHANGE, this);
}

feather::DOMString CSSStyleDeclaration::removeProperty(const DOMString &property) {
    auto it = find(indeces, property);
    if (it != indeces.end()) {
        auto pit = props.begin() + it->second;
        auto val = pit->val;
        props.erase(pit);
        indeces.erase(it);
        return val;
    }
    return "";
}

feather::UnaryPair<feather::DOMString> CSSStyleDeclaration::item(feather::UInt idx) const {
    return idx < props.size() ? props[idx].getProp() : std::make_pair("", "");
}

feather::DOMString CSSStyleDeclaration::getCssText() const {
    DOMString str;
    for (const auto &prop : props) str += prop.toString();
    return str;
}