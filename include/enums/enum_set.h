//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_ENUM_SET_H
#define FEATHER_ENUM_SET_H
namespace feather {
    template<typename E, typename size_type>
    class EnumSet {
    public:
        inline void add(E e) { field |= static_cast<size_type>(1) << static_cast<size_type>(e); }

        inline void remove(E e) { field &= ~(static_cast<size_type>(1) << static_cast<size_type>(e)); }

        inline void addAll(EnumSet<E, size_type> other) { field |= other.field; }

        inline void removeAll(EnumSet<E, size_type> other) { field &= ~other.field; }

        inline bool contains(E e) const { return (field & 1 << static_cast<size_type>(e)) != 0; }

    private:
        size_type field;
    };
}

#endif //FEATHER_ENUM_SET_H
