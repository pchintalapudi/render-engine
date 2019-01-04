//
// Created by binhe on 12/2/2018.
//

#ifndef FEATHER_CSS_STYLE_DECLARATION_H
#define FEATHER_CSS_STYLE_DECLARATION_H

#include "typedefs.h"
#include <unordered_map>
#include <tuple>

namespace feather {
    namespace css {
        namespace rules {
            class CSSStyleDeclaration {
            private:
                std::unordered_map<DOMString, std::tuple<DOMString, DOMString>> properties;
            public:
                void setProperty(feather::DOMString property, feather::DOMString value, feather::DOMString priority);

                feather::DOMString getPropertyPriority(DOMString);

                feather::DOMString getPropertyValue(DOMString);

                feather::DOMString removeProperty(DOMString);

                feather::DOMString item(feather::UInt);
            };
        }
    }
}

#endif //FEATHER_CSS_STYLE_DECLARATION_H
