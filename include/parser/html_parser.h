//
// Created by prem on 1/28/2019.
//

#ifndef CURL_HTML_PARSER_H
#define CURL_HTML_PARSER_H

#include "nodes/elements/html_elements/html_element_includes.h"
#include "html_trie.h"

namespace feather {
    namespace parser {

#define UNICODE_REPL 65533

        enum class InsertionMode {
            INITIAL,
            BEFORE_HTML,
            BEFORE_HEAD,
            IN_HEAD,
            IN_HEAD_NOSCRIPT,
            AFTER_HEAD,
            IN_BODY,
            TEXT,
            IN_TABLE,
            IN_TABLE_TEXT,
            IN_CAPTION,
            IN_COLUMN_GROUP,
            IN_TABLE_BODY,
            IN_ROW,
            IN_CELL,
            IN_SELECT,
            IN_SELECT_IN_TABLE,
            IN_TEMPLATE,
            AFTER_BODY,
            IN_FRAMESET,
            AFTER_FRAMESET,
            AFTER_AFTER_BODY,
            AFTER_AFTER_FRAMESET
        };

        enum class TokenType {
            DOCTYPE, START, END, COMMENT, CHARACTER, END_OF_FILE
        };

        constexpr bool isValidProp(int prop);

        class DOCTYPE_TOKEN_DATA {

        public:
            enum class DOCTYPE_PROPS {
                MISSING_NAME, MISSING_PUBLIC_ID, MISSING_SYSTEM_ID, FORCE_QUIRKS, __COUNT__
            };

            template<DOCTYPE_PROPS prop>
            inline std::enable_if<isValidProp(static_cast<int>(prop)), bool> isMissing() const {
                return props[prop];
            }

            inline bool isForceQuirks() const { return props[DOCTYPE_PROPS::FORCE_QUIRKS]; }

            inline void setForceQuirks(bool force) {
                if (force) props += DOCTYPE_PROPS::FORCE_QUIRKS; else props -= DOCTYPE_PROPS::FORCE_QUIRKS;
            }

            inline const DOMString &getName() const {
                return name;
            }

            inline void setName(DOMString name) {
                DOCTYPE_TOKEN_DATA::name = std::move(name);
                props -= DOCTYPE_PROPS::MISSING_NAME;
            }

            inline const DOMString &getPublicID() const {
                return publicID;
            }

            inline void setPublicID(DOMString publicID) {
                DOCTYPE_TOKEN_DATA::publicID = std::move(publicID);
                props -= DOCTYPE_PROPS::MISSING_PUBLIC_ID;
            }

            inline const DOMString &getSystemID() const {
                return systemID;
            }

            inline void setSystemID(DOMString systemID) {
                DOCTYPE_TOKEN_DATA::systemID = std::move(systemID);
                props -= DOCTYPE_PROPS::MISSING_SYSTEM_ID;
            }

        private:

            EnumSet<DOCTYPE_PROPS> props
                    = EnumSet<DOCTYPE_PROPS>((unsigned char) 0b111);

            DOMString name, publicID, systemID;
        };

        constexpr bool isValidProp(int prop) {
            return prop < static_cast<int>(DOCTYPE_TOKEN_DATA::DOCTYPE_PROPS::FORCE_QUIRKS);
        }

        struct REGULAR_TOKEN_DATA {

            DOMString tagName;
            bool selfClosing;
            Vector<UnaryPair<DOMString>> attributes;
        };

        typedef UInt CHARACTER_TOKEN_DATA;
        typedef DOMString COMMENT_TOKEN_DATA;

        typedef Variant<DOCTYPE_TOKEN_DATA, REGULAR_TOKEN_DATA,
                CHARACTER_TOKEN_DATA, COMMENT_TOKEN_DATA> TokenData;

        struct Token {
            TokenType type;
            TokenData data;
        };

