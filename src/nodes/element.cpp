//
// Created by prem on 11/13/2018.
//

#include <algorithm>
#include <sstream>
#include "include/nodes/element.h"

void dom::Element::setClassName(DOMString className) {
    classList.clear();
    std::stringstream stream;
    for (unsigned long i = 0; i < className.length(); i++) {
        if (className[i] == ' ') {
            DOMString str = stream.str();
            if (!std::all_of(str.begin(), str.end(), isspace))
                classList.add(str);
            stream.clear();
        }
        stream << className[i];
    }
    DOMString str = stream.str();
    if (!std::all_of(str.begin(), str.end(), isspace))
        classList.add(str);
}

void dom::Element::setInnerHTML(DOMString html) {
    getChildNodes().clear();
    //TODO: Parse html and add as node children
}

DOMString dom::Element::computeInnerHTML() const {
    std::stringstream output;
    output << "<" << tagName;
    for (unsigned long i = 0; i < attributes.getLength(); i++) {
        Attr *attr = attributes.getItem(i);
        output << " " << attr->getName() << "=\"" << attr->getValue() << "\"";
    }
    output << " >";
    for (unsigned long i = 0; i < children.getLength(); output << children.getItem(i++)->getInnerHTML());
    output << "</" << tagName << ">";
    return output.str();
}