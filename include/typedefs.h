//
// Created by prem on 11/30/2018.
//

#ifndef FEATHER_TYPEDEFS_H
#define FEATHER_TYPEDEFS_H

#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace feather {
    typedef std::string DOMString;
    typedef std::int_fast8_t Byte;
    typedef std::int_fast16_t Short;
    typedef std::int_fast32_t Int;
    typedef std::int_fast64_t Long;
    typedef std::uint_fast8_t UByte;
    typedef std::uint_fast16_t UShort;
    typedef std::uint_fast32_t UInt;
    typedef std::uint_fast64_t ULong;
    template<typename E>
    using Vector = std::vector<E>;
    template<typename K, typename V>
    using Map = std::map<K, V>;
    template<typename E>
    using Set = std::set<E>;
    template<typename P>
    using WeakPointer = std::weak_ptr<P>;
    template<typename P>
    using StrongPointer = std::shared_ptr<P>;
    template<typename P>
    using UniquePointer = std::shared_ptr<P>;
    template<typename F>
    using Function = std::function<F>;
    template<typename E>
    using WeakSet = std::set<WeakPointer<E>, std::owner_less<WeakPointer<E>>>;
    template<typename E>
    using WeakMap = std::map<WeakPointer<E>, std::owner_less<WeakPointer<E>>>;

    class Base {
    public:
        virtual void gc(UByte) {}
    };
}
#endif //FEATHER_TYPEDEFS_H
