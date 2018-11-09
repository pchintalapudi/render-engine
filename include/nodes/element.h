//
// Created by prem on 11/6/2018.
//

#ifndef FEATHER_HTML_ELEMENT_H
#define FEATHER_HTML_ELEMENT_H

#include "utils/dom_token_list.h"
#include "utils/named_node_map.h"
#include "node.h"
#include "inon_document_type_child_node.h"

class Element : public Node, public INonDocumentTypeChildNode {
public:

    inline NamedNodeMap *getAttributes() { return &this->attributes; }

    inline DOMTokenList *getClassList() { return &this->classList; }

    inline DOMString getClassName() const { return this->classList.getValue(); }

    inline void setClassName(DOMString className) {
        this->classList.clear();
        this->classList.add(className);
    }

    inline double getWidth() const {
        return this->clientWidth;
    }

    inline double getHeight() const {
        return this->clientHeight;
    }

    inline double getLeft() const {
        return this->clientLeft;
    }

    inline double getTop() const {
        return this->clientTop;
    }

    inline DOMString getComputedName() const {
        return *this->computedName;
    }

    inline DOMString getComputedRole() const {
        return *this->computedRole;
    }

    DOMString getId() const { return *id; }

    void setId(DOMString id) { this->id = new std::string(id); }

    DOMString getInnerHTML() const;

    void setInnerHTML(DOMString innerHTML);

    DOMString getLocalName() const;

    DOMString *getNamespaceURI() const;

    Element *nextElementSibling() const override;

    DOMString getOuterHTML() const;

    void setOuterHTML(DOMString outerHTML);

    //TODO: -\( :| ) /- what prefixes/namespaces are
    inline DOMString *getPrefix() { return this->getNamespaceURI(); }

    Element *previousElementSibling() const override;

    virtual ~Element() {
        delete computedRole;
        delete computedName;
        delete id;
    };
private:
    NamedNodeMap attributes;
    ElementDOMTokenList classList;
    mutable double clientWidth;
    mutable double clientHeight;
    mutable double clientLeft;
    mutable double clientTop;
    mutable bool clientDimensionsValid;
    DOMString *computedName;
    DOMString *computedRole;
    DOMString *id;
};

#endif //FEATHER_HTML_ELEMENT_H
