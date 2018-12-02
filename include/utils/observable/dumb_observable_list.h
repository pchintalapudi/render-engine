//
// Created by prem on 11/28/2018.
//

#ifndef FEATHER_DUMB_OBSERVABLE_LIST_H
#define FEATHER_DUMB_OBSERVABLE_LIST_H

#include "observable_list.h"
#include "event/event_type.h"

namespace observable {
    template<typename T>
    class DumbObservableList;
}

template<typename T>
class observable::DumbObservableList : public ObservableList<T> {
public:

    inline void add(T val) {
        source.push_back(val);
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

    inline void addAll(std::vector<T> vals) {
        source.insert(source.end(), vals.begin(), vals.end());
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

    inline void clear() {
        source.clear();
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

    inline void insert(unsigned long index, T val) {
        source.insert(source.begin() + index, val);
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

    inline void insert(unsigned long index, std::vector<T> vals) {
        source.insert(source.begin() + index, vals.begin(), vals.end());
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

    T remove(unsigned long index) {
        auto old = source[index];
        source.erase(old);
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
        return old;
    }

    inline T remove(int index) { return remove(static_cast<unsigned long>(index)); }

    inline void remove(T val) {
        for (unsigned long i = 0; i < this->size(); i++) if (source[i] == val) source.erase(source.begin() + i);
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

    std::vector<T> remove(unsigned long begin, unsigned long end) {
        std::vector<T> removed;
        for (; begin < end; removed.push(source[begin++]));
        source.erase(source.begin() + begin, source.begin() + end);
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
        return removed;
    }

    inline T set(unsigned long i, T val) {
        source[i] = val;
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

    inline void swap(std::vector<T> newSource) {
        source.swap(newSource);
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

protected:
    const std::vector<T> *compute() const override { return &source; }

private:
    std::vector<T> source;
};

#endif //FEATHER_DUMB_OBSERVABLE_LIST_H
