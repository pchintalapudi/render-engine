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

    Node(DOMString baseURI, DOMString name, NodeType nodeType, Node *parent)
            : baseURI(baseURI), name(name), nodeType(nodeType), parent(parent), ownerValid(false) {}

    Node(DOMString name, NodeType nodeType, Node &parent)
            : baseURI(parent.baseURI), name(name), nodeType(nodeType), parent(&parent), ownerValid(false) {}

    inline DOMString getBaseURI() const { return baseURI; }

    inline NodeList &getChildNodes() const { return childNodes; }

    inline Node *getFirstChild() const { return childNodes.size() ? childNodes.get(0) : nullptr; }

    inline bool isConnected() const { return getOwner() != nullptr; }

    inline Node *getLastChild() const { return childNodes.size() ? childNodes.get(childNodes.size() - 1) : nullptr; }

    Node *getNextSibling() const;

    inline DOMString getNodeName() const { return name; }

    inline NodeType getNodeType() const { return nodeType; }

    inline DOMString *getNodeValue() const { return nodeValue; }

    inline void setNodeValue(DOMString value) { nodeValue = new DOMString(value); }

    Document *getOwner() const;

    inline Node *getParentNode() const { return parent; }

    void setParentNode(Node *parent);

    Element *getParentElement() const;

    Node *getPreviousSibling() const;

    Node *appendChild(Node *child);

    virtual Node *cloneNode() = 0;

    unsigned char compareDocumentPosition(const Node *other) const;

    bool contains(const Node *other) const;

    Node *getRootNode() const;

    inline bool hasChildNodes() const { return childNodes.size() != 0; }

    Node *insertBefore(Node *child);

    Node *insertAfter(Node *child);

    virtual bool isEqualNode(const Node *other) const = 0;

    inline bool isSameNode(const Node *other) const { return this == other; }

    void normalize();

    void removeChild(Node *child);

    void replaceChild(Node *replacement, Node *target);

    std::vector<Node *> buildDispatchChain() const;

    ~Node() override;

protected:
    inline long long edit(long long) const override {
        return observable::generate(observable::EventType::INTERNAL_CHANGE);
    }

    bool handle(long long l) const override;

    Node *getThis() const { return thisRef; }

private:
    const DOMString baseURI;
    mutable NodeList childNodes;
    const DOMString name;
    const NodeType nodeType;
    DOMString *nodeValue;
    Node *parent;
    mutable Document *owner;
    mutable bool ownerValid;
    Node *const thisRef = this;//Quick and dirty override for getting around const correctness

    inline Document *computeOwner() const {
        return !parent || nodeType == NodeType::DOCUMENT_NODE ? nullptr : parent->getOwner();
    }
};

#endif //FEATHER_NODE_H
