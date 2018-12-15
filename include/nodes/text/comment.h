//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_COMMENT_H
#define FEATHER_COMMENT_H

#include "character_data.h"

namespace feather {
    namespace dom {
        class Comment : public CharacterData {
            Comment(DOMString baseURI, StrongPointer<Node> parent)
                    : CharacterData(baseURI, "#comment", NodeType::COMMENT_NODE, parent, text) {}

            Comment(DOMString baseURI, StrongPointer<Node> parent, DOMString text)
                    : CharacterData(baseURI, "#comment", NodeType::COMMENT_NODE, parent, text) {}
        };
    }
}
#endif //FEATHER_COMMENT_H
