//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_DOCUMENT_H
#define FEATHER_DOCUMENT_H

#include "../node.h"

namespace feather {
    namespace dom {

        class DocumentOrShadowRoot : public Node {
        };

        class Document : public DocumentOrShadowRoot {
            StrongPointer <Element> getDocumentElement();
        };
    }
}

#endif //FEATHER_DOCUMENT_H
