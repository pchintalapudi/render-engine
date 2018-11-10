//
// Created by prem on 11/10/2018.
//

#include "include/nodes/character_data.h"
#include "include/nodes/element.h"

void CharacterData::deleteData(unsigned long offset, unsigned long length) {
    setData(getData().substr(0, offset) + getData().substr(offset + length, getLength() - offset - length));
}

void CharacterData::insertData(unsigned long offset, DOMString toInsert) {
    setData(getData().substr(0, offset) + toInsert + getData().substr(offset, getLength() - offset));
}

void CharacterData::replaceData(unsigned long offset, unsigned long length, DOMString replacement) {
    setData(getData().substr(0, offset) + replacement +
            getData().substr(offset + length, getLength() - offset - length));
}

Element *CharacterData::getPreviousElementSibling() const {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        auto idx = std::find(children.begin(), children.end(), this);
        for (unsigned long i = idx - children.begin(); i > -1; i--) {
            auto child = children[i];
            if (child->getNodeType() == NodeType::ELEMENT_NODE) {
                return static_cast<Element *>(child);
            }
        }
    }
    return nullptr;
}

Element *CharacterData::getNextElementSibling() const {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        auto idx = std::find(children.begin(), children.end(), this);
        for (unsigned long i = idx - children.end(); i < children.size(); i++) {
            auto child = children[i];
            if (child->getNodeType() == NodeType::ELEMENT_NODE) {
                return static_cast<Element *>(child);
            }
        }
    }
    return nullptr;
}

void CharacterData::before(std::vector<Node *> &nodes) {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        std::for_each(nodes.begin(), nodes.end(), [this](Node *child) {
            if (child->getParentNode())
                child->getParentNode()->removeChild(child);
            child->setParentNode(getParentNode());
            child->setOwner(getOwner());
        });
        children.insert(std::find(children.begin(), children.end(), this), nodes.begin(), nodes.end());
    }
}

void CharacterData::after(Node *node) {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        if (node->getParentNode())
            node->getParentNode()->removeChild(node);
        node->setParentNode(getParentNode());
        node->setOwner(getOwner());
        children.insert(std::find(children.begin(), children.end(), this) + 1, node);
    }
}

void CharacterData::after(std::vector<Node *> &nodes) {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        std::for_each(nodes.begin(), nodes.end(), [this](Node *child) {
            if (child->getParentNode())
                child->getParentNode()->removeChild(child);
            child->setParentNode(getParentNode());
            child->setOwner(getOwner());
        });
        children.insert(std::find(children.begin(), children.end(), this), nodes.begin(), nodes.end());
    }
}

void CharacterData::replaceWith(Node *node) {
    if (getParentNode())
        getParentNode()->replaceChild(node, this);
}

void CharacterData::replaceWith(std::vector<Node *> &nodes) {
    if (getParentNode()) {
        auto children = getParentNode()->getChildNodes();
        auto idx = std::find(children.begin(), children.end(), this);
        children.erase(idx);
        std::for_each(nodes.begin(), nodes.end(), [this](Node *child) {
            if (child->getParentNode())
                child->getParentNode()->removeChild(child);
            child->setParentNode(getParentNode());
            child->setOwner(getOwner());
        });
        children.insert(idx, nodes.begin(), nodes.end());
        setParentNode(nullptr);
        setOwner(nullptr);
    }
}