//
// Created by prem on 11/28/2018.
//

#ifndef FEATHER_FILTERED_LIST_H
#define FEATHER_FILTERED_LIST_H

#include "include/utils/observable/observable_list.h"

namespace observable {
    template<typename T>
    class FilteredList;
}

template<typename T>
class observable::FilteredList : public ObservableList<T> {
public:
    FilteredList(ObservableList <T> *other, std::function<bool(T)> filter) : other(other), filter(filter) {
        other->addInvalidator(this, this->invalidator);
    }

    ~FilteredList() {
        if (other) other->removeInvalidator(this);
    }

protected:
    const std::vector<T> *compute() const override {
        filtered.clear();
        for (unsigned long i = 0; i < other->size(); i++) {
            auto element = other->get(i);
            if (filter(element)) filtered.push_back(element);
        }
        return &filtered;
    }

private:
    ObservableList <T> *other;
    std::function<bool(T)> filter;
    mutable std::vector<T> filtered;
};

#endif //FEATHER_FILTERED_LIST_H
