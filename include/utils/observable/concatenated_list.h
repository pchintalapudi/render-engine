//
// Created by prem on 11/29/2018.
//

#ifndef FEATHER_CONCATENATED_LIST_H
#define FEATHER_CONCATENATED_LIST_H

#include "observable_list.h"

namespace observable {
    template<typename T>
    class ConcatenatedList;
}

template<typename T>
class observable::ConcatenatedList : public ObservableList<T> {
public:
    void addList(ObservableList <T> *observableList) {}

protected:
    const std::vector<T> *compute() const override {
        concat.clear();
        for (auto obs : observed) {
            if (obs) {
                concat.reserve(concat.size() + obs->size());
                for (unsigned long i = 0; i < obs->size(); i++) concat.push_back(obs->get(i));
            }
        }
        return &concat;
    }

private:
    std::vector<ObservableList < T>*>
    observed;
    mutable std::vector<T> concat;
};

#endif //FEATHER_CONCATENATED_LIST_H
