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

        explicit EnumSet(size_type field) : field(field) {}

        inline EnumSet<E, size_type> &add(E e) {
            field |= static_cast<size_type>(1) << static_cast<unsigned char>(e);
            return *this;
        }

        inline auto operator+=(E e) { return add(e); }

        inline EnumSet<E, size_type> operator+(E e) const { return EnumSet(field).add(e); }

        inline EnumSet<E, size_type> &remove(E e) {
            field &= ~(static_cast<size_type>(1) << static_cast<unsigned char>(e));
            return *this;
        }

        inline auto operator-=(E e) { return remove(e); }

        inline EnumSet<E, size_type> operator-(E e) const { return EnumSet(field).remove(e); }

        inline EnumSet<E, size_type> &addAll(const EnumSet<E, size_type> &other) {
            field |= other.field;
            return *this;
        }

        inline auto operator+=(const EnumSet<E, size_type> &other) { return addAll(other); }

        inline EnumSet<E, size_type> operator+(const EnumSet<E, size_type> &other) const {
            return EnumSet(field | other.field);
        }

        inline EnumSet<E, size_type> removeAll(const EnumSet<E, size_type> &other) {
            field &= ~other.field;
            return *this;
        }

        inline auto operator-=(const EnumSet<E, size_type> &other) { return removeAll(other); }

        inline EnumSet<E, size_type> operator-(const EnumSet<E, size_type> &other) {
            return EnumSet(field & ~other.field);
        }

        inline bool contains(E e) const { return (field & 1 << static_cast<unsigned char>(e)) != 0; }

        inline bool containsAll(const EnumSet<E, size_type> other) const {
            return (field & other.field) == other.field;
        }

        inline bool containsAny(const EnumSet<E, size_type> other) const { return (field & other.field) != 0; }

        inline bool containsNone(const EnumSet<E, size_type> other) const { return (field & other.field) == 0; }

        inline bool operator==(const EnumSet<E, size_type> &other) const { return field == other.field; }

        inline bool operator!=(const EnumSet<E, size_type> &other) const { return field != other.field; };

        inline EnumSet<E, size_type> invert() const { return EnumSet<E, size_type>(~field); }

        inline EnumSet<E, size_type> operator~() const { return invert(); }

        inline size_type toBitfield() const { return field; }

        inline void fromBitfield(size_type field) { this->field = field; }

        inline bool operator!() const { return !field; }

        explicit inline operator bool() const { return field != 0; }

    private:
        size_type field;
    };
}

#endif //FEATHER_ENUM_SET_H
