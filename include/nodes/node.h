//
// Created by prem on 11/9/2018.
//

#ifndef FEATHER_NODE_H
#define FEATHER_NODE_H

#include "include/events/event_target.h"
#include "node_type.h"

class Element;

class Document;

class Node : public EventTarget {
public:

    Node(DOMString baseURI, DOMString name, NodeType nodeType, Document *owner, Node *parent)
            : baseURI(baseURI), name(name), nodeType(nodeType), owner(owner), parent(parent) {}

    Node(DOMString name, NodeType nodeType, Node &parent)
            : baseURI(parent.baseURI), name(name), nodeType(nodeType), owner(parent.owner), parent(&parent) {}

    inline DOMString getBaseURI() { return baseURI; }

    inline std::vector<Node *> getChildNodes() { return childNodes; }

    inline Node *getFirstChild() { return childNodes.size() ? childNodes[0] : nullptr; }

    inline bool isConnected() { return owner != nullptr; }

    inline Node *getLastChild() { return childNodes.size() ? childNodes[childNodes.size() - 1] : nullptr; }

    Node *getNextSibling();

    inline DOMString getNodeName() { return name; }

    inline NodeType getNodeType() { return nodeType; }

    inline DOMString getNodeValue() { return nodeValue; }

    inline void setNodeValue(DOMString value) { nodeValue = value; }

    inline Document *getOwner() { return owner; }

    inline void setOwner(Document *owner) { this->owner = owner; }

    inline Node *getParentNode() { return parent; }

    inline void setParentNode(Node *parent) { this->parent = parent; }

    inline Element *getParentElement();

    Node *getPreviousSibling();

    Node *appendChild(Node *child);

    virtual Node *cloneNode() = 0;

    unsigned char compareDocumentPosition(Node *other);

    bool contains(Node *other);

    Node *getRootNode();

    inline bool hasChildNodes() { return childNodes.size() != 0; }

    void insertBefore(Node *child);

    virtual bool isEqualNode(Node *other) = 0;

    inline bool isSameNode(Node *other) { return this == other; }

    void normalize();

    void removeChild(Node *child);

    void replaceChild(Node *replacement, Node *target);

    virtual ~Node() override {
        for (auto child : childNodes) {
            delete child;
        }
    }

private:
    const DOMString baseURI;
    std::vector<Node *> childNodes;
    const DOMString name;
    const NodeType nodeType;
    DOMString nodeValue;
    Document *owner;
    Node *parent;
};

#endif //FEATHER_NODE_H
