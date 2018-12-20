//
// Created by binhe on 12/2/2018.
//

#ifndef FEATHER_CSS_STYLE_DECLARATION_H
#define FEATHER_CSS_STYLE_DECLARATION_H

#include "typedefs.h"

namespace feather {
    namespace css {

        class CSSStyleDeclaration {
        public:
            void setProperty(DOMString property, DOMString value, DOMString priority);

            DOMString getPropertyPriority(DOMString priority);

            DOMString getPropertyValue(DOMString property);

            DOMString removeProperty(DOMString property);

            DOMString item(int index);

            void getProperty();
        };
    }
}

#endif //FEATHER_CSS_STYLE_DECLARATION_H
