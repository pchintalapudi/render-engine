//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_COMMENT_H
#define FEATHER_COMMENT_H

#include "character_data.h"

namespace dom {
    class Comment;
}

class dom::Comment : public CharacterData {
public:
    Comment(DOMString baseURI, Node *parent)
            : CharacterData(baseURI, "#comment", NodeType::COMMENT_NODE, parent) {}

    explicit Comment(Node &parent) : CharacterData("#comment", NodeType::COMMENT_NODE, parent) {}

    Comment(DOMString baseURI, Node *parent, DOMString text)
            : Comment(baseURI, parent) { setData(text); }

    Comment(Node &parent, DOMString text) : Comment(parent) { setData(text); }
};

#endif //FEATHER_COMMENT_H
