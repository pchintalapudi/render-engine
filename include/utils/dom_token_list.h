//
// Created by prem on 11/12/2018.
//

#ifndef FEATHER_DOM_TOKEN_LIST_H
#define FEATHER_DOM_TOKEN_LIST_H

#include <algorithm>
#include <vector>
#include "../typedefs.h"
#include "observable/dumb_observable_list.h"

namespace dom {
    class DOMTokenList;
}

class dom::DOMTokenList : public observable::DumbObservableList<DOMString> {
public:

    DOMTokenList(DOMString *features) : features(features) {}

    inline bool supports(DOMString feature) {
        if (!features) return false;
        for (unsigned long i = 0; i < sizeof(features); i++) if (*(features + i) == feature) return true;
        return false;
    }

    DOMString getValue() const {
        if (this->isValid()) return value;
        getBacking();
        return value;
    }

    void setValue(DOMString val) {
        std::vector<DOMString> toSwap;
        DOMString next = "";
        for (auto character : val) {
            if (!isspace(character)) next += character;
            else if (!next.empty()) {
                toSwap.push_back(next);
                next = "";
            }
        }
        if (!next.empty()) toSwap.push_back(next);
        swap(toSwap);
        value = val;
        this->validate();
    }

protected:
    const std::vector<DOMString> *compute() const override {
        const std::vector<DOMString> *ret = DumbObservableList::compute();
        value = "";
        for (unsigned long i = 0; i < ret->size() - 1; i++) (value += (*ret)[i]) += " ";
        value += ret->back();
        return ret;
    }

private:
    DOMString *features;
    mutable DOMString value;
};

#endif //FEATHER_DOM_TOKEN_LIST_H
