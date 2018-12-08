//
// Created by prem on 12/6/2018.
//

#include "include/utils/data_set_view.h"
#include "include/nodes/attrs/standard_attr.h"

DOMString kebabCase(DOMString camelCased) {
    DOMString kebabCased = "data-";
    kebabCased.reserve(5 + camelCased.length());
    for (auto character : camelCased) {
        if (isupper(character)) (kebabCased += '-') += tolower(character);
        else kebabCased += character;
    }
    return kebabCased;
}

DOMString *dom::DataSetView::getAttribute(DOMString name) const {
    DOMString kebab = kebabCase(name);
    auto attr = watched.getNamedItem(kebab);
    return attr ? new DOMString(attr->getValue()) : nullptr;
}

void dom::DataSetView::setAttribute(DOMString name, DOMString val) {
    watched.setNamedItem(new StandardAttr(kebabCase(name), owner, val));
}

bool dom::DataSetView::contains(DOMString name) const {
    return watched.contains(kebabCase(name));
}