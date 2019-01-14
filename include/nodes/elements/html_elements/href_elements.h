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

                inline DOMString getHref() const { return getAttributeSafe("href"); }

                inline void setHref(DOMString href) { setAttribute("href", std::move(href)); }
            };

            class HTMLAnchorElement : public Hyperlink {
            public:
                inline DOMString getDownload() const { return getAttributeSafe("download"); }

                inline void setDownload(DOMString download) { setAttribute("download", std::move(download)); }

                inline DOMString getMedia() const { return getAttributeSafe("media"); }

                inline void setMedia(DOMString media) { setAttribute("media", std::move(media)); }

                inline DOMString getReferrerPolicy() const { return getAttributeSafe("referrer"); }

                inline void setReferrerPolicy(DOMString referrer) { setAttribute("referrer", std::move(referrer)); }

                inline DOMString getRel() const { return getAttributeSafe("rel"); }

                inline void setRel(DOMString rel) { setAttribute("rel", std::move(rel)); }

                Vector<DOMString> getRelList() const;

                inline DOMString getTarget() const { return getAttributeSafe("target"); }

                inline void setTarget(DOMString target) { setAttribute("target", std::move(target)); }

                inline StrongPointer<const DOMString> getText() const { return getTextContent(); }
            };

            class HTMLAreaElement : public Hyperlink {
            public:

                inline DOMString getAlt() const { return getAttributeSafe("alt"); }

                inline void setAlt(DOMString alt) { setAttribute("alt", std::move(alt)); }

                inline DOMString getCoords() const { return getAttributeSafe("coords"); }

                inline void setCoords(DOMString coords) { setAttribute("coords", std::move(coords)); }

                inline DOMString getDownload() const { return getAttributeSafe("download"); }

                inline void setDownload(DOMString download) { setAttribute("download", std::move(download)); }

                inline DOMString getHrefLang() const { return getAttributeSafe("hreflang"); }

                inline void setHrefLang(DOMString hrefLang) { setAttribute("hreflang", std::move(hrefLang)); }

                inline DOMString getMedia() const { return getAttributeSafe("media"); }

                inline void setMedia(DOMString media) { setAttribute("media", std::move(media)); }

                inline DOMString getReferrerPolicy() const { return getAttributeSafe("referrer"); }

                inline void setReferrerPolicy(DOMString referrer) { setAttribute("referrer", std::move(referrer)); }

                inline DOMString getRel() const { return getAttributeSafe("rel"); }

                inline void setRel(DOMString rel) { setAttribute("rel", std::move(rel)); }

                Vector<DOMString> getRelList() const;

                inline DOMString getShape() const { return getAttributeSafe("shape"); }

                inline void setShape(DOMString shape) { setAttribute("shape", std::move(shape)); }

                inline DOMString getTarget() const { return getAttributeSafe("target"); }

                inline void setTarget(DOMString target) { setAttribute("target", std::move(target)); }

                inline StrongPointer<const DOMString> getText() const { return getTextContent(); }
            };

            class HTMLBaseElement : public HTMLElement {
            public:
                inline DOMString getHref() const { return getAttributeSafe("href"); }

                inline void setHref(DOMString href) { setAttribute("href", std::move(href)); }

                inline DOMString getTarget() const { return getAttributeSafe("target"); }

                inline void setTarget(DOMString target) { setAttribute("target", std::move(target)); }
            };

            class LinkStyle {
            public:
                inline StrongPointer <css::CSSStyleSheet> getSheet() { return stylesheet; }

                inline void setSheet(StrongPointer <css::CSSStyleSheet> sheet) { stylesheet = std::move(sheet); }

            private:
                StrongPointer <css::CSSStyleSheet> stylesheet;
            };

            class HTMLLinkElement : public HTMLElement, public LinkStyle {
            public:
                inline DOMString getAs() const { return getAttributeSafe("as"); }

                inline void setAs(DOMString as) { setAttribute("as", std::move(as)); }

                inline DOMString getCrossOrigin() const { return getAttributeSafe("crossorigin"); }

                inline void setCrossOrigin(DOMString crossorigin) {
                    setAttribute("crossorigin", std::move(crossorigin));
                }

                inline bool isDisabled() const { return hasAttribute("disabled"); }

                inline void setDisabled(bool disabled) { toggleAttribute("disabled", disabled); }

                inline DOMString getHref() const { return getAttributeSafe("href"); }

                inline void setHref(DOMString href) { setAttribute("href", std::move(href)); }

                inline DOMString getHreflang() const { return getAttributeSafe("hreflang"); }

                inline void setHreflang(DOMString hreflang) { setAttribute("hreflang", std::move(hreflang)); }

                inline DOMString getMedia() const { return getAttributeSafe("media"); }

                inline void setMedia(DOMString media) { setAttribute("media", std::move(media)); }

                inline DOMString getReferrerPolicy() const { return getAttributeSafe("referrer"); }

                inline void setReferrerPolicy(DOMString referrer) { setAttribute("referrer", std::move(referrer)); }

                inline DOMString getRel() const { return getAttributeSafe("rel"); }

                inline void setRel(DOMString rel) { setAttribute("rel", std::move(rel)); }

                inline Vector<DOMString> getRelList() const;

                inline Vector<DOMString> getSizes() const;

                inline DOMString getType() const { return getAttributeSafe("type"); }

                inline void setType(DOMString type) { setAttribute("type", std::move(type)); }
            };

            class HTMLSourceElement : public HTMLElement {
            public:
                inline DOMString getKeySystem() const { return getAttributeSafe("keysystem"); }

                inline void setKeySystem(DOMString keySystem) { setAttribute("keysystem", std::move(keySystem)); }

                inline DOMString getMedia() const { return getAttributeSafe("media"); }

                inline void setMedia(DOMString media) { setAttribute("media", std::move(media)); }

                inline DOMString getSizes() const { return getAttributeSafe("sizes"); }

                inline void setSizes(DOMString sizes) { setAttribute("sizes", std::move(sizes)); }

                inline DOMString getSrc() const { return getAttributeSafe("src"); }

                inline void setSrc(DOMString src) { setAttribute("src", std::move(src)); }

                inline DOMString getSrcset() const { return getAttributeSafe("srcset"); }

                inline void setSrcset(DOMString srcset) { setAttribute("srcset", std::move(srcset)); }

                inline DOMString getType() const { return getAttributeSafe("type"); }

                inline void setType(DOMString type) { setAttribute("type", std::move(type)); }
            };
        }
    }
}
#endif //CURL_HREF_ELEMENTS_H
