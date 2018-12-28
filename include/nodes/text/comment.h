//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_COMMENT_H
#define FEATHER_COMMENT_H

#include "character_data.h"

namespace feather {
    namespace dom {
        class Comment : public CharacterData {
            Comment(DOMString baseURI, const StrongPointer<Node> &parent)
                    : CharacterData(std::move(baseURI), "#comment", NodeType::COMMENT_NODE, parent) {}

            Comment(DOMString baseURI, const StrongPointer<Node> &parent, DOMString text)
                    : CharacterData(std::move(baseURI), "#comment", NodeType::COMMENT_NODE, parent, std::move(text)) {}
        };
    }
}
#endif //FEATHER_COMMENT_H
