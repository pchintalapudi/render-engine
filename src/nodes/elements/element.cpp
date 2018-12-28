//
// Created by prem on 12/13/2018.
//

#include "nodes/elements/element.h"

using namespace feather::dom;

feather::StrongPointer<feather::DOMString> Element::getAttribute(feather::DOMString name) const {
    auto attr = attributes->getNamedItem(std::move(name));
    return attr ? std::make_shared<DOMString>(attr->getValue()) : StrongPointer<DOMString>();
}

feather::StrongPointer<Element> Element::getClosest(feather::DOMString selector) const {
    StrongPointer <Element> ref = getThisRef();
    auto sel = selector::CSSSelector::parse(std::move(selector), ref);
    while (!sel.matches(ref) && (ref = ref->getParentElement()));
    return ref;
}

DOMRect Element::getBoundingClientRect() const {
    auto vec = getClientRects();
    DOMRect dr;
    for (auto cr : vec) {
        if (cr.getTop() < dr.getY()) dr.setY(cr.getTop());
        if (cr.getLeft() < dr.getX()) dr.setX(cr.getLeft());
        if (cr.getRight() > dr.getWidth() + dr.getX()) dr.setWidth(cr.getRight() - dr.getX());
        if (cr.getBottom() > dr.getHeight() + dr.getY()) dr.setHeight(cr.getBottom() - dr.getY());
    }
    return vec.empty() || dr.getX() + dr.getY() + dr.getWidth() + dr.getBottom() != 0 ? dr : vec.front();
}

namespace {
    //Taken from stackoverflow: https://stackoverflow.com/a/2112111
    constexpr unsigned int hasher(const char *input) {
        return *input ? static_cast<unsigned int>(*input) + 33 * hasher(input + 1) : 5381;
    }
}

void Element::insertAdjacentElement(feather::DOMString position, feather::StrongPointer<feather::dom::Element> e) {
    switch (hasher(position.c_str())) {
        case hasher("beforebegin"):
            this->insertBefore(e, std::static_pointer_cast<Node>(shared_from_this()));
            break;
        case hasher("afterbegin"):
            getChildNodes().insert(0, e);
            break;
        case hasher("beforeend"):
            getChildNodes().add(e);
            break;
        case hasher("afterend"):
            this->insertAfter(e, std::static_pointer_cast<Node>(shared_from_this()));
            break;
        default:
            break;
    }
}

namespace {
    feather::DOMString encode(feather::DOMString str) {
        feather::UInt reserve = str.length();
        for (auto c : str) {
            switch (c) {
                case '&':
                    reserve += 4;
                    break;
                case '<':
                case '>':
                    reserve += 3;
                    break;
                default:
                    break;
            }
        }
        feather::DOMString modded;
        modded.reserve(reserve);
        for (auto c : str) {
            switch (c) {
                default:
                    modded += c;
                    break;
                case '&':
                    modded += "&amp;";
                    break;
                case '<':
                    modded += "&lt;";
                    break;
                case '>':
                    modded += "&gt;";
                    break;
            }
        }
        return modded;
    }
}

feather::DOMString Element::cacheInnerHTML() const {
    DOMString html;
    UInt reserve = 0;
    for (UInt i = 0; i < getChildNodes().size(); i++) {
        auto child = getChildNodes().get(i);
        switch (child->getNodeTypeInternal()) {
            case NodeType::ELEMENT_NODE:
                //It's computed here, but the repeated call shouldn't matter since the value is cached.
                reserve += std::static_pointer_cast<Element>(child)->getOuterHtml().length();
                break;
            case NodeType::TEXT_NODE:
                reserve += encode(*child->getNodeValue()).length();
                break;
            case NodeType::COMMENT_NODE:
                reserve += 4 + child->getNodeValue()->length() + 3;
                break;
            default:
                //See below
                break;
        }
    }
    html.reserve(reserve);
    for (UInt i = 0; i < getChildNodes().size(); i++) {
        auto child = getChildNodes().get(i);
        switch (child->getNodeTypeInternal()) {
            case NodeType::ELEMENT_NODE:
                //It's been computed before, so this is actually a very cheap call
                html += std::static_pointer_cast<Element>(child)->getInnerHtml();
                break;
            case NodeType::TEXT_NODE:
                html += encode(*child->getNodeValue());
                break;
            case NodeType::COMMENT_NODE:
                html += "<!--" + *child->getNodeValue() + "-->";
                break;
            default:
                //Idc about the other random nodes; they dont really matter
                break;
        }
    }
    return html;
}

feather::DOMString Element::cacheOuterHTML() const {
    DOMString html;
    UInt reserve = 0;
    DOMString attrs = attributes->toHTML();
    DOMString inner = getInnerHtml();
    reserve += 1 + getTagName().length() + attrs.length() + 1 + inner.length() + 2 + getTagName().length() + 1;
    html.reserve(reserve);
    html += "<";
    html += getTagName();
    html += attrs;
    html += ">";
    html += inner;
    html += "</";
    html += getTagName();
    html += ">";
    innerHTMLValid = true;
    return innerHTML = html;
}

feather::StrongPointer<Element> Element::getNextElementSibling() const {
    if (nextSibling->isValid()) return nextSibling->get().lock();
    if (getParentElement()) {
        updateLinkedList();
        return nextSibling->get().lock();
    } else if (getParentNode()) {
        auto idx = getIndex();
        auto children = getParentNode()->getChildNodes();
        for (idx++; idx < children.size(); idx++) {
            auto child = children.get(idx);
            if (child->getNodeTypeInternal() == NodeType::ELEMENT_NODE) {
                auto eChild = std::static_pointer_cast<Element>(child);
                nextSibling->set(eChild);
                eChild->prevSibling->set(getThisRef());
                return eChild;
            }
        }
    }
    nextSibling->set(StrongPointer<Element>());
    return StrongPointer<Element>();
}

feather::StrongPointer<Element> Element::getPreviousElementSibling() const {
    if (prevSibling->isValid()) return prevSibling->get().lock();
    if (getParentElement()) {
        updateLinkedList();
        return prevSibling->get().lock();
    } else if (getParentNode()) {
        auto idx = getIndex();
        auto children = getParentNode()->getChildNodes();
        while (idx-- > 0) {
            auto child = children.get(idx);
            if (child->getNodeTypeInternal() == NodeType::ELEMENT_NODE) {
                auto eChild = std::static_pointer_cast<Element>(child);
                prevSibling->set(eChild);
                eChild->nextSibling->set(getThisRef());
                return eChild;
            }
        }
    }
    prevSibling->set(StrongPointer<Element>());
    return StrongPointer<Element>();
}

void Element::updateLinkedList() const {
    auto children = getChildren();
    switch (children->size()) {
        default: {
            for (UInt i = 1; i < children->size() - 1; i++) {
                auto child = children->getItem(i);
                child->prevSibling->set(children->getItem(i - 1));
                child->nextSibling->set(children->getItem(i + 1));
            }
        }
            [[fallthrough]];
        case 2: {
            children->getItem(0)->nextSibling->set(children->getItem(1));
            children->getItem(children->size() - 1)->prevSibling->set(children->getItem(children->size() - 2));
        }
            [[fallthrough]];
        case 1: {
            children->getItem(0)->prevSibling->set(StrongPointer<Element>());
            children->getItem(children->size() - 1)->nextSibling->set(StrongPointer<Element>());
            break;
        }
        case 0:
            break;
    }
    if (children->size() > 2) {
    } else if (children->size() > 1) {

    } else {

    }
}