//
// Created by prem on 11/7/2018.
//

#include <algorithm>
#include <vector>
#include "include/nodes/text.h"
#include "include/nodes/document.h"
#include "include/nodes/node.h"
#include "include/nodes/element.h"

Node::Node(Node &other) {
    this->baseURI = other.baseURI;
    this->childNodes = other.childNodes ? new std::vector(*(other.childNodes)) : nullptr;
    this->name = other.name;
    this->nodeType = other.nodeType;
    this->owner = other.owner;
    this->parent = other.parent;
    this->textContent = other.textContent;
}

const bool Node::isConnected() const {
    auto parent = this->parent;
    while (parent && parent->nodeType != DOCUMENT_NODE) {
        parent = parent->parent;
    }
    return parent != nullptr;
}

Node *const Node::getNextSibling() const {
    auto parent = this->parent;
    if (parent) {
        std::vector<Node *> *children = parent->childNodes;
        if (children) {
            auto it = std::find(children->begin(), children->end(), this);
            if (it != children->end() && it - children->begin() < children->size() - 1) {
                return (*children)[it - children->begin() + 1];
            }
        }
    }
    return nullptr;
}

Node *const Node::getPreviousSibling() const {
    auto parent = this->parent;
    if (parent) {
        std::vector<Node *> *children = parent->childNodes;
        if (children) {
            auto it = std::find(children->begin(), children->end(), this);
            if (it != children->end() && it - children->begin() > 0) {
                return (*children)[it - children->begin() - 1];
            }
        }
    }
    return nullptr;
}

Element *const Node::getParentElement() const {
    Node *parent = this->parent;
    return parent && parent->getNodeType() == ELEMENT_NODE ? dynamic_cast<Element *>(parent) : nullptr;
}

Node *Node::getFirstChild() const {
    const std::vector<Node *> *children = this->childNodes;
    return children ? (children->size()) ? children->front() : nullptr : nullptr;
};

Node *Node::getLastChild() const {
    std::vector<Node *> *children = this->childNodes;
    return children ? children->back() : nullptr;
};

const unsigned char Node::compareDocumentPosition(Node const &other) const {
    //Idk what this is supposed to do so we'll be noncompliant here
    //TODO: make this method
    return 0;
}

const bool Node::contains(Node const &other) const {
    if (this == &other) return true;
    auto children = this->childNodes;
    if (children) {
        for (unsigned long i = 0; i < children->size(); i++) {
            if ((*children)[i]->contains(other)) {
                return true;
            }
        }
    }
    return false;
}

Node *const Node::getRootNode() const {
    return this->owner ? this->owner : nullptr;
}

void Node::insertBefore(Node &child, Node *ref) {
    auto children = this->getChildNodes();
    if (children)
        if (ref) {
            auto it = std::find(children->begin(), children->end(), &child);
            if (it != children->end()) {
                children->insert(it, &child);
                return;
            }
        }
    this->appendChild(child);
}

void Node::normalize() {
    auto children = this->childNodes;
    if (children) {
        for (unsigned long i = children->size() - 1; i > -1; i--) {
            if ((*children)[i]->nodeType == ELEMENT_NODE) {
                (*children)[i]->normalize();
            } else if ((*children)[i]->nodeType == TEXT_NODE) {
                if (i > 0 && (*children)[i - 1]->nodeType == TEXT_NODE) {
                    (*children)[i - 1] = nullptr;//new Text()
                    children->erase(children->begin() + i);
                } else {
                    auto text = (*children)[i]->textContent;
                    if (std::all_of(text.begin(), text.end(), isspace)) {
                        children->erase(children->begin() + i);
                    }
                }
            }
        }
    }
}

void Node::removeChild(Node const &child) {
    auto children = this->getChildNodes();
    if (children) {
        auto it = std::find(children->begin(), children->end(), &child);
        if (it != children->end()) {
            children->erase(it);
        }
    }
};

void Node::replaceChild(Node &newChild, Node &oldChild) {
    auto children = this->getChildNodes();
    if (children) {
        std::replace(children->begin(), children->end(), &oldChild, &newChild);
    }
}
