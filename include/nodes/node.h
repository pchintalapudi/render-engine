//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_INODE_H
#define FEATHER_INODE_H

#include <algorithm>
#include <string>
#include <vector>
#include "include/events/event_target.h"
#include "node_type.h"

class Element;

class Document;

class Node : public EventTarget {
public:

    Node(std::string baseURI, std::string name, NodeType nodeType, Document *owner, Node *parent);

    Node(Node &other);

    //Properties
    inline std::string getBaseURI() const { return this->baseURI; };

    inline std::vector<Node *> *const getChildNodes() { return this->childNodes; };

    Node *getFirstChild() const;

    const bool isConnected() const;

    Node *getLastChild() const;

    Node *const getNextSibling() const;

    inline std::string getName() const { return this->name; };

    inline const unsigned short getNodeType() const { return this->nodeType; };

    virtual const std::string *getNodeValue() const { return nullptr; };

    virtual void setNodeValue(std::string *value) {};

    const Document *getOwnerDocument() const { return this->owner; };

    Node *const getParentNode() const { return this->parent; };

    inline Element *const getParentElement() const;

    Node *const getPreviousSibling() const;

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

    const unsigned char compareDocumentPosition(Node const &other) const;

    const bool contains(Node const &other) const;

    inline Node *const getRootNode() const;

    const inline bool hasChildNodes() const {
        auto children = this->childNodes;
        return children && children->size();
    }

    void insertBefore(Node &child, Node *ref);

    inline const bool isDefaultNamespace(std::string const &uri) const { return uri == this->nameSpace; }

    virtual const bool isEqualNode(Node const &other) const = 0;

    inline const bool isSameNode(Node &other) const {
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

    virtual ~Node() {}

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
