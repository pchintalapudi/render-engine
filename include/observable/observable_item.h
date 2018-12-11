//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_OBSERVABLE_ITEM_H
#define FEATHER_OBSERVABLE_ITEM_H

#include "invalidatable.h"

namespace feather {
    namespace observable {
        template<typename I>
        class ObservableItem : public Invalidatable {
        public:

            ObservableItem() = default;

            explicit ObservableItem(RegularEnumSet<InvEvent> required) : required(required) {}

            inline I get() { return i; }

            inline void set(I i) {
                this->i = i;
                validate();
            }

        protected:
            void modify(RegularEnumSet<InvEvent> &s, const Invalidatable *i) const override {
                if (s.containsAll(required))s.add(InvEvent::INVALIDATED); else s.remove(InvEvent::INVALIDATED);
            }

        private:
            I i;
            RegularEnumSet<InvEvent> required = RegularEnumSet<InvEvent>();
        };
    }
}
#endif //FEATHER_OBSERVABLE_ITEM_H
