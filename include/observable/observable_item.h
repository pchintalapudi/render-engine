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

            explicit WatchedObservableItem(RegularEnumSet <InvEvent> required)
                    : required(required) {}

            explicit WatchedObservableItem(RegularEnumSet <InvEvent> required, RegularEnumSet <InvEvent> anyOf)
                    : required(required), anyOf(anyOf) {}

            explicit WatchedObservableItem(I i) : i(i) {}

            explicit WatchedObservableItem(I i, RegularEnumSet <InvEvent> required, RegularEnumSet <InvEvent> anyOf)
                    : i(i), required(required), anyOf(anyOf) {}

            inline I get() const { return i; }

            inline void set(I i) {
                this->i = i;
                validate();
            }

        protected:
            void modify(RegularEnumSet <InvEvent> &s, const Invalidatable *) const override {
                if (s.containsAll(required)) s += InvEvent::INVALIDATE_THIS; else s -= InvEvent::INVALIDATE_THIS;
            }

        private:
            I i{};
            RegularEnumSet <InvEvent> required{}, anyOf{};
        };

        template<typename I>
        class SourceObservableItem : public Invalidatable {
        public:

            SourceObservableItem() = default;

            explicit SourceObservableItem(I init) : i(std::move(init)) {}

            inline const I &get() const { return i; }

            inline void set(I i) {
                this->i = i;
                invalidate(RegularEnumSet<InvEvent>(), this);
            }

            SourceObservableItem &operator=(I i) {
                set(i);
                return *this;
            }

            const I &operator*() const {
                return get();
            }

            const I *operator->() const {
                return &i;
            }

        protected:
            void modify(RegularEnumSet <InvEvent> &s, const Invalidatable *) const override {
                s - InvEvent::INVALIDATE_THIS;
            }

        private:
            I i{};
        };
    }
}
#endif //FEATHER_OBSERVABLE_ITEM_H
