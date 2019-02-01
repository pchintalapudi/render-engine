//
// Created by prem on 1/28/2019.
//

#ifndef CURL_HTML_PARSER_H
#define CURL_HTML_PARSER_H

#include "nodes/elements/html_elements/html_element_includes.h"

namespace feather {
    namespace parser {

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

        struct DOCTYPE_TOKEN_DATA {
        private:
            enum class DOCTYPE_PROPS {
                MISSING_NAME, MISSING_PUBLIC_ID, MISSING_SYSTEM_ID, FORCE_QUIRKS, __COUNT__
            };

            EnumSet<DOCTYPE_PROPS, unsigned char> props
                    = EnumSet<DOCTYPE_PROPS, unsigned char>((unsigned char) 0b111);

        public:

            DOCTYPE_TOKEN_DATA() = default;

            DOMString name, publicID, systemID;

            bool isMissingName() const { return props[DOCTYPE_PROPS::MISSING_NAME]; }

            bool isMissingPubliID() const { return props[DOCTYPE_PROPS::MISSING_PUBLIC_ID]; }

            bool isMissingSystemID() const { return props[DOCTYPE_PROPS::MISSING_SYSTEM_ID]; }

            bool isForceQuirks() const { return props[DOCTYPE_PROPS::FORCE_QUIRKS]; }
        };

        struct REGULAR_TOKEN_DATA {

            DOMString tagName;
            bool selfClosing;
            Vector<UnaryPair<DOMString>> attributes;
        };

        struct CHARACTER_TOKEN_DATA {

            DOMString data;
        };

        typedef Variant<DOCTYPE_TOKEN_DATA, REGULAR_TOKEN_DATA, CHARACTER_TOKEN_DATA> TokenData;

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

        struct UTF_8_CHARACTER {
            const char *nextChar;
            unsigned char length : 2;

            operator DOMString() { return DOMString(nextChar, length); }
        };

        class HTMLStateMachine {

            bool operator<<(UTF_8_CHARACTER c);

        private:

            void resetBuf() {
                tempBuf.clear();
            }

            bool dumpBuf() {
                emit(ltToken(), solidusToken());
                emitted.reserve(emitLength = 2 + tempBuf.size());
                for (auto &chr : tempBuf)
                    emitted.push_back(Token{TokenType::CHARACTER, TokenData{CHARACTER_TOKEN_DATA{std::move(chr)}}});
                reconsume = true;
                return true;
            }

            template<class... T>
            bool emit(T... tokens) {
                emitLength = sizeof...(tokens);
                std::array<Token, sizeof...(tokens)> arr = {tokens...};
                std::copy(arr.begin(), arr.end(), emitted.begin());
                return true;
            }

            inline Token bangToken() const {
                return {TokenType::CHARACTER, TokenData{CHARACTER_TOKEN_DATA{"!"}}};
            }

            inline Token solidusToken() const {
                return {TokenType::CHARACTER, TokenData{CHARACTER_TOKEN_DATA{"/"}}};
            }

            inline Token ltToken() const {
                return {TokenType::CHARACTER, TokenData{CHARACTER_TOKEN_DATA{"<"}}};
            }

            inline Token gtToken() const {
                return {TokenType::CHARACTER, TokenData{CHARACTER_TOKEN_DATA{">"}}};
            }

            inline Token dashToken() const {
                return {TokenType::CHARACTER, TokenData{CHARACTER_TOKEN_DATA{"-"}}};
            }

            inline Token replacementToken() const {
                return {TokenType::CHARACTER, TokenData{CHARACTER_TOKEN_DATA{"\uFFFD"}}};
            }

            inline Token eofToken() const {
                return {TokenType::END_OF_FILE, TokenData()};
            }

            inline Token defaultToken(DOMString c) const {
                return {TokenType::CHARACTER, TokenData{CHARACTER_TOKEN_DATA{std::move(c)}}};
            }

            inline bool isGoodEndTag() {
                return !name.empty() && name == lastStart;
            }

            inline Token endTagToken() {
                return {TokenType::END, TokenData{REGULAR_TOKEN_DATA{std::move(name), true, {}}}};
            }

            inline DOMString stringify() {
                DOMString string;
                UInt reserve = 0;
                for (const auto &str : tempBuf) reserve += str.length();
                string.reserve(reserve);
                for (const auto &str : tempBuf) string += str;
                return string;
            }

            ParserState dataState, returnState;
            bool reconsume;
            DOMString lastStart;
            Vector<DOMString> tempBuf;
            DOMString name, attr, val;
            Vector<Token> emitted;
            UInt emitLength;
        };
    }
}
#endif //CURL_HTML_PARSER_H
