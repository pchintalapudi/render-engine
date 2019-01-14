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
        else if (!e && compareType(e, name)) e = child;
    }
    return e;
}

namespace {

    bool isFormElement(const feather::StrongPointer<Element> &p) {
        switch (p->getElementType()) {
            case KnownElements::HTMLButtonElement:
            case KnownElements::HTMLFieldSetElement:
            case KnownElements::HTMLInputElement:
            case KnownElements::HTMLLabelElement:
            case KnownElements::HTMLLegendElement:
            case KnownElements::HTMLMeterElement:
            case KnownElements::HTMLOptGroupElement:
            case KnownElements::HTMLOptionElement:
            case KnownElements::HTMLOutputElement:
            case KnownElements::HTMLParamElement:
            case KnownElements::HTMLSelectElement:
            case KnownElements::HTMLTextAreaElement:
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