//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_HTML_ELEMENT_H
#define FEATHER_HTML_ELEMENT_H

#include "../element.h"

namespace feather {

    namespace css {
        class CSSStyleDeclaration;
    }

    namespace dom {

        namespace html {

            class HTMLElement;

            class DataSetView : public observable::Invalidatable {
            public:
                DataSetView(NamedNodeMap &watched, HTMLElement &owner) : watched(watched), owner(owner) {}

                StrongPointer<Attr> getAttr(const DOMString &attr) const;

                void setAttr(const DOMString &attr, DOMString value);

                bool hasAttribute(const DOMString &attr) const { return watched.contains(attr); }

            private:
                NamedNodeMap &watched;
                HTMLElement &owner;
            };

            class HTMLElement : public Element {
            public:

                HTMLElement(DOMString baseURI, DOMString tagName, const StrongPointer<Node> &parent,
                            KnownElements type) : Element(std::move(baseURI), std::move(tagName), parent, type),
                                                  realAccessKey(""), dataset(*getAttributes(), *this) {
                    bind(StrongPointer<Invalidatable>(shared_from_this(), &cachedInnerText));
                }

                inline DOMString getAccessKey() const { return getAttributeSafe("accesskey"); }

                inline void setAccessKey(DOMString key) { setAttribute("accesskey", std::move(key)); }

                inline DOMString getAssignedAccessKey() const { return realAccessKey; }

                inline bool isContentEditable() const { return hasAttribute("contenteditable"); }

                inline void setContentEditable(bool contenteditable) {
                    toggleAttribute("contenteditable", contenteditable);
                }

                inline StrongPointer<const DataSetView> getDataSet() const {
                    return StrongPointer<const DataSetView>(shared_from_this(), &dataset);
                }

                inline StrongPointer<DataSetView> getDataSet() {
                    return StrongPointer<DataSetView>(shared_from_this(), &dataset);
                }

                inline TriValue getDir() const {
                    auto dir = getAttributeSafe("dir");
                    return dir.empty() ? TriValue::AUTO : dir == "rtl" ? TriValue::NO : TriValue::YES;
                }

                inline bool isDraggable() const { return hasAttribute("draggable"); }

                inline bool isHidden() const { return hasAttribute("hidden"); }

                inline bool isInert() const { return hasAttribute("inert"); }

                DOMString getInnerText() const;

                void setInnerText(DOMString innerText);

                DOMString getLang() const;

                inline bool isNomodule() const { return hasAttribute("nomodule"); }

                inline ULong getNonce() const { return nonce; }

                inline void setNonce(ULong nonce) { this->nonce = nonce; }

                inline double getOffsetHeight() const { return offsets[static_cast<int>(Dimensions::HEIGHT)]; }

                inline double getOffsetWidth() const { return offsets[static_cast<int>(Dimensions::WIDTH)]; }

                inline double getOffsetTop() const { return offsets[static_cast<int>(Dimensions::TOP)]; }

                inline double getOffsetLeft() const { return offsets[static_cast<int>(Dimensions::LEFT)]; }

                StrongPointer<Element> getOffsetParent() const;

                inline UInt getTabIndex() const { return std::stoul(getAttributeSafe("tabindex")); }

                inline DOMString getTitle() const { return getAttributeSafe("title"); }

                void blur();

                void click();

                void focus();

            private:
                DOMString realAccessKey{};
                DataSetView dataset;
                ULong nonce{};
                double offsets[4]{};

                //Caches
                observable::WatchedObservableItem<DOMString> cachedInnerText;

                DOMString computeInnerText() const;
            };
        }
    }
}
#endif //FEATHER_HTML_ELEMENT_H
