//
// Created by prem on 11/8/2018.
//

#include "include/nodes/text.h"

Text::Text(Text &other) : CharacterData(other) {

}

Text *Text::cloneNode() {
    auto text = new Text(*this);
    text->setAssignedSlot(this->getAssignedSlot());
    return text;
}

Text *Text::splitText(unsigned long offset) {
    auto length = this->getData().length();
    if (offset > length) {
        return nullptr;
    }
    auto newString = this->substringData(offset, length - offset);
    auto parent = this->getParentNode();
    Text *newNode = new Text(this->getBaseURI(), this->getOwnerDocument(), parent, newString);
    this->setData(this->substringData(0, offset));
    if (parent) {
        this->after(newNode);
    }
    return newNode;
}

bool Text::isEqualNode(Node const &other) const {
    if (other.getNodeType() != TEXT_NODE) {
        return false;
    }
    Text const &text = dynamic_cast<Text const &>(other);
    return this->getData() == text.getData();
}