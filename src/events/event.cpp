//
// Created by prem on 12/10/2018.
//

#include "events/event.h"
#include "events/event_target.h"

using namespace feather::js;

Event::Event(bool bubbles, bool cancelable, bool composed, feather::DOMString &&name,
             WeakPointer <EventTarget> &&originalTarget,
             feather::Vector<std::shared_ptr<feather::js::EventTarget>> realPath,
             EventType type)
        : realPath(realPath), current(realPath.begin()), originalTarget(originalTarget), name(name), type(type) {
    if (bubbles) properties += EventProperties::BUBBLES;
    if (cancelable) properties += EventProperties::CANCELABLE;
    if (composed) properties += EventProperties::COMPOSED;
}

Event::Event(bool bubbles, bool cancelable, bool composed, bool trusted, WeakPointer <EventTarget> &&originalTarget,
             feather::Vector<std::shared_ptr<feather::js::EventTarget>> realPath, feather::js::EventType type)
        : realPath(std::move(realPath)), current(this->realPath.begin()), originalTarget(originalTarget), name(""),
          type(type) {
    if (bubbles) properties += EventProperties::BUBBLES;
    if (cancelable) properties += EventProperties::CANCELABLE;
    if (composed) properties += EventProperties::COMPOSED;
    if (trusted) properties += EventProperties::TRUSTED;
}

feather::StrongPointer<EventTarget> Event::getEventTarget() {
    for (auto it = realPath.end() - 1; it != realPath.begin(); it--) {
        if ((*it)->isHiddenTarget()) return *(it + 1);
    }
    return realPath.front()->isHiddenTarget() ? *(realPath.begin() + 1) : realPath.front();
}

feather::Vector<feather::StrongPointer<feather::js::EventTarget>> Event::getComposedPath() const {
    Vector <StrongPointer<EventTarget>> v;
    for (auto it = realPath.end() - 1; it != realPath.begin() && !(*it)->isHiddenTarget(); v.push_back(*it--));
    if (!realPath.front()->isHiddenTarget()) v.push_back(realPath.front());
    return v;
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