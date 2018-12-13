//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_ELEMENT_H
#define FEATHER_ELEMENT_H

#include "../node.h"
#include "../slotable.h"
#include "../documents/shadow_root.h"
#include "../utils/dom_token_list.h"
#include "../utils/named_node_map.h"

namespace feather {
    namespace dom {

        enum class Dimensions {
            HEIGHT, LEFT, TOP, WIDTH
        };

        class Element : public Node, public Slotable {
        public:

            inline StrongPointer<NamedNodeMap> getAttributes() const { return attributes; }

            inline StrongPointer<DOMTokenList> getClassList() const { return classList; }

            inline DOMString getClassName() const { return classList->getValue(); }

            inline void setClassName(DOMString name) { classList->setValue(name); }

            inline double getClientHeight() const { return clientDim[static_cast<int>(Dimensions::HEIGHT)]; }

            inline double getClientLeft() const { return clientDim[static_cast<int>(Dimensions::LEFT)]; }

            inline double getClientTop() const { return clientDim[static_cast<int>(Dimensions::TOP)]; }

            inline double getClientWidth() const { return clientDim[static_cast<int>(Dimensions::WIDTH)]; }

            //TODO: Implement me
            DOMString getId() const;

            //TODO: Implement me
            void setId(DOMString id);

            //TODO: Implement me
            DOMString getInnerHtml() const;

            //TODO: Implement me
            void setInnerHtml(DOMString innerHtml);

            inline DOMString getLocalName() const { return localName; }

            inline DOMString getNamespaceURI() const { return ns; }

            inline StrongPointer<Element> getNextElementSibling() const {
                return getElementAfterChild(std::static_pointer_cast<const Node>(shared_from_this()));
            }

            //TODO: Implement me
            DOMString getOuterHtml() const;

            //TODO: Implement me
            void setOuterHtml(DOMString html);

            inline DOMString getPrefix() const { return prefix; }

            inline StrongPointer<Element> getPreviousElementSibling() const {
                return getElementBeforeChild(std::static_pointer_cast<const Node>(shared_from_this()));
            }

            inline double getScrollHeight() { return scrollDim[static_cast<int>(Dimensions::HEIGHT)]; }

            inline double getScrollLeft() { return scrollDim[static_cast<int>(Dimensions::LEFT)]; }

            inline double getScrollTop() { return scrollDim[static_cast<int>(Dimensions::TOP)]; }

            inline double getScrollWidth() { return scrollDim[static_cast<int>(Dimensions::WIDTH)]; }

            inline StrongPointer<ShadowRoot> getShadowRoot() { return shadowRoot; }

            inline DOMString getSlot() { return slot; }

            inline DOMString getTagName() { return getNodeName(); }

        private:
            StrongPointer<NamedNodeMap> attributes;
            StrongPointer<DOMTokenList> classList;
            double clientDim[4];
            DOMString ns, prefix, localName;
            double scrollDim[4];
            StrongPointer<ShadowRoot> shadowRoot;
            DOMString slot;
        };
    }
}

#endif //FEATHER_ELEMENT_H
