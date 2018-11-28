//
// Created by prem on 11/9/2018.
//

#ifndef FEATHER_NODE_H
#define FEATHER_NODE_H

#include "include/utils/observable/invalidatable.h"
#include "include/utils/node_list.h"
#include "include/events/event_target.h"
#include "node_type.h"

namespace dom {
    class Element;

    class Document;

    class Node;
}

class dom::Node : public js::EventTarget, public observable::Invalidatable {
public:

    Node(DOMString baseURI, DOMString name, NodeType nodeType, Document *owner, Node *parent)
            : baseURI(baseURI), name(name), nodeType(nodeType), owner(owner), parent(parent) {}

    Node(DOMString name, NodeType nodeType, Node &parent)
            : baseURI(parent.baseURI), name(name), nodeType(nodeType), owner(parent.owner), parent(&parent) {}

    inline DOMString getBaseURI() const { return baseURI; }

    inline NodeList &getChildNodes() const { return childNodes; }

    inline Node *getFirstChild() const { return childNodes.size() ? childNodes.get(0) : nullptr; }

    inline bool isConnected() const { return owner != nullptr; }

    inline Node *getLastChild() const { return childNodes.size() ? childNodes.get(childNodes.size() - 1) : nullptr; }

    Node *getNextSibling() const;

    inline DOMString getNodeName() const { return name; }

    inline NodeType getNodeType() const { return nodeType; }

    inline DOMString *getNodeValue() const { return nodeValue; }

    inline void setNodeValue(DOMString value) { nodeValue = new DOMString(value); }

    inline Document *getOwner() const { return owner; }

    inline void setOwner(Document *owner) { this->owner = owner; }

    inline Node *getParentNode() const { return parent; }

    inline void setParentNode(Node *parent) { this->parent = parent; }

    Element *getParentElement() const;

    Node *getPreviousSibling() const;

    Node *appendChild(Node *child);

    virtual Node *cloneNode() = 0;

    unsigned char compareDocumentPosition(const Node *other) const;

    bool contains(const Node *other) const;

    Node *getRootNode() const;

    inline bool hasChildNodes() const { return childNodes.size() != 0; }

    void insertBefore(Node *child);

    virtual bool isEqualNode(const Node *other) const = 0;

    inline bool isSameNode(const Node *other) const { return this == other; }

    void normalize();

    void removeChild(Node *child);

    void replaceChild(Node *replacement, Node *target);

    ~Node() override {
        childNodes.forEach([](auto child) { delete child; });
        delete nodeValue;
    }

private:
    const DOMString baseURI;
    mutable NodeList childNodes;
    const DOMString name;
    const NodeType nodeType;
    DOMString *nodeValue;
    Document *owner;
    Node *parent;
};

#endif //FEATHER_NODE_H
