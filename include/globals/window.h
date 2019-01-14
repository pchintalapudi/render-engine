//
// Created by prem on 1/9/2019.
//

#ifndef CURL_WINDOW_H
#define CURL_WINDOW_H

#include "window_utils.h"
#include "navigator.h"
#include "nodes/documents/document.h"
#include "storage.h"
#include "geom/dom_rect.h"

namespace feather {

    namespace crypto {
        class Crypto;
    }

    namespace global {
        enum Orientation {
            PORTRAIT_PRIMARY, PORTRAIT_SECONDARY, LANDSCAPE_PRIMARY, LANDSCAPE_SECONDARY
        };

        class ScreenOrientation {
        public:
            inline Orientation getOrientation() const {
                return orientation;
            }

            double getAngle();

        private:
            Orientation orientation;
        };

        class Screen : public js::EventTarget {
        public:
            double getAvailTop();

            double getAvailLeft();

            double getAvailHeight();

            double getAvailWidth();

            unsigned char getColorDepth() { return 24; }

            double getHeight();

        private:
            observable::SourceObservableItem<ScreenOrientation> orientation;
        };

        class Window : public js::EventTarget {
        public:

            inline StrongPointer<void> getConsole() { return StrongPointer<void>(); }

            inline StrongPointer<void> getCustomElements() { return StrongPointer<void>(); }

            inline StrongPointer<crypto::Crypto> getCrypto() { return crypto; }

            inline double getDevicePixelRatio() { return dpr; }

            inline StrongPointer<dom::Document> getDocument() { return document; }

            inline StrongPointer<js::Event> getEvent() { return event; }

            inline void setEvent(StrongPointer<js::Event> event) { this->event = std::move(event); }

            inline StrongPointer<dom::html::HTMLElement>
            getFrameElement() { return StrongPointer<dom::html::HTMLElement>(); }

            StrongPointer<const wlists::FrameList> getFrameList() const;

            StrongPointer<wlists::FrameList> getFrameList();

            inline bool getFullscreen() { return *fullscreen; }

            inline void setFullscreen(bool fullscreen) { this->fullscreen = fullscreen; }

            inline StrongPointer<const observable::SourceObservableItem<bool>> getFullscreenProperty() const {
                return StrongPointer<const observable::SourceObservableItem<bool>>(shared_from_this(), &fullscreen);
            }

            inline StrongPointer<observable::SourceObservableItem<bool>> getFullscreenProperty() {
                return StrongPointer<observable::SourceObservableItem<bool>>(shared_from_this(), &fullscreen);
            }

            inline StrongPointer<const History> getHistory() const {
                return StrongPointer<const History>(shared_from_this(), &history);
            }

            inline StrongPointer<History> getHistory() {
                return StrongPointer<History>(shared_from_this(), &history);
            }

            inline double getInnerHeight() const { return *height; }

            inline double getInnerWidth() const { return *width; }

            inline StrongPointer<const observable::SourceObservableItem<double>> getHeightProperty() const {
                return StrongPointer<const observable::SourceObservableItem<double>>(shared_from_this(), &height);
            }

            inline StrongPointer<observable::SourceObservableItem<double>> getHeightProperty() {
                return StrongPointer<observable::SourceObservableItem<double>>(shared_from_this(), &height);
            }

            inline StrongPointer<const observable::SourceObservableItem<double>> getWidthProperty() const {
                return StrongPointer<const observable::SourceObservableItem<double>>(shared_from_this(), &width);
            }

            inline StrongPointer<observable::SourceObservableItem<double>> getWidthProperty() {
                return StrongPointer<observable::SourceObservableItem<double>>(shared_from_this(), &width);
            }

            inline bool isSecureContext() const { return secureContext; }

            UInt getLength() const;

            inline bool isLocationBarVisible() const { return locationBarVisible; }

            StrongPointer<storage::Storage> getStorage() const;

            inline bool isMenuBarVisible() const { return menuBarVisible; }

            inline const DOMString &getName() const { return name; }

            inline void setName(DOMString name) { this->name = std::move(name); }

            inline StrongPointer<const Navigator> getNavigator() const {
                return StrongPointer<const Navigator>(shared_from_this(), &navigator);
            }

            inline StrongPointer<Navigator> getNavigator() {
                return StrongPointer<Navigator>(shared_from_this(), &navigator);
            }

            inline StrongPointer<Window> getOpener() const { return opener; }

            void navigateTo(DOMString href);

            void reload(bool force);

        private:
            StrongPointer<crypto::Crypto> crypto;
            double dpr = 1;
            StrongPointer<dom::Document> document;
            StrongPointer<js::Event> event;
            StrongPointer<wlists::FrameList> frameList;
            observable::SourceObservableItem<bool> fullscreen;
            History history{};
            observable::SourceObservableItem<double> height, width;
            Location location{};
            bool secureContext = false;
            bool locationBarVisible = true;
            storage::Storage sessionStorage{};
            StrongPointer<storage::Storage> localStorage;
            bool menuBarVisible = true;
            DOMString name{};
            Navigator navigator{};
            StrongPointer<Window> opener;
            observable::SourceObservableItem<dom::ObservableDOMRect> outerDimensions;
        };
    }
}
#endif //CURL_WINDOW_H
