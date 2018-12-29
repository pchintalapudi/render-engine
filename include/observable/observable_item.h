//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_OBSERVABLE_ITEM_H
#define FEATHER_OBSERVABLE_ITEM_H

#include "invalidatable.h"

namespace feather {
    namespace observable {
        template<typename I>
        class WatchedObservableItem : public Invalidatable {
        public:

            WatchedObservableItem() = default;

            explicit WatchedObservableItem(const RegularEnumSet <InvEvent> &required) : required(required) {}

            inline I get() const { return i; }

            inline void set(I i) {
                this->i = i;
                validate();
            }

        protected:
            void modify(RegularEnumSet <InvEvent> &s, const Invalidatable *) const override {
                if (s.containsAll(required)) s.add(InvEvent::INVALIDATED); else s.remove(InvEvent::INVALIDATED);
            }

        private:
            I i;
            const RegularEnumSet <InvEvent> required = RegularEnumSet<InvEvent>();
        };

        template<typename I>
        class SourceObservableItem : public Invalidatable {
        public:

            SourceObservableItem() = default;

            explicit SourceObservableItem(I init) : i(std::move(init)) {}

            inline I get() const { return i; }

            inline void set(I i) {
                this->i = i;
                invalidate(RegularEnumSet<InvEvent>(), this);
            }

        protected:
            void modify(RegularEnumSet <InvEvent> &s, const Invalidatable *) const override {
                s.remove(InvEvent::INVALIDATED);
            }

        private:
            I i = I();
        };
    }
}
#endif //FEATHER_OBSERVABLE_ITEM_H
