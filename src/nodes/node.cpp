//
// Created by prem on 11/10/2018.
//

#include <algorithm>
#include <deque>
#include <sstream>
#include "include/nodes/text.h"
#include "include/nodes/element.h"
#include "include/nodes/document.h"

dom::Element *dom::Node::getParentElement() const {
    return parent && parent->getNodeType() == NodeType::ELEMENT_NODE ? static_cast<Element *>(parent) : nullptr;
}

dom::Node *dom::Node::getNextSibling() const {
    if (parent) {
        bool ret = false;
        for (unsigned long i = 0; i < childNodes.size(); i++) {
            auto child = childNodes.get(i);
            if (ret) return child;
            if (child == this) ret = true;
        }
    }
    return nullptr;
}

dom::Node *dom::Node::getPreviousSibling() const {
    if (parent) {
        Node *prev = nullptr;
        for (unsigned long i = 0; i < childNodes.size(); i++) {
            auto child = childNodes.get(i);
            if (child == this) return prev;
            prev = child;
        }
    }
    return nullptr;
}

void switchParent(dom::Node *switched, dom::Node *parent) {
    switched->getParentNode()->removeChild(switched);
    switched->setParentNode(parent);
}

dom::Node *dom::Node::appendChild(Node *child) {
    if (nodeType == NodeType::ELEMENT_NODE || nodeType == NodeType::DOCUMENT_FRAGMENT_NODE ||
        (nodeType == NodeType::DOCUMENT_NODE && childNodes.size() < 2)) {
        if (child->nodeType != NodeType::DOCUMENT_FRAGMENT_NODE
            && child->nodeType != NodeType::ATTRIBUTE_NODE
            && child->nodeType != NodeType::DOCUMENT_NODE) {
            childNodes.add(child);
            switchParent(child, this);
        } else if (child->childNodes.size() > 0) {
            if (child->nodeType == NodeType::DOCUMENT_NODE) {
                auto nextChild = child->childNodes.get(1);
                childNodes.add(nextChild);
                switchParent(nextChild, this);
            } else if (child->nodeType == NodeType::DOCUMENT_FRAGMENT_NODE) {
                childNodes.reserve(child->childNodes.size());
                child->childNodes.forEach([this](Node *node) {
                    switchParent(node, this);
                    childNodes.add(node);
                });
            }
            child->childNodes.clear();
        }
    }
    return child;
}

bool dom::Node::contains(const dom::Node *other) const {
    return other == this || childNodes.anyMatch([other](Node *child) { return child->contains(other); });
}

dom::Node *dom::Node::getRootNode() const {
    if (getOwner()) {
        return getOwner();
    }
    Node *node = parent;
    if (!node) return getThis();
    while (node->parent) node = node->parent;
    return node;
}

dom::Node *dom::Node::insertBefore(Node *child) {
    if (parent) {
        if (child->parent) child->parent->removeChild(child);
        child->setParentNode(parent);
        parent->childNodes.insert(parent->childNodes.indexOf(this), child);
        return child;
    }
    return nullptr;
}

dom::Node *dom::Node::insertAfter(dom::Node *child) {
    if (parent) {
        if (child->parent) child->parent->removeChild(child);
        child->setParentNode(parent);
        parent->childNodes.insert(parent->childNodes.indexOf(this) + 1, child);
        return child;
    }
    return nullptr;
}

void dom::Node::normalize() {
    unsigned long size = 0;
    for (unsigned long i = childNodes.size() - 1; i > 0; i--) {
        if (childNodes.get(i)->nodeType == NodeType::TEXT_NODE) {
            if (childNodes.get(i - 1)->nodeType != NodeType::TEXT_NODE) {
                std::vector<Node *> erased = childNodes.remove(i, i + size);
                std::stringstream sstream;
                std::for_each(erased.begin(), erased.end(),
                              [&sstream](Node *child) {
                                  sstream << child->getNodeValue();
                                  delete child;
                              });
                DOMString concatenated = sstream.str();
                if (!std::all_of(concatenated.begin(), concatenated.end(), isspace)) {
                    childNodes.insert(i, new Text(*this, concatenated));
                }
                size = 0;
            } else size++;
        }
    }
    if (size) {
        std::vector<Node *> erased = childNodes.remove(0, size);
        DOMString concat;
        for (auto node : erased) {
            concat += *node->getNodeValue();
            delete node;
        }
        if (!std::all_of(concat.begin(), concat.end(), isspace)) {
            childNodes.insert(0, new Text(*this, concat));
        }
    } else if (childNodes.get(0)->nodeType == NodeType::TEXT_NODE) {
        DOMString text = *childNodes.get(0)->nodeValue;
        if (std::all_of(text.begin(), text.end(), isspace)) {
            delete childNodes.remove(0);
        }
    }
}

void dom::Node::removeChild(Node *child) {
    child->setParentNode(nullptr);
    childNodes.remove(child);
}

void dom::Node::replaceChild(Node *replacement, Node *target) {
    replacement->setParentNode(this);
    target->setParentNode(nullptr);
    childNodes.set(childNodes.indexOf(target), replacement);
}

const dom::Node *traverseTree(dom::Node *root, const dom::Node *node1, const dom::Node *node2) {
    for (unsigned long i = 0; i < root->getChildNodes().size(); i++) {
        auto child = root->getChildNodes().get(i);
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

unsigned char dom::Node::compareDocumentPosition(const Node *other) const {
    if (this == other) {
        return 0;
    }
    if (!other || other->getRootNode() != getRootNode()) {
        return 1u | (1u << 5u) | (1u << (this < other ? 1u : 2u));
    }
    if (other->contains(this)) {
        return (1u << 1u) | (1u << 3u);
    }
    if (this->contains(other)) {
        return (1u << 2u) | (1u << 4u);
    }
    auto pointer = traverseTree(getOwner(), other, this);
    if (pointer && other == pointer) {
        return 1u << 1u;
    }
    return 1u << 2u;
}

bool dom::Node::handle(long long l) const {
    if (observable::present(l, observable::EventType::PARENT_CHANGE)) {
        ownerValid = false;
        long long parentChange = observable::generate(observable::EventType::PARENT_CHANGE);
        childNodes.forEach([parentChange](Node *child) { child->invalidate(parentChange); });
    }
    return true;
}

void dom::Node::setParentNode(dom::Node *parent) {
    invalidate(observable::generate(observable::EventType::PARENT_CHANGE));
    this->parent = parent;
}

dom::Document *dom::Node::getOwner() const {
    if (ownerValid) return owner;
    ownerValid = true;
    return owner = computeOwner();
}

std::vector<dom::Node *> dom::Node::buildDispatchChain() const {
    std::list<Node *> chain;
    Node *node = getThis();
    chain.push_front(node);
    while ((node = node->getParentNode()))chain.push_front(node);
    std::vector<Node *> dispatchChain;
    dispatchChain.reserve(chain.size());
    dispatchChain.insert(dispatchChain.begin(), chain.begin(), chain.end());
    return dispatchChain;
}

dom::Node::~Node() {
    childNodes.forEach([](auto child) { delete child; });
    delete nodeValue;
}
