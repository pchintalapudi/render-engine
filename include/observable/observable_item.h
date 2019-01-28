//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_OBSERVABLE_ITEM_H
#define FEATHER_OBSERVABLE_ITEM_H

#include "invalidatable.h"

namespace feather {
    namespace observable {

        template<typename I, bool source, StrongPointer<Invalidatable>(*convert)(const I &) = nullptr,
                typename required = RegularEnumSet<InvEvent>(), typename ignore = RegularEnumSet<InvEvent>()>
        class ObservableItem : public Invalidatable {
        public:
            ObservableItem() = default;

            explicit ObservableItem(I i) {
                operator=(std::move(i));
            }

            inline const I &operator*() const { return i; }

            inline const I *operator->() const { return &i; }

            inline ObservableItem &operator=(I i) {
                if (convert) {
                    unbindFrom(convert(this->i));
                    bindTo(convert(i));
                }
                this->i = std::move(i);
                if (source) invalidate(RegularEnumSet<InvEvent>(), this);
                else validate();
                return *this;
            }

        private:
            I i{};
        };
    }
}
#endif //FEATHER_OBSERVABLE_ITEM_H
