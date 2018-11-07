//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_INODE_H
#define FEATHER_INODE_H

#include <algorithm>
#include <string>
#include <vector>
#include "include/events/ievent_target.h"
#include "node_type.h"

class IElement;

class IDocument;

class INode : IEventTarget {
public:
    //Properties
    virtual const std::string &baseURI() = 0;

    virtual std::vector<INode *> *childNodes() = 0;

    INode *firstChild();

    virtual const bool isConnected() = 0;

    INode *lastChild();

    virtual INode *nextSibling() = 0;

    virtual const std::string &name() = 0;

    virtual const unsigned short nodeType() = 0;

    virtual std::string *nodeValue() = 0;

    virtual void nodeValue(std::string *value) = 0;

    virtual const IDocument *ownerDocument() = 0;

    virtual const INode *parentNode() = 0;

    virtual const IElement *parentElement() = 0;

    virtual const INode *previousSibling() = 0;

    virtual std::string *textContent() = 0;

    virtual void textContent(std::string *content) = 0;

    //Methods

    virtual void appendChild(INode &child) {
        auto children = this->childNodes();
        if (children) {
            children->push_back(&child);
        }
    };

    virtual const INode &cloneNode() = 0;

    virtual unsigned char compareDocumentPosition(INode const &other) = 0;

    virtual bool contains(INode const &other) = 0;

    virtual const INode &getRootNode() = 0;

    inline bool hasChildNodes() {
        auto children = this->childNodes();
        return children && children->size();
    }

    virtual void insertBefore(INode &child, INode *ref);

    virtual const bool isDefaultNamespace(std::string const &uri) = 0;

    virtual const bool isEqualNode(INode const &other) = 0;

    inline const bool isSameNode(INode &other) {
        return this == &other;
    }

    virtual std::string *lookupNamespaceURI(std::string *prefix) = 0;

    virtual void normalize() = 0;

    virtual void removeChild(INode const &child);

    virtual void replaceChild(INode &newChild, INode &oldChild);
};

#endif //FEATHER_INODE_H
