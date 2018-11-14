//
// Created by prem on 11/14/2018.
//

#ifndef FEATHER_SHADOW_ROOT_H
#define FEATHER_SHADOW_ROOT_H

#include "element.h"

class ShadowRoot : Element {
private:
    bool open;
    Element *host;

};
#endif //FEATHER_SHADOW_ROOT_H
