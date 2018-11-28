//
// Created by prem on 11/28/2018.
//

#ifndef FEATHER_HTML_COLLECTION_H
#define FEATHER_HTML_COLLECTION_H

#include "observable/filtered_list.h"
#include "node_list.h"

namespace dom {
    class Element;

    class HTMLCollection;
}

class dom::HTMLCollection : public observable::ObservableList<Element *> {
public:
    explicit HTMLCollection(ObservableList<Node *> *watched) : watched(watched) {
        if (watched) watched->addInvalidator(this, [this]() { this->invalidate(); });
    }

    ~HTMLCollection() {
        if (watched) watched->removeInvalidator(this);
    }

protected:
    const std::vector<Element *> *compute() const;

private:
    ObservableList<Node *> *watched;
    mutable std::vector<Element *> backing;
};

#endif //FEATHER_HTML_COLLECTION_H
