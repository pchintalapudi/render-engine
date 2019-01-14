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

            inline bool getMode() { return open; }

            inline StrongPointer <Element> getHost() { return host.expired() ? nullptr : host.lock(); }

        private:
            bool open;
            WeakPointer <Element> host;
        };
    }
}
#endif //FEATHER_SHADOW_ROOT_H
