//
// Created by prem on 12/10/2018.
//

#include "events/event.h"

using namespace feather::js;

Event::Event(bool bubbles, bool cancelable, bool composed, feather::DOMString name,
             feather::js::EventTarget &originalTarget,
             feather::List<std::shared_ptr<feather::js::EventTarget>> realPath,
             EventType type)
        : realPath(realPath), current(realPath.begin()), originalTarget(originalTarget), name(name), type(type) {
    if (bubbles) properties.add(EventProperties::BUBBLES);
    if (cancelable) properties.add(EventProperties::CANCELABLE);
    if (composed) properties.add(EventProperties::COMPOSED);
}

Event::Event(bool bubbles, bool cancelable, bool composed, bool trusted, feather::js::EventTarget &originalTarget,
             feather::List<std::shared_ptr<feather::js::EventTarget>> realPath, feather::js::EventType type)
        : realPath(realPath), current(realPath.begin()), originalTarget(originalTarget), name(""), type(type) {
    if (bubbles) properties.add(EventProperties::BUBBLES);
    if (cancelable) properties.add(EventProperties::CANCELABLE);
    if (composed) properties.add(EventProperties::COMPOSED);
    if (trusted) properties.add(EventProperties::TRUSTED);
}

//TODO: Implement me
EventTarget &Event::getEventTarget() {
    //Preconditions: The first node must not be a shadow root (this is easily satisfied by the condition for a shadow root.)
    //Algorithm:
    //1. Assign first node in list to currentNode
    //2. Check if next node is a shadow root, and if it is closed.
    //2a. Return the node
    //2b. If the node is not a shadow root, assign the node to currentNode
    //3. goto 2.
}

//TODO: Implement me
feather::List<feather::StrongPointer<feather::js::EventTarget>> Event::getComposedPath() const {
    //Follow the above algorithm
}

namespace {
    const feather::Map<feather::DOMString, feather::js::EventType> SEMap = {};

    const feather::Map<feather::js::EventType, feather::DOMString> ESMap = {};
}

feather::DOMString feather::js::toString(feather::js::EventType et) {
    return ESMap.at(et);
}

feather::js::EventType feather::js::fromString(feather::DOMString s) {
    return SEMap.at(s);
}