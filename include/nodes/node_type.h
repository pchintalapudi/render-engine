//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_NODETYPE_H
#define FEATHER_NODETYPE_H
enum NodeType {
    NOT_A_NODE,//This is there to make all offsets correct (the spec defines them as starting from 1)
    ELEMENT_NODE,
    ATTRIBUTE_NODE,
    TEXT_NODE,
    CDATA_SECTION_NODE,
    ENTITY_REFERENCE_NODE,
    ENTITY_NODE,
    PROCESSING_INSTRUCTION_NODE,
    COMMENT_NODE,
    DOCUMENT_NODE,
    DOCUMENT_TYPE_NODE,
    DOCUMENT_FRAGMENT_NODE,
    NOTATION_NODE
};
#endif //FEATHER_NODETYPE_H
