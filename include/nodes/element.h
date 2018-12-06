//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_ELEMENT_H
#define FEATHER_ELEMENT_H

#include "include/geom/dom_rect.h"
#include "include/utils/full_child_list.h"
#include "include/utils/dom_token_list.h"
#include "include/utils/named_node_map.h"
#include "attrs/standard_attr.h"
#include "attrs/class_attr.h"
#include "interfaces/slotable.h"
#include "interfaces/child_node.h"
#include "interfaces/parent_node.h"
#include "interfaces/non_document_type_child_node.h"
#include "include/utils/selectors/css_selector_parsing_impl.h"
#include "text.h"

namespace dom {
    class Element;

    class ShadowRoot;
}

class dom::Element
        : public Node, public ChildNode, public ParentNode, public NonDocumentTypeChildNode, public Slotable {
public:

    Element(DOMString tagName, DOMString baseURI, Node *parent);

    explicit Element(Element &other);

    inline NamedNodeMap &getAttributes() { return attributes; }

    inline DOMTokenList &getClassList() { return classList; }

    inline const NamedNodeMap &getAttributes() const { return attributes; }

    inline const DOMTokenList &getClassList() const { return classList; }

    inline DOMString getClassName() const { return classList.getValue(); }

    inline void setClassName(DOMString className) { classList.setValue(className); }

    inline double getClientLeft() const { return clientDim[0]; }

    inline double getClientTop() const { return clientDim[1]; }

    inline double getClientWidth() const { return clientDim[2]; }

    inline double getClientHeight() const { return clientDim[3]; }

    inline DOMString getComputedName() const { return computedName; }

    inline DOMString getComputedRole() const { return computedRole; }

    inline void setId(DOMString id) { attributes.setNamedItem(new StandardAttr("id", this, id)); }

    inline DOMString getId() const { return attributes.getNamedItem("id")->getValue(); }

    inline DOMString getInnerHTML() const { return computeInnerHTML(); }

    void setInnerHTML(DOMString html);

    inline DOMString getOuterHTML() const { return computeOuterHTML(); }

    void setOuterHTML(DOMString html);

    inline double getScrollLeft() const { return scrollDim[0]; }

    inline double getScrollTop() const { return scrollDim[1]; }

    inline void setScrollLeft(double scrollLeft) {
        scrollDim[0] = scrollLeft;
        invalidate(observable::generate(observable::EventType::INTERNAL_CHANGE));
    }

    inline void setScrollTop(double scrollTop) {
        scrollDim[1] = scrollTop;
        invalidate(observable::generate(observable::EventType::INTERNAL_CHANGE));
    }

    inline double getScrollWidth() const { return scrollDim[2]; }

    inline double getScrollHeight() const { return scrollDim[3]; }

    inline ShadowRoot *getShadowRoot() const { return closed ? nullptr : shadow; }

    inline DOMString getSlot() const { return slot; }

    inline DOMString getTagName() const { return tagName; }

    ShadowRoot *attachShadow(bool closed);

    Element *closest(DOMString selector);

    DOMString *getAttribute(DOMString attributeName);

    inline std::vector<DOMString> getAttributeNames() const { return attributes.keys(); }

    inline dom::HTMLCollection &getChildren() override { return children; }

    inline const dom::HTMLCollection &getChildren() const override { return children; }

    DOMRect getBoundingClientRect();

    std::vector<DOMRect> &getClientRects();

    observable::FilteredList<Element *> *getElementsByClassName(DOMString classNames);

    observable::FilteredList<Element *> *getElementsByTagName(DOMString tagName);

    inline bool hasAttribute(DOMString attribute) { return attributes.contains(attribute); }

    inline bool hasAttributes() { return attributes.size() > 0; }

    //TODO: somehow figure out what pointer capture means.
    inline bool hasPointerCapture(unsigned long) { return false; }

    Element *insertAdjacentElement(DOMString where, Element *element);

    Element *insertAdjacentHTML(DOMString where, DOMString html);

    Text *insertAdjacentText(DOMString where, DOMString text);

    inline void append(std::vector<Node *> &children) override { getChildNodes().addAll(children); }

    inline void prepend(std::vector<Node *> &children) override { getChildNodes().insertAll(0, children); }

    inline bool matches(DOMString selector) { return css::parse(selector).matches(this); }

    Element *querySelector(DOMString selector) const override;

    std::vector<Element *> querySelectorAll(DOMString selector) const override;

    //TODO: Make this work; It'll likely reside on the Document implementation
    inline void releasePointerCapture(unsigned long) {}

    inline void remove() override { if (getParentNode()) getParentNode()->removeChild(this); }

    inline void removeAttribute(DOMString attr) { delete attributes.getNamedItem(attr); }

    //TODO: Implement me
    void requestFullscreen() {}

    //TODO: Implement me
    void requestPointerLock(unsigned long) {}

    inline void scrollTo(double x, double y) {
        setScrollLeft(x);
        setScrollTop(y);
    }

    inline void scrollBy(double x, double y) { scrollTo(x + getScrollLeft(), y + getScrollTop()); }

    //TODO: Implement me; will require much styling/computation
    void scrollIntoView();

    //TODO: Implement me
    void setPointerLock(unsigned long) {}

    bool toggleAttribute(DOMString attr);

    bool toggleAttribute(DOMString attr, bool force);

    //Internal implementation methods
    dom::FullChildList &getAllChildren() const { return allChildren; }

    virtual ~Element();

private:
    NamedNodeMap attributes;
    DOMTokenList classList;
    double clientDim[4];
    DOMString computedName;
    DOMString computedRole;
    double scrollDim[4];
    DOMString slot;
    DOMString tagName;
    ShadowRoot *shadow;
    bool closed;
    HTMLCollection children;

    //Caches
    mutable dom::FullChildList allChildren;

    DOMString computeInnerHTML() const;

    DOMString computeOuterHTML() const;

    DOMString computeHTML() const;

    void computeStringValue(Node *node, std::stringstream &output) const;

    Element *querySelectorInternal(css::CSSSelector &selector) const;

    void querySelectorAllInternal(css::CSSSelector &selector, std::vector<Element *> &list) const;
};

#endif //FEATHER_ELEMENT_H
