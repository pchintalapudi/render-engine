//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_INODE_H
#define FEATHER_INODE_H

#include <algorithm>
#include <string>
#include <vector>
#include "node_type.h"

class Element;

class Document;

class INode {
public:
    //Properties
    virtual const std::string &baseURI() = 0;

    virtual std::vector<INode &> *childNodes() = 0;

    INode *firstChild() {
        auto children = this->childNodes();
        return children ? children->size() ? &children[0] : nullptr : nullptr;
    };

    virtual const bool isConnected() = 0;

    INode *lastChild() {
        auto children = this->childNodes();
        auto size = children ? children->size() : 0;
        return children && size ? &children[size - 1] : nullptr;
    }

    virtual INode *nextSibling() = 0;

    virtual const std::string &name() = 0;

    virtual const unsigned short nodeType() = 0;

    virtual std::string *nodeValue() = 0;

    virtual void nodeValue(std::string *value) = 0;

    virtual const Document *ownerDocument() = 0;

    virtual const INode *parentNode() = 0;

    virtual const Element *parentElement() = 0;

    virtual const INode *previousSibling() = 0;

    virtual std::string *textContent() = 0;

    virtual void textContent(std::string *content) = 0;

    //Methods

    virtual void appendChild(INode const &child) {
        auto children = this->childNodes();
        if (children) {
            children->push_back(child);
        }
    };

    virtual const INode &cloneNode() = 0;

    virtual unsigned char compareDocumentPosition(INode const &other) = 0;

    virtual bool contains(INode const &other) = 0;

    virtual const INode &getRootNode() = 0;

    bool hasChildNodes() {
        auto children = this->childNodes();
        return children && children->size();
    }

    virtual void insertBefore(INode &child, INode *ref) {
        auto children = this->childNodes();
        if (children)
            if (ref) {
                auto it = std::find(children->begin(), children->end(), child);
                if (it != children->end()) {
                    children->insert(it, child);
                    return;
                }
            }
        this->appendChild(child);
    }

    virtual bool isDefaultNamespace(string const &uri) = 0;

    virtual bool isEqualNode(INode const &other) = 0;

    const bool isSameNode(INode const &other) {
        return (*this) == other;
    }

    virtual string *lookupNamespaceURI(string *prefix) = 0;

    virtual void normalize() = 0;

    virtual void removeChild(INode const &child) {
        auto children = this->childNodes();
        if (children) {
            auto it = std::find(children->begin(), children->end(), child);
            if (it != children->end()) {
                children->erase(it);
            }
        }
    };

    virtual void replaceChild(INode const &newChild, INode const &oldChild) {
        auto children = this->childNodes();
        if (children) {
            std::replace(children->begin(), children->end(), oldChild, newChild);
        }
    }

    virtual ~VNode() {};
};

#endif //FEATHER_INODE_H
