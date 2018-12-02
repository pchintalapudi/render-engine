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
    ConcatenatedList() {
        observed->addInvalidator(this, this->invalidator);
    }

protected:
    const std::vector<T> *compute() const override {
        concat.clear();
        unsigned long netSize = 0;
        for (unsigned long i = 0; i < observed->size(); i++) {
            auto obs = observed->get(i);
            if (obs) netSize += obs->size();
        }
        concat.reserve(netSize);
        for (unsigned long i = 0; i < observed->size(); i++) {
            auto obs = observed->get(i);
            if (obs) {
                for (unsigned long j = 0; j < obs->size(); j++) {
                    concat.push_back(obs->get(j));
                }
            }
        }
        return &concat;
    }

private:
    ObservableList<ObservableList<T> *> *
    observed;
    mutable std::vector<T> concat;
};

#endif //FEATHER_CONCATENATED_LIST_H
