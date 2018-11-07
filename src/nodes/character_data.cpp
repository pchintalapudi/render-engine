//
// Created by prem on 11/7/2018.
//

#include <algorithm>
#include "include/nodes/character_data.h"

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

void CharacterData::replaceWith(std::vector<Node *> &toInsert) {
    auto parent = this->getParentNode();
    if (parent) {
        auto children = parent->getChildNodes();
        if (children) {
            auto it = std::find(children->begin(), children->end(), this);
            children->insert(it, toInsert.begin(), toInsert.end());
            auto idx = toInsert.size() + it;
            if (idx < children->size()) {

            }
        }
    }
}