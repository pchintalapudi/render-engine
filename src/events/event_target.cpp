//
// Created by prem on 11/9/2018.
//

#include "include/events/event_target.h"

void js::EventTarget::dispatchEvent(Event &event) const {
    unsigned long idx = -1;
    while (event.getDeepPath()[++idx] != this);
    bool atEnd = idx + 1 == event.getDeepPath().size();
    if (atEnd) event.setEventPhase(EventPhase::AT_TARGET);
    for (const auto &type : event.getTypeList()) {
        if (handlers.find(type) != handlers.end()) {
            for (auto &handler : *handlers.at(type)) {
                handler(event);
                if (event.isViolentlyConsumed()) return;
            }
        }
    }
    if (atEnd && event.getBubbles()) event.setEventPhase(EventPhase::BUBBLING_PHASE);
    if (event.getBubbles() && !event.isConsumed() && event.getEventPhase() == EventPhase::BUBBLING_PHASE && idx > 0)
        event.getDeepPath()[idx - 1]->dispatchEvent(event);
    else if (idx < event.getDeepPath().size() - 1 && !event.isConsumed() &&
             event.getEventPhase() == EventPhase::CAPTURING_PHASE)
        event.getDeepPath()[idx + 1]->dispatchEvent(event);
}