//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_SHADOW_ROOT_H
#define FEATHER_SHADOW_ROOT_H

#include "element.h"
#include "document_or_shadow_root.h"

namespace dom {
    class ShadowRoot;
}

class dom::ShadowRoot : DocumentOrShadowRoot, Element {
private:
    bool open;
    Element *host;

};
#endif //FEATHER_SHADOW_ROOT_H
