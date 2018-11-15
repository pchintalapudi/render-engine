//
// Created by prem on 11/9/2018.
//

#ifndef FEATHER_EVENT_PHASE_H
#define FEATHER_EVENT_PHASE_H

namespace js {
    enum class EventPhase;
}
enum class js::EventPhase {
    NONE, CAPTURING_PHASE, AT_TARGET, BUBBLING_PHASE
};
#endif //FEATHER_EVENT_PHASE_H
