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
    if (getParentNode()) {
        auto children = getChildNodes();
        DOMString ret = getData();
        auto next = getNextSibling();
        while (next && next->getNodeTypeInternal() == NodeType::TEXT_NODE) ret += *next->getNodeValue();
        return ret;
    } else return getData();
}

feather::StrongPointer<Text> Text::splitText(feather::ULong offset) {
    auto length = getLength();
    auto substr = substringData(offset, length - offset);
    deleteData(offset, length - offset);
    StrongPointer <Text> ptr = std::make_shared<Text>(DOMString(getBaseURI()), getParentNode(), std::move(substr));
    if (getParentNode().get()) getParentNode()->insertAfter(ptr, std::static_pointer_cast<Node>(shared_from_this()));
    return ptr;
}

feather::StrongPointer<Node> Text::cloneNode(bool) const {
    return Text::create(getBaseURI(), StrongPointer<Node>(), getData());
}