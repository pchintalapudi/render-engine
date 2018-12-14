//
// Created by prem on 12/11/2018.
//

#include "nodes/text/text.h"

using namespace feather::dom;

bool Text::isWhitespace() const {
    for (auto c : getData()) if (!isspace(c)) return false;
    return true;
}

feather::DOMString Text::getWholeText() const {
    if (!getParentNode().get()) return getData();
    auto children = getParentNode()->getChildNodes();
    auto me = std::static_pointer_cast<const Node>(shared_from_this());
    bool real = false;
    DOMString concat = "";
    concat.reserve(getLength());
    for (UInt i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child->getNodeTypeInternal() == NodeType::TEXT_NODE) {
            concat += std::static_pointer_cast<Text>(child)->getData();
            real = real || (child == me);
        } else if (!concat.empty()) {
            if (!real) concat.clear();
            else break;
        }
    }
    return concat;
}

feather::StrongPointer<Text> Text::splitText(feather::ULong offset) {
    auto length = getLength();
    auto substr = substringData(offset, length - offset);
    deleteData(offset, length - offset);
    StrongPointer <Text> ptr = std::make_shared<Text>(getBaseURI(), getParentNode(), substr);
    if (getParentNode().get()) getParentNode()->insertAfter(ptr, std::static_pointer_cast<Node>(shared_from_this()));
    return ptr;
}

feather::StrongPointer<Node> Text::cloneNode(bool deep) const {
    return std::static_pointer_cast<Node>(std::make_shared<Text>(getBaseURI(), nullptr, getData()));
}

bool Text::isEqualNode(const feather::dom::Node &other) const {
    return other.getNodeTypeInternal() == getNodeTypeInternal() &&
           static_cast<const Text &>(other).getData() == getData();
}