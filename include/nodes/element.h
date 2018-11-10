//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_ELEMENT_H
#define FEATHER_ELEMENT_H

#include "include/nodes/interfaces/child_node.h"
#include "include/nodes/interfaces/parent_node.h"
#include "include/nodes/interfaces/non_document_type_child_node.h"
#include "node.h"

class Element : public Node, public ChildNode, public ParentNode, public NonDocumentTypeChildNode {
};

#endif //FEATHER_ELEMENT_H
