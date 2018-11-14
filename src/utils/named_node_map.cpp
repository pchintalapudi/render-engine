//
// Created by prem on 11/14/2018.
//

#include <sstream>
#include "include/utils/named_node_map.h"

Attr *NamedNodeMap::getClassAttr() const {
    if (classList) {
        if (classAttr && checksum == classList->getChecksum()) {
            return classAttr;
        } else {
            checksum = classList->getChecksum();
            delete classAttr;
            return new Attr("class", owner, classList->getValue());
        }
    }
    return nullptr;
}

void NamedNodeMap::setClassAttr(Attr *classAttr) {
    delete this->classAttr;
    this->classAttr = classAttr;
    classList->clear();
    auto val = classAttr->getValue();
    std::stringstream stream;
    for (int i = 0; i < val.length(); i++) {
        if (val[i] == ' ') {
            DOMString str = stream.str();
            if (!std::all_of(str.begin(), str.end(), isspace))
                classList->add(str);
            stream.clear();
        }
        stream << val[i];
    }
    DOMString str = stream.str();
    if (!std::all_of(str.begin(), str.end(), isspace))
        classList->add(str);
    checksum = classList->getChecksum();
}

void NamedNodeMap::setNamedItem(Attr &data) {
    if (data.getName() == "class") {
        setClassAttr(&data);
        insertionOrder.push_back("class");
    } else {
        dataMap[data.getName()] = &data;
        insertionOrder.push_back(data.getName());
    }
}