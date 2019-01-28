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

#define ATTRIBUTE(attr, attrPascalCase) inline const DOMString &get##attrPascalCase() const {return getAttributeSafe(#attr);} inline void set##attrPascalCase(DOMString attr) {setAttribute(#attr, std::move(attr));}
#define B_ATTRIBUTE(attr, attrPascalCase) inline bool is##attrPascalCase() const {return hasAttribute(#attr);} inline void set##attrPascalCase(bool (attr)) {toggleAttribute(#attr, attr);}
#define L_ATTRIBUTE(attr, attrPascalCase) inline Long get##attrPascalCase() const {return std::stoull(#attr);} inline void set##attrPascalCase(Long (attr)) {setAttribute(#attr, std::to_string(attr));}
#define STATIC_FACTORY(name) static StrongPointer<name> create(DOMString baseURI, const StrongPointer<Node> &parent) {return std::make_shared<name>(std::move(baseURI), parent);}
#define CLONE_DEC(name) StrongPointer<Node> cloneNode(bool deep) const override;
#define HTML_ELEMENT_CONSTRUCTOR(name, tag, id) name(DOMString baseURI, const StrongPointer<Node> &parent) : HTMLElement(std::move(baseURI), #tag, parent, id) {} STATIC_FACTORY(name) CLONE_DEC(name)

            class HTMLElement : public Element {
            public:

                HTMLElement(DOMString baseURI, DOMString tagName, const StrongPointer<Node> &parent,
                            KnownElements type) : Element(std::move(baseURI), std::move(tagName), parent, type),
                                                  realAccessKey(""), dataset(*getAttributes(), *this) {
                    bindTo(StrongPointer<Invalidatable>(shared_from_this(), &cachedInnerText));
                }

                ATTRIBUTE(accesskey, AccessKey)

                inline DOMString getAssignedAccessKey() const { return realAccessKey; }

                B_ATTRIBUTE(contenteditable, ContentEditable)

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

                B_ATTRIBUTE(draggable, Draggable)

                B_ATTRIBUTE(hidden, Hidden)

                B_ATTRIBUTE(inert, Inert)

                const DOMString &getInnerText() const;

                void setInnerText(DOMString innerText);

                DOMString getLang() const;

                B_ATTRIBUTE(nomodule, NoModule)

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
                mutable observable::ObservableItem<DOMString, false> cachedInnerText{};

                DOMString computeInnerText() const;
            };
        }
    }
}
#endif //FEATHER_HTML_ELEMENT_H
