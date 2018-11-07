//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_INODE_H
#define FEATHER_INODE_H

#include <string>
#include <vector>
#include "html_element.h"
#include "node_type.h"

class INode {
public:
    //Properties
    virtual const std::string &baseURI() = 0;

    virtual std::vector<INode *> *childNodes() = 0;

    INode *firstChild() {
        auto children = this->childNodes();
        return children ? children->size() ? children[0] : nullptr : nullptr;
    };

    virtual const bool isConnected() = 0;

    INode *lastChild() {
        auto children = this->childNodes();
        auto size = children ? children->size() : 0;
        return children && size ? children[size - 1] : nullptr;
    }

    virtual INode *nextSibling() = 0;

    virtual const std::string &name() = 0;

    virtual const unsigned short nodeType() = 0;

    virtual std::string *nodeValue() = 0;

    virtual void nodeValue(std::string *value) = 0;

    virtual const INode *ownerDocument() = 0;

    virtual const INode *parentNode() = 0;

    virtual const Element *parentElement() = 0;

    virtual const INode *previousSibling() = 0;

    virtual std::string *textContent() = 0;

    virtual void textContent(std::string *content) = 0;

    //Methods

    virtual void appendChild(INode *child) {
        auto children = this->childNodes();
        if (children) {
            children->push_back(child);
        }
    };

    virtual INode *cloneNode() = 0;

    

    virtual void

    virtual ~VNode() {};
};

#endif //FEATHER_INODE_H
