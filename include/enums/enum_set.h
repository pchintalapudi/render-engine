//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_ENUM_SET_H
#define FEATHER_ENUM_SET_H
namespace feather {
    typedef std::string DOMString;
    typedef std::int_fast16_t Short;
    typedef std::int_fast32_t Int;
    typedef std::int_fast64_t Long;
    typedef std::uint_fast16_t UShort;
    typedef std::uint_fast32_t UInt;
    typedef std::uint_fast64_t ULong;
    //Nonstandard gcc extended integer sizes
//    typedef __int128 Huge;
//    typedef unsigned __int128 UHuge;


    template<ULong num>
    struct UNumericType {
        typedef typename UNumericType<num - 1>::type type;
    };

    template<>
    struct UNumericType<0> {
        typedef unsigned char type;
    };

    template<>
    struct UNumericType<8> {
        typedef UShort type;
    };

    template<>
    struct UNumericType<16> {
        typedef UInt type;
    };

    template<>
    struct UNumericType<32> {
        typedef ULong type;
    };

//    template<>
//    struct UNumericType<64> {
//        typedef UHuge type;
//    };

    template<>
    struct UNumericType<64> {
    };

    template<typename E>
    class EnumSet {
    public:
        typedef typename UNumericType<static_cast<ULong>(E::__COUNT__)>::type size_type;

        EnumSet() = default;

        template<class... T>
        explicit EnumSet(T... initial) : field(0) {
            std::array<E, sizeof...(initial)> arr = {initial...};
            for (auto e : arr) field |= static_cast<size_type>(1) << static_cast<unsigned char>(e);
        }

        explicit EnumSet(size_type field) : field(field) {}

        inline auto operator+=(E e) {
            field |= static_cast<size_type>(1) << static_cast<unsigned char>(e);
            return *this;
        }

        inline EnumSet operator+(E e) const {
            return EnumSet(field | static_cast<size_type>(1) << static_cast<unsigned char>(e));
        }

        inline auto operator-=(E e) {
            field &= ~(static_cast<size_type>(1) << static_cast<unsigned char>(e));
            return *this;
        }

        inline EnumSet operator-(E e) const {
            return EnumSet(field & ~(static_cast<size_type>(1) << static_cast<unsigned char>(e)));
        }

        inline auto operator+=(const EnumSet<E> &other) {
            field |= other.field;
            return *this;
        }

        inline EnumSet operator+(const EnumSet<E> &other) const {
            return EnumSet(field | other.field);
        }

        inline auto operator-=(const EnumSet<E> &other) {
            field &= ~other.field;
            return *this;
        }

        inline EnumSet operator-(const EnumSet<E> &other) {
            return EnumSet(field & ~other.field);
        }

        inline bool operator[](E e) const { return (field & 1 << static_cast<unsigned char>(e)) != 0; }

        inline bool operator==(const EnumSet<E> &other) const { return field == other.field; }

        inline bool operator!=(const EnumSet<E> &other) const { return field != other.field; };

        inline EnumSet operator~() const { return EnumSet<E>(~field); }

        inline explicit operator size_type() const { return field; }

        inline EnumSet &operator=(size_type field) {
            this->field = field;
            return *this;
        }

        inline bool operator!() const { return !field; }

        explicit inline operator bool() const { return field != 0; }

        size_type field;
    };
}

#endif //FEATHER_ENUM_SET_H
