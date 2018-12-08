//
// Created by prem on 12/7/2018.
//

#ifndef FEATHER_EVENT_TYPES_H
#define FEATHER_EVENT_TYPES_H

#include <functional>
#include <map>
#include <vector>
#include "typedefs.h"

namespace js {

    class Event;

    namespace event_types {
        enum class DOMEvent {
            EVENT, NULL_VALUE
        };

        const unsigned long DOM_EVENT_COUNT = static_cast<int>(DOMEvent::NULL_VALUE);
        const unsigned char DATA_CHUNK = sizeof(short);

        class DOMEventSet;

        class DOMEventHandlerMap;
    }
}

class js::event_types::DOMEventSet {
public:

    DOMEventSet() {}

    DOMEventSet(const DOMEventSet &other) { addAll(other); }

    inline bool present(DOMEvent event) const {
        return !!(*(data + static_cast<int>(event) / DATA_CHUNK) & (1u << static_cast<int>(event) % DATA_CHUNK));
    }

    inline void add(DOMEvent event) {
        *(data + static_cast<int>(event) / DATA_CHUNK) |= (1u << static_cast<int>(event) % DATA_CHUNK);
    }

    inline void remove(DOMEvent event) {
        *(data + static_cast<int>(event) / DATA_CHUNK) &= ~(1u << static_cast<int>(event) % DATA_CHUNK);
    }

    void addAll(const DOMEventSet &other);

    void removeAll(const DOMEventSet &other);

    inline DOMEventSet &clone() { return *new DOMEventSet(*this); }

    //DOMEventMap hook

    unsigned short getRaw(unsigned char c) const { return data[c]; }

private:
    unsigned short data[DOM_EVENT_COUNT / DATA_CHUNK + 1];
};

class js::event_types::DOMEventHandlerMap {
public:

    inline void addEventHandler(DOMEvent event, std::function<void(Event &)> *handler) {
        getEventHandlers(event).push_back(handler);
    }

    void removeEventHandler(DOMEvent event, std::function<void(Event &)> *handler);

    inline void setEndEventHandler(DOMEvent event, std::function<void(Event &)> handler) {
        getEndEventHandlers()[static_cast<int>(event)] = handler;
    }

    inline std::function<void(Event &)> *getEndEventHandler(DOMEvent event) {
        return endEventHandlers ? &endEventHandlers[static_cast<int>(event)] : nullptr;
    }

    inline void registerEventHandler(DOMString eventType, std::function<void(Event &)> *handler) {
        getArbitraryHandlerVector(eventType).push_back(handler);
    }

    void unregisterEventHandler(DOMString eventType, std::function<void(Event &)> *handler);

    void fireAll(Event &event);

    ~DOMEventHandlerMap();

private:
    mutable std::vector<std::function<void(js::Event &)> *> **eventHandlers;
    mutable std::function<void(js::Event &)> *endEventHandlers;
    mutable std::map<DOMString, std::vector<std::function<void(js::Event &)> *> *> *arbitraryEventHandlers;

    inline std::vector<std::function<void(Event &)> *> **getEventHandlerArray() const {
        return eventHandlers ? eventHandlers
                             : (eventHandlers = new std::vector<std::function<void(Event &)> *> *[DOM_EVENT_COUNT]);
    }

    std::vector<std::function<void(Event &)> *> &getEventHandlers(DOMEvent eventType) const;

    inline std::function<void(Event &)> *getEndEventHandlers() const {
        return endEventHandlers ? endEventHandlers
                                : (endEventHandlers = new std::function<void(Event &)>[DOM_EVENT_COUNT]);
    }

    inline std::map<DOMString, std::vector<std::function<void(Event &)> *> *> &getArbitraryEventHandlers() const {
        return arbitraryEventHandlers
               ? *arbitraryEventHandlers
               : *(arbitraryEventHandlers = new std::map<DOMString, std::vector<std::function<void(Event &)> *> *>());
    }

    std::vector<std::function<void(Event &)> *> &getArbitraryHandlerVector(DOMString type);
};

#endif //FEATHER_EVENT_TYPES_H
