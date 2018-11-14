//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_STYLESHEET_H
#define FEATHER_STYLESHEET_H

#include <vector>
#include "include/typedefs.h"

class Node;

class StyleSheet {
public:
    inline bool getDisabled() { return disabled; }

    inline DOMString getHref() { return href; }

    inline std::vector<DOMString> getMedia() { return media; }

    inline Node *getOwnerNode() { return ownerNode; }

    inline StyleSheet *getParent() { return parent; }

    inline DOMString getTitle() { return title; }

    inline DOMString getType() { return type; }

private:
    bool disabled;
    DOMString href;
    std::vector<DOMString> media;
    Node *ownerNode;
    StyleSheet *parent;
    DOMString title;
    DOMString type;
};

#endif //FEATHER_STYLESHEET_H
