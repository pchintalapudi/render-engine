//
// Created by prem on 1/10/2019.
//

#ifndef CURL_NAVIGATOR_H
#define CURL_NAVIGATOR_H

#include "typedefs.h"

namespace feather {
    namespace global {
        namespace navigator {
            class NavigatorID {
            private:
                DOMString appCodeName = "Mozilla",
                        appName = "Netscape",
                        appVersion = "4.0",
                        platform = "",
                        product = "Gecko",
                        productSub = "20000000",
                        userAgent = "Feather/1.0",
                        vendor = "Feather",
                        vendorSub = "";
            public:
                const DOMString &getAppCodeName() const {
                    return appCodeName;
                }

                const DOMString &getAppName() const {
                    return appName;
                }

                const DOMString &getAppVersion() const {
                    return appVersion;
                }

                const DOMString &getPlatform() const {
                    return platform;
                }

                const DOMString &getProduct() const {
                    return product;
                }

                const DOMString &getProductSub() const {
                    return productSub;
                }

                const DOMString &getUserAgent() const {
                    return userAgent;
                }

                const DOMString &getVendor() const {
                    return vendor;
                }

                const DOMString &getVendorSub() const {
                    return vendorSub;
                }
            };

            class NavigatorLanguage {
            private:
                DOMString language{};
                Vector <DOMString> languages{};
            public:
                const DOMString &getLanguage() const {
                    return language;
                }

                const Vector <DOMString> &getLanguages() const {
                    return languages;
                }
            };

            class NavigatorContentUtils {
                void registerProtocolHandler(DOMString scheme, DOMString url, DOMString title);

                void unregisterProtocolHandler(DOMString scheme, DOMString url);
            };

            class NavigatorCookies {
            public:
                inline bool isCookieEnabled() const { return cookieEnabled; }

            private:
                bool cookieEnabled = false;
            };

            class NavigatorConcurrentHardware {
            public:
                inline ULong getHardwareConcurrency() { return 1; }
            };
        }

        class Navigator
                : navigator::NavigatorID,
                  navigator::NavigatorContentUtils,
                  navigator::NavigatorCookies,
                  navigator::NavigatorLanguage,
                  navigator::NavigatorConcurrentHardware {
        };
    }
}
#endif //CURL_NAVIGATOR_H
