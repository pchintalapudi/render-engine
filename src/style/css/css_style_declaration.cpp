//
// Created by binhe on 12/17/2018.
//

#include "style/css/rules/css_style_declaration.h"

using namespace feather::css;

feather::DOMString CSSStyleDeclaration::getPropertyValue(const feather::DOMString &property) const {
    auto it = indeces.find(property);
    return it == indeces.end() ? "" : props[it->second].val;
}

feather::DOMString CSSStyleDeclaration::getPropertyPriority(const feather::DOMString &property) const {
    auto it = indeces.find(property);
    return it == indeces.end() || !props[it->second].priority ? "" : "!important";
}

void CSSStyleDeclaration::setProperty(const feather::DOMString &property, feather::DOMString value, bool priority) {
    auto it = indeces.find(property);
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
    auto it = indeces.find(property);
    if (it != indeces.end()) {
        auto pit = props.begin() + it->second;
        auto val = pit->val;
        props.erase(pit);
        indeces.erase(it);
        return val;
    }
    return "";
}

feather::Tuple<feather::DOMString, feather::DOMString, bool> CSSStyleDeclaration::item(feather::UInt idx) const {
    if (idx < size()) {
        auto item = props[idx];
        return {item.key, item.val, item.priority};
    }
    return {"", "", false};
}

const feather::DOMString &CSSStyleDeclaration::getCssText() const {
    if (isValid()) return styleString;
    styleString.clear();
    for (const auto &prop : props) styleString += prop.toString();
    return styleString;
}