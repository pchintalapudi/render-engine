//
// Created by prem on 11/10/2018.
//

#include <algorithm>
#include <deque>
#include <sstream>
#include "include/nodes/node.h"
#include "include/nodes/element.h"
#include "include/nodes/document.h"

Element *Node::getParentElement() const {
    return parent && parent->getNodeType() == NodeType::ELEMENT_NODE ? static_cast<Element *>(parent) : nullptr;
}

Node *Node::getNextSibling() const {
    if (parent) {
        auto idx = std::find(parent->childNodes.begin(), parent->childNodes.end(), this);
        if (idx < parent->childNodes.end() - 1) {
            return parent->childNodes[idx - parent->childNodes.begin() + 1];
        }
    }
    return nullptr;
}

Node *Node::getPreviousSibling() const {
    if (parent) {
        auto idx = std::find(parent->childNodes.begin(), parent->childNodes.end(), this);
        if (idx > parent->childNodes.begin()) {
            return parent->childNodes[idx - parent->childNodes.begin() - 1];
        }
    }
    return nullptr;
}

Node *Node::appendChild(Node *child) {
    if (nodeType == NodeType::ELEMENT_NODE || nodeType == NodeType::DOCUMENT_FRAGMENT_NODE ||
        (nodeType == NodeType::DOCUMENT_NODE && childNodes.size() < 2))
        if (child->nodeType != NodeType::DOCUMENT_FRAGMENT_NODE && child->nodeType != NodeType::ATTRIBUTE_NODE &&
            child->nodeType != NodeType::DOCUMENT_NODE) {
            childNodes.push_back(child);
            child->parent = this;
            child->owner = owner;
        } else if (child->childNodes.size() > 0) {
            if (child->nodeType == NodeType::DOCUMENT_NODE) {
                auto nextChild = childNodes[1];
                childNodes.push_back(nextChild);
                nextChild->parent = this;
                nextChild->owner = this->owner;
            } else if (child->nodeType == NodeType::DOCUMENT_FRAGMENT_NODE) {
                for (auto nextChild : child->childNodes) {
                    nextChild->parent = this;
                    nextChild->owner = this->owner;
                    childNodes.push_back(nextChild);
                }
            }
            child->childNodes.clear();
        }
    return child;
}

bool Node::contains(const Node *other) const {
    if (other == this) {
        return true;
    }
    for (auto child : childNodes) {
        if (child->contains(other)) {
            return true;
        }
    }
    return false;
}

Node *Node::getRootNode() const {
    if (owner) {
        return owner;
    }
    Node *node = parent;
    if (node)
        while (node->parent) {
            node = node->parent;
        }
    else return const_cast<Node *>(this);
}

void Node::insertBefore(Node *child) {
    if (parent) {
        if (child->parent)
            child->parent->removeChild(child);
        child->parent = parent;
        child->owner = owner;
        parent->childNodes.insert(std::find(parent->childNodes.begin(), parent->childNodes.end(), this), child);
    }
}

void Node::normalize() {
    std::deque<DOMString> texts;
    for (unsigned long i = childNodes.size() - 1; i > 0; i--) {
        if (childNodes[i]->nodeType == NodeType::TEXT_NODE) {
            if (childNodes[i - 1]->nodeType == NodeType::TEXT_NODE) {
                texts.push_front(childNodes[i]->nodeValue);
            } else {
                std::for_each(childNodes.begin() + i, childNodes.begin() + i + texts.size(),
                              [](auto text) { delete text; });
                childNodes.erase(childNodes.begin() + i, childNodes.begin() + i + texts.size());
                std::stringstream sstream;
                std::for_each(texts.rbegin(), texts.rend(), [&sstream](auto text) { sstream << text; });
                DOMString concatenated = sstream.str();
                if (!std::all_of(concatenated.begin(), concatenated.end(), isspace)) {
                    //TODO: Insert a new text node here
                }
                texts.clear();
            }
        }
    }
    if (texts.size()) {
        std::for_each(childNodes.begin(), childNodes.begin() + texts.size(),
                      [](auto text) { delete text; });
        childNodes.erase(childNodes.begin(), childNodes.begin() + texts.size());
        std::stringstream sstream;
        std::for_each(texts.rbegin(), texts.rend(), [&sstream](auto text) { sstream << text; });
        DOMString concatenated = sstream.str();
        if (!std::all_of(concatenated.begin(), concatenated.end(), isspace)) {
            //TODO: Insert a new text node here
        }
    } else if (childNodes[0]->nodeType == NodeType::TEXT_NODE) {
        DOMString text = childNodes[0]->nodeValue;
        if (std::all_of(text.begin(), text.end(), isspace)) {
            delete childNodes[0];
            childNodes.erase(childNodes.begin());
        }
    }
}

void Node::removeChild(Node *child) {
    child->parent = nullptr;
    child->owner = nullptr;
    childNodes.erase(std::remove(childNodes.begin(), childNodes.end(), child), childNodes.end());
}

void Node::replaceChild(Node *replacement, Node *target) {
    replacement->parent = this;
    target->parent = nullptr;
    replacement->owner = owner;
    target->owner = nullptr;
    std::replace(childNodes.begin(), childNodes.end(), target, replacement);
}

const Node *traverseTree(Node *root, const Node *node1, const Node *node2) {
    for (auto child : root->getChildNodes()) {
        if (child == node1) {
            return node1;
        } else if (child == node2) {
            return node2;
        }
        auto pointer = traverseTree(child, node1, node2);
        if (pointer) {
            return pointer;
        }
    }
    return nullptr;
}

unsigned char Node::compareDocumentPosition(const Node *other) const {
    if (this == other) {
        return 0;
    }
    if (!other || other->getRootNode() != getRootNode()) {
        return 1 | (1 << 5) | (1 << (this < other ? 1 : 2));
    }
    if (other->contains(this)) {
        return (1 << 1) | (1 << 3);
    }
    if (this->contains(other)) {
        return (1 << 2) | (1 << 4);
    }
    auto pointer = traverseTree(owner, other, this);
    if (pointer && other == pointer) {
        return 1 << 1;
    }
    return 1 << 2;
}
