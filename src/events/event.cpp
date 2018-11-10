//
// Created by prem on 11/9/2018.
//
#include <chrono>
#include <vector>
#include "include/events/event.h"
#include "include/events/event_target.h"

Event::Event(bool bubbles, bool cancelable, bool composed, EventPhase eventPhase,
             EventTarget &target, DOMString type, bool trusted)
        : bubbles(bubbles), cancelable(cancelable), composed(composed),
          defaultPrevented(false),
          eventPhase(eventPhase), originalTarget(target),
          timeStamp(std::chrono::system_clock::now().time_since_epoch() /
                    std::chrono::milliseconds(1)), type(type), trusted(trusted),
          consumed(false), violentlyConsumed(false) {
}