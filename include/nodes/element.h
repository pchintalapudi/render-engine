//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_ELEMENT_H
#define FEATHER_ELEMENT_H

#include "include/utils/named_node_map.h"
#include "include/nodes/interfaces/child_node.h"
#include "include/nodes/interfaces/parent_node.h"
#include "include/nodes/interfaces/non_document_type_child_node.h"
#include "node.h"

class Element : public Node, public ChildNode, public ParentNode, public NonDocumentTypeChildNode {
public:
    void setId(DOMString id) {
        this->id = id;
    }
    DOMString getId() {
        return id;
    }
    bool matchesSelector(DOMString selector) {

    }
    ~Element() {
        delete before;
        delete after;
    }
private:
    HTMLCollection elementChildren;
    DOMString id;
    Node *before;
    Node *after;
    std::vector<DOMString> pseudoClasses;
};

#endif //FEATHER_ELEMENT_H
