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

namespace feather {
    namespace dom {

        class ShadowRoot;

        enum class Dimensions {
            HEIGHT, LEFT, TOP, WIDTH
        };

        class FilteredByClassName : public observable::Invalidatable {
        public:
            inline StrongPointer<Element> get(UInt idx) { return getCached()[idx].lock(); }

            inline UInt size() { return getCached().size(); }

        protected:
            void modify(RegularEnumSet<observable::InvEvent> &s, const Invalidatable *) const override;

        private:
            DOMString className;
            mutable Vector<WeakPointer<Element>> cached;

            Vector<WeakPointer<Element>> &getCached();
        };

        class FilteredByTagName : public observable::Invalidatable {
        private:
            DOMString tagName;
        };

        class FilteredByTagNameNS : public observable::Invalidatable {
        private:
            DOMString name;
        };

        class Element : public Node, public Slotable {
        public:

            inline StrongPointer<NamedNodeMap> getAttributes() const { return attributes; }

            inline StrongPointer<DOMTokenList> getClassList() const { return classList; }

            inline DOMString getClassName() const { return classList->getValue(); }

            inline void setClassName(DOMString name) { classList->setValue(std::move(name)); }

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

            inline StrongPointer<Element> getNextElementSibling() const {
                return getElementAfterChild(std::static_pointer_cast<const Node>(shared_from_this()));
            }

            inline DOMString getOuterHtml() const { return outerHTMLValid ? outerHTML : cacheOuterHTML(); }

            //TODO: Implement me
            void setOuterHtml(DOMString html);

            inline DOMString getPrefix() const { return prefix; }

            inline StrongPointer<Element> getPreviousElementSibling() const {
                return getElementBeforeChild(std::static_pointer_cast<const Node>(shared_from_this()));
            }

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

            //TODO: Implement me
            StrongPointer<Element> getClosest(DOMString selector) const;

            //TODO: Implement me
            StrongPointer<DOMString> getAttribute(DOMString name) const;

            inline DOMString getAttributeSafe(DOMString name) const {
                auto ptr = getAttribute(std::move(name));
                return ptr ? *ptr : "";
            }

            inline Vector<DOMString> getAttributeNames() const { return attributes->getKeys(); }

            //TODO: Implement me
            StrongPointer<DOMString> getAttributeNS(DOMString ns, DOMString name) const;

            //TODO: Implement me
            DOMRect getBoundingClientRect() const;

            //TODO: Implement me
            virtual Vector<DOMRect> getClientRects() const;

            //TODO: Implement me
            StrongPointer<FilteredByClassName> getElementsByClassName() const;

            //TODO: Implement me
            StrongPointer<FilteredByTagName> getElementsByTagName() const;

            //TODO: Implement me
            StrongPointer<FilteredByTagNameNS> getElementsByTagNameNS() const;

            inline bool hasAttribute(DOMString attr) const { return attributes->contains(std::move(attr)); }

            inline bool hasAttributeNS(const DOMString &ns, const DOMString &name) const {
                return attributes->contains(ns + ":" + name);
            }

            inline bool hasAttributes() const { return attributes && attributes->getLength(); }

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

            inline void removeAttribute(DOMString attr) { attributes->removeNamedItem(std::move(attr)); }

            inline void removeAttributes(DOMString ns, DOMString name) {
                attributes->removeNamedItemNS(std::move(ns), std::move(name));
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

            //TODO: Implement me
            void setAttribute(DOMString name, DOMString value);

            inline void setAttribute(const DOMString &ns, const DOMString &local, DOMString value) {
                setAttribute(ns + ":" + local, std::move(value));
            }

            //TODO: Implement me
            void setPointerCapture(ULong id);

            //TODO: Implement me
            bool toggleAttribute(DOMString attr);

            //TODO: Implement me
            bool toggleAttribute(DOMString attr, bool force);

            //ChildNode impl
            inline void remove() {
                if (getParentNode())
                    getParentNode()->removeChild(std::static_pointer_cast<Node>(shared_from_this()));
            }

            inline void before(Vector<StrongPointer<Node>> insertBefore) {
                if (getParentNode())
                    getParentNode()->insertBeforeChild(std::move(insertBefore),
                                                       std::static_pointer_cast<Node>(shared_from_this()));
            }

            inline void after(Vector<StrongPointer<Node>> insertAfter) {
                if (getParentNode())
                    getParentNode()->insertAfterChild(std::move(insertAfter),
                                                      std::static_pointer_cast<Node>(shared_from_this()));
            }

            inline void replaceWith(Vector<StrongPointer<Node>> repl) {
                if (getParentNode())
                    getParentNode()->replaceChild(std::move(repl), std::static_pointer_cast<Node>(shared_from_this()));
            }

            //ParentNode impl

            inline UInt getChildElementCount() { return children->size(); }

            inline StrongPointer<HTMLCollection> getChildren() const { return children; }

            inline StrongPointer<Element> getFirstElementChild() {
                return children->size() ? children->getItem(0) : nullptr;
            }

            inline StrongPointer<Element> getLastElementChild() {
                return children->size() ? children->getItem(children->size() - 1) : nullptr;
            }

            inline StrongPointer<Element> getThisRef() const { return thisRef; }

        protected:

            //TODO: Implement me
            void modify(RegularEnumSet<observable::InvEvent> &s, const observable::Invalidatable *p) const override;

        private:
            StrongPointer<NamedNodeMap> attributes;
            StrongPointer<DOMTokenList> classList;
            double clientDim[4];
            DOMString ns, prefix, localName;
            double scrollDim[4];
            StrongPointer<ShadowRoot> shadowRoot;
            StrongPointer<HTMLCollection> children;

            //Caches
            mutable DOMString innerHTML;
            mutable bool innerHTMLValid;

            DOMString cacheInnerHTML() const;

            mutable DOMString outerHTML;
            mutable bool outerHTMLValid;

            DOMString cacheOuterHTML() const;

            StrongPointer<Element> thisRef;
        };
    }
}

#endif //FEATHER_ELEMENT_H
