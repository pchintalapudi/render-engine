//
// Created by prem on 11/9/2018.
//

#include "include/events/event_target.h"
#include "include/events/event_phase.h"

void js::EventTarget::dispatchEvent(Event &event) const {
    auto idx = std::find(event.getDeepPath().begin(), event.getDeepPath().end(), this);
    bool atEnd = idx - event.getDeepPath().begin() + 1 == event.getDeepPath().size();
    if (atEnd) {
        event.setEventPhase(EventPhase::AT_TARGET);
    }
    for (const auto &type : event.getTypeList()) {
        if (handlers.find(type) != handlers.end()) {
            for (auto &handler : *handlers.at(type)) {
                handler(event);
                if (event.isViolentlyConsumed()) {
                    return;
                }
            }
        }
    }
    if (atEnd && event.getBubbles()) {
        event.setEventPhase(EventPhase::BUBBLING_PHASE);
    }
    if (idx != event.getDeepPath().begin() && event.getBubbles() && !event.isConsumed() &&
        event.getEventPhase() == EventPhase::BUBBLING_PHASE && idx > event.getDeepPath().begin()) {
        event.getDeepPath()[idx - event.getDeepPath().begin() - 1]->dispatchEvent(event);
    } else if (idx != event.getDeepPath().end() - 1 && event.getBubbles() && !event.isConsumed()
               && event.getEventPhase() == EventPhase::CAPTURING_PHASE && idx < event.getDeepPath().end() - 1) {
        event.getDeepPath()[idx - event.getDeepPath().begin() + 1]->dispatchEvent(event);
    }
}