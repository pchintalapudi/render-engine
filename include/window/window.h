//
// Created by prem on 12/9/2018.
//

#ifndef FEATHER_WINDOW_H
#define FEATHER_WINDOW_H

#include "events/event_target.h"
#include "console.h"
#include "nodes/document.h"

namespace window {
    class Window;
}

class window::Window : public js::EventTarget<Window> {
public:
    Console &getConsole() { return console; }

private:
    Console console;
    dom::Document *document;
};

#endif //FEATHER_WINDOW_H
