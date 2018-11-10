//
// Created by prem on 11/9/2018.
//

#ifndef FEATHER_NODE_TYPE_H
#define FEATHER_NODE_TYPE_H
enum class NodeType {
    NOT_A_NODE,//Not a real value, but the indeces match only if this is the first enum constant
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
#endif //FEATHER_NODE_TYPE_H
