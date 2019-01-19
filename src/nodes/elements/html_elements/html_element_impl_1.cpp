//
// Created by prem on 1/19/2019.
//

#include "nodes/elements/html_elements/html_element_includes.h"

using namespace feather::dom::html;

#define CLONE_DEF(name) feather::StrongPointer<feather::dom::Node> feather::dom::html::name::cloneNode(bool deep) const {auto clone = std::make_shared<name>(getBaseURI(), StrongPointer<Node>()); cloneElementProperties(clone, deep); return std::static_pointer_cast<Node>(clone);}

//Clone function defs
CLONE_DEF(HTMLAnchorElement)

CLONE_DEF(HTMLAreaElement)

CLONE_DEF(HTMLBaseElement)

CLONE_DEF(HTMLLinkElement)

CLONE_DEF(HTMLSourceElement)

CLONE_DEF(HTMLBRElement)

CLONE_DEF(HTMLBodyElement)

CLONE_DEF(HTMLDListElement)

CLONE_DEF(HTMLDivElement)

CLONE_DEF(HTMLHRElement)

CLONE_DEF(HTMLHeadElement)

feather::StrongPointer<feather::dom::Node> HTMLHeadingElement::cloneNode(bool deep) const {
    auto clone = std::make_shared<HTMLHeadingElement>(getBaseURI(), getTagName()[1] - '0', StrongPointer<Node>());
    cloneElementProperties(clone, deep);
    return clone;
}

CLONE_DEF(HTMLHtmlElement)

CLONE_DEF(HTMLParagraphElement)

CLONE_DEF(HTMLPictureElement)

CLONE_DEF(HTMLPreElement)

CLONE_DEF(HTMLSpanElement)

CLONE_DEF(HTMLUListElement)