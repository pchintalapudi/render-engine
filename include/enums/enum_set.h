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

        template<class... T>
        explicit EnumSet(T... initial) {
            std::array<E, sizeof...(initial)> arr = {initial...};
            for (auto e : arr) this->operator+=(e);
        }

        explicit EnumSet(size_type field) : field(field) {}

        inline auto operator+=(E e) {
            field |= static_cast<size_type>(1) << static_cast<unsigned char>(e);
            return *this;
        }

        inline EnumSet<E, size_type> operator+(E e) const {
            return EnumSet(field | static_cast<size_type>(1) << static_cast<unsigned char>(e));
        }

        inline auto operator-=(E e) {
            field &= ~(static_cast<size_type>(1) << static_cast<unsigned char>(e));
            return *this;
        }

        inline EnumSet<E, size_type> operator-(E e) const {
            return EnumSet(field & ~(static_cast<size_type>(1) << static_cast<unsigned char>(e)));
        }

        inline auto operator+=(const EnumSet<E, size_type> &other) {
            field |= other.field;
            return *this;
        }

        inline EnumSet<E, size_type> operator+(const EnumSet<E, size_type> &other) const {
            return EnumSet(field | other.field);
        }

        inline auto operator-=(const EnumSet<E, size_type> &other) {
            field &= ~other.field;
            return *this;
        }

        inline EnumSet<E, size_type> operator-(const EnumSet<E, size_type> &other) {
            return EnumSet(field & ~other.field);
        }

        inline bool operator[](E e) const { return (field & 1 << static_cast<unsigned char>(e)) != 0; }

        inline bool operator==(const EnumSet<E, size_type> &other) const { return field == other.field; }

        inline bool operator!=(const EnumSet<E, size_type> &other) const { return field != other.field; };

        inline EnumSet<E, size_type> operator~() const { return EnumSet<E, size_type>(~field); }

        inline explicit operator size_type() const { return field; }

        inline EnumSet<E, size_type> &operator=(size_type field) {
            this->field = field;
            return *this;
        }

        inline bool operator!() const { return !field; }

        explicit inline operator bool() const { return field != 0; }

        size_type field;
    };
}

#endif //FEATHER_ENUM_SET_H
