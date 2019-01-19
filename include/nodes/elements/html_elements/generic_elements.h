//
// Created by prem on 12/30/2018.
//

#ifndef CURL_GENERIC_ELEMENTS_H
#define CURL_GENERIC_ELEMENTS_H

#include "html_element.h"

namespace feather {
    namespace dom {
        namespace html {
            class HTMLBRElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLBRElement, br, KnownElements::HTMLBRElement)
            };

            class HTMLBodyElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLBodyElement, body, KnownElements::HTMLBodyElement)
            };

            class HTMLDListElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLDListElement, datalist, KnownElements::HTMLDListElement)
            };

            class HTMLDivElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLDivElement, div, KnownElements::HTMLDivElement)
            };

            class HTMLHRElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLHRElement, hr, KnownElements::HTMLHRElement)
            };

            class HTMLHeadElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLHeadElement, head, KnownElements::HTMLHeadElement)
            };

            class HTMLHeadingElement : public HTMLElement {
            public:
                HTMLHeadingElement(DOMString baseURI, unsigned char hcount, const StrongPointer<Node> &parent)
                        : HTMLElement(std::move(baseURI), "h" + std::to_string(hcount), parent,
                                      static_cast<KnownElements>(int(KnownElements::HTMLH1Element) - 1 + hcount)) {}

                CLONE_DEC(HTMLHeadingElement)
            };

            class HTMLHtmlElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLHtmlElement, html, KnownElements::HTMLHtmlElement)
            };

            class HTMLParagraphElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLParagraphElement, p, KnownElements::HTMLParagraphElement)
            };

            class HTMLPictureElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLPictureElement, picture, KnownElements::HTMLPictureElement)
            };

            class HTMLPreElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLPreElement, pre, KnownElements::HTMLPreElement)
            };

            class HTMLSpanElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLSpanElement, span, KnownElements::HTMLSpanElement)
            };

            class HTMLUListElement : public HTMLElement {
            public:
                HTML_ELEMENT_CONSTRUCTOR(HTMLUListElement, ul, KnownElements::HTMLUListElement)
            };
        }
    }
}
#endif //CURL_GENERIC_ELEMENTS_H
