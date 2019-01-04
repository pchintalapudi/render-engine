//
// Created by binhe on 12/17/2018.
//

#include "style/css/rules/css_style_declaration.h"

using namespace feather::css::rules;

feather::DOMString CSSStyleDeclaration::getPropertyValue(feather::DOMString property) const {
    return this->properties.at(property).first;
}

feather::DOMString CSSStyleDeclaration::getPropertyPriority(feather::DOMString property) const {
    return this->properties.at(property).second ? "!important" : "";
}

void CSSStyleDeclaration::setProperty(feather::DOMString property, feather::DOMString value,
                                      bool priority) {
    this->properties[property] = {value, priority};
}

feather::DOMString CSSStyleDeclaration::removeProperty(DOMString property) {
    this->properties.erase(property);
    return property;
}

feather::DOMString CSSStyleDeclaration::item(feather::UInt idx) const {
    return (*std::next(std::begin(this->properties), idx)).first;
}