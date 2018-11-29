//
// Created by prem on 11/26/2018.
//

#ifndef FEATHER_OBSERVABLE_LIST_H
#define FEATHER_OBSERVABLE_LIST_H

#include <algorithm>
#include <functional>
#include <map>
#include <vector>
#include "invalidatable.h"

namespace observable {
    template<typename T>
    class ObservableList;
}

template<typename T>
class observable::ObservableList : public Invalidatable {
public:
    ObservableList() : backing() {}

    inline T get(unsigned long index) const {
        return getBacking()[index];
    }

    unsigned long indexOf(const T value) const {
        for (unsigned long i = 0; i < size(); i++) if (get(i) == value) return i;
        return ~static_cast<unsigned long>(0);
    }

    std::vector<T> subvector(unsigned long start, unsigned long end) const {
        return std::vector<T>(getBacking()->begin() + start, getBacking()->begin() + end);
    }

    template<typename Op>
    inline void forEach(Op op) const {
        std::for_each(getBacking().begin(), getBacking().end(), op);
    }

    template<typename Op>
    inline void forEach(unsigned long start, unsigned long end, Op op) const {
        std::for_each(getBacking()->begin() + start, getBacking()->begin() + end, op);
    }

    template<typename Mapper, typename R>
    std::vector<R> map(Mapper mapper) const {
        std::vector<R> vec;
        for (T back : getBacking()) vec.push_back(mapper(back));
        return vec;
    }

    template<typename Filter>
    std::vector<T> filter(Filter filter) const {
        std::vector<T> vec;
        for (T back : getBacking()) if (filter(back)) vec.push_back(back);
        return vec;
    }

    template<typename Filter>
    bool anyMatch(Filter filter) const {
        for (T back : getBacking()) if (filter(back)) return true;
        return false;
    }

    template<typename Filter>
    inline bool allMatch(Filter filter) const {
        return anyMatch([filter](T node) { return !filter(node); });
    }

    template<typename Filter>
    inline bool noneMatch(Filter filter) const { return !anyMatch(filter); }

    bool contains(T t) { return std::find(getBacking().begin(), backing->end(), t) != backing->end(); }

    inline unsigned long size() const { return getBacking().size(); }

protected:

    virtual const std::vector<T> *compute() const = 0;

    inline void deleteBacking() { delete backing; }

    const std::vector<T> &getBacking() const {
        if (this->isValid()) return *backing;
        backing = compute();
        validate();
        return *backing;
    }

private:
    mutable const std::vector<T> *backing;
};

#endif //FEATHER_OBSERVABLE_LIST_H
