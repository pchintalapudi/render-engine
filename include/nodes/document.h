//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_DOCUMENT_H
#define FEATHER_DOCUMENT_H

#include "include/css/pseudoclass_manager.h"
#include "node.h"

namespace dom {
    class Document;
}

class dom::Document : public Node {
public:
    inline css::PseudoclassManager &getPseudoclassManager() { return pseudoclassManager; }

    inline dom::Element *getDocumentElement() { return root; }

private:
    css::PseudoclassManager pseudoclassManager;
    dom::Element *root;
};

#endif //FEATHER_DOCUMENT_H
