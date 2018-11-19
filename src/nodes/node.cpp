//
// Created by prem on 11/10/2018.
//

#include <algorithm>
#include <deque>
#include <sstream>
#include "include/nodes/text.h"
#include "include/nodes/node.h"
#include "include/nodes/element.h"
#include "include/nodes/document.h"

dom::Element *dom::Node::getParentElement() const {
    return parent && parent->getNodeType() == NodeType::ELEMENT_NODE ? static_cast<Element *>(parent) : nullptr;
}

dom::Node *dom::Node::getNextSibling() const {
    if (parent) {
        auto idx = parent->childNodes.indexOf(this);
        if (~idx && idx < parent->childNodes.size() - 1) {
            return parent->childNodes.get(idx + 1);
        }
    }
    return nullptr;
}

dom::Node *dom::Node::getPreviousSibling() const {
    if (parent) {
        auto idx = parent->childNodes.indexOf(this);
        if (~idx && idx > 0) {
            return parent->childNodes.get(idx - 1);
        }
    }
    return nullptr;
}

void switchParent(dom::Node *switched, dom::Node *parent) {
    switched->getParentNode()->removeChild(switched);
    switched->setParentNode(parent);
    switched->setOwner(parent->getOwner());
}

dom::Node *dom::Node::appendChild(Node *child) {
    if (nodeType == NodeType::ELEMENT_NODE || nodeType == NodeType::DOCUMENT_FRAGMENT_NODE ||
        (nodeType == NodeType::DOCUMENT_NODE && childNodes.size() < 2)) {
        if (child->nodeType != NodeType::DOCUMENT_FRAGMENT_NODE
            && child->nodeType != NodeType::ATTRIBUTE_NODE
            && child->nodeType != NodeType::DOCUMENT_NODE) {
            childNodes.push_back(child);
            switchParent(child, this);
        } else if (child->childNodes.size() > 0) {
            if (child->nodeType == NodeType::DOCUMENT_NODE) {
                auto nextChild = child->childNodes.get(1);
                childNodes.push_back(nextChild);
                switchParent(nextChild, this);
            } else if (child->nodeType == NodeType::DOCUMENT_FRAGMENT_NODE) {
                child->childNodes.forEach([this](Node *node) {
                    switchParent(node, this);
                    childNodes.push_back(node);
                });
//                for (auto nextChild : child->childNodes) {
//                    nextChild->parent = this;
//                    nextChild->owner = this->owner;
//                    childNodes.push_back(nextChild);
//                }
            }
            child->childNodes.clear();
        }
    }
    return child;
}

bool dom::Node::contains(const dom::Node *other) const {
    if (other == this) {
        return true;
    }
    return childNodes.anyMatch([other](Node *child) { return child->contains(other); });
}

dom::Node *dom::Node::getRootNode() const {
    if (owner) {
        return owner;
    }
    Node *node = parent;
    if (node)
        while (node->parent) {
            node = node->parent;
        }
    return const_cast<Node *>(this);
}

void dom::Node::insertBefore(Node *child) {
    if (parent) {
        if (child->parent)
            child->parent->removeChild(child);
        child->parent = parent;
        child->owner = owner;
        parent->childNodes.insert(parent->childNodes.indexOf(this), child);
    }
}

void dom::Node::normalize() {
    unsigned long size = 0;
    for (unsigned long i = childNodes.size() - 1; i > 0; i--) {
        if (childNodes.get(i)->nodeType == NodeType::TEXT_NODE) {
            if (childNodes.get(i - 1)->nodeType != NodeType::TEXT_NODE) {
                std::vector<Node *> erased = childNodes.erase(i, i + size);
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
        std::vector<Node *> erased = childNodes.erase(0, size);
        std::stringstream sstream;
        std::for_each(erased.begin(), erased.end(), [&sstream](Node *erase) {
            sstream << erase->getNodeValue();
            delete erase;
        });
        DOMString concatenated = sstream.str();
        if (!std::all_of(concatenated.begin(), concatenated.end(), isspace)) {
            childNodes.insert(0, new Text(*this, concatenated));
        }
    } else if (childNodes.get(0)->nodeType == NodeType::TEXT_NODE) {
        DOMString text = *childNodes.get(0)->nodeValue;
        if (std::all_of(text.begin(), text.end(), isspace)) {
            delete childNodes.erase(0);
        }
    }
}

void dom::Node::removeChild(Node *child) {
    child->parent = nullptr;
    child->owner = nullptr;
    childNodes.erase(child);
}

void dom::Node::replaceChild(Node *replacement, Node *target) {
    replacement->parent = this;
    target->parent = nullptr;
    replacement->owner = owner;
    target->owner = nullptr;
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
