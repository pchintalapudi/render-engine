//
// Created by prem on 12/6/2018.
//

#ifndef FEATHER_STYLESHEET_LIST_H
#define FEATHER_STYLESHEET_LIST_H

#include "observable/smart_observable_list.h"
#include "css/css_style_sheet.h"

namespace css {
    class StylesheetList;
}

class css::StylesheetList : public observable::SmartObservableList<CSSStyleSheet> {
};

#endif //FEATHER_STYLESHEET_LIST_H
