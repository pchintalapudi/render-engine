//
// Created by prem on 11/10/2018.
//

#include "include/nodes/character_data.h"
#include "include/nodes/element.h"

void dom::CharacterData::deleteData(unsigned long offset, unsigned long length) {
    setData(getData().substr(0, offset) + getData().substr(offset + length, getLength() - offset - length));
}

void dom::CharacterData::insertData(unsigned long offset, DOMString toInsert) {
    setData(getData().substr(0, offset) + toInsert + getData().substr(offset, getLength() - offset));
}

void dom::CharacterData::replaceData(unsigned long offset, unsigned long length, DOMString replacement) {
    setData(getData().substr(0, offset) + replacement +
            getData().substr(offset + length, getLength() - offset - length));
}

dom::Element *dom::CharacterData::getPreviousElementSibling() const {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        bool found = false;
        for (unsigned long idx = children.size(); idx-- > 0;) {
            auto child = children.get(idx);
            if (found && child->getNodeType() == NodeType::ELEMENT_NODE) return static_cast<Element *>(child);
            if (child == this) found = true;
        }
    }
    return nullptr;
}

dom::Element *dom::CharacterData::getNextElementSibling() const {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        Element *found = nullptr;
        for (unsigned long idx = children.size(); idx-- > 0;) {
            auto child = children.get(idx);
            if (child == this) return found;
            if (child->getNodeType() == NodeType::ELEMENT_NODE) found = static_cast<Element *>(child);
        }
    }
    return nullptr;
}

void dom::CharacterData::before(std::vector<Node *> &nodes) {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        std::for_each(nodes.begin(), nodes.end(), [this](Node *child) {
            if (child->getParentNode())
                child->getParentNode()->removeChild(child);
            child->setParentNode(getParentNode());
        });
        children.insertAll(children.indexOf(this) + 1, nodes);
    }
}

void dom::CharacterData::after(Node *node) {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        if (node->getParentNode())
            node->getParentNode()->removeChild(node);
        node->setParentNode(getParentNode());
        children.insert(children.indexOf(this) + 1, node);
    }
}

void dom::CharacterData::after(std::vector<Node *> &nodes) {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        std::for_each(nodes.begin(), nodes.end(), [this](Node *child) {
            if (child->getParentNode())
                child->getParentNode()->removeChild(child);
            child->setParentNode(getParentNode());
        });
        children.insertAll(children.indexOf(this) + 1, nodes);
    }
}

void dom::CharacterData::replaceWith(Node *node) {
    if (getParentNode())
        getParentNode()->replaceChild(node, this);
}

void dom::CharacterData::replaceWith(std::vector<Node *> &nodes) {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        auto idx = children.indexOf(this);
        children.remove(idx);
        std::for_each(nodes.begin(), nodes.end(), [this](Node *child) {
            if (child->getParentNode())
                child->getParentNode()->removeChild(child);
            child->setParentNode(getParentNode());
        });
        children.insertAll(idx, nodes);
        setParentNode(nullptr);
    }
}

void dom::CharacterData::remove() {
    if (getParentNode()) {
        getParentNode()->getChildNodes().remove(this);
        setParentNode(nullptr);
    }
}