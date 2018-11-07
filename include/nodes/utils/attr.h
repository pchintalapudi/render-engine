//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_ATTR_H
#define FEATHER_ATTR_H

#include <string>

class Element;

class Attr {
public:

    Attr(std::string localName, Element &owner) : Attr(std::string(""), localName, owner, std::string("")) {
    }

    Attr(std::string nameSpace, std::string localName, Element &owner) : Attr(nameSpace, localName, owner,
                                                                              *(new std::string(""))) {}

    Attr(std::string localName, Element &owner, std::string value) : Attr("", localName, owner,
                                                                          value) {}

    Attr(std::string nameSpace, std::string localName,
         Element &owner, std::string value)
            : nameSpace(*new std::string(nameSpace)),
              localName(*new std::string(localName)),
              owner(owner), value(*new std::string(value)) {}

    const std::string getName() {
        return std::string(this->nameSpace + this->localName);
    };

    const std::string &getNamespaceURI() {
        return this->nameSpace;
    };

    const std::string &getLocalName() {
        return this->localName;
    };

    const std::string &getPrefix() {
        return this->nameSpace;
    };

    const Element &getOwnerElement() {
        return this->owner;
    };

    inline const bool specified() { return true; };

    std::string &getValue() {
        return this->value;
    };

    void setValue(std::string &value) {
        this->value = value;
    }

    ~Attr() {
        delete &this->localName;
        delete &this->nameSpace;
        delete &this->value;
    }

private:
    const std::string localName;
    const std::string nameSpace;
    const Element &owner;
    std::string value;
};

#endif //FEATHER_ATTR_H
