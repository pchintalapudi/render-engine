//
// Created by prem on 12/10/2018.
//

#include "events/event_target.h"

using namespace feather::js;

void EventTarget::registerHandler(feather::DOMString s,
                                  feather::StrongPointer<std::function<void(feather::js::Event &)>> p) {
    auto type = fromString(s);
    if (type == EventType::CUSTOM) {
        getCustomMap().emplace(s, p);
    } else {
        getHandlers().emplace(type, p);
    }
}

void EventTarget::registerEndHandler(feather::DOMString s,
                                     feather::StrongPointer<std::function<void(feather::js::Event &)>> p) {
    //There are no custom on-event attributes; we don't have to deal with that edge case :)
    getEndHandlers().emplace(fromString(s), p);
}

void EventTarget::unregisterEndHandler(feather::DOMString s,
                                       feather::StrongPointer<std::function<void(feather::js::Event &)>> p) {
    if (endHandlers) {
        auto type = fromString(s);
        if ((*endHandlers)[type] == p) endHandlers->erase(type);
    }
}

void EventTarget::unregisterHandler(feather::DOMString s,
                                    feather::StrongPointer<std::function<void(feather::js::Event &)>> p) {
    if (handlers) {
        auto type = fromString(s);
        if (type == EventType::CUSTOM) {
            if (customMap) {
                auto iterPair = customMap->equal_range(s);
                for (; iterPair.first != iterPair.second; iterPair.first++) {
                    if (iterPair.first->second == p) {
                        customMap->erase(iterPair.first);
                        break;
                    }
                }
            }
        } else {
            auto iterPair = handlers->equal_range(type);
            for (; iterPair.first != iterPair.second; iterPair.first++) {
                if (iterPair.first->second == p) {
                    handlers->erase(iterPair.first);
                    break;
                }
            }
        }
    }
}

bool EventTarget::dispatchEvent(feather::js::Event &e) {
    auto type = e.getInternalType();
    //Event prep phase
    bool atEnd = e.getEventPhase() == EventPhase::CAPTURING && e.atEnd();
    if (atEnd) e.setEventPhase(EventPhase::AT_TARGET);
    //Event handling phase
    if (type == EventType::CUSTOM && customMap) {
        auto iterPair = customMap->equal_range(e.getType());
        for (; iterPair.first != iterPair.second; iterPair.first++) if (!e.immediate())(*iterPair.first->second)(e);
    } else if (handlers) {
        auto iterPair = handlers->equal_range(e.getInternalType());
        for (; iterPair.first != iterPair.second; iterPair.first++) if (!e.immediate())(*iterPair.first->second)(e);
    }
    //Event dispatching phase
    if (!e.consumed()) {
        switch (e.getEventPhase()) {
            case EventPhase::AT_TARGET:
                e.setEventPhase(EventPhase::BUBBLING);
                [[fallthrough]];
            case EventPhase::BUBBLING:
                if (!e.atStart()) return e.shiftTarget()->dispatchEvent(e);
                else return true;
            case EventPhase::CAPTURING:
                return e.shiftTarget()->dispatchEvent(e);
            default:
                return true;
        }
    } else return false;
}

EventTarget::~EventTarget() {
    delete handlers;
    delete endHandlers;
    delete customMap;
}