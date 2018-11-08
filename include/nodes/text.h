//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_TEXT_H
#define FEATHER_TEXT_H

#include <string>
#include "character_data.h"
#include "element.h"
#include "node.h"
#include "slotable.h"

class Text : public CharacterData, public Slotable {
public:

    Text(std::string baseURI, Document *const owner, Node *parent) : CharacterData(baseURI, owner, parent) {}

    Text(Document *owner, Node &parent) : CharacterData(owner, parent) {}

    Text(std::string baseURI, Document *const owner, Node *parent, std::string text)
            : Text(baseURI, owner, parent) { this->setData(text); }

    Text(Document *owner, Node &parent, std::string text) : Text(owner, parent) { this->setData(text); }

    Text(Text &other);

    std::string getWholeText();

    inline Element *getAssignedSlot() const override { return this->assignedSlot; };

    inline void setAssignedSlot(Element *slot) override { this->assignedSlot = slot; }

    Text *splitText(unsigned long offset);

    Text *cloneNode() override;

    bool isEqualNode(Node const &other) const override;

    ~Text() override {}

private:
    Element *assignedSlot;
};

#endif //FEATHER_TEXT_H
