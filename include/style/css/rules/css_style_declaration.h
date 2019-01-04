//
// Created by binhe on 12/2/2018.
//

#ifndef FEATHER_CSS_STYLE_DECLARATION_H
#define FEATHER_CSS_STYLE_DECLARATION_H

#include "typedefs.h"

namespace feather {
    namespace css {
        namespace rules {
            class CSSStyleDeclaration {
            private:
                Map<DOMString, Pair<DOMString, bool>> properties;
            public:
                void setProperty(DOMString property, DOMString value, bool priority);

                DOMString getPropertyPriority(DOMString) const;

                DOMString getPropertyValue(DOMString) const;

                DOMString removeProperty(DOMString);

                DOMString item(UInt) const;
            };
        }
    }
}

#endif //FEATHER_CSS_STYLE_DECLARATION_H
