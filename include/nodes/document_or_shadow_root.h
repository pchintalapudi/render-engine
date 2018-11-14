//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_DOCUMENT_OR_SHADOW_ROOT_H
#define FEATHER_DOCUMENT_OR_SHADOW_ROOT_H

class Element;
class DocumentOrShadowRoot {
private:
    Element *active;
    Element *fullScreen;
    Element *pointerLock;

};
#endif //FEATHER_DOCUMENT_OR_SHADOW_ROOT_H
