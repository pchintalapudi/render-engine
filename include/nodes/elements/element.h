//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_ELEMENT_H
#define FEATHER_ELEMENT_H

#include "../node.h"
#include "../slotable.h"
#include "../utils/dom_token_list.h"
#include "../utils/named_node_map.h"
#include "../utils/html_collection.h"
#include "selectors/css_selector.h"
#include "geom/dom_rect.h"
#include "style/css/rules/css_style_declaration.h"

namespace feather {
    namespace dom {

        class ShadowRoot;

        enum class Dimensions {
            HEIGHT, LEFT, TOP, WIDTH
        };

        namespace elists {
            class FilteredByTagName;

            class FilteredByTagNameNS;

            class FilteredByClassName;
        }
        class Element : public Node, public Slotable {
        public:

            Element(DOMString baseURI, DOMString tagName, const StrongPointer<Node> &parent);

            inline StrongPointer<const NamedNodeMap> getAttributes() const {
                return StrongPointer<const NamedNodeMap>(shared_from_this(), &attributes);
            }

            inline StrongPointer<NamedNodeMap> getAttributes() {
                return StrongPointer<NamedNodeMap>(shared_from_this(), &attributes);
            }

            inline StrongPointer<const DOMTokenList> getClassList() const {
                return StrongPointer<const DOMTokenList>(shared_from_this(), &classList);
            }

            inline StrongPointer<DOMTokenList> getClassList() {
                return StrongPointer<DOMTokenList>(shared_from_this(), &classList);
            }

            inline DOMString getClassName() const { return classList.getValue(); }

            inline void setClassName(DOMString name) { classList.setValue(std::move(name)); }

            inline double getClientHeight() const { return clientDim[static_cast<int>(Dimensions::HEIGHT)]; }

            inline double getClientLeft() const { return clientDim[static_cast<int>(Dimensions::LEFT)]; }

            inline double getClientTop() const { return clientDim[static_cast<int>(Dimensions::TOP)]; }

            inline double getClientWidth() const { return clientDim[static_cast<int>(Dimensions::WIDTH)]; }

            inline DOMString getId() const { return getAttributeSafe("id"); }

            inline void setId(DOMString id) { setAttribute("id", std::move(id)); }

            inline DOMString getInnerHtml() const { return innerHTMLValid ? innerHTML : cacheInnerHTML(); }

            //TODO: Implement me
            void setInnerHtml(DOMString innerHtml);

            inline DOMString getLocalName() const { return localName; }

            inline DOMString getNamespaceURI() const { return ns; }

            inline StrongPointer<Element> getNextElementSibling() const;

            inline DOMString getOuterHtml() const { return outerHTMLValid ? outerHTML : cacheOuterHTML(); }

            //TODO: Implement me
            void setOuterHtml(DOMString html);

            inline DOMString getPrefix() const { return prefix; }

            StrongPointer<Element> getPreviousElementSibling() const;

            inline double getScrollHeight() const { return scrollDim[static_cast<int>(Dimensions::HEIGHT)]; }

            inline double getScrollLeft() const { return scrollDim[static_cast<int>(Dimensions::LEFT)]; }

            inline double getScrollTop() const { return scrollDim[static_cast<int>(Dimensions::TOP)]; }

            inline double getScrollWidth() const { return scrollDim[static_cast<int>(Dimensions::WIDTH)]; }

            inline StrongPointer<ShadowRoot> getShadowRoot() { return shadowRoot; }

            inline DOMString getSlot() const { return getAttributeSafe("slot"); }

            inline DOMString getTagName() const { return getNodeName(); }

            inline StrongPointer<Function<void(js::Event &)>> getOnFullScreenChange() const {
                return getEndHandler("onfullscreenchange");
            }

            inline void setOnFullScreenChange(StrongPointer<Function<void(js::Event &)>> func) {
                this->registerEndHandler("onfullscreenchange", std::move(func));
            }

            inline StrongPointer<Function<void(js::Event &)>> getOnFullScreenError() const {
                return getEndHandler("onfullscreenerror");
            }

            inline void setOnFullScreenError(StrongPointer<Function<void(js::Event &)>> func) {
                this->registerEndHandler("onfullscreenerror", std::move(func));
            }

            //TODO: Implement me
            StrongPointer<ShadowRoot> attachShadow(bool open);

            StrongPointer<Element> getClosest(DOMString selector) const;

            StrongPointer<DOMString> getAttribute(DOMString name) const;

            inline DOMString getAttributeSafe(DOMString name) const {
                auto ptr = getAttribute(std::move(name));
                return ptr ? *ptr : "";
            }

            inline Vector<DOMString> getAttributeNames() const { return attributes.getKeys(); }

            StrongPointer<DOMString> getAttributeNS(DOMString ns, DOMString name) const;

            DOMRect getBoundingClientRect() const;

            Vector<DOMRect> getClientRects() const;

            StrongPointer<elists::FilteredByClassName> getElementsByClassName(DOMString className) const;

            StrongPointer<elists::FilteredByTagName> getElementsByTagName(DOMString tagName) const;

            StrongPointer<elists::FilteredByTagNameNS> getElementsByTagNameNS(DOMString ns, DOMString tagName) const;

            inline bool hasAttribute(DOMString attr) const { return attributes.contains(std::move(attr)); }

            inline bool hasAttributeNS(const DOMString &ns, const DOMString &name) const {
                return attributes.contains(ns + ":" + name);
            }

