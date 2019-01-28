//
// Created by prem on 12/31/2018.
//

#ifndef CURL_TABLE_ELEMENTS_H
#define CURL_TABLE_ELEMENTS_H

#include "html_element.h"

namespace feather {
    namespace dom {
        namespace html {
            class HTMLTableCaptionElement : public HTMLElement {
            };

            class HTMLTableCellElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLTableColElement : public HTMLElement {
            public:

                L_ATTRIBUTE(span, Span)
            };

            class HTMLTableElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLTableRowElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLTableSectionElement : public HTMLElement {
                //TODO: write class
            };
        }
    }
}
#endif //CURL_TABLE_ELEMENTS_H
