//
// Created by prem on 12/30/2018.
//

#ifndef CURL_OUTPUT_ELEMENTS_H
#define CURL_OUTPUT_ELEMENTS_H

#include "html_element.h"

namespace feather {
    namespace dom {
        namespace html {
            class HTMLMediaElement : public HTMLElement {
                //TODO: Oh dear god this is going to be a pain
            };

            class HTMLAudioElement : public HTMLMediaElement {
            };

            class HTMLVideoElement : public HTMLMediaElement {
            public:
                inline DOMString getHeight() const { return getAttributeSafe("height"); }

                inline void setHeight(DOMString height) { setAttribute("height", std::move(height)); }

                inline DOMString getPoster() const { return getAttributeSafe("poster"); }

                inline void setPoster(DOMString poster) { setAttribute("poster", std::move(poster)); }

                inline UInt getVideoHeight() const { return vHeight; }

                inline UInt getVideoWidth() const { return vWidth; }

                inline DOMString getWidth() const { return getAttributeSafe("width"); }

                inline void setWidth(DOMString width) { setAttribute("width", std::move(width)); }

            private:
                UInt vHeight, vWidth;
            };

            class HTMLCanvasElement : public HTMLElement {
                //TODO: Implement canvas
            };

            class HTMLDialogElement : public HTMLElement {
            public:
                inline bool isOpen() const { return hasAttribute("open"); }

                inline void setOpen(bool open) { toggleAttribute("open", open); }

                inline DOMString getReturnValue() const { return rValue; }

                inline void setReturnValue(DOMString returnValue) { rValue = std::move(returnValue); }

            private:
                DOMString rValue;
            };

            class HTMLEmbedElement : public HTMLElement {
            public:
                inline DOMString getHeight() const { return getAttributeSafe("height"); }

                inline void setHeight(DOMString height) { setAttribute("height", std::move(height)); }

                inline DOMString getSrc() const { return getAttributeSafe("src"); }

                inline void setSrc(DOMString src) { setAttribute("src", std::move(src)); }

                inline DOMString getType() const { return getAttributeSafe("type"); }

                inline void setType(DOMString type) { setAttribute("type", std::move(type)); }

                inline DOMString getWidth() const { return getAttributeSafe("width"); }

                inline void setWidth(DOMString width) { setAttribute("width", std::move(width)); }
            };

            class HTMLIframeElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLImageElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLObjectElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLProgressElement : public HTMLElement {
                //TODO: write class
            };

            class HTMLTrackElement : public HTMLElement {
                //TODO: write class
            };
        }
    }
}
#endif //CURL_OUTPUT_ELEMENTS_H
