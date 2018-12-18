//
// Created by prem on 12/13/2018.
//

#include "nodes/elements/element.h"

using namespace feather::dom;

feather::StrongPointer<feather::DOMString> Element::getAttribute(feather::DOMString name) const {
    auto attr = attributes->getNamedItem(std::move(name));
    return attr ? std::make_shared<DOMString>(attr->getValue()) : std::make_shared<DOMString>();
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