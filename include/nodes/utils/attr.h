//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_ATTR_H
#define FEATHER_ATTR_H

#include <string>

class IElement;

class Attr {
public:

    Attr(std::string localName, IElement &owner) : Attr(std::string(""), localName, owner, std::string("")) {
    }

    Attr(std::string nameSpace, std::string localName, IElement &owner) : Attr(nameSpace, localName, owner,
                                                                              *(new std::string(""))) {}

    Attr(std::string localName, IElement &owner, std::string value) : Attr("", localName, owner,
                                                                          value) {}

    Attr(std::string nameSpace, std::string localName,
         IElement &owner, std::string value)
            : nameSpace(*new std::string(nameSpace)),
              localName(*new std::string(localName)),
              owner(owner), value(*new std::string(value)) {}

    const std::string &getName() {
        return this->nameSpace + this->localName;
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

    const IElement &getOwnerElement() {
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
    const IElement &owner;
    std::string value;
};

#endif //FEATHER_ATTR_H
