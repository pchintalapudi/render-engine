//
// Created by prem on 12/29/2018.
//

#include "nodes/elements/html_elements/html_element.h"
#include "nodes/documents/document.h"
#include "nodes/text/text.h"

using namespace feather::dom::html;

namespace {
    feather::DOMString kebabCase(const feather::DOMString &camelCase) {
        feather::DOMString copy = "data-";
        feather::UInt add = 0;
        for (const auto &c : camelCase) if (isupper(c)) add++;
        copy.reserve(add + camelCase.length() + 5);
        for (const auto &c : camelCase) {
            if (isupper(c)) copy += char('-') + tolower(c);
            else copy += c;
        }
        return copy;
    }
}

feather::StrongPointer<feather::dom::Attr> DataSetView::getAttr(const feather::DOMString &attr) const {
    return watched.getNamedItem(kebabCase(attr));
}

void DataSetView::setAttr(const feather::DOMString &attr, feather::DOMString value) {
    owner.setAttribute(kebabCase(attr), std::move(value));
}

void HTMLElement::focus() {
    auto owner = getOwnerDocument();
    if (owner) {
        Vector <StrongPointer<const Element>> elements;
        auto ref = getThisRef();
        do elements.push_back(ref);
        while ((ref = ref->getParentElement()));
        owner->getPseudoclassManager().setFocused(std::move(elements));
        //TODO: Fire event
    }
}

void HTMLElement::blur() {
    auto owner = getOwnerDocument();
    if (owner && owner->getPseudoclassManager().containsFocus(getThisRef())) {
        owner->getPseudoclassManager().clearFocused();
        //TODO: Fire event
    }
}

void HTMLElement::click() {
    //TODO: fire event
}

feather::DOMString HTMLElement::getInnerText() const {
    if (cachedInnerText->isValid()) return cachedInnerText->get();
    else {
        auto it = computeInnerText();
        cachedInnerText->set(it);
        return it;
    }
}

void HTMLElement::setInnerText(feather::DOMString innerText) {
    auto children = getChildNodes();
    for (UInt i = 0; i < children->size(); children->get(i++)->clearParentNode());
    children->clear();
    cachedInnerText->set(innerText);
    auto text = Text::create(getBaseURI(), getSharedFromThis(), std::move(innerText));
    children->add(text);
}

feather::DOMString HTMLElement::computeInnerText() const {
    return "";
    //TODO: Actually compute inner text
}

feather::DOMString HTMLElement::getLang() const {
    return "en-US";
    //TODO: Actually determine language
}

feather::StrongPointer<feather::dom::Element> HTMLElement::getOffsetParent() const {
    return getParentElement();
    //TODO: Actually follow the algorithm for computing offset parents
}