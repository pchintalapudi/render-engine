//
// Created by prem on 1/10/2019.
//

#ifndef CURL_WINDOW_UTILS_H
#define CURL_WINDOW_UTILS_H

#include "observable/invalidatable.h"

namespace feather {
    namespace global {

        template<typename Derived>
        class URLUtils {
        public:

            inline DOMString getHref() const { return static_cast<Derived *>(this)->getHref(); }

            inline void setHref(DOMString href) { static_cast<Derived *>(this)->setHref(std::move(href)); }

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

            Vector <DOMString> getPath() const;

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

            DOMString original;
            UInt pointers[static_cast<int>(URLPointer::__COUNT__)];
        };

        class Window;

        namespace wlists {

            class FrameList;
        }

        class History : public observable::Invalidatable {
        public:
            struct HistoryData {
                DOMString data, title, url;

                explicit operator bool() const { return !url.empty(); }
            };

            const HistoryData &operator*() const { return history[index]; }

            HistoryData &operator*() { return history[index]; }

            void operator<<(HistoryData next) {
                history.erase(history.begin() + index++, history.end());
                history.push_back(std::move(next));
                invalidate();
            }

            inline HistoryData &operator++() { return *this += 1; }

            inline HistoryData &operator--() { return *this -= 1; }

            inline HistoryData &operator+=(Int dist) {
                auto idx = index += dist < Int(history.size()) && index > 0 ? index : index -= dist;
                invalidate();
                return history[idx];
            }

            inline HistoryData &operator-=(Int dist) { return *this += -dist; }

            inline UInt size() const { return history.size(); }

        private:
            Vector <HistoryData> history;
            UInt index = ~0u;

            void invalidate() {}
        };

        class Location : public URLUtils<Location> {
        public:
            DOMString getHref() const;

            inline void setHref(DOMString href) { assign(std::move(href)); }

            void assign(DOMString href);

            void replace(DOMString href);

            void reload(bool force);

        private:
            WeakPointer <Window> owner;
        };
    }
}
#endif //CURL_WINDOW_UTILS_H
