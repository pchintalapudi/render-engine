//
// Created by prem on 11/13/2018.
//

#include <algorithm>
#include <set>
#include <sstream>
#include "typedefs.h"
#include "include/nodes/processing_instruction.h"
#include "include/nodes/text.h"
#include "include/nodes/element.h"
#include "include/nodes/shadow_root.h"
#include "include/utils/selectors/css_selector_parsing_impl.h"

namespace {
    DOMString empty[0];
}

dom::Element::Element(DOMString tagName, DOMString baseURI, Document *owner, Node *parent)
        : Node(baseURI, tagName, NodeType::ELEMENT_NODE, owner, parent),
          classList(empty), children(getChildNodes()), allChildren(children) {
    Attr *classAttr = new ClassAttr(this, classList);
    attributes.setNamedItem(classAttr);
}

void dom::Element::setInnerHTML(DOMString html) {
    getChildNodes().clear();
    //TODO: Parse html and add as node children
    //getChildNodes().insert(getChildNodes().size(), nodes);
}

void dom::Element::computeStringValue(Node *child, std::stringstream &output) const {
    switch (child->getNodeType()) {
        case NodeType::ELEMENT_NODE:
            output << static_cast<Element *>(child)->computeInnerHTML();
            break;
        case NodeType::TEXT_NODE:
            output << *child->getNodeValue();
            break;
        case NodeType::PROCESSING_INSTRUCTION_NODE:
            output << "?" << static_cast<ProcessingInstruction *>(child)->getTarget() << " "
                   << *child->getNodeValue() << "?";
            break;
        case NodeType::CDATA_SECTION_NODE:
            output << "[CDATA[" << *child->getNodeValue() << "]]";
            break;
        case NodeType::COMMENT_NODE:
            output << "!--" << *child->getNodeValue() << "--";
            break;
        case NodeType::DOCUMENT_FRAGMENT_NODE:
            computeStringValue(child->getFirstChild(), output);
            break;
        case NodeType::DOCUMENT_NODE:
            //TODO: Handle document nodes
            break;
        default:
            //Our browser doesn't generate these...
            break;
    }
}

DOMString dom::Element::computeInnerHTML() const {
    std::stringstream output;
    output << computeHTML();
    for (unsigned long i = 0; i < getChildNodes().size(); computeStringValue(getChildNodes().get(i++), output));
    output << "/" << tagName << "";
    return output.str();
}

DOMString dom::Element::computeHTML() const {
    std::stringstream output;
    output << "" << tagName;
    for (unsigned long i = 0; i < attributes.size(); output << attributes.getItem(i++));
    output << "";
    return output.str();
}

DOMString dom::Element::computeOuterHTML() const {
    std::stringstream output;
    output << computeHTML() << computeInnerHTML() << "/" << tagName << "";
    return output.str();
}

void dom::Element::setOuterHTML(DOMString html) {
    //TODO: parse html and replace this node with the parsed node(s)
    //getParentNode()->replaceChild(this, parsed);
}

const std::set<DOMString> shadowAllowed = {"article",
                                           "aside",
                                           "blockquote",
                                           "body",
                                           "div",
                                           "footer",
                                           "h1",
                                           "h2",
                                           "h3",
                                           "h4",
                                           "h5",
                                           "h6",
                                           "header",
                                           "main",
                                           "nav",
                                           "p",
                                           "section",
                                           "span"};

dom::ShadowRoot *dom::Element::attachShadow(bool closed) {
    if (shadowAllowed.find(tagName) == shadowAllowed.end()) return nullptr;
    this->closed = closed;
    //TODO: Create a new shadow root and assign it to the field. Also return it in the same statement.
}

dom::Element *dom::Element::closest(DOMString selector) {
    auto css_selector = css::parse(selector);
    auto parent = this;
    while (parent && !css_selector.matches(parent)) parent = parent->getParentElement();
    return parent;
}

DOMString *dom::Element::getAttribute(DOMString attributeName) {
    auto attr = attributes.getNamedItem(attributeName);
    return attr ? new DOMString(attr->getValue()) : nullptr;
}

observable::FilteredList<dom::Element *> *dom::Element::getElementsByClassName(DOMString classNames) {
    std::vector<DOMString> classes = parseStringList(classNames);
    std::function<bool(Element *)> filter = [classes](Element *element) {
        for (const auto clazz : classes) if (!element->getClassList().contains(clazz)) return false;
        return true;
    };
    return new observable::MutableFilteredList<Element *>(&this->getChildren(), filter);
}

observable::FilteredList<dom::Element *> *dom::Element::getElementsByTagName(DOMString tagName) {
    std::function<bool(Element *)> func = [tagName](Element *element) {
        return element->getTagName() == tagName;
    };
    return new observable::FilteredList<Element *>(&allChildren,
                                                   tagName == "*" ? [](Element *) { return true; } : func);
}

dom::Element::~Element() {
    delete shadow;
}