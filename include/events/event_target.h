//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_EVENT_TARGET_H
#define FEATHER_EVENT_TARGET_H

#include "observable/invalidatable.h"
#include "event.h"

namespace feather {
    namespace js {

        class EventTarget : public observable::Invalidatable {
        public:
            EventTarget() = default;

            void registerHandler(DOMString s, StrongPointer<Function<void(Event &)>>);

            void unregisterHandler(DOMString, StrongPointer<Function<void(Event &)>>);

            void registerEndHandler(DOMString, StrongPointer<Function<void(Event &)>>);

            void unregisterEndHandler(DOMString, StrongPointer<Function<void(Event &)>>);

            void dispatchEvent(Event &);

            ~EventTarget() override;

        private:
            mutable Multimap<EventType, StrongPointer<Function<void(Event &)>>> *handlers = nullptr;
            mutable Map<EventType, StrongPointer<Function<void(Event &)>>> *endHandlers = nullptr;
            mutable Multimap<DOMString, StrongPointer<Function<void(Event &)>>> *customMap = nullptr;

            inline Multimap<EventType, StrongPointer<Function<void(Event &)>>> &getHandlers() const {
                return handlers ? *handlers
                                : *(handlers = new Multimap<EventType, StrongPointer<Function<void(Event &)>>>());
            }

            inline Multimap<DOMString, StrongPointer<Function<void(Event &)>>> &getCustomMap() const {
                return customMap ? *customMap
                                 : *(customMap = new Multimap<DOMString, StrongPointer<Function<void(Event &)>>>());
            }

            inline Map<EventType, StrongPointer<Function<void(Event &)>>> &getEndHandlers() const {
                return endHandlers ? *endHandlers
                                   : *(endHandlers = new Map<EventType, StrongPointer<Function<void(Event &)>>>());
            }
        };
    }
}
#endif //FEATHER_EVENT_TARGET_H
