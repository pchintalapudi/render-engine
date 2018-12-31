//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_ENUM_SET_H
#define FEATHER_ENUM_SET_H
namespace feather {
    template<typename E, typename size_type>
    class EnumSet {
    public:

        EnumSet() = default;

        EnumSet(const EnumSet &other) : EnumSet(other.field) {}

        explicit EnumSet(size_type field) : field(field) {}

        inline EnumSet<E, size_type> &add(E e) {
            field |= static_cast<size_type>(1) << static_cast<size_type>(e);
            return *this;
        }

        inline EnumSet<E, size_type> &remove(E e) {
            field &= ~(static_cast<size_type>(1) << static_cast<size_type>(e));
            return *this;
        }

        inline EnumSet<E, size_type> &addAll(const EnumSet<E, size_type> other) {
            field |= other.field;
            return *this;
        }

        inline EnumSet<E, size_type> removeAll(const EnumSet<E, size_type> other) {
            field &= ~other.field;
            return *this;
        }

        inline bool contains(E e) const { return (field & 1 << static_cast<size_type>(e)) != 0; }

        inline bool containsAll(const EnumSet<E, size_type> other) const {
            return (field & other.field) == other.field;
        }

        inline bool containsAny(const EnumSet<E, size_type> other) const { return (field & other.field) != 0; }

        inline bool containsNone(const EnumSet<E, size_type> other) const { return (field & other.field) == 0; }

        bool operator==(const EnumSet<E, size_type> &other) const { return field == other.field; }

        inline EnumSet<E, size_type> invert() {
            field = ~field;
            return *this;
        }

        inline EnumSet<E, size_type> clone() const { return EnumSet<E, size_type>(field); }

        inline size_type toSizeType() const { return field; }

    private:
        size_type field;
    };
}

#endif //FEATHER_ENUM_SET_H
