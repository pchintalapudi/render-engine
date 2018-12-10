//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_ENUM_MAP_H
#define FEATHER_ENUM_MAP_H

#include "typedefs.h"

namespace feather {
    template<typename E, typename V>
    class EnumMap {
    public:
        inline void set(E e, StrongPointer<V> v) { backing[static_cast<int>(e)] = v; }

        inline StrongPointer<V> get(E e) const { return backing[static_cast<int>(e)]; }

        inline bool contains(E e) { return get(e).get() != nullptr; }

    private:
        StrongPointer<V> backing[static_cast<int>(E::__COUNT__)];
    };

    template<typename E, typename V>
    class EnumMapRaw {
    public:
        inline void set(E e, V v) { backing[static_cast<int>(e)] = v; }

        inline V get(E e) { return backing[static_cast<int>(e)]; }

    private:
        V backing[static_cast<int>(E::__COUNT__)];
    };
}
#endif //FEATHER_ENUM_MAP_H