        enum class ParserState {
            DATA,
            RCDATA,
            RAWTEXT,
            SCRIPT_DATA,
            PLAINTEXT,
            TAG_OPEN,
            END_TAG_OPEN,
            TAG_NAME,
            RCDATA_LESS_THAN_SIGN,
            RCDATA_END_TAG_OPEN,
            RCDATA_END_TAG_NAME,
            RAWTEXT_LESS_THAN_SIGN,
            RAWTEXT_END_TAG_OPEN,
            RAWTEXT_END_TAG_NAME,
            SCRIPT_DATA_LESS_THAN_SIGN,
            SCRIPT_DATA_END_TAG_OPEN,
            SCRIPT_DATA_END_TAG_NAME,
            SCRIPT_DATA_ESCAPE_START,
            SCRIPT_DATA_ESCAPE_START_DASH,
            SCRIPT_DATA_ESCAPE,
            SCRIPT_DATA_ESCAPE_DASH,
            SCRIPT_DATA_ESCAPE_DASH_DASH,
            SCRIPT_DATA_ESCAPE_LESS_THAN_SIGN,
            SCRIPT_DATA_ESCAPE_END_TAG_OPEN,
            SCRIPT_DATA_ESCAPE_END_TAG_NAME,
            SCRIPT_DATA_DOUBLE_ESCAPE_START,
            SCRIPT_DATA_DOUBLE_ESCAPE,
            SCRIPT_DATA_DOUBLE_ESCAPE_DASH,
            SCRIPT_DATA_DOUBLE_ESCAPE_DASH_DASH,
            SCRIPT_DATA_DOUBLE_ESCAPE_LESS_THAN_SIGN,
            SCRIPT_DATA_DOUBLE_ESCAPE_END,
            BEFORE_ATTRIBUTE_NAME,
            ATTRIBUTE_NAME,
            AFTER_ATTRIBUTE_NAME,
            BEFORE_ATTRIBUTE_VALUE,
            DQUOTE_ATTRIBUTE_VALUE,
            SQUOTE_ATTRIBUTE_VALUE,
            UQUOTE_ATTRIBUTE_VALUE,
            QUOTE_AFTER_ATTRIBUTE_VALUE,
            SELF_CLOSING_START_TAG,
            BOGUS_COMMENT,
            MARKUP_DECLARATION_OPEN,
            COMMENT_START,
            COMMENT_START_DASH,
            COMMENT,
            COMMENT_LESS_THAN_SIGN,
            COMMENT_LESS_THAN_SIGN_BANG,
            COMMENT_LESS_THAN_SIGN_BANG_DASH,
            COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH,
            COMMENT_END_DASH,
            COMMENT_END,
            COMMENT_END_BANG,
            DOCTYPE,
            BEFORE_DOCTYPE_NAME,
            DOCTYPE_NAME,
            AFTER_DOCTYPE_NAME,
            AFTER_DOCTYPE_PUBLIC_KEYWORD,
            BEFORE_DOCTYPE_PUBLIC_IDENTIFIER,
            DQUOTE_DOCTYPE_PUBLIC_IDENTIFIER,
            SQUOTE_DOCTYPE_PUBLIC_IDENTIFIER,
            AFTER_DOCTYPE_PUBLIC_IDENTIFIER,
            BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS,
            AFTER_DOCTYPE_SYSTEM_KEYWORD,
            BEFORE_DOCTYPE_SYSTEM_IDENTIFIER,
            DQUOTE_DOCTYPE_SYSTEM_IDENTIFIER,
            SQUOTE_DOCTYPE_SYSTEM_IDENTIFIER,
            AFTER_DOCTYPE_SYSTEM_IDENTIFIER,
            BOGUS_DOCTYPE,
            CDATA_SECTION,
            CDATA_SECTION_BRACKET,
            CDATA_SECTION_END,
            CHARACTER_REFERENCE,
            NAMED_CHARACTER_REFERENCE,
            AMBIGUOUS_AMPERSAND,
            NUMERIC_CHARACTER_REFERENCE,
            HEXADECIMAL_CHARACTER_REFERENCE_START,
            DECIMAL_CHARACTER_REFERENCE_START,
            HEXADECIMAL_CHARACTER_REFERENCE,
            DECIMAL_CHARACTER_REFERENCE,
            NUMERIC_CHARACTER_REFERENCE_END
        };

