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

            Text(DOMString baseURI, const StrongPointer<Node> &parent)
                    : CharacterData(std::move(baseURI), "#text", NodeType::TEXT_NODE, parent), Slotable() {}

            Text(DOMString baseURI, const StrongPointer<Node> &parent, DOMString initText)
                    : CharacterData(std::move(baseURI), "#text", NodeType::TEXT_NODE, parent, std::move(initText)),
                      Slotable() {}

            bool isWhitespace() const;

            DOMString getWholeText() const;

            StrongPointer<Text> splitText(ULong offset);

            StrongPointer<Node> cloneNode(bool deep) const override;

            bool isEqualNode(const Node &other) const override;

            static inline StrongPointer<Text> create(DOMString baseURI, const StrongPointer<Node> &parent) {
                return std::make_shared<Text>(std::move(baseURI), parent);
            }

            static inline StrongPointer<Text>
            create(DOMString baseURI, const StrongPointer<Node> &parent, DOMString initText) {
                return std::make_shared<Text>(std::move(baseURI), parent, std::move(initText));
            }

        protected:

            Text(DOMString baseURI, DOMString name, NodeType nodeType, const StrongPointer<Node> &parent)
                    : CharacterData(std::move(baseURI), std::move(name), nodeType, parent), Slotable() {}

            Text(DOMString baseURI, DOMString name, NodeType nodeType, const StrongPointer<Node> &parent,
                 DOMString init)
                    : CharacterData(std::move(baseURI), std::move(name), nodeType, parent, std::move(init)),
                      Slotable() {}
        };
    }
}
#endif //FEATHER_TEXT_H
