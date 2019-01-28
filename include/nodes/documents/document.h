//
// Created by prem on 12/11/2018.
//

#ifndef FEATHER_DOCUMENT_H
#define FEATHER_DOCUMENT_H

#include "nodes/text/processing_instruction.h"
#include "nodes/text/c_data_section.h"
#include "nodes/text/comment.h"
#include "nodes/utils/attr.h"
#include "style/css/pseudoclass_manager.h"

namespace feather {
    namespace dom {

        class DocumentFragment;

        namespace dlists {
            class ObjectMappedList;

            class FormMappedList;

            class ImageMappedList;

            class LinkMappedList;

            class EmbedMappedList;

            class ScriptMappedList;
        }
        namespace html {
            class HTMLElement;

            class HTMLHtmlElement;

            class HTMLHeadElement;

            class HTMLBodyElement;
        }

        class DocumentOrShadowRoot {
        public:
            css::PseudoclassManager &getPseudoclassManager() { return pseudoclassManager; }

        private:
            css::PseudoclassManager pseudoclassManager;
        };

        class DocumentType : public Node {
        public:
            inline const DOMString &getName() const { return name; }

            inline const DOMString &getPublicId() const { return getEmptyString(); }

            inline const DOMString &getSystemId() const { return getEmptyString(); }

            inline void remove() { getParentNode()->removeChild(getSharedFromThis()); }

        private:
            DOMString name{};
        };

        class Document : public Node, public DocumentOrShadowRoot {
        public:

            StrongPointer<html::HTMLBodyElement> getBody() const;

            void setBody(StrongPointer<html::HTMLElement> body);

            const DOMString &getCharacterSet() const { return characterSet; }

            inline TriValue getMode() const { return quirks; }

            inline DOMString getContentType() const { return "application/xml"; }

            StrongPointer<DocumentType> getDocType() const;

            StrongPointer<html::HTMLHtmlElement> getDocumentElement() const;

            inline DOMString getDocumentURI() const { return documentURI; }

            StrongPointer<dlists::ObjectMappedList> getEmbeds() const;

            //TODO: FontFaceSet, document.fonts()

            StrongPointer<dlists::FormMappedList> getForms() const;

            StrongPointer<html::HTMLHeadElement> getHead();

            inline bool isHidden() { return hidden; }

            StrongPointer<dlists::ImageMappedList> getImages() const;

            //TODO: DOMImplementation

            inline DOMString getLastStyleSheet() const { return lastStyleSheet; }

            StrongPointer<dlists::LinkMappedList> getLinks() const;

            StrongPointer<dlists::EmbedMappedList> getPlugins() const;

            inline DOMString getPreferredStyleSheet() const { return preferredStyleSheet; }

            StrongPointer<dlists::ScriptMappedList> getScripts() const;

            StrongPointer<html::HTMLElement> getScrollingElement() const;

            inline DOMString getSelectedStyleSheet() const { return preferredStyleSheet; }

            void adoptNode(StrongPointer<Node> other);

            StrongPointer<Attr> createAttribute(DOMString attr) const;

            StrongPointer<CDATASection> createCDATASection(DOMString data) const;

            StrongPointer<Comment> createComment(DOMString comment) const;

            StrongPointer<DocumentFragment> createDocumentFragment() const;

            StrongPointer<Element> createElement(DOMString tagName) const;

            StrongPointer<ProcessingInstruction> createProcessingInstruction
                    (DOMString target, DOMString data) const;

            //TODO: ranges

            StrongPointer<Text> createTextNode(DOMString val) const;

            //TODO: pointer lock

            //TODO: dlists

            //TODO: storage

            void importNode(StrongPointer<Node> s);

            //TODO: storage and promises

            //TODO: pointer lock

            StrongPointer<Element> getElementById(const DOMString &id) const;

            StrongPointer<Element> querySelector(const DOMString &selector) const;

            Vector<StrongPointer<Element>> querySelectorAll(const DOMString &selector) const;

            inline StrongPointer<HTMLCollection> getChildren() {
                return StrongPointer<HTMLCollection>(shared_from_this(), &children);
            }

            inline StrongPointer<const HTMLCollection> getChildren() const {
                return StrongPointer<const HTMLCollection>(shared_from_this(), &children);
            }

            inline UInt getChildElementCount() const { return children.size(); }

        private:
            DOMString characterSet{};
            TriValue quirks = TriValue::NO;
            DOMString documentURI{};
            bool hidden = false;
            DOMString lastStyleSheet{};
            DOMString preferredStyleSheet{};
            HTMLCollection children;

            mutable StrongPointer<dlists::ObjectMappedList> objects{};
            mutable StrongPointer<dlists::FormMappedList> forms{};
            mutable StrongPointer<dlists::ImageMappedList> images{};
            mutable StrongPointer<dlists::LinkMappedList> links{};
            mutable StrongPointer<dlists::EmbedMappedList> embeds{};
            mutable StrongPointer<dlists::ScriptMappedList> scripts{};
        };
    }
}

#endif //FEATHER_DOCUMENT_H