        constexpr bool isAttributeState(ParserState state) {
            return state != ParserState::DATA && state != ParserState::RCDATA;
        }

        class HTMLStateMachine {

            bool operator<<(char *c);

        private:

            void resetBuf() {
                tempBuf.clear();
            }

            void resetAttr() {
                checkAddAttr();
                attr.clear();
                val.clear();
            }

            bool dumpBuf() {
                for (UInt i = 0, size = utf8::charSize(tempBuf[i]); i < tempBuf.length();
                     size = utf8::charSize(tempBuf[i += size])) {
                    emitted.push_back(Token{TokenType::CHARACTER,
                                            TokenData{utf8::toCodePoint(tempBuf.c_str() + i, (unsigned char) size)}});
                }
                return true;
            }

            template<class... T>
            bool emit(T... tokens) {
                std::array<Token, sizeof...(tokens)> arr = {tokens...};
                std::copy(arr.begin(), arr.end(), emitted.begin());
                return true;
            }

#define TOKENDEF(prefix, symbol) inline Token prefix##Token() const {return {TokenType::CHARACTER, TokenData{CHARACTER_TOKEN_DATA{symbol}}};}

            TOKENDEF(bang, '!')

            TOKENDEF(solidus, '/')

            TOKENDEF(lt, '<')

            TOKENDEF(gt, '>')

            TOKENDEF(dash, '-')

            TOKENDEF(replacement, UNICODE_REPL);

            TOKENDEF(eof, (UInt) EOF);

            TOKENDEF(rb, ']');

#undef TOKENDEF

            inline Token defaultToken(char *c) const {
                return {TokenType::CHARACTER, TokenData{CHARACTER_TOKEN_DATA{utf8::toCodePoint(c)}}};
            }

            inline bool isGoodEndTag() {
                return !name.empty() && name == lastStart.back();
            }

            inline Token endTagToken() {
                lastStart.pop_back();
                return {TokenType::END, TokenData{REGULAR_TOKEN_DATA{std::move(name), true, {}}}};
            }

            inline Token startTagToken() {
                if (!selfClosing) lastStart.push_back(name);
                return {TokenType::START, TokenData{REGULAR_TOKEN_DATA{std::move(name), selfClosing, getAttrVec()}}};
            }

            inline Token commentToken() {
                return {TokenType::COMMENT, TokenData{COMMENT_TOKEN_DATA{std::move(comment)}}};
            }

            Vector<UnaryPair<DOMString>> getAttrVec() {
                if (!attr.empty() || !val.empty()) checkAddAttr();
                Vector<UnaryPair<DOMString>> vec{};
                vec.reserve(attributes.size());
                for (auto &pair : attributes) vec.push_back(std::move(pair));
                attributes.clear();
                return vec;
            }

            void checkAddAttr() {
                if (!attr.empty() && !val.empty()) {
                    if (attributes.find(attr) != attributes.end()) {
                        attributes[attr] = std::move(val);
                    }
                    resetAttr();
                }
            }

            void resetDoctype() {
                forceQuirks = !(nameMissing = publicIDMissing = systemIDMissing = true);
                name.clear();
                systemID.clear();
                publicID.clear();
            }

            inline Token doctypeToken() {
                DOCTYPE_TOKEN_DATA data{};
                if (!nameMissing) data.setName(std::move(name));
                if (!publicIDMissing) data.setPublicID(std::move(publicID));
                if (!systemIDMissing) data.setSystemID(std::move(systemID));
                data.setForceQuirks(forceQuirks);
                return {TokenType::DOCTYPE, TokenData{data}};
            }

            ParserState dataState, returnState;
            bool reconsume, selfClosing, reconsumeMarkup, forceQuirks,
                    publicIDMissing, systemIDMissing, nameMissing;
            TriValue markupType;
            DOMString tempBuf, name, attr, val, comment, markup, publicID, systemID;
            Map<DOMString, DOMString> attributes;
            Vector<Token> emitted;
            Vector<DOMString> lastStart;
            EscapeCodeTrieTraverser traverser{nullptr};
        };
    }
}
#endif //CURL_HTML_PARSER_H
