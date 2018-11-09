//
// Created by prem on 11/7/2018.
//

#include <algorithm>
#include <cstdarg>
#include "include/nodes/character_data.h"
#include "include/nodes/document.h"
#include "include/nodes/node.h"

class Element;
CharacterData::CharacterData(CharacterData &other) : Node(other) {
    this->data = other.data;
}

void CharacterData::remove() {
    auto parent = this->getParentNode();
    if (parent) {
        parent->removeChild(*this);
    }
}

void CharacterData::before(std::vector<Node *> &toInsert) {
    auto parent = this->getParentNode();
    if (parent) {
        auto children = parent->getChildNodes();
        if (children) {
            auto it = std::find(children->begin(), children->end(), this);
            children->insert(it, toInsert.begin(), toInsert.end());
        }
    }
}

void CharacterData::before(Node *toInsert) {
    auto parent = this->getParentNode();
    if (parent) {
        parent->insertBefore(*toInsert, this);
    }
}

void CharacterData::after(std::vector<Node *> &toInsert) {
    auto parent = this->getParentNode();
    if (parent) {
        auto children = parent->getChildNodes();
        if (children) {
            auto it = std::find(children->begin(), children->end(), this);
            if (it != children->end()) {
                children->insert(it + 1, toInsert.begin(), toInsert.end());
            } else {
                children->insert(children->end() - 1, toInsert.begin(), toInsert.end());
            }
        }
    }
}

void CharacterData::after(Node *toInsert) {
    auto parent = this->getParentNode();
    if (parent) {
        auto children = parent->getChildNodes();
        if (children) {
            auto it = std::find(children->begin(), children->end(), this);
            if (it != children->end()) {
                children->insert(it + 1, toInsert);
            } else {
                children->push_back(toInsert);
            }
        }
    }
}

void CharacterData::replaceWith(std::vector<Node *> &toInsert) {
    auto parent = this->getParentNode();
    if (parent) {
        auto children = parent->getChildNodes();
        if (children) {
            auto it = std::find(children->begin(), children->end(), this);
            children->insert(it, toInsert.begin(), toInsert.end());
            auto idx = toInsert.size() + it;
            if (idx - children->begin() < children->size()) {
                children->erase(idx);
            }
        }
    }
}

void CharacterData::replaceWith(Node *toInsert) {
    auto parent = this->getParentNode();
    if (parent) {
        auto children = parent->getChildNodes();
        if (children) {
            auto it = std::find(children->begin(), children->end(), this);
            (*children)[it - children->begin()] = toInsert;
        }
    }
}

Element *CharacterData::previousElementSibling() const {
    auto parent = this->getParentNode();
    if (parent) {
        auto children = parent->getChildNodes();
        if (children) {
            for (unsigned long i = 0; i < children->size() && (*children)[i] != this; i++) {
                if ((*children)[i]->getNodeType() == ELEMENT_NODE) {
                    return dynamic_cast<Element *>((*children)[i]);
                }
            }
        }
    }
    return nullptr;
}