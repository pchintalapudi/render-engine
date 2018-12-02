//
// Created by prem on 12/1/2018.
//

#ifndef FEATHER_MUTABLE_FILTERED_LIST_H
#define FEATHER_MUTABLE_FILTERED_LIST_H

#include "filtered_list.h"

namespace observable {
    template<typename T>
    class MutableFilteredList;
}

template<typename T>
class observable::MutableFilteredList : public FilteredList<T> {
public:
    MutableFilteredList(ObservableList <T> *watched, std::function<bool(T)> filter)
            : FilteredList<T>(watched, filter) {}

protected:
    bool handle(long long) const override { return true; }
};

#endif //FEATHER_MUTABLE_FILTERED_LIST_H
