//
// Created by prem on 12/15/2018.
//

#include "css/pseudoclass_manager.h"
#include "nodes/elements/element.h"

using namespace feather::css;

namespace {
    bool isALink(feather::DOMString tag, feather::DOMString href) {
        return (tag == "a" || tag == "area" || tag == "link") && !href.empty();
    }
}

bool PseudoclassManager::isAnyLink(feather::StrongPointer<feather::dom::Element> e) {
    return isALink(e->getTagName(), e->getAttributeSafe("href"));
}

//TODO: Implement me
bool PseudoclassManager::isBlank(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isChecked(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isDefault(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isDefined(feather::StrongPointer<feather::dom::Element>) {
    return true;
}

//TODO: Implement me
bool PseudoclassManager::isDir(feather::StrongPointer<feather::dom::Element>, bool ltr) {
    //Lmao we don't support the other way yet so this is likely to yield good results
    return ltr;
}

//TODO: Implement me
bool PseudoclassManager::isDisabled(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isEmpty(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isEnabled(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

bool PseudoclassManager::isFirstChild(feather::StrongPointer<feather::dom::Element> e) {
    if (!e) return false;
    if (!e->getParentNode()) return true;
    auto children = e->getParentNode()->getChildNodes();
    for (UInt i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child && child->getNodeTypeInternal() == dom::NodeType::ELEMENT_NODE) {
            return child == e;
        }
    }
    return false;
}

bool PseudoclassManager::isFirstOfType(feather::StrongPointer<feather::dom::Element> e, feather::DOMString type) {
    if (!e) return false;
    if (!e->getParentNode()) return true;
    auto children = e->getParentNode()->getChildNodes();
    for (UInt i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child && child->getNodeTypeInternal() == dom::NodeType::ELEMENT_NODE &&
            (type.empty() || child->getNodeName() == type))
            return child == e;
    }
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isFullscreen(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

bool PseudoclassManager::isHost(feather::StrongPointer<feather::dom::Element> e) {
    return e && e->getNodeTypeInternal() == dom::NodeType::SHADOW_ROOT;
}

//TODO: Implement me
bool PseudoclassManager::isIndeterminate(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isInRange(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isInvalid(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isLang(feather::StrongPointer<feather::dom::Element>, feather::DOMString lang) {
    //Heuristic lmao
    return lang == "en-US";
}

bool PseudoclassManager::isLastChild(feather::StrongPointer<feather::dom::Element> e) {
    if (!e) return false;
    if (!e->getParentNode()) return true;
    auto children = e->getParentNode()->getChildNodes();
    for (UInt i = children.size(); i-- > 0;) {
        auto child = children.get(i);
        if (child && child->getNodeTypeInternal() == dom::NodeType::ELEMENT_NODE) return e == child;
    }
    return false;
}

bool PseudoclassManager::isLastOfType(feather::StrongPointer<feather::dom::Element> e, feather::DOMString type) {
    if (!e) return false;
    if (!e->getParentNode()) return true;
    auto children = e->getParentNode()->getChildNodes();
    for (UInt i = children.size(); i-- > 0;) {
        auto child = children.get(i);
        if (child && child->getNodeTypeInternal() == dom::NodeType::ELEMENT_NODE &&
            (type.empty() || type == e->getNodeName()))
            return e == child;
    }
    return false;
}

//TODO: Track visited links
bool PseudoclassManager::isLink(feather::StrongPointer<feather::dom::Element> e) {
    return isAnyLink(e) && true;
}

bool PseudoclassManager::isNthChild(feather::StrongPointer<feather::dom::Element> e, feather::Long a,
                                    feather::Long b) {
    if (!e) return false;
    if (!e->getParentNode()) return !b;
    auto children = e->getParentNode()->getChildNodes();
    UInt idx = 0;
    for (UInt i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child == e) break;
        if (child->getNodeTypeInternal() == dom::NodeType::ELEMENT_NODE) idx++;
    }
    //We're trying to invert An + b = idx into n = (idx - b) / A,
    //where n is some arbitrary integer
    auto dif = static_cast<Long>(idx) - b;
    return a != 0 ? static_cast<Long>(static_cast<ULong>(dif) ^ static_cast<ULong>(a)) >= 0 && !(dif % a) : !dif;
}

bool PseudoclassManager::isNthOfType(feather::StrongPointer<feather::dom::Element> e, feather::Long a, feather::Long b,
                                     feather::DOMString type) {
    if (!e) return false;
    if (!e->getParentNode()) return !b;
    auto children = e->getParentNode()->getChildNodes();
    UInt idx = 0;
    for (UInt i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child == e) break;
        if (child->getNodeTypeInternal() == dom::NodeType::ELEMENT_NODE &&
            (type.empty() || child->getNodeName() == type))
            idx++;
    }
    //We're trying to invert An + b = idx into n = (idx - b) / A,
    //where n is some arbitrary integer
    auto dif = static_cast<Long>(idx) - b;
    return a != 0 ? static_cast<Long>(static_cast<ULong>(dif) ^ static_cast<ULong>(a)) >= 0 && !(dif % a) : !dif;
}

bool PseudoclassManager::isNthLastChild(feather::StrongPointer<feather::dom::Element> e, feather::Long a,
                                        feather::Long b) {
    if (!e) return false;
    if (!e->getParentNode()) return !b;
    auto children = e->getParentNode()->getChildNodes();
    UInt idx = 0;
    for (UInt i = children.size(); i-- > 0;) {
        auto child = children.get(i);
        if (child == e) break;
        if (child->getNodeTypeInternal() == dom::NodeType::ELEMENT_NODE) idx++;
    }
    //We're trying to invert An + b = idx into n = (idx - b) / A,
    //where n is some arbitrary integer
    auto dif = static_cast<Long>(idx) - b;
    return a != 0 ? static_cast<Long>(static_cast<ULong>(dif) ^ static_cast<ULong>(a)) >= 0 && !(dif % a) : !dif;
}

bool PseudoclassManager::isNthLastOfType(feather::StrongPointer<feather::dom::Element> e, feather::Long a,
                                         feather::Long b, feather::DOMString type) {
    if (!e) return false;
    if (!e->getParentNode()) return !b;
    auto children = e->getParentNode()->getChildNodes();
    UInt idx = 0;
    for (UInt i = children.size(); i-- > 0;) {
        auto child = children.get(i);
        if (child == e) break;
        if (child->getNodeTypeInternal() == dom::NodeType::ELEMENT_NODE &&
            (type.empty() || child->getNodeName() == type))
            idx++;
    }
    //We're trying to invert An + b = idx into n = (idx - b) / A,
    //where n is some arbitrary integer
    auto dif = static_cast<Long>(idx) - b;
    return a != 0 ? static_cast<Long>(static_cast<ULong>(dif) ^ static_cast<ULong>(a)) >= 0 && !(dif % a) : !dif;
}

bool PseudoclassManager::isOnlyChild(feather::StrongPointer<feather::dom::Element> e) {
    if (!e) return false;
    if (!e->getParentNode()) return true;
    auto children = e->getParentNode()->getChildNodes();
    for (UInt i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child->getNodeTypeInternal() == dom::NodeType::ELEMENT_NODE && child != e) return false;
    }
    return true;
}

bool PseudoclassManager::isOnlyOfType(feather::StrongPointer<feather::dom::Element> e, feather::DOMString type) {
    if (!e) return false;
    if (!e->getParentNode()) return true;
    auto children = e->getParentNode()->getChildNodes();
    for (UInt i = 0; i < children.size(); i++) {
        auto child = children.get(i);
        if (child->getNodeTypeInternal() == dom::NodeType::ELEMENT_NODE &&
            (type.empty() || child->getNodeName() == type) && child != e)
            return false;
    }
    return true;
}

//TODO: Implement me
bool PseudoclassManager::isOptional(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isOutOfRange(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isPlaceholderShown(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isReadOnly(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isReadWrite(feather::StrongPointer<feather::dom::Element>) {
    return true;
}

//TODO: Implement me
bool PseudoclassManager::isRequired(feather::StrongPointer<feather::dom::Element>) {
    return true;
}

bool PseudoclassManager::isRoot(feather::StrongPointer<feather::dom::Element> e) {
    if (!e) return false;
    auto parent = e->getParentNode();
    return !parent || parent->getNodeTypeInternal() == dom::NodeType::DOCUMENT_NODE;
}

//TODO: Implement me
bool PseudoclassManager::isTarget(feather::StrongPointer<feather::dom::Element>) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isValid(feather::StrongPointer<feather::dom::Element>) {
    return true;
}

//TODO: Track visited links
bool PseudoclassManager::isVisited(feather::StrongPointer<feather::dom::Element> e) {
    return isAnyLink(e) && false;
}