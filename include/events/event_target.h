//
// Created by prem on 11/9/2018.
//

#ifndef FEATHER_EVENT_TARGET_H
#define FEATHER_EVENT_TARGET_H

#include <algorithm>
#include <map>
#include "event.h"

class EventTarget {
public:

    EventTarget() {}

    inline void addEventListener(DOMString type, void (*listener)(Event &)) {
        (handlers.find(type) == handlers.end() ? handlers[type] = new std::vector<void (*)(Event &)>()
                                               : handlers[type])->push_back(listener);
    }

    inline void removeEventListener(DOMString type, void (*listener)(Event &)) {
        if (handlers.find(type) != handlers.end()) {
            std::vector<void (*)(Event &)> *handlerList = handlers[type];
            handlerList->erase(std::remove(handlerList->begin(), handlerList->end(), listener), handlerList->end());
        }
    }

    void dispatchEvent(Event &event) const;

    virtual ~EventTarget() {
        for (const auto& vec : handlers) {
            delete vec.second;
        }
    }

private:
    std::map<DOMString, std::vector<void (*)(Event &)> *> handlers;
};

#endif //FEATHER_EVENT_TARGET_H
