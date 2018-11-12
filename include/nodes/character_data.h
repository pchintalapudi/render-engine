//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_CHARACTER_DATA_H
#define FEATHER_CHARACTER_DATA_H

#include "include/nodes/interfaces/child_node.h"
#include "include/nodes/interfaces/non_document_type_child_node.h"
#include "node.h"

class CharacterData : public Node, public ChildNode, public NonDocumentTypeChildNode {
public:
    CharacterData(DOMString baseURI, DOMString name, NodeType nodeType, Document *owner, Node *parent)
            : Node(baseURI, name, nodeType, owner, parent) {}

    CharacterData(DOMString name, NodeType nodeType, Node &parent) : Node(name, nodeType, parent) {}

    inline DOMString getData() const { return *getNodeValue(); }

    inline void setData(DOMString data) { setNodeValue(data); }

    inline unsigned long getLength() const { return getData().length(); }

    inline void appendData(DOMString toAppend) { setData(getData() + toAppend); }

    void deleteData(unsigned long offset, unsigned long length);

    void insertData(unsigned long offset, DOMString toInsert);

    void replaceData(unsigned long offset, unsigned long length, DOMString replacement);

    inline DOMString substringData(unsigned long offset, unsigned long length) const {
        return getData().substr(offset, length);
    }

    void remove();

    inline void before(Node *node) override { insertBefore(node); };

    void before(std::vector<Node *> &nodes) override;

    void after(Node *node) override;

    void after(std::vector<Node *> &nodes) override;

    void replaceWith(Node *node) override;

    void replaceWith(std::vector<Node *> &nodes) override;

    Element *getPreviousElementSibling() const override;

    Element *getNextElementSibling() const override;
};

#endif //FEATHER_CHARACTER_DATA_H
