//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_TEXT_H
#define FEATHER_TEXT_H

#include "include/nodes/interfaces/slotable.h"
#include "character_data.h"

class Text : public CharacterData, public Slotable {
public:
    Text(DOMString baseURI, Document *owner, Node *parent)
            : CharacterData(baseURI, "#text", NodeType::TEXT_NODE, owner, parent) {}

    explicit Text(Node &parent) : CharacterData("#text", NodeType::TEXT_NODE, parent) {}

    Text(DOMString baseURI, Document *owner, Node *parent, DOMString text) : Text(baseURI, owner, parent) {
        setData(text);
    }

    Text(Node &parent, DOMString text) : Text(parent) { setData(text); }

    DOMString getWholeText() const;

    Text *cloneNode() override;

    bool isEqualNode(const Node *other) const override;

protected:
    //This is necessary for CDataSection to provide a correct name attribute
    Text(DOMString baseURI, DOMString name, NodeType nodeType, Document *owner, Node *parent)
            : CharacterData(baseURI, name, nodeType, owner, parent) {}

    Text(DOMString name, NodeType nodeType, Node &parent) : CharacterData(name, nodeType, parent) {}
};

#endif //FEATHER_TEXT_H
