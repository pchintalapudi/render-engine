//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_NAMED_NODE_MAP_H
#define FEATHER_NAMED_NODE_MAP_H

#include <map>
#include <string>
#include <vector>
#include "attr.h"

class NamedNodeMap {
public:

    NamedNodeMap() : indeces(new std::map<std::string, int>()), attributes(new std::vector<Attr *>()) {
    }

    Attr *getNamedItem(std::string const &name) {
        return (this->indeces->find(name) == this->indeces->end()) ? nullptr
                                                                   : (*this->attributes)[(*this->indeces)[name]];
    }

    void setNamedItem(std::string const &name, Attr &attr) {
        if (this->indeces->find(name) == this->indeces->end()) {
            this->attributes->push_back(&attr);
            (*this->indeces)[name] = this->attributes->size() - 1;
        } else {
            (*this->attributes)[(*this->indeces)[name]] = &attr;
        }
    }

    void removeNamedItem(std::string const &name) {
        if (this->indeces->find(name) != this->indeces->end()) {
            this->attributes->erase(this->attributes->begin() + (*this->indeces)[name]);
            this->indeces->erase(name);
        }
    }

    Attr *getNamedItemNS(std::string const &nmspace, std::string const &localName) {
        return this->getNamedItem(nmspace + localName);
    }

    void setNamedItemNS(std::string const &nmspace, std::string const &localName, Attr &attr) {
        this->setNamedItem(nmspace + localName, attr);
    }

    void removeNamedItemNS(std::string const &nmspace, std::string const &localName) {
        this->removeNamedItem(nmspace + localName);
    }

    ~NamedNodeMap() {
        delete this->indeces;
        delete this->attributes;
    }

private:
    std::map<std::string, int> *indeces;
    std::vector<Attr *> *attributes;
};

#endif //FEATHER_NAMED_NODE_MAP_H
