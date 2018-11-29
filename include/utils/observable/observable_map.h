//
// Created by prem on 11/28/2018.
//

#ifndef FEATHER_OBSERVABLE_MAP_H
#define FEATHER_OBSERVABLE_MAP_H

#include <map>
#include "invalidatable.h"

namespace observable {
    template<class K, class V>
    class ObservableMap;
}

template<class K, class V>
class observable::ObservableMap : public Invalidatable {
public:
    V get(K key) { return getBacking()[key]; }

    unsigned long size() { return backing->size(); }

protected:
    virtual std::map<K, V> *compute() = 0;

    std::map<K, V> &getBacking() {
        if (isValid()) return backing;
        backing = compute();
        validate();
        return *backing;
    }

private:
    std::map<K, V> *backing;
    std::map<K, void *> hardcoded;
};

#endif //FEATHER_OBSERVABLE_MAP_H
