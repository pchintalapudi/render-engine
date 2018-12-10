//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_BITFIELDS_H
#define FEATHER_BITFIELDS_H

#include "typedefs.h"

namespace feather {
    template<typename Bitfield, typename IdxSize>
    inline bool present(Bitfield l, IdxSize idx) { return (l & 1u << idx) != 0; }

    template<typename Bitfield, typename IdxSize>
    inline Bitfield set(Bitfield l, IdxSize idx) { return l | 1u << idx; }

    template<typename Bitfield, typename IdxSize>
    inline Bitfield unset(Bitfield l, IdxSize idx) { return l & ~(1u << idx); }
}
#endif //FEATHER_BITFIELDS_H
