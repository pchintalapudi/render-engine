//
// Created by prem on 12/30/2018.
//

#ifndef CURL_HREF_ELEMENTS_H
#define CURL_HREF_ELEMENTS_H

#include "html_element.h"
#include "style/style_sheet.h"

namespace feather {
    namespace dom {

        namespace html {
            class HyperlinkUtils {
            public:
                HyperlinkUtils() = default;

                inline DOMString getHref() const { return href; }

                void setHref(DOMString protocol);

                inline DOMString getOrigin() const { return getProtocol() + getHost(); }

                inline DOMString getProtocol() const { return getPart(URLPointer::SCHEME, URLPointer::USERNAME); }

                void setProtocol(DOMString protocol);

                inline DOMString getUsername() const { return getPart(URLPointer::USERNAME, URLPointer::PASSWORD); }

                void setUsername(DOMString username);

                inline DOMString getPassword() const { return getPart(URLPointer::PASSWORD, URLPointer::HOSTNAME); }

                void setPassword(DOMString password);

                inline DOMString getHost() const { return getPart(URLPointer::HOSTNAME, URLPointer::PATHNAME); }

                void setHost(DOMString host);

                inline DOMString getHostName() const { return getPart(URLPointer::HOSTNAME, URLPointer::PORT); }

                void setHostName(DOMString hostName);

                inline UShort getPort() const {
                    auto port = getPart(URLPointer::PORT, URLPointer::PATHNAME);
                    return port.empty() ? ~0u : std::stoi(port.substr(1));
                }

                void setPort(UShort port);

                Vector<DOMString> getPath() const;

                void setPath(DOMString path);

                inline DOMString getSearch() const { return getPart(URLPointer::SEARCH, URLPointer::HASH); }

                void setSearch(DOMString search);

                inline DOMString getHash() const { return getPart(URLPointer::HASH, URLPointer::END); }

                void setHash(DOMString hash);

                inline DOMString toString() const { return getHref(); }

            private:

                enum class URLPointer {
                    SCHEME, USERNAME, PASSWORD, HOSTNAME, PORT, PATHNAME, SEARCH, HASH, END, __COUNT__
                };

                inline DOMString getPart(URLPointer start, URLPointer end) const {
                    return getHref().substr(static_cast<UInt>(start),
                                            static_cast<UInt>(end) - static_cast<UInt>(start));
                }

                DOMString href;
                DOMString original;
                UInt pointers[static_cast<int>(URLPointer::__COUNT__)];
            };

            class HTMLAnchorElement : public HTMLElement, private HyperlinkUtils {
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

                inline StrongPointer<DOMString> getText() const { return getTextContent(); }
            };

            class HTMLAreaElement : public HTMLElement, private HyperlinkUtils {
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

                inline StrongPointer<DOMString> getText() const { return getTextContent(); }
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
                inline StrongPointer<css::StyleSheet> getSheet() { return stylesheet; }

                inline void setSheet(StrongPointer<css::StyleSheet> sheet) { stylesheet = std::move(sheet); }

            private:
                StrongPointer<css::StyleSheet> stylesheet;
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
