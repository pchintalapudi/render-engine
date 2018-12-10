//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_INVALIDATION_EVENT_H
#define FEATHER_INVALIDATION_EVENT_H

#include "bitfields.h"

namespace feather {
    namespace observable {
        typedef Long EventCountSize;

        enum class InvEvent {
            INVALIDATED, LIST_CHANGE, SET_CHANGE, MAP_CHANGE, DIRECTION, RELAYOUT, REPAINT
        };

        inline bool present(EventCountSize l, InvEvent event) { return feather::present(l, static_cast<UByte>(event)); }

        inline EventCountSize set(EventCountSize l, InvEvent i) { return feather::set(l, static_cast<UByte>(i)); }

        inline EventCountSize unset(EventCountSize l, InvEvent i) { return feather::unset(l, static_cast<UByte>(i)); }
    }
}
#endif //FEATHER_INVALIDATION_EVENT_H
