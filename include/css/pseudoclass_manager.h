//
// Created by prem on 12/14/2018.
//

#ifndef FEATHER_PSEUDOCLASS_MANAGER_H
#define FEATHER_PSEUDOCLASS_MANAGER_H

#include "observable/invalidatable.h"

namespace feather {
    namespace dom {
        class Node;

        class Element;

        class ShadowRoot;
    }
    namespace css {

        class PseudoclassManager : observable::Invalidatable {
        public:
            inline void setActive(Vector<feather::StrongPointer<const dom::Element>> active) {
                moused = active;
                this->active = true;
                invalidate();
            }

            inline bool isActive(StrongPointer<const dom::Element> e) {
                //Have reason to believe most people look for the active element
                //closest to the end, rather than the beginning
                return moused.size() && active && std::find(moused.rbegin(), moused.rend(), e) != moused.rend();
            }

            inline void clearActive() { moused.clear(); }

            inline void setHover(Vector<feather::StrongPointer<const dom::Element>> hover) {
                moused = hover;
                active = false;
                invalidate();
            }

            inline bool isHover(StrongPointer<const dom::Element> e) {
                //See reasoning for isActive above
                return moused.size() && !active && std::find(moused.rbegin(), moused.rend(), e) != moused.rend();
            }

            inline void clearHover() { moused.clear(); }

            inline void setFocused(Vector<feather::StrongPointer<const dom::Element>> focused) {
                this->focused = focused;
                invalidate();
            }

            inline bool isFocused(StrongPointer<const dom::Element> e) { return focused.size() && focused.back() == e; }

            inline bool isFocusVisible(StrongPointer<const dom::Element> e) { return isFocused(e) && false; }

            inline bool containsFocus(StrongPointer<const dom::Element> e) {
                return focused.size() && std::find(focused.rbegin(), focused.rend(), e) != focused.rend();
            }

            inline void clearFocused() { focused.clear(); }

            //document-stateless pseudoclasses
            static bool isAnyLink(StrongPointer<const dom::Element> e);

            static bool isBlank(StrongPointer<const dom::Element> e);

            static bool isChecked(StrongPointer<const dom::Element> e);

            static bool isDefault(StrongPointer<const dom::Element> e);

            static bool isDefined(StrongPointer<const dom::Element> e);

            static bool isDir(StrongPointer<const dom::Element> e, bool ltr);

            static bool isDisabled(StrongPointer<const dom::Element> e);

            static bool isEmpty(StrongPointer<const dom::Element> e);

            static bool isEnabled(StrongPointer<const dom::Element> e);

            static bool isFirstChild(StrongPointer<const dom::Element> e);

            static bool isFirstOfType(StrongPointer<const dom::Element> e, DOMString type);

            static bool isFullscreen(StrongPointer<const dom::Element> e);

            static bool isHost(StrongPointer<const dom::Element> e);

            static bool isIndeterminate(StrongPointer<const dom::Element> e);

            static bool isInRange(StrongPointer<const dom::Element> e);

            static bool isInvalid(StrongPointer<const dom::Element> e);

            static bool isLang(StrongPointer<const dom::Element> e, DOMString lang);

            static bool isLastChild(StrongPointer<const dom::Element> e);

            static bool isLastOfType(StrongPointer<const dom::Element> e, DOMString type);

            static bool isLink(StrongPointer<const dom::Element> e);

            static bool isNthChild(StrongPointer<const dom::Element> e, Long a, Long b);

            static bool isNthLastChild(StrongPointer<const dom::Element> e, Long a, Long b);

            static bool isNthOfType(StrongPointer<const dom::Element> e, Long a, Long b, DOMString type);

            static bool isNthLastOfType(StrongPointer<const dom::Element> e, Long a, Long b, DOMString type);

            static bool isOnlyChild(StrongPointer<const dom::Element> e);

            static bool isOnlyOfType(StrongPointer<const dom::Element> e, DOMString type);

            static bool isOptional(StrongPointer<const dom::Element> e);

            static bool isOutOfRange(StrongPointer<const dom::Element> e);

            static bool isPlaceholderShown(StrongPointer<const dom::Element> e);

            static bool isReadOnly(StrongPointer<const dom::Element> e);

            static bool isReadWrite(StrongPointer<const dom::Element> e);

            static bool isRequired(StrongPointer<const dom::Element> e);

            static bool isRoot(StrongPointer<const dom::Element> e);

            inline static bool isScope(StrongPointer<const dom::Element> e, StrongPointer<const dom::Element> scope) {
                return e == scope;
            }

            static bool isTarget(StrongPointer<const dom::Element> e);

            static bool isValid(StrongPointer<const dom::Element> e);

            static bool isVisited(StrongPointer<const dom::Element> e);

        private:
            Vector<feather::StrongPointer<const dom::Element>> moused = Vector<StrongPointer<const dom::Element>>();
            bool active = false;
            Vector<feather::StrongPointer<const dom::Element>> focused = Vector<StrongPointer<const dom::Element>>();

            inline void invalidate() const {
                observable::Invalidatable::invalidate(
                        RegularEnumSet<observable::InvEvent>(
                                1u << static_cast<int>(observable::InvEvent::PSEUDOCLASS_CHANGE)
                                | 1u << static_cast<int>(observable::InvEvent::RESTYLE)
                        ), this
                );
            }
        };
    }
}
#endif //FEATHER_PSEUDOCLASS_MANAGER_H
