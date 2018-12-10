//
// Created by prem on 12/10/2018.
//

#ifndef FEATHER_INVALIDATION_EVENT_H
#define FEATHER_INVALIDATION_EVENT_H

#include "typedefs.h"

namespace feather {
    namespace observable {
        typedef Long EventCountSize;

        enum class InvEvent {
            INVALIDATED, PROPAGATE, LIST_CHANGE, SET_CHANGE, MAP_CHANGE, DIRECTION, RELAYOUT, REPAINT, __COUNT__
        };
    }
}
#endif //FEATHER_INVALIDATION_EVENT_H
