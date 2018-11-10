//
// Created by prem on 11/9/2018.
//
#include <chrono>
#include <vector>
#include "include/events/event.h"
#include "include/events/event_target.h"

Event::Event(bool bubbles, bool cancelable, bool composed, EventTarget &target,
             std::vector<DOMString> types, bool trusted)
        : bubbles(bubbles), cancelable(cancelable), composed(composed),
          defaultPrevented(false), originalTarget(target), eventPhase(EventPhase::CAPTURING_PHASE),
          timeStamp(std::chrono::system_clock::now().time_since_epoch() /
                    std::chrono::milliseconds(1)), types(types), trusted(trusted),
          consumed(false), violentlyConsumed(false) {
}