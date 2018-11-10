//
// Created by prem on 11/10/2018.
//

#ifndef FEATHER_SLOTABLE_H
#define FEATHER_SLOTABLE_H

class Element;

class Slotable {
public:
    inline Element *getAssignedSlot() { return assignedSlot; }

    inline void setAssignedSlot(Element *slot) { assignedSlot = slot; }

private:
    Element *assignedSlot;
};

#endif //FEATHER_SLOTABLE_H
