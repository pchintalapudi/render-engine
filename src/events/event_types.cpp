//
// Created by prem on 12/7/2018.
//

#include "include/events/event_types.h"
#include "include/events/event.h"

void js::event_types::DOMEventSet::addAll(const js::event_types::DOMEventSet &other) {
    for (unsigned long i = 0; i < DOM_EVENT_COUNT / DATA_CHUNK + 1; i++) data[i] |= other.data[i];
}

void js::event_types::DOMEventSet::removeAll(const js::event_types::DOMEventSet &other) {
    for (unsigned long i = 0; i < DOM_EVENT_COUNT / DATA_CHUNK + 1; i++) data[i] &= ~other.data[i];
}

void js::event_types::DOMEventHandlerMap::removeEventHandler(js::event_types::DOMEvent event,
                                                             std::function<void(Event &)> *handler) {
    if (eventHandlers) {
        auto vec = eventHandlers[static_cast<int>(event) / DATA_CHUNK];
        if (vec) vec->erase(std::remove(vec->begin(), vec->end(), handler));
    }
}

std::vector<std::function<void(js::Event &)> *> &js::event_types::DOMEventHandlerMap::getEventHandlers(
        js::event_types::DOMEvent eventType) const {
    std::vector<std::function<void(js::Event &)> *> **pointer
            = getEventHandlerArray() + static_cast<int>(eventType);
    if (!*pointer) *pointer = new std::vector<std::function<void(js::Event &)> *>();
    return **pointer;
}

js::event_types::DOMEventHandlerMap::~DOMEventHandlerMap() {
    if (eventHandlers) {
        for (unsigned long i = 0; i < DOM_EVENT_COUNT; i++)
            delete eventHandlers[i];
        delete[] eventHandlers;
    }
    delete[] endEventHandlers;
    delete arbitraryEventHandlers;
}

std::vector<std::function<void(js::Event &)> *> &
js::event_types::DOMEventHandlerMap::getArbitraryHandlerVector(DOMString type) {
    auto map = getArbitraryEventHandlers();
    return map[type] ? *map[type] : *(map[type] = new std::vector<std::function<void(js::Event &)> *>());
}

void js::event_types::DOMEventHandlerMap::unregisterEventHandler(DOMString eventType,
                                                                 std::function<void(js::Event &)> *handler) {
    if (arbitraryEventHandlers) {
        std::vector<std::function<void(js::Event &)> *> *vec = (*arbitraryEventHandlers)[eventType];
        vec->erase(std::remove(vec->begin(), vec->end(), handler));
    }
}

void js::event_types::DOMEventHandlerMap::fireAll(js::Event &event) {
    auto types = event.getTypes();
    for (unsigned char i = 0; i < DOM_EVENT_COUNT / DATA_CHUNK + 1; i++) {
        unsigned short raw = types.getRaw(i);
        if (raw) {
            for (unsigned char j = 0; j < DATA_CHUNK; j++) {
                if (raw & 1u << j && eventHandlers[i]) {
                    for (auto handler : *eventHandlers[i]) {
                        (*handler)(event);
                        if (event.isViolentlyConsumed()) return;
                    }
                    if (endEventHandlers) endEventHandlers[i * DATA_CHUNK + j](event);
                }
            }
        }
    }
    if (event.isViolentlyConsumed()) return;
    if (arbitraryEventHandlers && !event.getCustomName().empty()) {
        auto vec = (*arbitraryEventHandlers)[event.getCustomName()];
        if (vec) {
            for (auto handler : *vec) {
                (*handler)(event);
                if (event.isViolentlyConsumed()) return;
            }
        }
    }
}