//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_SLOTABLE_H
#define FEATHER_SLOTABLE_H

#include "typedefs.h"

namespace feather {
    namespace dom {
        class HTMLSlotElement;

        class Slotable {
        public:
            inline StrongPointer <HTMLSlotElement> getAssignedSlot() {
                return slot.expired() ? StrongPointer<HTMLSlotElement>() : slot.lock();
            }

            inline void setAssignedSlot(const StrongPointer <HTMLSlotElement> &slotPointer) { slot = slotPointer; }

        private:
            WeakPointer <HTMLSlotElement> slot;
        };
    }
}
#endif //FEATHER_SLOTABLE_H
