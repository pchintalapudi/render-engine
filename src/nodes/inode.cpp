//
// Created by prem on 11/7/2018.
//

#include "include/nodes/inode.h"

INode *INode::firstChild() {
    std::vector<INode *> *children = this->childNodes();
    return children ? (children->size()) ? children->front() : nullptr : nullptr;
};

INode *INode::lastChild() {
    std::vector<INode *> *children = this->childNodes();
    return children ? children->back() : nullptr;
};

void INode::insertBefore(INode &child, INode *ref) {
    auto children = this->childNodes();
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

void INode::removeChild(INode const &child) {
    auto children = this->childNodes();
    if (children) {
        auto it = std::find(children->begin(), children->end(), &child);
        if (it != children->end()) {
            children->erase(it);
        }
    }
};

void INode::replaceChild(INode &newChild, INode &oldChild) {
    auto children = this->childNodes();
    if (children) {
        std::replace(children->begin(), children->end(), &oldChild, &newChild);
    }
}



