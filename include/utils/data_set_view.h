//
// Created by prem on 12/6/2018.
//

#ifndef FEATHER_DATA_SET_VIEW_H
#define FEATHER_DATA_SET_VIEW_H

#include "named_node_map.h"

namespace dom {
    class DataSetView;
}
class dom::DataSetView {
public:
    DOMString *getAttribute(DOMString name) const;

    void setAttribute(DOMString name, DOMString val);

    bool contains(DOMString name) const;

private:
    NamedNodeMap &watched;
    Element *owner;
};

#endif //FEATHER_DATA_SET_VIEW_H
