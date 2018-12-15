//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_DOCUMENT_H
#define FEATHER_DOCUMENT_H

#include "../node.h"
#include "css/pseudoclass_manager.h"

namespace feather {
    namespace dom {

        class DocumentOrShadowRoot : public Node {
            css::PseudoclassManager getPseudoclassManager() { return pseudoclassManager; }

        private:
            css::PseudoclassManager pseudoclassManager;
        };

        class Document : public DocumentOrShadowRoot {
            StrongPointer <Element> getDocumentElement();
        };
    }
}

#endif //FEATHER_DOCUMENT_H
