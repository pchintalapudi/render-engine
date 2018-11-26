//
// Created by prem on 11/14/2018.
//

#include <sstream>
#include "include/utils/named_node_map.h"

dom::Attr *dom::NamedNodeMap::getClassAttr() const {
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

void dom::NamedNodeMap::setClassAttr(dom::Attr *classAttr) {
    delete this->classAttr;
    this->classAttr = classAttr;
    classList->clear();
    auto val = classAttr->getValue();
    std::stringstream stream;
    for (unsigned long i = 0; i < val.length(); i++) {
        if (val[i] == ' ') {
            DOMString str = stream.str();
            if (!std::all_of(str.begin(), str.end(), isspace))
                classList->add(str);
            stream.str("");
            stream.clear();
        }
        stream << val[i];
    }
    DOMString str = stream.str();
    if (!std::all_of(str.begin(), str.end(), isspace)) classList->add(str);
    checksum = classList->getChecksum();
}

void dom::NamedNodeMap::setNamedItem(dom::Attr &data) {
    if (data.getName() == "class") {
        setClassAttr(&data);
        insertionOrder.push_back("class");
    } else {
        dataMap[data.getName()] = &data;
        insertionOrder.push_back(data.getName());
    }
}

std::vector<DOMString> dom::NamedNodeMap::keys() {
    std::vector<DOMString> keys;
    for (auto it : dataMap) keys.push_back(it.first);
    if (classAttr) keys.push_back("class");
}