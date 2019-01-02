//
// Created by prem on 12/30/2018.
//

#ifndef CURL_HREF_ELEMENTS_H
#define CURL_HREF_ELEMENTS_H

#include "html_element.h"

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

            class HTMLLinkElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLSourceElement : public HTMLElement {
                //TODO: write class
            };
        }
    }
}
#endif //CURL_HREF_ELEMENTS_H
