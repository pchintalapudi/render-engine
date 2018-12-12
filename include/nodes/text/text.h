//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_TEXT_H
#define FEATHER_TEXT_H

#include "character_data.h"
#include "../slotable.h"

namespace feather {
    namespace dom {
        class Text : public CharacterData, public Slotable {
        public:

            Text(DOMString baseURI, StrongPointer<Node> parent)
                    : CharacterData(baseURI, "#text", NodeType::TEXT_NODE, parent), Slotable() {}

            Text(DOMString baseURI, StrongPointer<Node> parent, DOMString initText)
                    : CharacterData(baseURI, "#text", NodeType::TEXT_NODE, parent, initText), Slotable() {}

            bool isWhitespace() const;

            DOMString getWholeText() const;

            StrongPointer<Text> splitText(ULong offset);

            StrongPointer<Node> cloneNode() const override;

            bool isEqualNode(const Node &other) const override;

        protected:

            Text(DOMString baseURI, DOMString name, NodeType nodeType, StrongPointer<Node> parent)
                    : CharacterData(baseURI, name, nodeType, parent), Slotable() {}

            Text(DOMString baseURI, DOMString name, NodeType nodeType, StrongPointer<Node> parent, DOMString init)
                    : CharacterData(baseURI, name, nodeType, parent, init), Slotable() {}
        };
    }
}
#endif //FEATHER_TEXT_H