            inline bool hasAttributes() const { return attributes.getLength() != 0; }

            //TODO: Implement me
            bool hasPointerCapture(ULong id) const;

            //TODO: Implement me
            void insertAdjacentElement(DOMString position, StrongPointer<Element> e);

            //TODO: Implement me
            void insertAdjacentHTML(DOMString position, DOMString html);

            //TODO: Implement me
            void insertAdjacentText(DOMString position, DOMString text);

            inline bool matches(DOMString selector) const {
                return selector::CSSSelector::parse(std::move(selector),
                                                    std::static_pointer_cast<const Element>(shared_from_this()))
                        .matches(std::static_pointer_cast<const Element>(shared_from_this()));
            }

            inline StrongPointer<Element> querySelector(DOMString selector) const {
                return selector::CSSSelector::parse(std::move(selector),
                                                    std::static_pointer_cast<const Element>(shared_from_this()))
                        .querySelector(std::static_pointer_cast<const Element>(shared_from_this()));
            }

            inline Vector<StrongPointer<Element>> querySelectorAll(DOMString selector) const {
                return selector::CSSSelector::parse(std::move(selector),
                                                    std::static_pointer_cast<const Element>(shared_from_this()))
                        .querySelectorAll(std::static_pointer_cast<const Element>(shared_from_this()));
            }

            //TODO: Implement me
            void releasePointerCapture(ULong id);

            inline void removeAttribute(DOMString attr) { attributes.removeNamedItem(std::move(attr)); }

            inline void removeAttributes(const DOMString &ns, const DOMString &name) {
                attributes.removeNamedItemNS(ns, name);
            }

            //TODO: Implement me
            void requestFullscreen(Function<void(bool)> onFinish);

            //TODO: Implement me
            void scroll(double x, double y);

            //TODO: Implement me
            void scrollBy(double x, double y);

            inline void scrollIntoView() { scrollIntoView(true); }

            //TODO: Implement me
            void scrollIntoView(bool alignToTop);

            inline void scrollTo(double x, double y) { scroll(x, y); }

            void setAttribute(DOMString name, DOMString value);

            inline void setAttribute(const DOMString &ns, const DOMString &local, DOMString value) {
                setAttribute(ns + ":" + local, std::move(value));
            }

            //TODO: Implement me
            void setPointerCapture(ULong id);

            bool toggleAttribute(DOMString attr);

            bool toggleAttribute(DOMString attr, bool force);

            //ChildNode impl
            inline void remove() {
                if (getParentNode())
                    getParentNode()->removeChild(std::static_pointer_cast<Node>(shared_from_this()));
            }

            inline void before(Vector<StrongPointer<Node>> insertBefore) {
                if (getParentNode())
                    getParentNode()->insertBeforeChildNDTCN(getSharedFromThis(), std::move(insertBefore));
            }

            inline void after(Vector<StrongPointer<Node>> insertAfter) {
                if (getParentNode())
                    getParentNode()->insertAfterChildNDTCN(getSharedFromThis(), std::move(insertAfter));
            }

            inline void replaceWith(Vector<StrongPointer<Node>> repl) {
                if (getParentNode())
                    getParentNode()->replaceChildNDTCN(getSharedFromThis(), std::move(repl));
            }

            //ParentNode impl

            inline UInt getChildElementCount() const { return children.size(); }

            inline StrongPointer<const HTMLCollection> getChildren() const {
                return StrongPointer<const HTMLCollection>(shared_from_this(), &children);
            }

            inline StrongPointer<HTMLCollection> getChildren() {
                return StrongPointer<HTMLCollection>(shared_from_this(), &children);
            }

            inline StrongPointer<Element> getFirstElementChild() const {
                return !children.empty() ? children.get(0) : StrongPointer<Element>();
            }

            inline StrongPointer<Element> getLastElementChild() const {
                return !children.empty() ? children.get(children.size() - 1) : StrongPointer<Element>();
            }

            inline StrongPointer<Element> getThisRef() const { return thisRef; }

            UInt getElementIndex() const;

            UInt getLastElementIndex() const;

            UInt getTypedElementIndex() const;

            UInt getLastTypedElementIndex() const;

            StrongPointer<Node> cloneNode(bool deep) const override;

            bool isEqualNode(const StrongPointer<const Node> &other) const override;

            StrongPointer<css::CSSStyleDeclaration> getComputedStyle() {
                if (!computedStyle->isValid()) computeStyle();
                return computedStyle;
            }

        protected:

            //TODO: Implement me
            void
            modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *p) const override {}

        private:
            NamedNodeMap attributes;
            DOMTokenList classList;
            double clientDim[4];
            DOMString ns, prefix, localName;
            double scrollDim[4];
            StrongPointer<ShadowRoot> shadowRoot;
            HTMLCollection children;

            //Caches
            mutable DOMString innerHTML;
            mutable bool innerHTMLValid;

            DOMString cacheInnerHTML() const;

            mutable DOMString outerHTML;
            mutable bool outerHTMLValid;

            DOMString cacheOuterHTML() const;

            StrongPointer<Element> thisRef;

            mutable observable::WatchedObservableItem<Pair<UInt, UInt>> indeces, typedIndeces;

            void updateElementIndeces() const;

            void updatedTypedIndeces() const;

            StrongPointer<css::CSSStyleDeclaration> computedStyle;

            void computeStyle();
        };
    }
}

#endif //FEATHER_ELEMENT_H
