//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_DOCUMENT_OR_SHADOW_ROOT_H
#define FEATHER_DOCUMENT_OR_SHADOW_ROOT_H

#include "node.h"
#include "include/css/pseudoclass_manager.h"
#include "utils/stylesheet_list.h"

namespace dom {
    class Element;

    class DocumentOrShadowRoot;
}
class dom::DocumentOrShadowRoot : public Node {
public:
    inline Element *getActiveElement() { return pseudoclassManager.getFocused(); }

    inline Element *getFullscreen() { return fullscreen; }

    inline void setFullscreen(Element *element) { fullscreen = element; }

    inline Element *getPointerLock() { return pointerLock; }

    inline void setPointerLock(Element *element) { pointerLock = element; }

    inline css::StylesheetList &getStylesheets() { return stylesheetList; }

protected:
    inline css::PseudoclassManager &getPseudoclassManagerInternal() { return pseudoclassManager; }

private:
    css::PseudoclassManager pseudoclassManager;

    Element *fullscreen;

    Element *pointerLock;

    css::StylesheetList stylesheetList;
};

#endif //FEATHER_DOCUMENT_OR_SHADOW_ROOT_H
