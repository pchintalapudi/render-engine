//
// Created by prem on 1/8/2019.
//

#include "nodes/documents/document.h"
#include "nodes/elements/html_elements/html_element_includes.h"

using namespace feather::dom;

namespace {
    template<typename ElementType, const char *string>
    feather::Pair<bool, feather::StrongPointer<ElementType>> mapper(const feather::StrongPointer<Element> &ref) {
        return ref && ref->getTagName() == string
               ? std::make_pair(true, std::static_pointer_cast<ElementType>(ref))
               : std::make_pair(false, feather::StrongPointer<ElementType>());
    }

    template<typename ElementType, const char *string>
    using TagMappedList = feather::observable::SketchyObservableListWrapper
            <feather::StrongPointer<feather::dom::Element>, feather::dom::HTMLCollection,
                    feather::StrongPointer<ElementType>, mapper<ElementType, string>>;

    feather::Pair<bool, feather::StrongPointer<html::Hyperlink>>
    linkMapper(const feather::StrongPointer<Element> &ref) {
        return ref && (ref->getTagName() == "a" || ref->getTagName() == "area")
               ? std::make_pair(true, std::static_pointer_cast<html::Hyperlink>(ref->getThisRef()))
               : std::make_pair(false, feather::StrongPointer<html::Hyperlink>());
    }
}

namespace feather::dom::dlists {
    static const char obj[] = "obj", form[] = "form", image[] = "img", embed[] = "embed", script[] = "script";


    class ObjectMappedList : public TagMappedList<html::HTMLObjectElement, obj> {
        using SketchyObservableListWrapper::SketchyObservableListWrapper;
    };

    class FormMappedList : public TagMappedList<html::HTMLFormElement, form> {
        using SketchyObservableListWrapper::SketchyObservableListWrapper;
    };

    class ImageMappedList : public TagMappedList<html::HTMLImageElement, image> {
        using SketchyObservableListWrapper::SketchyObservableListWrapper;
    };

    class LinkMappedList : public observable::SketchyObservableListWrapper<StrongPointer<Element>, HTMLCollection,
            StrongPointer<html::Hyperlink>, linkMapper> {
        using SketchyObservableListWrapper::SketchyObservableListWrapper;
    };

    class EmbedMappedList : public TagMappedList<html::HTMLEmbedElement, embed> {
        using SketchyObservableListWrapper::SketchyObservableListWrapper;
    };

    class ScriptMappedList : public TagMappedList<html::HTMLScriptElement, script> {
        using SketchyObservableListWrapper::SketchyObservableListWrapper;
    };
}

feather::StrongPointer<feather::dom::html::HTMLHtmlElement> Document::getDocumentElement() const {
    StrongPointer <NodeList> children = getChildNodes();
    for (UInt i = children->size(); i-- > 0;) {
        auto child = children->get(i);
        if (child->getNodeTypeInternal() == NodeType::ELEMENT_NODE &&
            std::static_pointer_cast<Element>(child)->getTagName() == "html")
            return std::static_pointer_cast<html::HTMLHtmlElement>(child);
    }
    return StrongPointer<html::HTMLHtmlElement>();
}

feather::StrongPointer<feather::dom::html::HTMLHeadElement> Document::getHead() {
    auto dElement = getDocumentElement();
    if (dElement) {
        auto children = dElement->getChildren();
        for (UInt i = 0; i < children->size(); i++) {
            auto child = children->get(i);
            if (child->getTagName() == "head") return std::static_pointer_cast<html::HTMLHeadElement>(child);
        }
    }
    return StrongPointer<html::HTMLHeadElement>();
}

feather::StrongPointer<feather::dom::html::HTMLBodyElement> Document::getBody() const {
    auto dElement = getDocumentElement();
    if (dElement) {
        auto children = dElement->getChildren();
        for (UInt i = children->size(); i-- > 0;) {
            auto child = children->get(i);
            if (child->getTagName() == "body") return std::static_pointer_cast<html::HTMLBodyElement>(child);
        }
    }
    return StrongPointer<html::HTMLBodyElement>();
}

#define SUMMON_LIST(LIST_TYPE, REF) (REF) ? (REF) : ((REF) = std::make_shared<LIST_TYPE>(getDocumentElement()->getChildren()))

feather::StrongPointer<feather::dom::dlists::ObjectMappedList> Document::getEmbeds() const {
    return SUMMON_LIST(dlists::ObjectMappedList, objects);
}

feather::StrongPointer<feather::dom::dlists::FormMappedList> Document::getForms() const {
    return SUMMON_LIST(dlists::FormMappedList, forms);
}

feather::StrongPointer<feather::dom::dlists::ImageMappedList> Document::getImages() const {
    return SUMMON_LIST(dlists::ImageMappedList, images);
}

feather::StrongPointer<feather::dom::dlists::EmbedMappedList> Document::getPlugins() const {
    return SUMMON_LIST(dlists::EmbedMappedList, embeds);
}

feather::StrongPointer<feather::dom::dlists::LinkMappedList> Document::getLinks() const {
    return SUMMON_LIST(dlists::LinkMappedList, links);
}

feather::StrongPointer<feather::dom::dlists::ScriptMappedList> Document::getScripts() const {
    return SUMMON_LIST(dlists::ScriptMappedList, scripts);
}

feather::StrongPointer<feather::dom::html::HTMLElement> Document::getScrollingElement() const {
    return getDocumentElement();
}