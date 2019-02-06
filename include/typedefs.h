//
// Created by prem on 11/30/2018.
//

#ifndef FEATHER_TYPEDEFS_H
#define FEATHER_TYPEDEFS_H

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include "engine.h"
#include "enums/enum_set.h"
#include "enums/enum_map.h"

#define DEBUG_MODE

namespace feather {
    template<typename E, UInt length>
    using Array = std::array<E, length>;
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
    template<typename ...Types>
    using Tuple = std::tuple<Types...>;
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
    template<typename E, typename V>
    using EnumMap = EnumMapRaw<E, StrongPointer<V>>;
    template<typename E>
    using List = std::list<E>;
    template<typename E>
    using Deque = std::deque<E>;
    template<typename... T>
    using Variant = std::variant<T...>;

    template<typename T>
    const StrongPointer<T> &getEmptyPtr();

    const DOMString &getEmptyString();

    enum class TriValue {
        AUTO, NO, YES
    };

    typedef std::chrono::milliseconds Millis;
    typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
    const auto currentTime = std::chrono::system_clock::now;

    namespace profile {
        UInt profileStart();

        void profileEnd(UInt id, DOMString name = "");
    }

    class Base : public std::enable_shared_from_this<Base> {
    public:
        virtual void gc(unsigned char) {}

        WeakPointer<Base> getWeakPointerToThis() { return WeakPointer<Base>(this->shared_from_this()); }
    };

    namespace utf8 {
        unsigned char charSize(char c);

        UInt toCodePoint(const char *c);

        UInt toCodePoint(const char *c, unsigned char size);

        void appendCodePoint(DOMString &str, const char *c);

        void appendCodePoint(DOMString &str, UInt codePoint);

        bool isAscii(char c);
    }
}
#endif //FEATHER_TYPEDEFS_H
