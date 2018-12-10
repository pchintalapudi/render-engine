//
// Created by prem on 12/7/2018.
//

#ifndef FEATHER_EVENT_TYPES_H
#define FEATHER_EVENT_TYPES_H

#include <functional>
#include <map>
#include <vector>
#include "typedefs.h"

namespace js {

    class Event;

    enum class WindowEvents {
        CACHED,
        ERROR,
        ABORT,
        LOAD,
        BEFORE_UNLOAD,
        UNLOAD,
        ONLINE,
        OFFLINE,
        FOCUS,
        BLUR,
        OPEN,
        MESSAGE,
        CLOSE,
        PAGE_HIDE,
        PAGE_SHOW,
        POP_STATE,
        ANIMATION_START,
        ANIMATION_END,
        ANIMATION_ITERATION,
        TRANSITION_START,
        TRANSITION_CANCEL,
        TRANSITION_END,
        TRANSITION_RUN,
        RESET,
        SUBMIT,
        BEFORE_PRINT,
        AFTER_PRINT,
        COMPOSITION_START,
        COMPOSITION_UPDATE,
        COMPOSITION_END,
        FULLSCREEN_CHANGE,
        FULLSCREEN_ERROR,
        RESIZE,
        SCROLL,
        CUT,
        COPY,
        PASTE,
        KEYDOWN,
        KEYPRESS,
        KEYUP,
        AUX_CLICK,
        CLICK,
        CONTEXT_MENU,
        DBL_CLICK,
        MOUSE_DOWN,
        MOUSE_ENTER,
        MOUSE_LEAVE,
        MOUSE_MOVE,
        MOUSE_OVER,
        MOUSE_OUT,
        MOUSE_UP,
        POINTER_LOCK_CHANGE,
        POINTER_LOCK_ERROR,
        SELECT,
        WHEEL,
        DRAG,
        DRAG_END,
        DRAG_ENTER,
        DRAG_START,
        DRAG_LEAVE,
        DRAG_OVER,
        DROP,
        AUDIO_PROCESS,
        CAN_PLAY,
        CAN_PLAY_THROUGH,
        COMPLETE,
        DURATION_CHANGE,
        EMPTIED,
        ENDED,
        LOADED_DATA,
        LOADED_METADATA,
        PAUSE,
        PLAY,
        PLAYING,
        RATE_CHANGE,
        SEEKED,
        SEEKING,
        STALLED,
        SUSPEND,
        TIME_UPDATE,
        VOLUME_CHANGE,
        WAITING,
        LOAD_END,
        LOAD_START,
        PROGRESS,
        TIMEOUT,
        CHANGE,
        STORAGE,
        CHECKING,
        DOWNLOADING,
        NOUPDATE,
        OBSOLETE,
        UPDATE_READY,
        BROADCAST,
        CHECKBOX_STATE_CHANGE,
        HASH_CHANGE,
        INPUT,
        RADIO_STATE_CHANGE,
        READY_STATE_CHANGE,
        VALUE_CHANGE,
        INVALID,
        LOCALIZED,
        SHOW
    };
}
#endif //FEATHER_EVENT_TYPES_H
