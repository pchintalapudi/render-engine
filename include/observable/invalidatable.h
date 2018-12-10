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

    void bind(WeakPointer<Invalidatable>) const;

    inline void bind(StrongPointer<Invalidatable> dependent) const { bind(WeakPointer<Invalidatable>(dependent)); }

    void unbind(WeakPointer<Invalidatable>) const;

    inline void unbind(StrongPointer<Invalidatable> dependent) const { unbind(WeakPointer<Invalidatable>(dependent)); }

    void gc(UByte) override;

    virtual ~Invalidatable() = default;

protected:
    void invalidate(EventCountSize l) const;

    virtual void modify(EventCountSize *l) const { *l = set(*l, InvEvent::INVALIDATED); }

private:
    mutable bool valid = false;
    mutable Long lastInvalidationCall = 0;
    mutable WeakSet<Invalidatable> dependents = WeakSet<Invalidatable>();
};

#endif //FEATHER_INVALIDATABLE_H
