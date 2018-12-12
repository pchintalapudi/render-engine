//
// Created by prem on 11/30/2018.
//

#ifndef FEATHER_TYPEDEFS_H
#define FEATHER_TYPEDEFS_H

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include "enums/enum_set.h"
#include "enums/enum_map.h"

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
    //Nonstandard gcc extended integer sizes
    typedef __int128 Huge;
    typedef unsigned __int128 UHuge;
    template<typename E>
    using Vector = std::vector<E>;
    template<typename K, typename V>
    using Map = std::map<K, V>;
    template<typename K, typename V>
    using Multimap = std::multimap<K, V>;
    template<typename E>
    using Set = std::set<E>;
    template<typename K, typename V>
    using Pair = std::pair<K, V>;
    template<typename P>
    using UnaryPair = std::pair<P, P>;
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
    template<typename E>
    using RegularEnumSet = EnumSet<E, ULong>;
    template<typename E>
    using JumboEnumSet = EnumSet<E, UHuge>;
    template<typename E, typename V>
    using EnumMap = EnumMapRaw<E, StrongPointer<V>>;
    template<typename E>
    using List = std::list<E>;
    template<typename E>
    using Deque = std::deque<E>;

    typedef std::chrono::milliseconds Millis;
    typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
    const auto currentTime = std::chrono::system_clock::now;

    class Base : public std::enable_shared_from_this<Base> {
    public:
        virtual void gc(UByte) {}

        WeakPointer<Base> getWeakPointerToThis() { return WeakPointer<Base>(this->shared_from_this()); }
    };
}
#endif //FEATHER_TYPEDEFS_H
