//
// Created by prem on 11/28/2018.
//

#ifndef FEATHER_SMART_OBSERVABLE_LIST_H
#define FEATHER_SMART_OBSERVABLE_LIST_H

#include "observable_list.h"
#include "event/event_type.h"

namespace observable {
    template<typename T>
    class SmartObservableList;
}

template<class T>
class observable::SmartObservableList : public ObservableList<T> {
public:
    void set(unsigned long index, T t) {
        source[index]->removeInvalidator(this);
        t->addInvalidator(this, this->invalidator);
        source[index] = t;
    }

    void add(T t) {
        t->addInvalidator(this, this->invalidator);
        source.push_back(t);
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

    void addAll(const std::vector<T> &elements) {
        for (auto element : elements) {
            element->addInvalidator(this, this->invalidator);
            source.push_back(element);
        }
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

    void insert(unsigned long index, T t) {
        t->addInvalidator(this, this->invalidator);
        source.insert(source.begin() + index, t);
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

    void insertAll(unsigned long index, const std::vector<T> &elements) {
        for (auto element : elements) element->addInvalidator(this, this->invalidator);
        source.insert(source.begin() + index, elements.begin(), elements.end());
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

    T remove(unsigned long index) {
        auto t = source[index];
        source[index]->removeInvalidator(this);
        source.erase(source.begin() + index);
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
        return t;
    }

    inline T remove(int index) { return remove(static_cast<unsigned long>(index)); }

    void remove(const T t) {
        for (unsigned long i = 0; i < source.size(); i++) {
            if (source[i] == t) {
                remove(i);
                return;
            }
        }
    }

    std::vector<T> remove(unsigned long begin, unsigned long end) {
        std::vector<T> erased;
        for (unsigned long i = begin; i < end; i++) {
            source[i]->removeInvalidator(this);
            erased.push_back(source[i]);
        }
        source.erase(source.begin() + begin, source.begin() + end);
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
        return erased;
    }

    inline void reserve(unsigned long amount) { source.reserve(amount); }

    void clear() {
        source.clear();
        this->invalidate(observable::generate(observable::EventType::LIST_CHANGE));
    }

protected:
    const std::vector<T> *compute() const override { return &source; }

private:
    std::vector<T> source;
};

#endif //FEATHER_SMART_OBSERVABLE_LIST_H
