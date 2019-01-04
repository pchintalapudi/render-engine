//
// Created by prem on 12/12/2018.
//

#ifndef FEATHER_HTML_ELEMENT_H
#define FEATHER_HTML_ELEMENT_H

#include "../element.h"

namespace feather {
    namespace dom {

        namespace html {
            class DataSetView {
            };

            class HTMLElement : public Element {
            public:
                inline DOMString getAccessKey() const { return accessKey; }

                void setAccessKey(DOMString key);

                inline DOMString getAssignedAccessKey() const { return realAccessKey; }

                bool isContentEditable() const { return contentEditable; }

                inline StrongPointer<DataSetView> getDataSet() const { return dataset; }

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

                //TODO: Get a CSSStyleDeclaration
                StrongPointer<void> getStyle() const;

                //TODO: Get a CSSStyleDeclaration
                void setStyle(StrongPointer<void> style);

                inline UInt getTabIndex() const { return std::stoul(getAttributeSafe("tabindex")); }

                inline DOMString getTitle() const { return getAttributeSafe("title"); }

                void blur();

                void click();

                void focus();

            private:
                DOMString accessKey;
                DOMString realAccessKey;
                bool contentEditable;
                StrongPointer<DataSetView> dataset;
                ULong nonce;
                double offsets[4];

                //Caches
                StrongPointer<observable::WatchedObservableItem<DOMString>> cachedInnerText;

                DOMString computeInnerText() const;
            };
        }
    }
}
#endif //FEATHER_HTML_ELEMENT_H
