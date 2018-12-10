//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_INVALIDATABLE_H
#define FEATHER_INVALIDATABLE_H

#include "../typedefs.h"

namespace feather {
    namespace observable {
        class Invalidatable;
    }
}

class feather::observable::Invalidatable : public Base {
public:

    Invalidatable() = default;

    inline bool isValid() { return valid; }

    void bind(WeakPointer<Invalidatable>);

    inline void bind(StrongPointer<Invalidatable> dependent) { bind(WeakPointer<Invalidatable>(dependent)); }

    void unbind(WeakPointer<Invalidatable>);

    inline void unbind(StrongPointer<Invalidatable> dependent) { unbind(WeakPointer<Invalidatable>(dependent)); }

    void gc(UByte) override;

    virtual ~Invalidatable() = default;

protected:
    void invalidate(Long);

    virtual void modify(Long *) {}

private:
    bool valid = false;
    Long lastInvalidationCall = 0;
    WeakSet<Invalidatable> dependents = WeakSet<Invalidatable>();
};

#endif //FEATHER_INVALIDATABLE_H
