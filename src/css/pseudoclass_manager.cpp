//
// Created by prem on 12/15/2018.
//

#include "style/css/pseudoclass_manager.h"
#include "nodes/elements/element.h"

using namespace feather::css;

namespace {
    bool isALink(const feather::DOMString &tag, const feather::DOMString &href) {
        return (tag == "a" || tag == "area" || tag == "link") && !href.empty();
    }
}

bool PseudoclassManager::isAnyLink(const feather::StrongPointer<const feather::dom::Element> &e) {
    return isALink(e->getTagName(), e->getAttributeSafe("href"));
}

//TODO: Implement me
bool PseudoclassManager::isBlank(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isChecked(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isDefault(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isDefined(const feather::StrongPointer<const feather::dom::Element> &e) {
    return true;
}

//TODO: Implement me
bool PseudoclassManager::isDir(const feather::StrongPointer<const feather::dom::Element> &, bool ltr) {
    //Lmao we don't support the other way yet so this is likely to yield good results
    return ltr;
}

//TODO: Implement me
bool PseudoclassManager::isDisabled(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isEmpty(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isEnabled(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

bool PseudoclassManager::isFirstChild(const feather::StrongPointer<const feather::dom::Element> &e) {
    return e && !(e->getElementIndex());
}

bool PseudoclassManager::isFirstOfType(const feather::StrongPointer<const feather::dom::Element> &e,
                                       const feather::DOMString &type) {
    return e && (type.empty() || e->getTagName() == type) && !e->getTypedElementIndex();
}

//TODO: Implement me
bool PseudoclassManager::isFullscreen(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

bool PseudoclassManager::isHost(const feather::StrongPointer<const feather::dom::Element> &e) {
    return e && e->getNodeTypeInternal() == dom::NodeType::SHADOW_ROOT;
}

//TODO: Implement me
bool PseudoclassManager::isIndeterminate(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isInRange(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isInvalid(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isLang(const feather::StrongPointer<const feather::dom::Element> &,
                                const feather::DOMString &lang) {
    //Heuristic lmao
    return lang == "en-US";
}

bool PseudoclassManager::isLastChild(const feather::StrongPointer<const feather::dom::Element> &e) {
    return e && !e->getLastElementIndex();
}

bool PseudoclassManager::isLastOfType(const feather::StrongPointer<const feather::dom::Element> &e,
                                      const feather::DOMString &type) {
    return e && (type.empty() || e->getTagName() == type) && !e->getLastTypedElementIndex();
}

//TODO: Track visited links
bool PseudoclassManager::isLink(const feather::StrongPointer<const feather::dom::Element> &e) {
    return isAnyLink(e) && true;
}

bool PseudoclassManager::isNthChild(const feather::StrongPointer<const feather::dom::Element> &e, feather::Long a,
                                    feather::Long b) {
    if (!e) return false;
    if (!e->getParentNode()) return !b;
    //We're trying to invert An + b = idx into n = (idx - b) / A,
    //where n is some arbitrary integer
    auto dif = Long(e->getElementIndex()) - b;
    return a ? !(Long((ULong) dif ^ (ULong) a) < 0 || dif % a) : !dif;
}

bool
PseudoclassManager::isNthOfType(const feather::StrongPointer<const feather::dom::Element> &e, feather::Long a,
                                feather::Long b,
                                feather::DOMString type) {
    if (!(e && (type.empty() || e->getTagName() == type))) return false;
    if (!e->getParentNode()) return !b;
    //We're trying to invert An + b = idx into n = (idx - b) / A,
    //where n is some arbitrary integer
    auto dif = Long(e->getTypedElementIndex()) - b;
    return a ? !(Long((ULong) dif ^ (ULong) a) < 0 || dif % a) : !dif;
}

bool PseudoclassManager::isNthLastChild(const feather::StrongPointer<const feather::dom::Element> &e,
                                        feather::Long a, feather::Long b) {
    if (!e) return false;
    if (!e->getParentNode()) return !b;
    //We're trying to invert An + b = idx into n = (idx - b) / A,
    //where n is some arbitrary integer

    auto dif = Long(e->getLastElementIndex()) - b;
    return a ? !(Long((ULong) dif ^ (ULong) a) < 0 || dif % a) : !dif;
}

bool PseudoclassManager::isNthLastOfType(const feather::StrongPointer<const feather::dom::Element> &e, feather::Long a,
                                         feather::Long b, feather::DOMString type) {
    if (!(e && (type.empty() || e->getTagName() == type))) return false;
    if (!e->getParentNode()) return !b;
    //We're trying to invert An + b = idx into n = (idx - b) / A,
    //where n is some arbitrary integer
    auto dif = Long(e->getLastTypedElementIndex()) - b;
    return a ? !(Long((ULong) dif ^ (ULong) a) < 0 || dif % a) : !dif;
}

bool PseudoclassManager::isOnlyChild(const feather::StrongPointer<const feather::dom::Element> &e) {
    return e && !e->getElementIndex() && !e->getLastElementIndex();
}

bool PseudoclassManager::isOnlyOfType(const feather::StrongPointer<const feather::dom::Element> &e,
                                      const feather::DOMString &type) {
    return e && e->getTagName() == type && !e->getTypedElementIndex() && !e->getLastTypedElementIndex();
}

//TODO: Implement me
bool PseudoclassManager::isOptional(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isOutOfRange(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isPlaceholderShown(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isReadOnly(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isReadWrite(const feather::StrongPointer<const feather::dom::Element> &e) {
    return true;
}

//TODO: Implement me
bool PseudoclassManager::isRequired(const feather::StrongPointer<const feather::dom::Element> &e) {
    return true;
}

bool PseudoclassManager::isRoot(const feather::StrongPointer<const feather::dom::Element> &e) {
    if (!e) return false;
    auto parent = e->getParentNode();
    return !parent || parent->getNodeTypeInternal() == dom::NodeType::DOCUMENT_NODE;
}

//TODO: Implement me
bool PseudoclassManager::isTarget(const feather::StrongPointer<const feather::dom::Element> &e) {
    return false;
}

//TODO: Implement me
bool PseudoclassManager::isValid(const feather::StrongPointer<const feather::dom::Element> &e) {
    return true;
}

//TODO: Track visited links
bool PseudoclassManager::isVisited(const feather::StrongPointer<const feather::dom::Element> &e) {
    return isAnyLink(std::move(e)) && false;
}