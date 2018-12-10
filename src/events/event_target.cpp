////
//// Created by prem on 11/9/2018.
////
//
//#include "include/events/event_target.h"
//
//using namespace js::event_types;
//
//namespace {
//
//    struct DOMEventSetBuilder {
//        DOMEventSet set;
//
//        DOMEventSetBuilder &add(DOMEvent event) {
//            set.add(event);
//            return *this;
//        }
//    };
//
//    const std::map<DOMString, const DOMEventSet> standardTypeMap = {
//            {"animationevent",          DOMEventSetBuilder().add(DOMEvent::ANIMATION).set},
//            {"beforeinputevent",        DOMEventSetBuilder().add(DOMEvent::BEFORE_INPUT).set},
//            {"beforeunloadevent",       DOMEventSetBuilder().add(DOMEvent::BEFORE_UNLOAD).set},
//            {"blobevent",               DOMEventSetBuilder().add(DOMEvent::BLOB).set},
//            {"clipboardevent",          DOMEventSetBuilder().add(DOMEvent::CLIPBOARD).set},
//            {"closeevent",              DOMEventSetBuilder().add(DOMEvent::CLOSE).set},
//            {"compositionevent",        DOMEventSetBuilder().add(DOMEvent::COMPOSITION).set},
//            {"cssfontfaceloadevent",    DOMEventSetBuilder().add(DOMEvent::CSS_FONT_FACE_LOAD).set},
//            {"customevent",             DOMEventSetBuilder().add(DOMEvent::CUSTOM_EVENT).set},
//            {"devicelightevent",        DOMEventSetBuilder().add(DOMEvent::DEVICE_LIGHT).set},
//            {"devicemotionevent",       DOMEventSetBuilder().add(DOMEvent::DEVICE_MOTION).set},
//            {"deviceorientationevent",  DOMEventSetBuilder().add(DOMEvent::DEVICE_MOTION).set},
//            {"deviceproximityevent",    DOMEventSetBuilder().add(DOMEvent::DEVICE_ORIENTATION).set},
//            {"domtransactionevent",     DOMEventSetBuilder().add(DOMEvent::DOM_TRANSACTION).set},
//            {"dragevent",               DOMEventSetBuilder().add(DOMEvent::DRAG).add(DOMEvent::MOUSE).set},
//            {"editingbeforeinputevent", DOMEventSetBuilder().add(DOMEvent::EDITING_BEFORE_INPUT).set},
//            {"errorevent",              DOMEventSetBuilder().add(DOMEvent::ERROR)},
//            {"fetchevent",              DOMEventSetBuilder().add(DOMEvent::FETCH)},
//            {"focusevent",              DOMEventSetBuilder().add(DOMEvent::FOCUS).add(DOMEvent::UI)},
//            {"gamepadevent",            DOMEventSetBuilder().add(DOMEvent::GAMEPAD)},
//            {"hashchangeevent",         DOMEventSetBuilder().add(DOMEvent::HASH_CHANGE)},
//            {"idbversionchangeevent",   DOMEventSetBuilder().add(DOMEvent::IDB_VERSION_CHANGE)},
//            {"inputevent",              DOMEventSetBuilder().add(DOMEvent::INPUT)}
//    };
//}
//
//void js::EventTarget::dispatchEvent(Event &event) const {
//    unsigned long idx = -1;
//    while (event.getDeepPath()[++idx] != this);
//    bool atEnd = idx + 1 == event.getDeepPath().size();
//    if (handlerMap) {
//        if (atEnd) event.setEventPhase(EventPhase::AT_TARGET);
//        handlerMap->fireAll(event);
//    }
//    if (atEnd && event.getBubbles()) event.setEventPhase(EventPhase::BUBBLING_PHASE);
//    if (event.getBubbles() && !event.isConsumed() && event.getEventPhase() == EventPhase::BUBBLING_PHASE && idx > 0)
//        event.getDeepPath()[idx - 1]->dispatchEvent(event);
//    else if (idx < event.getDeepPath().size() - 1 && !event.isConsumed() &&
//             event.getEventPhase() == EventPhase::CAPTURING_PHASE)
//        event.getDeepPath()[idx + 1]->dispatchEvent(event);
//}
//
//void js::EventTarget::addEventHandler(DOMString type, std::function<void(Event &)> *handler) {
//
//}