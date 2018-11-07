//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_ATTR_H
#define FEATHER_ATTR_H

#include <string>

class Element;

class Attr {
public:
    virtual const std::string &getName() = 0;

    virtual const std::string &getNamespaceURI() = 0;

    virtual const std::string &getLocalName() = 0;

    virtual const std::string &getPrefix() = 0;

    virtual const Element &getOwnerElement() = 0;

    inline const bool specified() { return true; };

    virtual std::string &getValue() = 0;

    virtual void setValue(std::string const &value);
};

#endif //FEATHER_ATTR_H
