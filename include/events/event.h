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
            DEFAULT, CUSTOM, __COUNT__
        };

        DOMString toString(EventType et);

        EventType fromString(DOMString s);

        enum class EventProperties {
            BUBBLES, CANCELABLE, COMPOSED, DEFAULT, TRUSTED, CONSUMED, IMMEDIATE
        };

        enum class EventPhase {
            CAPTURING, AT_TARGET, BUBBLING
        };

        class Event {
        public:

            Event(bool bubbles, bool cancelable, bool composed, bool trusted,
                  EventTarget &originalTarget, List<StrongPointer<EventTarget>> realPath,
                  EventType type);

            Event(bool bubbles, bool cancelable, bool composed, DOMString name,
                  EventTarget &originalTarget, List<StrongPointer<EventTarget>> realPath,
                  EventType type);

            inline bool getBubbles() const { return properties.contains(EventProperties::BUBBLES); }

            inline bool getCancelBubble() const { return properties.contains(EventProperties::CONSUMED); }

            inline void setCancelBubble(bool b) { if (b) stopPropagation(); }

            List<StrongPointer<EventTarget>> getDeepPath() const { return getComposedPath(); }

            inline bool getDefaultPrevented() const { return properties.contains(EventProperties::DEFAULT); }

            inline bool getCancelable() const { return properties.contains(EventProperties::CANCELABLE); }

            inline bool getComposed() const { return properties.contains(EventProperties::COMPOSED); }

            EventTarget &getEventTarget();

            inline Long getTimestamp() {
                return std::chrono::duration_cast<Millis>(timestamp.time_since_epoch()).count();
            }

            inline EventPhase getEventPhase() { return phase; }

            inline void setEventPhase(EventPhase phase) { this->phase = phase; }

            inline bool getReturnValue() const { return properties.contains(EventProperties::DEFAULT); }

            void setReturnValue(bool);

            inline EventTarget &getOriginalTarget() { return originalTarget; }

            inline DOMString getType() const { return name; }

            bool isTrusted() const { return properties.contains(EventProperties::TRUSTED); }

            List<StrongPointer<EventTarget>> getComposedPath() const;

            inline void preventDefault() { properties.remove(EventProperties::DEFAULT); }

            inline void stopPropagation() { properties.add(EventProperties::CONSUMED); }

            inline void stopImmediatePropagation() {
                properties.add(EventProperties::IMMEDIATE);
                properties.add(EventProperties::CONSUMED);
            }

            inline bool consumed() { return properties.contains(EventProperties::CONSUMED); }

            inline bool immediate() { return properties.contains(EventProperties::IMMEDIATE); }

            inline EventType getInternalType() { return type; }

            inline List<StrongPointer<EventTarget>>::iterator &getNextTarget() { return current; }

            inline bool atEnd() { return current == realPath.end(); }

            inline bool atStart() { return current == realPath.begin(); }

            inline StrongPointer<EventTarget> shiftTarget() {
                return static_cast<int>(phase) ? *current-- : *current++;
            }

        private:
            List<StrongPointer<EventTarget>> realPath;
            List<StrongPointer<EventTarget>>::iterator current;
            EventTarget &originalTarget;
            const TimePoint timestamp = currentTime();
            DOMString name;
            EventPhase phase = EventPhase::CAPTURING;
            EnumSet<EventProperties, UByte> properties;
            EventType type;
        };
    }
}
#endif //FEATHER_EVENT_H
