//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_ENUM_MAP_H
#define FEATHER_ENUM_MAP_H

namespace feather {
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
