//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_ELEMENT_H
#define FEATHER_ELEMENT_H

#include "../node.h"
#include "../slotable.h"
#include "../utils/dom_token_list.h"
#include "../utils/named_node_map.h"
#include "../utils/html_collection.h"
#include "selectors/css_selector.h"
#include "geom/dom_rect.h"
#include "style/css/rules/css_style_declaration.h"

namespace feather {
    namespace dom {

        class ShadowRoot;

        enum class Dimensions {
            HEIGHT, LEFT, TOP, WIDTH
        };

        namespace elists {
            class FilteredByTagName;

            class FilteredByTagNameNS;

            class FilteredByClassName;
        }

        enum class KnownElements {
            HTMLAnchorElement,
            HTMLAreaElement,
            HTMLAudioElement,
            HTMLBRElement,
            HTMLBaseElement,
            HTMLBaseFontElement,
            HTMLBodyElement,
            HTMLButtonElement,
            HTMLCanvasElement,
            HTMLContentElement,
            HTMLCustomElement,
            HTMLDListElement,
            HTMLDataElement,
            HTMLDataListElement,
            HTMLDialogElement,
            HTMLDivElement,
            HTMLDocument,
            HTMLEmbedElement,
            HTMLFieldSetElement,
            HTMLFormElement,
            HTMLFrameSetElement,
            HTMLHRElement,
            HTMLHeadElement,
            HTMLH1Element,
            HTMLH2Element,
            HTMLH3Element,
            HTMLH4Element,
            HTMLH5Element,
            HTMLH6Element,
            HTMLHtmlElement,
            HTMLIFrameElement,
            HTMLImageElement,
            HTMLInputElement,
            HTMLIsIndexElement,
            HTMLKeygenElement,
            HTMLLIElement,
            HTMLLabelElement,
            HTMLLegendElement,
            HTMLLinkElement,
            HTMLMapElement,
            HTMLMediaElement,
            HTMLMetaElement,
            HTMLMeterElement,
            HTMLModElement,
            HTMLOListElement,
            HTMLObjectElement,
            HTMLOptGroupElement,
            HTMLOptionElement,
            HTMLOutputElement,
            HTMLParagraphElement,
            HTMLParamElement,
            HTMLPictureElement,
            HTMLPreElement,
            HTMLProgressElement,
            HTMLQuoteElement,
            HTMLScriptElement,
            HTMLSelectElement,
            HTMLShadowElement,
            HTMLSourceElement,
            HTMLSpanElement,
            HTMLStyleElement,
            HTMLTableCaptionElement,
            HTMLTableCellElement,
            HTMLTableColElement,
            HTMLTableElement,
            HTMLTableHeaderCellElement,
            HTMLTableRowElement,
            HTMLTableHeaderSectionElement,
            HTMLTableBodySectionElement,
            HTMLTableFooterSectionElement,
            HTMLTemplateElement,
            HTMLTextAreaElement,
            HTMLTimeElement,
            HTMLTitleElement,
            HTMLTrackElement,
            HTMLUListElement,
            HTMLVideoElement,
            SVGAElement,
            SVGAltGlyphElement,
            SVGAnimateColorElement,
            SVGAnimateElement,
            SVGAnimateMotionElement,
            SVGAnimateTransformElement,
            SVGAnimationElement,
            SVGCircleElement,
            SVGClipPathElement,
            SVGComponentTransferFunctionElement,
            SVGCursorElement,
            SVGDefsElement,
            SVGDescElement,
            SVGDocument,
            SVGEllipseElement,
            SVGFEBlendElement,
            SVGFEColorMatrixElement,
            SVGFEComponentTransferElement,
            SVGFECompositeElement,
            SVGFEConvolveMatrixElement,
            SVGFEDiffuseLightingElement,
            SVGFEDisplacementMapElement,
            SVGFEDistantLightElement,
            SVGFEDropShadowElement,
            SVGFEFloodElement,
            SVGFEFuncAElement,
            SVGFEFuncBElement,
            SVGFEFuncGElement,
            SVGFEFuncRElement,
            SVGFEGaussianBlurElement,
            SVGFEImageElement,
            SVGFEMergeElement,
            SVGFEMergeNodeElement,
            SVGFEMorphologyElement,
            SVGFEOffsetElement,
            SVGFEPointLightElement,
            SVGFESpecularLightingElement,
            SVGFESpotLightElement,
            SVGFETileElement,
            SVGFETurbulenceElement,
            SVGFilterElement,
            SVGFontElement,
            SVGFontFaceElement,
            SVGFontFaceFormatElement,
            SVGFontFaceNameElement,
            SVGFontFaceSrcElement,
            SVGFontFaceUriElement,
            SVGForeignObjectElement,
            SVGGElement,
            SVGGlyphElement,
            SVGGradientElement,
            SVGGraphicsElement,
            SVGHKernElement,
            SVGImageElement,
            SVGLineElement,
            SVGLinearGradientElement,
            SVGMPathElement,
            SVGMarkerElement,
            SVGMaskElement,
            SVGMetadataElement,
            SVGMissingGlyphElement,
            SVGPathElement,
            SVGPatternElement,
            SVGPolygonElement,
            SVGPolylineElement,
            SVGRadialGradientElement,
            SVGRectElement,
            SVGSVGElement,
            SVGScriptElement,
            SVGSetElement,
            SVGStopElement,
            SVGStyleElement,
            SVGSwitchElement,
            SVGSymbolElement,
            SVGTRefElement,
            SVGTSpanElement,
            SVGTextContentElement,
            SVGTextElement,
            SVGTextPathElement,
            SVGTextPositioningElement,
            SVGTitleElement,
            SVGUseElement,
            SVGVKernElement,
            SVGViewElement
        };

