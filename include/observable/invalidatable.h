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

    void unbind(WeakPointer<Invalidatable>) const;

    void gc(UByte) override;

    virtual ~Invalidatable() = default;

protected:
    void invalidate(RegularEnumSet<InvEvent>, const Invalidatable *) const;

    virtual void modify(RegularEnumSet<InvEvent> &s, const Invalidatable *) const { s.add(InvEvent::INVALIDATED); }

    void validate() const { valid = true; }

private:
    mutable bool valid = false;
    mutable WeakSet<Invalidatable> dependents = WeakSet<Invalidatable>();
};

#endif //FEATHER_INVALIDATABLE_H
