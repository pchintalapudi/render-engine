//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_ELEMENT_H
#define FEATHER_ELEMENT_H

#include "include/geom/dom_rect.h"
#include "include/utils/dom_token_list.h"
#include "include/utils/named_node_map.h"
#include "interfaces/slotable.h"
#include "interfaces/child_node.h"
#include "interfaces/parent_node.h"
#include "interfaces/non_document_type_child_node.h"
#include "node.h"

namespace dom {
    class Element;

    class ShadowRoot;
}

class ClassTokenList : public dom::DOMTokenList {
    inline bool supports(DOMString feature) override {
        (void) feature;
        return false;
    }
};

class dom::Element
        : public Node, public ChildNode, public ParentNode, public NonDocumentTypeChildNode, public Slotable {
public:

    explicit Element(DOMString tagName);

    inline NamedNodeMap &getAttributes() { return attributes; }

    inline DOMTokenList &getClassList() { return classList; }

    inline DOMString getClassName() const { return classList.getValue(); }

    void setClassName(DOMString className);

    inline double getClientLeft() const { return clientDim[0]; }

    inline double getClientTop() const { return clientDim[1]; }

    inline double getClientWidth() const { return clientDim[2]; }

    inline double getClientHeight() const { return clientDim[3]; }

    inline DOMString getComputedName() { return computedName; }

    inline DOMString getComputedRole() { return computedRole; }

    inline void setId(DOMString id) { attributes.setNamedItem(*new Attr("id", this, id)); }

    inline DOMString getId() { return attributes.getNamedItem("id")->getValue(); }

    inline DOMString getInnerHTML() { return computeInnerHTML(); }

    void setInnerHTML(DOMString html);

    inline DOMString getOuterHTML() { return computeOuterHTML(); }

    void setOuterHTML(DOMString html);

    inline double getScrollLeft() { return scrollDim[0]; }

    inline double getScrollTop() { return scrollDim[1]; }

    inline void setScrollLeft(double scrollLeft) { scrollDim[0] = scrollLeft; }

    inline void setScrollTop(double scrollTop) { scrollDim[1] = scrollTop; }

    inline double getScrollWidth() { return scrollDim[2]; }

    inline double getScrollHeight() { return scrollDim[3]; }

    inline ShadowRoot *getShadowRoot() { return closed ? nullptr : shadow; }

    inline DOMString getSlot() { return slot; }

    inline DOMString getTagName() { return tagName; }

    ShadowRoot *attachShadow(bool closed);

    Element *closest(DOMString selector);

    DOMString *getAttribute(DOMString attributeName);

    inline std::vector<DOMString> getAttributeNames() { return attributes.keys(); }

    inline dom::HTMLCollection &getChildren() override { return children; }

    inline const dom::HTMLCollection &getChildren() const override { return children; }

    DOMRect getBoundingClientRect();

    std::vector<DOMRect> getClientRects();

    std::vector<Element *> getElementsByClassName(DOMString classNames);

    virtual ~Element();

private:
    NamedNodeMap attributes;
    ClassTokenList classList;
    double clientDim[4];
    DOMString computedName;
    DOMString computedRole;
    double scrollDim[4];
    DOMString slot;
    DOMString tagName;
    ShadowRoot *shadow;
    bool closed;

    HTMLCollection children;

    DOMString computeInnerHTML() const;

    DOMString computeOuterHTML() const;

    DOMString computeHTML() const;

    void computeStringValue(Node *node, std::stringstream &output) const;
};

#endif //FEATHER_ELEMENT_H
