//
// Created by prem on 12/30/2018.
//

#ifndef CURL_OUTPUT_ELEMENTS_H
#define CURL_OUTPUT_ELEMENTS_H

#include "html_element.h"

namespace feather {
    namespace dom {
        namespace html {

#define SIZED_ELEMENT ATTRIBUTE(height, Height) ATTRIBUTE(width, Width)

            class HTMLMediaElement : public HTMLElement {
                //TODO: Oh dear god this is going to be a pain
            };

            class HTMLAudioElement : public HTMLMediaElement {
            };

            class HTMLVideoElement : public HTMLMediaElement {
            public:

                ATTRIBUTE(poster, Poster)

                inline UInt getVideoHeight() const { return vHeight; }

                inline UInt getVideoWidth() const { return vWidth; }

                SIZED_ELEMENT

            private:
                UInt vHeight{}, vWidth{};
            };

            class HTMLCanvasElement : public HTMLElement {
                //TODO: Implement canvas
            };

            class HTMLDialogElement : public HTMLElement {
            public:

                B_ATTRIBUTE(open, Open)

                inline DOMString getReturnValue() const { return rValue; }

                inline void setReturnValue(DOMString returnValue) { rValue = std::move(returnValue); }

            private:
                DOMString rValue{};
            };

            class HTMLEmbedElement : public HTMLElement {
            public:

                SIZED_ELEMENT

                ATTRIBUTE(src, Src)

                ATTRIBUTE(type, Type)
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

#undef SIZED_ELEMENT
        }
    }
}
#endif //CURL_OUTPUT_ELEMENTS_H