        DOMString encodeType(const DOMString &input);

        KnownElements getType(const DOMString &encoded);

        class Element : public Node, public Slotable {
        public:

            Element(DOMString baseURI, DOMString tagName, const StrongPointer<Node> &parent, KnownElements type);

            inline StrongPointer<const NamedNodeMap> getAttributes() const {
                return StrongPointer<const NamedNodeMap>(shared_from_this(), &attributes);
            }

            inline StrongPointer<NamedNodeMap> getAttributes() {
                return StrongPointer<NamedNodeMap>(shared_from_this(), &attributes);
            }

            inline StrongPointer<const DOMTokenList> getClassList() const {
                return StrongPointer<const DOMTokenList>(shared_from_this(), &classList);
            }

            inline StrongPointer<DOMTokenList> getClassList() {
                return StrongPointer<DOMTokenList>(shared_from_this(), &classList);
            }

            inline StrongPointer<const css::CSSStyleDeclaration> getStyle() const {
                return StrongPointer<const css::CSSStyleDeclaration>(shared_from_this(), &style);
            }

            inline StrongPointer<css::CSSStyleDeclaration> getStyle() {
                return StrongPointer<css::CSSStyleDeclaration>(shared_from_this(), &style);
            }

            inline const DOMString &getClassName() const { return classList.getValue(); }

            inline void setClassName(DOMString name) { classList.setValue(std::move(name)); }

            inline double getClientHeight() const { return clientDim[static_cast<int>(Dimensions::HEIGHT)]; }

            inline double getClientLeft() const { return clientDim[static_cast<int>(Dimensions::LEFT)]; }

            inline double getClientTop() const { return clientDim[static_cast<int>(Dimensions::TOP)]; }

            inline double getClientWidth() const { return clientDim[static_cast<int>(Dimensions::WIDTH)]; }

            inline const DOMString &getId() const { return getAttributeSafe("id"); }

            inline void setId(DOMString id) { setAttribute("id", std::move(id)); }

            inline const DOMString &getInnerHtml() const {
                return innerHTML.isValid() ? *innerHTML : constructInnerHTML();
            }

            //TODO: Implement me
            void setInnerHtml(DOMString innerHtml);

            inline const DOMString &getLocalName() const { return localName; }

            inline const DOMString &getNamespaceURI() const { return ns; }

            inline StrongPointer<Element> getNextElementSibling() const;

