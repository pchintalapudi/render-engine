//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_INODE_H
#define FEATHER_INODE_H

#include <algorithm>
#include <string>
#include <vector>
#include "../events/event_target.h"
#include "node_type.h"

class Element;

class Document;

class Node : public EventTarget {
public:

    Node(std::string name, NodeType nodeType, Document *const owner, Node &parent)
            : Node(parent.getBaseURI(), name, nodeType, owner, &parent) {}

    Node(std::string baseURI, std::string name, NodeType nodeType, Document *owner, Node *parent);

    Node(Node &other);

    //Properties
    inline std::string getBaseURI() const { return this->baseURI; };

    inline std::vector<Node *> *const getChildNodes() { return this->childNodes; };

    Node *getFirstChild() const;

    bool isConnected() const;

    Node *getLastChild() const;

    Node *getNextSibling() const;

    inline std::string getNodeName() const { return this->name; };

    inline unsigned short getNodeType() const { return this->nodeType; };

    virtual std::string *getNodeValue() const { return nullptr; };

    virtual void setNodeValue(std::string *const value) {};

    Document *getOwnerDocument() const { return this->owner; };

    Node *getParentNode() const { return this->parent; };

    inline Element *getParentElement() const;

    Node *getPreviousSibling() const;

    std::string getTextContent() const;

    void setTextContent(std::string content);

    //Methods

    void appendChild(Node &child) {
        auto children = this->getChildNodes();
        if (children) {
            children->push_back(&child);
        }
    };

    virtual Node *cloneNode() = 0;

    unsigned char compareDocumentPosition(Node const &other) const;

    bool contains(Node const &other) const;

    inline Node *getRootNode() const;

    inline bool hasChildNodes() const {
        auto children = this->childNodes;
        return children && children->size();
    }

    void insertBefore(Node &child, Node *ref);

    inline bool isDefaultNamespace(std::string const &uri) const { return uri == this->nameSpace; }

    virtual bool isEqualNode(Node const &other) const = 0;

    inline bool isSameNode(Node &other) const {
        return this == &other;
    }

    std::string lookupPrefix(std::string nameSpace) const {
        //IDK what namespaces and prefixes are
        //TODO: Fix this
        return this->nameSpace;
    };

    std::string lookupNamespaceURI(std::string *prefix) const {
        //IDK what namespaces and prefixes are
        //TODO: Fix this too
        return this->nameSpace;
    };

    void normalize();

    void removeChild(Node const &child);

    void replaceChild(Node &newChild, Node &oldChild);

    void clearChildren() {
        if (childNodes)
            childNodes->clear();
    }

    void cleanupChildren() {
        if (childNodes)
            std::for_each(childNodes->begin(), childNodes->end(), [](auto child) { delete child; });
    }

    virtual ~Node() {
        cleanupChildren();
        delete childNodes;
    }

private:
    std::string baseURI;
    std::vector<Node *> *childNodes;
    std::string name;
    NodeType nodeType;
    Document *owner;
    Node *parent;
    std::string nameSpace;

    std::string getConcatText() const;
};

#endif //FEATHER_INODE_H
