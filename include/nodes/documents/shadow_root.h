//
// Created by prem on 12/13/2018.
//

#ifndef FEATHER_SHADOW_ROOT_H
#define FEATHER_SHADOW_ROOT_H

#include "document.h"
#include "../elements/element.h"

namespace feather {
    namespace dom {
        class ShadowRoot : public Element, public DocumentOrShadowRoot {
        public:

            ShadowRoot(DOMString baseURI, const StrongPointer <Element> &parent)
                    : Element(std::move(baseURI), "shadow", parent, KnownElements::HTMLShadowElement),
                      DocumentOrShadowRoot() {}

            inline bool getMode() { return open; }

            inline StrongPointer <Element> getHost() { return std::static_pointer_cast<Element>(getParentNode()); }

        private:
            bool open = false;
        };
    }
}
#endif //FEATHER_SHADOW_ROOT_H
