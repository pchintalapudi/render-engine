//
// Created by prem on 12/13/2018.
//

#include "nodes/utils/html_collection.h"
#include "nodes/elements/element.h"

using namespace feather::dom;

feather::Pair<bool, feather::StrongPointer<Element>>
feather::dom::nodeToElement(const feather::StrongPointer<feather::dom::Node> &p) {
    return p && p->getNodeTypeInternal() == NodeType::ELEMENT_NODE
           ? std::make_pair(true, std::static_pointer_cast<Element>(p))
           : std::make_pair(false, StrongPointer<Element>());
}

HTMLCollection::HTMLCollection(feather::StrongPointer<feather::dom::NodeList> nodeList)
        : SketchyObservableListWrapper(std::move(nodeList)) {
    if (nodeList) nodeList->bind(std::static_pointer_cast<observable::Invalidatable>(shared_from_this()));
}

feather::StrongPointer<Element> HTMLCollection::getNamedItem(feather::DOMString name) const {
    StrongPointer <Element> e{};
    for (UInt i = 0; i < size(); i++) {
        auto child = get(i);
        if (child->getId() == name) return child;
        else if (!e && child->getTagName() == name) e = child;
    }
    return e;
}

void HTMLCollection::modify(feather::RegularEnumSet<feather::observable::InvEvent> &s,
                            const feather::observable::Invalidatable *p) const {
    if (s.contains(observable::InvEvent::ELEMENT_INDEX_CHANGE)) {
        s += observable::InvEvent::INVALIDATE_THIS;
    } else {
        s -= observable::InvEvent::INVALIDATE_THIS;
    }
}

namespace {
    //Taken from stackoverflow: https://stackoverflow.com/a/2112111
    constexpr unsigned int hasher(const char *input) {
        return *input ? static_cast<unsigned int>(*input) + 33 * hasher(input + 1) : 5381;
    }

    bool isFormElement(const feather::StrongPointer<Element> &p) {
        switch (hasher(p->getTagName().c_str())) {
            case hasher("button"):
            case hasher("fieldset"):
            case hasher("input"):
            case hasher("label"):
            case hasher("legend"):
            case hasher("meter"):
            case hasher("optgroup"):
            case hasher("option"):
            case hasher("output"):
            case hasher("param"):
            case hasher("select"):
            case hasher("textarea"):
                return true;
            default:
                return false;
        }
    }
}

feather::Pair<bool, feather::StrongPointer<Element>>
feather::dom::formFilter(const feather::StrongPointer<feather::dom::Element> &p) {
    return p && isFormElement(p) ? std::make_pair(true, p) : std::make_pair(false, StrongPointer<Element>());
}