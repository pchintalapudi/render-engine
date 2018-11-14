//
// Created by prem on 11/9/2018.
//

#ifndef FEATHER_EVENT_TARGET_H
#define FEATHER_EVENT_TARGET_H

#include <algorithm>
#include <map>
#include <functional>
#include "event.h"

class EventTarget {
public:

    EventTarget() {}

    inline void addEventListener(DOMString type, std::function<void(Event &)> listener) {
        (handlers.find(type) == handlers.end() ? handlers[type] = new std::vector<std::function<void(Event &)>>()
                                               : handlers[type])->push_back(listener);
    }

    template<typename Listener>
    inline void removeEventListener(DOMString type, Listener listener) {
        if (handlers.find(type) != handlers.end()) {
            auto handlerList = handlers[type];
            handlerList->erase(std::remove(handlerList->begin(), handlerList->end(), listener), handlerList->end());
        }
    }

    void dispatchEvent(Event &event) const;

    virtual ~EventTarget() {
        for (const auto &vec : handlers) {
            delete vec.second;
        }
    }

private:
    std::map<DOMString, std::vector<std::function<void(Event &)>> *> handlers;
};

#endif //FEATHER_EVENT_TARGET_H
