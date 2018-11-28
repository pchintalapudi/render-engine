//
// Created by prem on 11/28/2018.
//

#ifndef FEATHER_SMART_OBSERVABLE_LIST_H
#define FEATHER_SMART_OBSERVABLE_LIST_H

#include "observable_list.h"

namespace observable {
    template<typename T>
    class SmartObservableList;
}

template<class T>
class observable::SmartObservableList : public ObservableList<T> {
public:
    void set(unsigned long index, T t) {
        source[index]->removeInvalidator(this);
        t->addInvalidator(this, invalidation);
        source[index] = t;
    }

    void add(T t) {
        t->addInvalidator(this, [this]() { this->invalidate(); });
        source.push_back(t);
        this->invalidate();
    }

    void addAll(std::vector<T> elements) {
        for (auto element : elements) {
            element->addInvalidator(this, invalidation);
            source.push_back(element);
        }
        this->invalidate();
    }

    void insert(unsigned long index, T t) {
        t->addInvalidator(this, invalidation);
        source.insert(source.begin() + index, t);
        this->invalidate();
    }

    void insertAll(unsigned long index, std::vector<T> &elements) {
        for (auto element : elements) element->addInvalidator(this, invalidation);
        source.insert(source.begin() + index, elements.begin(), elements.end());
        this->invalidate();
    }

    T remove(unsigned long index) {
        auto t = source[index];
        source[index]->removeInvalidator(this);
        source.erase(source.begin() + index);
        this->invalidate();
        return t;
    }

    inline T remove(int index) { return remove(static_cast<unsigned long>(index)); }

    void remove(T t) {
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
        this->invalidate();
        return erased;
    }

    void clear() {
        source.clear();
        this->invalidate();
    }

protected:
    const std::vector<T> *compute() const override { return &source; }

private:
    std::vector<T> source;
    const std::function<void()> invalidation = [this]() { this->invalidate(); };
};

#endif //FEATHER_SMART_OBSERVABLE_LIST_H
