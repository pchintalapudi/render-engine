//
// Created by prem on 11/8/2018.
//

#ifndef FEATHER_SLOTABLE_H
#define FEATHER_SLOTABLE_H

#include "element.h"

class Slotable {
    virtual Element *getAssignedSlot() const;

    virtual void setAssignedSlot(Element *slot);
};
#endif //FEATHER_SLOTABLE_H
