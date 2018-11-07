//
// Created by prem on 11/7/2018.
//

#ifndef FEATHER_EVENT_H
#define FEATHER_EVENT_H

#include <string>
#include <vector>

class IEventTarget;

class Event {
public:
    Event(IEventTarget &target, std::vector<std::string> &types, bool bubble)
            : target(target), types(types), bubble(bubble), consumed(false) {}

    const std::vector<std::string> &getTypes() {
        return this->types;
    }

    const IEventTarget &getTarget() {
        return this->target;
    }

    const bool bubbles() {
        return this->bubble;
    }

    const bool isConsumed() {
        return this->consumed;
    }

    void consume() {
        this->consumed = true;
    }

    virtual ~Event() = default;

private:
    const IEventTarget &target;
    const std::vector<std::string> &types;
    const bool bubble;
    bool consumed;
};

#endif //FEATHER_EVENT_H
