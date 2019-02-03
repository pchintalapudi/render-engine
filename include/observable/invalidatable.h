//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_INVALIDATABLE_H
#define FEATHER_INVALIDATABLE_H

#include "invalidation_event.h"

namespace feather {
    namespace observable {
        class Invalidatable;
    }
}

class feather::observable::Invalidatable : public Base {
public:

    Invalidatable() = default;

    inline bool isValid() const { return valid; }

    inline void bindTo(const StrongPointer<const Invalidatable> &p) const {
        p->bindInternal(std::static_pointer_cast<const Invalidatable>(shared_from_this()));
    }

    inline void unbindFrom(const StrongPointer<const Invalidatable> &p) const {
        p->unbindInternal(std::static_pointer_cast<const Invalidatable>(shared_from_this()));
    }

    void gc(unsigned char) override;

    virtual ~Invalidatable() = default;

protected:
    void invalidate(EnumSet<InvEvent>, const Invalidatable *) const;

    virtual EnumSet<InvEvent> modify(EnumSet<InvEvent> s, const Invalidatable *) const {
        return s + InvEvent::INVALIDATE_THIS;
    }

    void validate() const { valid = true; }

    void bindInternal(StrongPointer<const Invalidatable>) const;

    void unbindInternal(StrongPointer<const Invalidatable>) const;

private:
    mutable bool valid = false;
    mutable WeakSet<const Invalidatable> dependents{};
};

#endif //FEATHER_INVALIDATABLE_H
