//
// Created by prem on 11/8/2018.
//

#ifndef FEATHER_COMMENT_H
#define FEATHER_COMMENT_H

#include "character_data.h"
#include "document.h"
#include "node.h"

class Comment : public CharacterData {
public:
    Comment(std::string baseURI, Document *owner, Node *parentNode)
            : CharacterData(baseURI, owner, parentNode) {}

    Comment(Document *owner, Node &parentNode) : CharacterData(owner, parentNode) {}

    Comment(std::string baseURI, Document *owner, Node *parentNode, std::string data)
            : CharacterData(baseURI, owner, parentNode) { this->setData(data); }

    Comment(Document *owner, Node &parentNode, std::string data)
            : CharacterData(owner, parentNode) { this->setData(data); }
};

#endif //FEATHER_COMMENT_H
