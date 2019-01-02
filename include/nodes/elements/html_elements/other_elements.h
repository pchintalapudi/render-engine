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
                inline DOMString getValue() const { return getAttributeSafe("value"); }

                inline void setValue(DOMString value) { setAttribute("value", std::move(value)); }
            };

            class HTMLDataListElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLLIElement : public HTMLElement {
            public:
                inline Long getValue() const { return std::stoll(getAttributeSafe("value")); }

                inline void setValue(Long l) { setAttribute("value", std::to_string(l)); }
            };

            class HTMLMapElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLMetaElement : public HTMLElement {
                inline DOMString getContent() const { return getAttributeSafe("content"); }

                inline void setContent(DOMString content) { setAttribute("content", std::move(content)); }

                inline DOMString getHttpEquiv() const { return getAttributeSafe("http-equiv"); }

                inline void setHttpEquiv(DOMString httpEquiv) { setAttribute("http-equiv", std::move(httpEquiv)); }

                inline DOMString getName() const { return getAttributeSafe("name"); }

                inline void setName(DOMString name) { setAttribute("name", std::move(name)); }
            };

            class HTMLModeElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLOListElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLQuoteElement : public HTMLElement {
            public:
                inline DOMString getCite() const { return getAttributeSafe("cite"); }

                inline void setCite(DOMString cite) { setAttribute("cite", std::move(cite)); }
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
                inline DOMString getTime() const { return getAttributeSafe("datetime"); }

                inline void setTime(DOMString time) { setAttribute("datetime", std::move(time)); }
            };

            class HTMLTitleElement : public HTMLElement {
                inline DOMString getText() const { return getAttributeSafe("text"); }

                inline void setText(DOMString text) { setAttribute("text", std::move(text)); }
            };
        }
    }
}
#endif //CURL_OTHER_ELEMENTS_H
