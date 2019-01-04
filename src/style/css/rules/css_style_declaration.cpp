//
// Created by binhe on 12/17/2018.
//

#include "style/css/rules/css_style_declaration.h"

using namespace feather::css::rules;

feather::DOMString CSSStyleDeclaration::getPropertyValue(feather::DOMString property) {
    return std::get<0>(this->properties[property]);
}

feather::DOMString CSSStyleDeclaration::getPropertyPriority(feather::DOMString property) {
    return std::get<1>(this->properties[property]);
}

void CSSStyleDeclaration::setProperty(feather::DOMString property, feather::DOMString value,
                                      feather::DOMString priority) {
    this->properties[property] = std::make_tuple(value, priority);
}

feather::DOMString CSSStyleDeclaration::removeProperty(DOMString property) {
    this->properties.erase(property);
    return property;
}

feather::DOMString CSSStyleDeclaration::item(feather::UInt idx) {
    return (*std::next(std::begin(this->properties), idx)).first;
}