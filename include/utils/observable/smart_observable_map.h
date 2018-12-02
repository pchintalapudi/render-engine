//
// Created by prem on 11/28/2018.
//

#ifndef FEATHER_SMART_OBSERVABLE_MAP_H
#define FEATHER_SMART_OBSERVABLE_MAP_H

#include "observable_map.h"
#include "event/event_type.h"

namespace observable {
    template<typename K, typename V>
    class SmartObservableMap;
}
template<typename K, typename V>
class observable::SmartObservableMap : public ObservableMap<K, V> {
public:
    void set(K key, V value) {
        if (backing.find(key) != backing.end()) backing[key]->removeInvalidator(this);
        backing[key] = value;
        value->addInvalidator(this, [this]() { this->invalidate(); });
        this->invalidate(observable::generate(observable::EventType::MAP_CHANGE));
    }

protected:
    std::map<K, V> *compute() { return &backing; }

private:
    std::map<K, V> backing;
};

#endif //FEATHER_SMART_OBSERVABLE_MAP_H
