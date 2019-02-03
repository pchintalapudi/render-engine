//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_EVENT_H
#define FEATHER_EVENT_H

#include "typedefs.h"

namespace feather {
    namespace js {
        class EventTarget;

        enum class EventType {
            FULL_SCREEN_CHANGE, FULL_SCREEN_ERROR, DEFAULT, CUSTOM, __COUNT__
        };

        DOMString toString(EventType et);

        EventType fromString(DOMString s);

        enum class EventProperties {
            BUBBLES, CANCELABLE, COMPOSED, DEFAULT, TRUSTED, CONSUMED, IMMEDIATE, __COUNT__
        };

        enum class EventPhase {
            CAPTURING, AT_TARGET, BUBBLING
        };

        class Event {
        public:

            Event(bool bubbles, bool cancelable, bool composed, bool trusted,
                  WeakPointer<EventTarget> &&originalTarget, Vector<StrongPointer<EventTarget>> realPath,
                  EventType type);

            Event(bool bubbles, bool cancelable, bool composed, DOMString &&name,
                  WeakPointer<EventTarget> &&originalTarget, Vector<StrongPointer<EventTarget>> realPath,
                  EventType type);

            inline bool getBubbles() const { return properties[EventProperties::BUBBLES]; }

            inline bool getCancelBubble() const { return properties[EventProperties::CONSUMED]; }

            inline void setCancelBubble(bool b) { if (b) stopPropagation(); }

            Vector<StrongPointer<EventTarget>> getDeepPath() const { return getComposedPath(); }

            inline bool getDefaultPrevented() const { return properties[EventProperties::DEFAULT]; }

            inline bool getCancelable() const { return properties[EventProperties::CANCELABLE]; }

            inline bool getComposed() const { return properties[EventProperties::COMPOSED]; }

            StrongPointer<EventTarget> getEventTarget();

            inline Long getTimestamp() {
                return std::chrono::duration_cast<Millis>(timestamp.time_since_epoch()).count();
            }

            inline EventPhase getEventPhase() { return phase; }

            inline void setEventPhase(EventPhase phase) { this->phase = phase; }

            inline bool getReturnValue() const { return properties[EventProperties::DEFAULT]; }

            void setReturnValue(bool);

            inline StrongPointer<EventTarget> getOriginalTarget() {
                return originalTarget.expired() ? nullptr : originalTarget.lock();
            }

            inline DOMString getType() const { return name; }

            bool isTrusted() const { return properties[EventProperties::TRUSTED]; }

            Vector<StrongPointer<EventTarget>> getComposedPath() const;

            inline void preventDefault() { properties -= EventProperties::DEFAULT; }

            inline void stopPropagation() { properties += EventProperties::CONSUMED; }

            inline void stopImmediatePropagation() {
                properties += EventProperties::IMMEDIATE;
                properties += EventProperties::CONSUMED;
            }

            inline bool consumed() { return properties[EventProperties::CONSUMED]; }

            inline bool immediate() { return properties[EventProperties::IMMEDIATE]; }

            inline EventType getInternalType() { return type; }

            inline Vector<StrongPointer<EventTarget>>::iterator &getNextTarget() { return current; }

            inline bool atEnd() { return current == realPath.end(); }

            inline bool atStart() { return current == realPath.begin(); }

            inline StrongPointer<EventTarget> shiftTarget() {
                return static_cast<int>(phase) ? *current-- : *current++;
            }

        private:
            Vector<StrongPointer<EventTarget>> realPath;
            Vector<StrongPointer<EventTarget>>::iterator current;
            WeakPointer<EventTarget> originalTarget;
            const TimePoint timestamp = currentTime();
            DOMString name;
            EventPhase phase = EventPhase::CAPTURING;
            EnumSet<EventProperties> properties;
            EventType type;
        };
    }
}
#endif //FEATHER_EVENT_H