            inline const DOMString &getOuterHtml() const {
                return outerHTML.isValid() ? *outerHTML : constructOuterHTML();
            }

            //TODO: Implement me
            void setOuterHtml(DOMString html);

            inline const DOMString &getPrefix() const { return prefix; }

            StrongPointer<Element> getPreviousElementSibling() const;

            inline double getScrollHeight() const { return scrollDim[static_cast<int>(Dimensions::HEIGHT)]; }

            inline double getScrollLeft() const { return scrollDim[static_cast<int>(Dimensions::LEFT)]; }

            inline double getScrollTop() const { return scrollDim[static_cast<int>(Dimensions::TOP)]; }

            inline double getScrollWidth() const { return scrollDim[static_cast<int>(Dimensions::WIDTH)]; }

            inline StrongPointer<ShadowRoot> getShadowRoot() { return shadowRoot; }

            inline DOMString getSlot() const { return getAttributeSafe("slot"); }

            inline DOMString getTagName() const { return getNodeName(); }

            inline StrongPointer<Function<void(js::Event &)>> getOnFullScreenChange() const {
                return getEndHandler("onfullscreenchange");
            }

            inline void setOnFullScreenChange(StrongPointer<Function<void(js::Event &)>> func) {
                this->registerEndHandler("onfullscreenchange", std::move(func));
            }

            inline StrongPointer<Function<void(js::Event &)>> getOnFullScreenError() const {
                return getEndHandler("onfullscreenerror");
            }

            inline void setOnFullScreenError(StrongPointer<Function<void(js::Event &)>> func) {
                this->registerEndHandler("onfullscreenerror", std::move(func));
            }

            //TODO: Implement me
            StrongPointer<ShadowRoot> attachShadow(bool open);

            StrongPointer<Element> getClosest(const DOMString &selector) const;

            StrongPointer<DOMString> getAttribute(const DOMString &name) const;

            inline const DOMString &getAttributeSafe(const DOMString &name) const {
                auto ptr = getAttribute(name);
                return ptr ? *ptr : getEmptyString();
            }

            inline Vector<DOMString> getAttributeNames() const { return attributes.getKeys(); }

            StrongPointer<DOMString> getAttributeNS(DOMString ns, DOMString name) const;

            DOMRect getBoundingClientRect() const;

            Vector<DOMRect> getClientRects() const;

            StrongPointer<elists::FilteredByClassName> getElementsByClassName(DOMString className) const;

            StrongPointer<elists::FilteredByTagName> getElementsByTagName(DOMString tagName) const;

            StrongPointer<elists::FilteredByTagNameNS> getElementsByTagNameNS(DOMString ns, DOMString tagName) const;

            inline bool hasAttribute(const DOMString &attr) const { return attributes.contains(attr); }

            inline bool hasAttributeNS(const DOMString &ns, const DOMString &name) const {
                return attributes.contains(ns + ":" + name);
            }

            inline bool hasAttributes() const { return attributes.getLength() != 0; }

            //TODO: Implement me
            bool hasPointerCapture(ULong id) const;

            //TODO: Implement me
            void insertAdjacentElement(DOMString position, StrongPointer<Element> e);

            //TODO: Implement me
            void insertAdjacentHTML(DOMString position, DOMString html);

            //TODO: Implement me
            void insertAdjacentText(DOMString position, DOMString text);

            inline bool matches(const DOMString &selector) const {
                return selector::CSSSelector::parse(selector,
                                                    std::static_pointer_cast<const Element>(shared_from_this()))
                        .matches(std::static_pointer_cast<const Element>(shared_from_this()));
            }

            inline StrongPointer<Element> querySelector(const DOMString &selector) const {
                return selector::CSSSelector::parse(selector,
                                                    std::static_pointer_cast<const Element>(shared_from_this()))
                        .querySelector(std::static_pointer_cast<const Element>(shared_from_this()));
            }

            inline Vector<StrongPointer<Element>> querySelectorAll(const DOMString &selector) const {
                return selector::CSSSelector::parse(selector,
                                                    std::static_pointer_cast<const Element>(shared_from_this()))
                        .querySelectorAll(std::static_pointer_cast<const Element>(shared_from_this()));
            }

