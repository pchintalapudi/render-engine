//
// Created by prem on 11/8/2018.
//

#ifndef FEATHER_PROCESSING_INSTRUCTION_H
#define FEATHER_PROCESSING_INSTRUCTION_H

#include "character_data.h"

class ProcessingInstruction : public CharacterData {

public:
    ProcessingInstruction(Document *owner, Node &parent) : CharacterData(owner, parent) {}

    ProcessingInstruction(std::string baseURI, Document *owner, Node *parent)
            : CharacterData(baseURI, owner, parent) {}

    ProcessingInstruction(Document *owner, Node &parent, std::string target, std::string data)
            : CharacterData(owner, parent) {
        this->setData(data);
        this->target = new std::string(target);
    }

    ProcessingInstruction(std::string baseURI, Document *owner, Node *parent, std::string target, std::string data)
            : CharacterData(baseURI, owner, parent) {
        this->setData(data);
        this->target = new std::string(target);
    }

    std::string getTarget() {
        return *this->target;
    }

    void setTarget(std::string target) {
        delete this->target;
        this->target = new std::string(target);
    }

private:
    DOMString *target;
};

#endif //FEATHER_PROCESSING_INSTRUCTION_H
