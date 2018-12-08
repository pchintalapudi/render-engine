//
// Created by prem on 12/6/2018.
//

#ifndef FEATHER_HTML_ELEMENT_H
#define FEATHER_HTML_ELEMENT_H

#include "utils/data_set_view.h"
#include "../../element.h"
#include "../../document.h"

namespace dom {
    class HtmlElement;
}

class dom::HtmlElement : public Element {
public:
    DOMString getAccessKey() { return accessKey; }

    void setAccessKey(DOMString key) { accessKey = key; }

    //We don't support access keys yet
    DOMString getAccessKeyLabel() { return ""; }

    DOMString getContentEditable() { return contentEditable; }

    void setContentEditable(DOMString editable) { contentEditable = editable; }

    bool isContentEditable() {
        return contentEditable == "true"
               || (getParentElement() && getParentElement()->getElementType() == "html"
                   && static_cast<HtmlElement *>(getParentElement())->isContentEditable());
    }

    DataSetView &getDataSet() { return dataSetView; }

    const DataSetView &getDataSet() const { return dataSetView; }

    //TODO: Implement me
    DOMString getDir();

    //TODO: Implement me
    void setDir(DOMString dir);

    //TODO: Implement me
    bool getHidden();

    //TODO: Implement me
    void setHidden(bool hidden);

    //TODO: Implement me
    DOMString getInnerText();

    //TODO: Implement me
    DOMString getLang();


    inline long long getNonce() { return nonce; }

    inline void setNonce(long long nonce) { this->nonce = nonce; }

    inline double getOffsetLeft() { return offsets[0]; }

    inline double getOffsetTop() { return offsets[1]; }

    inline double getOffsetWidth() { return offsets[2]; }

    inline double getOffsetHeight() { return offsets[3]; }

    inline long long getTabIndex() { return tabIndex; }

    inline void setTabIndex(long long tabIndex) { this->tabIndex = tabIndex; }

    inline DOMString getTitle() { return title; }

    inline void setTitle(DOMString title) { this->title = title; }

    inline void blur() { getOwner()->getPseudoclassManager().defocus(this); }

    void click();

private:
    DOMString accessKey;
    DOMString contentEditable;
    DataSetView dataSetView;
    long long nonce;
    double offsets[4];
    long long tabIndex;
    DOMString title;
};

#endif //FEATHER_HTML_ELEMENT_H
