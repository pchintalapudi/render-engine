//
// Created by prem on 11/9/2018.
//

#ifndef FEATHER_EVENT_TARGET_H
#define FEATHER_EVENT_TARGET_H

#include <algorithm>
#include <map>
#include <functional>
#include "event_types.h"
#include "event.h"

namespace js {
    template<typename T>
    class EventTarget;
}
template<typename T>
class js::EventTarget {
public:

    EventTarget() {}

    void addEventHandler(DOMString type, std::function<void(Event &)> *handler);

    void removeEventHandler(DOMString type, std::function<void(Event &)> *handler);

    void setEndEventHandler(DOMString type, std::function<void(Event &)> *handler);

    void removeEndEventHandler(DOMString type);

    void dispatchEvent(Event &event) const;

//    ~EventTarget() { delete handlerMap; }

private:
//    mutable event_types::DOMEventHandlerMap *handlerMap = nullptr;
//
//    inline event_types::DOMEventHandlerMap &getHandlerMap() const {
//        return handlerMap ? *handlerMap : *(handlerMap = new event_types::DOMEventHandlerMap());
//    }
};

#endif //FEATHER_EVENT_TARGET_H
