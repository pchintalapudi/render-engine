//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_DOCUMENT_H
#define FEATHER_DOCUMENT_H

#include "../node.h"
#include "css/pseudoclass_manager.h"

namespace feather {
    namespace dom {

        class DocumentOrShadowRoot {
        public:
            css::PseudoclassManager getPseudoclassManager() { return pseudoclassManager; }

        private:
            css::PseudoclassManager pseudoclassManager;
        };

        class Document : public Node, public DocumentOrShadowRoot {
        public:
            StrongPointer<Element> getDocumentElement();
        };
    }
}

#endif //FEATHER_DOCUMENT_H
