//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_ELEMENT_H
#define FEATHER_ELEMENT_H

#include "include/nodes/interfaces/slotable.h"
#include "include/utils/dom_token_list.h"
#include "include/utils/named_node_map.h"
#include "include/nodes/interfaces/child_node.h"
#include "include/nodes/interfaces/parent_node.h"
#include "include/nodes/interfaces/non_document_type_child_node.h"
#include "node.h"

namespace dom {
    class ClassTokenList;

    class Element;
}

class dom::ClassTokenList : public DOMTokenList {
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

    inline void setId(DOMString id) { this->id = id; }

    inline DOMString getId() { return id; }

    inline DOMString getInnerHTML() { return computeInnerHTML(); }

    void setInnerHTML(DOMString html);

    inline double getScrollLeft() { return scrollDim[0]; }

    inline double getScrollTop() { return scrollDim[1]; }

    inline double getScrollWidth() { return scrollDim[2]; }

    inline double getScrollHeight() { return scrollDim[3]; }

    inline DOMString getSlot() { return slot; }

    inline DOMString getTagName() {
        return tagName;
    }

    inline dom::HTMLCollection &getChildren() override { return children; }

private:
    NamedNodeMap attributes;
    ClassTokenList classList;
    double clientDim[4];
    DOMString computedName;
    DOMString computedRole;
    DOMString id;
    double scrollDim[4];
    DOMString slot;
    DOMString tagName;

    HTMLCollection children;

    DOMString computeInnerHTML() const;
};

#endif //FEATHER_ELEMENT_H