            //TODO: Implement me
            void releasePointerCapture(ULong id);

            inline void removeAttribute(DOMString attr) { attributes.removeNamedItem(std::move(attr)); }

            inline void removeAttributes(const DOMString &ns, const DOMString &name) {
                attributes.removeNamedItemNS(ns, name);
            }

            //TODO: Implement me
            void requestFullscreen(Function<void(bool)> onFinish);

            //TODO: Implement me
            void scroll(double x, double y);

            //TODO: Implement me
            void scrollBy(double x, double y);

            inline void scrollIntoView() { scrollIntoView(true); }

            //TODO: Implement me
            void scrollIntoView(bool alignToTop);

            inline void scrollTo(double x, double y) { scroll(x, y); }

            void setAttribute(DOMString name, DOMString value);

            inline void setAttribute(const DOMString &ns, const DOMString &local, DOMString value) {
                setAttribute(ns + ":" + local, std::move(value));
            }

            //TODO: Implement me
            void setPointerCapture(ULong id);

            bool toggleAttribute(DOMString attr);

            bool toggleAttribute(DOMString attr, bool force);

            //ChildNode impl
            inline void remove() {
                if (getParentNode())
                    getParentNode()->removeChild(std::static_pointer_cast<Node>(shared_from_this()));
            }

            inline void before(const Vector<StrongPointer<Node>> &insertBefore) {
                if (getParentNode()) getParentNode()->insertBeforeChildNDTCN(getSharedFromThis(), insertBefore);
            }

            inline void after(const Vector<StrongPointer<Node>> &insertAfter) {
                if (getParentNode()) getParentNode()->insertAfterChildNDTCN(getSharedFromThis(), insertAfter);
            }

            inline void replaceWith(const Vector<StrongPointer<Node>> &repl) {
                if (getParentNode()) getParentNode()->replaceChildNDTCN(getSharedFromThis(), repl);
            }

            //ParentNode impl

            inline UInt getChildElementCount() const { return children.size(); }

            inline StrongPointer<const HTMLCollection> getChildren() const {
                return StrongPointer<const HTMLCollection>(shared_from_this(), &children);
            }

            inline StrongPointer<HTMLCollection> getChildren() {
                return StrongPointer<HTMLCollection>(shared_from_this(), &children);
            }

            inline StrongPointer<Element> getFirstElementChild() const {
                return !children.empty() ? children.front() : StrongPointer<Element>();
            }

            inline StrongPointer<Element> getLastElementChild() const {
                return !children.empty() ? children.back() : StrongPointer<Element>();
            }

            inline StrongPointer<Element> getThisRef() const { return thisRef; }

            UInt getElementIndex() const;

            UInt getLastElementIndex() const;

            UInt getTypedElementIndex() const;

            UInt getLastTypedElementIndex() const;

            bool isEqualNode(const StrongPointer<const Node> &other) const override;

            inline KnownElements getElementType() const { return type; }

        protected:

            void cloneElementProperties(const StrongPointer<Element> &destination, bool deep) const;

        private:
            KnownElements type;
            NamedNodeMap attributes{};
            DOMTokenList classList{};
            css::CSSStyleDeclaration style{};
            double clientDim[4];
            DOMString ns{}, prefix{}, localName;
            double scrollDim[4];
            StrongPointer<ShadowRoot> shadowRoot{};
            HTMLCollection children;

            //Caches
            mutable observable::ObservableItem<DOMString, false> innerHTML{}, outerHTML{};

            const DOMString &constructInnerHTML() const;

            const DOMString &constructOuterHTML() const;

            StrongPointer<Element> thisRef = std::static_pointer_cast<Element>(shared_from_this());

            mutable observable::ObservableItem<Pair<UInt, UInt>, false> indeces, typedIndeces;

            void updateElementIndeces() const;

            void updatedTypedIndeces() const;
        };

        bool compareType(const StrongPointer<const Element> &e, const DOMString &type);
    }
}

#endif //FEATHER_ELEMENT_H
