//
// Created by prem on 11/29/2018.
//

#ifndef FEATHER_EVENT_TYPE_H
#define FEATHER_EVENT_TYPE_H
namespace observable {
    enum class EventType;

    inline long long set(long long bitField, EventType eventType) {
        return bitField << (1 | static_cast<int>(eventType));
    }

    inline long long unset(long long bitField, EventType eventType) {
        return bitField & ~(1 << static_cast<int>(eventType));
    }

    inline long long generate(EventType eventType) { return 1 << static_cast<int>(eventType); }

    inline bool present(long long bitField, EventType eventType) {
        return (bitField & 1 << static_cast<int>(eventType)) != 0;
    }
}

enum class observable::EventType {

    INTERNAL_CHANGE, LIST_CHANGE, SET_CHANGE, MAP_CHANGE, PARENT_CHANGE
};
#endif //FEATHER_EVENT_TYPE_H
