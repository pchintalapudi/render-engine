//
// Created by prem on 12/1/2018.
//

#ifndef FEATHER_FULL_CHILD_LIST_H
#define FEATHER_FULL_CHILD_LIST_H

#include "html_collection.h"

namespace dom {
    class FullChildList;

    class Element;
}

class dom::FullChildList : public observable::ObservableList<Element *> {
public:
    explicit FullChildList(HTMLCollection &children) : children(children) {}

protected:
    std::vector<Element *> *compute() const override;

    bool handle(long long) const override;

private:
    HTMLCollection &children;
    mutable std::vector<Element *> backing;
};

#endif //FEATHER_FULL_CHILD_LIST_H
