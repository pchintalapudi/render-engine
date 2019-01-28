//
// Created by prem on 12/31/2018.
//

#ifndef CURL_OTHER_ELEMENTS_H
#define CURL_OTHER_ELEMENTS_H

#include "html_element.h"

namespace feather {
    namespace dom {
        namespace html {

            class HTMLDataElement : public HTMLElement {
            public:

                ATTRIBUTE(value, Value)
            };

            class HTMLDataListElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLLIElement : public HTMLElement {
            public:

                L_ATTRIBUTE(value, Value)
            };

            class HTMLMapElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLMetaElement : public HTMLElement {

                ATTRIBUTE(content, Content)

                inline DOMString getHttpEquiv() const { return getAttributeSafe("http-equiv"); }

                inline void setHttpEquiv(DOMString httpEquiv) { setAttribute("http-equiv", std::move(httpEquiv)); }

                ATTRIBUTE(name, Name)
            };

            class HTMLModeElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLOListElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLQuoteElement : public HTMLElement {
            public:

                ATTRIBUTE(cite, Cite)
            };

            class HTMLScriptElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLStyleElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLTemplateElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLTimeElement : public HTMLElement {
                ATTRIBUTE(time, Time)
            };

            class HTMLTitleElement : public HTMLElement {
                ATTRIBUTE(text, Text)
            };
        }
    }
}
#endif //CURL_OTHER_ELEMENTS_H
