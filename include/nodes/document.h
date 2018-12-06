//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_DOCUMENT_H
#define FEATHER_DOCUMENT_H

#include "document_or_shadow_root.h"

namespace dom {
    class Document;
}

class dom::Document : public DocumentOrShadowRoot {
public:

    inline css::PseudoclassManager &getPseudoclassManager() { return getPseudoclassManagerInternal(); }

    inline dom::Element *getDocumentElement() { return root; }

private:
    dom::Element *root;
};

#endif //FEATHER_DOCUMENT_H
