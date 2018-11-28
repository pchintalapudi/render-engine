//
// Created by prem on 11/28/2018.
//

#ifndef FEATHER_NODE_LIST_H
#define FEATHER_NODE_LIST_H

#include "observable/smart_observable_list.h"

namespace dom {
    class Node;

    class NodeList;
}

class dom::NodeList : public observable::SmartObservableList<Node *> {

};

#endif //FEATHER_NODE_LIST_H
