//
// Created by prem on 11/9/2018.
//

#ifndef FEATHER_EVENT_H
#define FEATHER_EVENT_H
namespace js {
    class EventTarget;

    class Event;
}

#include <chrono>
#include "../typedefs.h"
#include "event_phase.h"
#include "event_types.h"

namespace js {
    namespace event_properties {
        enum EventProperties {
            BUBBLES, CANCELABLE, COMPOSED, DEFAULT_PREVENTED, TRUSTED, CONSUMED, VIOLENT
        };
    }
}

class js::Event {
public:

    Event(bool bubbles, bool cancelable, bool composed, bool trusted, EventTarget &target,
          const event_types::DOMEventSet &typeset)
            : Event(bubbles, cancelable, composed, trusted, target, typeset, "") {}

    Event(bool bubbles, bool cancelable, bool composed, bool trusted,
          EventTarget &target, const event_types::DOMEventSet &typeset, const DOMString customName)
            : eventPhase(EventPhase::CAPTURING_PHASE), originalTarget(target), typeset(typeset), customName(customName),
              timeStamp(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1)),
              bitField((bubbles & 1u) << event_properties::BUBBLES
                       | (cancelable & 1u) << event_properties::CANCELABLE
                       | (composed & 1u) << event_properties::COMPOSED
                       | (trusted & 1u) << event_properties::TRUSTED) {}

    inline bool getBubbles() const { return bitField & 1u << event_properties::BUBBLES; }

    inline bool getCancelBubble() const { return bitField & 1u << event_properties::CONSUMED; }

    inline void setCancelBubble(bool cancel) {
        if (cancel) bitField |= 1u << event_properties::CONSUMED;
        else bitField &= ~(1u << event_properties::CONSUMED);
    }

    inline bool getCancelable() const { return bitField & 1u << event_properties::CANCELABLE; }

    inline bool getComposed() const { return bitField & 1u << event_properties::COMPOSED; }

    inline EventTarget &getCurrentTarget() const { return *deepPath.back(); }

    inline const std::vector<EventTarget *> &getDeepPath() const { return deepPath; }

    inline bool getDefaultPrevented() const { return bitField & 1u << event_properties::DEFAULT_PREVENTED; }

    inline EventPhase getEventPhase() const { return eventPhase; }

    inline void setEventPhase(EventPhase phase) { eventPhase = phase; }

    inline bool getReturnValue() const { return getDefaultPrevented(); }

    inline void setReturnValue(bool returnValue) {
        if (returnValue) preventDefault();
        else bitField &= ~(1u << event_properties::DEFAULT_PREVENTED);
    }

    inline EventTarget &getSrcElement() const { return originalTarget; }

    inline EventTarget &getTarget() const { return originalTarget; }

    inline unsigned long getTimeStamp() const { return timeStamp; }

    inline const event_types::DOMEventSet &getTypes() const { return typeset; }

    inline bool isTrusted() const { return bitField & 1u << event_properties::TRUSTED; }

    inline const std::vector<EventTarget *> getComposedPath() const { return deepPath; }

    inline void preventDefault() { bitField |= 1u << event_properties::DEFAULT_PREVENTED; }

    inline void stopPropagation() { bitField |= 1u << event_properties::CONSUMED; }

    inline void stopImmediatePropagation() {
        bitField |= 1u << event_properties::CONSUMED
                    | 1u << event_properties::VIOLENT;
    }

    inline bool isConsumed() const { return bitField & 1u << event_properties::CONSUMED; }

    inline bool isViolentlyConsumed() const { return bitField & 1u << event_properties::VIOLENT; }

    inline DOMString getCustomName() const { return customName; }

    virtual ~Event() {}

private:
    const std::vector<EventTarget *> deepPath;
    EventPhase eventPhase;
    EventTarget &originalTarget;
    const event_types::DOMEventSet typeset;
    DOMString customName;
    const unsigned long timeStamp;
    unsigned char bitField : 7;
};

#endif //FEATHER_EVENT_H
