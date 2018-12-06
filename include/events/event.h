//
// Created by prem on 11/9/2018.
//

#ifndef FEATHER_EVENT_H
#define FEATHER_EVENT_H
namespace js {
    class EventTarget;
    class Event;
}

#include <vector>
#include "../typedefs.h"
#include "event_phase.h"

class js::Event {
public:

    Event(bool bubbles, bool cancelable, bool composed, EventTarget &target, std::vector<DOMString> type, bool trusted);

    inline bool getBubbles() { return bubbles; }

    inline bool getCancelBubble() { return consumed; }

    inline void setCancelBubble(bool cancel) { consumed = cancel; }

    inline bool getCancelable() { return cancelable; }

    inline bool getComposed() { return composed; }

    inline EventTarget &getCurrentTarget() { return *deepPath.back(); }

    inline std::vector<EventTarget *> &getDeepPath() { return deepPath; }

    inline bool getDefaultPrevented() { return defaultPrevented; }

    inline EventPhase getEventPhase() { return eventPhase; }

    inline void setEventPhase(EventPhase phase) { eventPhase = phase; }

    inline bool getReturnValue() { return defaultPrevented; }

    inline void setReturnValue(bool returnValue) { defaultPrevented = returnValue; }

    inline EventTarget &getSrcElement() { return originalTarget; }

    inline EventTarget &getTarget() { return originalTarget; }

    inline unsigned long getTimeStamp() { return timeStamp; }

    inline DOMString getType() { return types.front(); }

    inline std::vector<DOMString> &getTypeList() { return types; }

    inline bool isTrusted() { return trusted; }

    inline std::vector<EventTarget *> getComposedPath() { return deepPath; }

    inline void preventDefault() { defaultPrevented = true; }

    inline void stopPropagation() { consumed = true; }

    inline void stopImmediatePropagation() {
        consumed = true;
        violentlyConsumed = true;
    }

    inline bool isConsumed() {
        return consumed;
    }

    inline bool isViolentlyConsumed() {
        return violentlyConsumed;
    }

    virtual ~Event() {
    }

private:
    bool bubbles;
    bool cancelable;
    bool composed;
    std::vector<EventTarget *> deepPath;
    bool defaultPrevented;
    EventPhase eventPhase;
    EventTarget &originalTarget;
    unsigned long timeStamp;
    std::vector<DOMString> types;
    bool trusted;
    bool consumed;
    bool violentlyConsumed;
};

#endif //FEATHER_EVENT_H
