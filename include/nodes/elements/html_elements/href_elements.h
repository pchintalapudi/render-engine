//
// Created by prem on 12/30/2018.
//

#ifndef CURL_HREF_ELEMENTS_H
#define CURL_HREF_ELEMENTS_H

#include "html_element.h"
#include "style/css/css_style_sheet.h"
#include "globals/window.h"

namespace feather {
    namespace dom {

        namespace html {

            class Hyperlink : public HTMLElement, global::URLUtils<Hyperlink> {
            public:

                using HTMLElement::HTMLElement;

                ATTRIBUTE(href, Href)

                ATTRIBUTE(download, Download)

                ATTRIBUTE(media, Media)

                ATTRIBUTE(referrer, ReferrerPolicy)

                ATTRIBUTE(rel, Rel)

                Vector<DOMString> getRelList() const;

                ATTRIBUTE(target, Target)

                inline StrongPointer<const DOMString> getText() const { return getTextContent(); }
            };

            class HTMLAnchorElement : public Hyperlink {
            public:
                HTMLAnchorElement(DOMString baseURI, const StrongPointer<Node> &parent)
                        : Hyperlink(std::move(baseURI), "a", parent, KnownElements::HTMLAnchorElement) {}

                CLONE_DEC(HTMLAnchorElement)
            };

            class HTMLAreaElement : public Hyperlink {
            public:

                HTMLAreaElement(DOMString baseURI, const StrongPointer<Node> &parent)
                        : Hyperlink(std::move(baseURI), "area", parent, KnownElements::HTMLAreaElement) {}

                ATTRIBUTE(alt, Alt)

                ATTRIBUTE(coords, Coords)

                ATTRIBUTE(hreflang, HrefLang)

                ATTRIBUTE(shape, Shape)

                CLONE_DEC(HTMLAreaElement)
            };

            class HTMLBaseElement : public HTMLElement {
            public:

                HTML_ELEMENT_CONSTRUCTOR(HTMLBaseElement, base, KnownElements::HTMLBaseElement)

                ATTRIBUTE(href, Href)

                ATTRIBUTE(target, Target)
            };

            class LinkStyle {
            public:
                inline StrongPointer<css::CSSStyleSheet> getSheet() { return stylesheet; }

                inline void setSheet(StrongPointer<css::CSSStyleSheet> sheet) { stylesheet = std::move(sheet); }

            private:
                StrongPointer<css::CSSStyleSheet> stylesheet;
            };

            class HTMLLinkElement : public HTMLElement, public LinkStyle {
            public:

                HTMLLinkElement(DOMString baseURI, const StrongPointer<Node> &parent)
                        : HTMLElement(std::move(baseURI), "link", parent, KnownElements::HTMLLinkElement),
                          LinkStyle() {}

                ATTRIBUTE(as, As)

                ATTRIBUTE(crossorigin, CrossOrigin)

                B_ATTRIBUTE(disabled, Disabled)

                ATTRIBUTE(href, Href)

                ATTRIBUTE(hreflang, HrefLang)

                ATTRIBUTE(media, Media)

                ATTRIBUTE(referrer, ReferrerPolicy)

                ATTRIBUTE(rel, Rel)

                inline Vector<DOMString> getRelList() const;

                inline Vector<DOMString> getSizes() const;

                ATTRIBUTE(types, Types)

                CLONE_DEC(HTMLLinkElement)
            };

            class HTMLSourceElement : public HTMLElement {
            public:

                HTML_ELEMENT_CONSTRUCTOR(HTMLSourceElement, src, KnownElements::HTMLSourceElement)

                ATTRIBUTE(keysystem, KeySystem)

                ATTRIBUTE(media, Media)

                ATTRIBUTE(sizes, Sizes)

                ATTRIBUTE(src, Src)

                ATTRIBUTE(srcset, SrcSet)

                ATTRIBUTE(type, Type)
            };
        }
    }
}
#endif //CURL_HREF_ELEMENTS_H
