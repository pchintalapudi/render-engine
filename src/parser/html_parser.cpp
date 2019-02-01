//
// Created by prem on 1/28/2019.
//

#include "parser/html_parser.h"

using namespace feather::parser;

#define ASCII_LOWER_ALPHA 'a':case 'b':case 'c': case 'd':case 'e':case 'f':case'g':case 'h':case 'i':case 'j':\
case 'k':case 'l':case 'm':case'n':case'o':case'p':case'r':case's':case't':case'u':case'v':case'w':case'x':\
case'y':case'z'
#define ASCII_UPPER_ALPHA 'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':\
case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z'
#define WHITESPACE '\t':case'\n':case'\f':case' '

#define END_TAG_SWITCH(label)\
switch (*c.nextChar) {\
case WHITESPACE:\
if (isGoodEndTag()) {\
dataState = ParserState::BEFORE_ATTRIBUTE_NAME;\
return false;\
}\
goto label;\
case '/':\
if (isGoodEndTag()) {\
dataState = ParserState::SELF_CLOSING_START_TAG;\
return false;\
}\
goto label;\
case '>':\
if (isGoodEndTag()) {\
dataState = ParserState::DATA;\
return emit(endTagToken());\
}\
goto label;\
case ASCII_UPPER_ALPHA: {\
name += char(*c.nextChar + 0x0020);\
tempBuf.push_back(DOMString() + *c.nextChar);\
return false;\
}\
case ASCII_LOWER_ALPHA: {\
char chr = *c.nextChar;\
name += chr;\
tempBuf.push_back(DOMString() + chr);\
return false;\
}\
}

bool HTMLStateMachine::operator<<(UTF_8_CHARACTER c) {
    reconsume = false;
    switch (dataState) {
        case ParserState::DATA:
            if (c.length > 1) return emit(defaultToken(c));
            switch (*c.nextChar) {
                case 0:
                default:
                    return emit(defaultToken(c));
                case '&':
                    returnState = ParserState::DATA;
                    dataState = ParserState::CHARACTER_REFERENCE;
                    return false;
                case '<':
                    dataState = ParserState::TAG_OPEN;
                    return false;
                case EOF:
                    return emit(eofToken());
            }
        case ParserState::RCDATA:
            if (c.length > 1) return emit(defaultToken(c));
            switch (*c.nextChar) {
                default:
                    return emit(defaultToken(c));
                case '&':
                    returnState = ParserState::RCDATA;
                    dataState = ParserState::CHARACTER_REFERENCE;
                    return false;
                case '<':
                    dataState = ParserState::RCDATA_LESS_THAN_SIGN;
                    return false;
                case EOF:
                    return emit(eofToken());
                case 0:
                    return emit(replacementToken());
            }
        case ParserState::RAWTEXT:
            if (c.length > 1) return emit(defaultToken(c));
            switch (*c.nextChar) {
                default:
                    return emit(defaultToken(c));
                case EOF:
                    return emit(eofToken());
                case '<':
                    dataState = ParserState::RAWTEXT_LESS_THAN_SIGN;
                    return false;
            }
        case ParserState::SCRIPT_DATA:
            if (c.length > 1) return emit(defaultToken(c));
            switch (*c.nextChar) {
                default:
                    return emit(defaultToken(c));
                case EOF:
                    return emit(defaultToken(c));
                case 0:
                    return emit(replacementToken());
                case '<':
                    dataState = ParserState::SCRIPT_DATA_LESS_THAN_SIGN;
                    return false;
            }
        case ParserState::PLAINTEXT:
            if (c.length > 1) return emit(defaultToken(c));
            switch (*c.nextChar) {
                default:
                    return emit(defaultToken(c));
                case EOF:
                    return emit(eofToken());
                case 0:
                    return emit(replacementToken());
            }
        case ParserState::TAG_OPEN:
            if (c.length > 1) return emit(ltToken());
            switch (*c.nextChar) {
                case '!':
                    dataState = ParserState::MARKUP_DECLARATION_OPEN;
                    return false;
                case '/':
                    dataState = ParserState::END_TAG_OPEN;
                    return false;
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    dataState = ParserState::TAG_NAME;
                    name.clear();
                    reconsume = true;
                    return false;
                case '?':
                    dataState = ParserState::BOGUS_COMMENT;
                    name.clear();
                    reconsume = true;
                    return false;
                case EOF:
                    return emit(ltToken(), eofToken());
                default:
                    return emit(ltToken());
            }
        case ParserState::END_TAG_OPEN:
            if (c.length > 1) {
                def1:
                dataState = ParserState::BOGUS_COMMENT;
                reconsume = true;
                return false;
            }
            switch (*c.nextChar) {
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    dataState = ParserState::TAG_NAME;
                    name.clear();
                    reconsume = true;
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return false;
                case EOF:
                    return emit(ltToken(), solidusToken());
                default:
                    goto def1;
            }
        case ParserState::TAG_NAME:
            if (c.length > 1) {
                tempBuf[0].reserve(tempBuf[0].length() + c.length);
                while (c.length--) tempBuf[0] += *c.nextChar++;
                return false;
            }
            switch (*c.nextChar) {
                case WHITESPACE:
                    dataState = ParserState::BEFORE_ATTRIBUTE_NAME;
                    return false;
                case '/':
                    dataState = ParserState::SELF_CLOSING_START_TAG;
                    return false;
                case ASCII_UPPER_ALPHA:
                    tempBuf[0] += (char) (*c.nextChar + 0x0020);
                    return false;
                case 0:
                    tempBuf[0] += "\uFFFD";
                    return false;
                case EOF:
                    return emit(eofToken());
                default:
                    tempBuf[0] += *c.nextChar;
                    return false;
            }
        case ParserState::RCDATA_LESS_THAN_SIGN:
            if (c.length > 1) {
                def2:
                reconsume = true;
                dataState = ParserState::RCDATA;
                return emit(ltToken());
            }
            switch (*c.nextChar) {
                case '/':
                    resetBuf();
                    dataState = ParserState::RCDATA_END_TAG_OPEN;
                    return false;
                default:
                    goto def2;
            }
        case ParserState::RCDATA_END_TAG_OPEN:
            if (c.length > 1) {
                def3:
                reconsume = true;
                dataState = ParserState::RCDATA;
                return emit(ltToken(), solidusToken());
            }
            switch (*c.nextChar) {
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    resetBuf();
                    reconsume = true;
                    dataState = ParserState::RCDATA_END_TAG_NAME;
                    return false;
                default:
                    goto def3;
            }
        case ParserState::RCDATA_END_TAG_NAME:
            if (c.length > 1) {
                def4:
                dataState = ParserState::RCDATA;
                return dumpBuf();
            }
            switch (*c.nextChar) {
                default:
                    goto def4;
                case WHITESPACE:
                    if (isGoodEndTag()) {
                        dataState = ParserState::BEFORE_ATTRIBUTE_NAME;
                        return false;
                    }
                    goto def4;
                case '/':
                    if (isGoodEndTag()) {
                        dataState = ParserState::SELF_CLOSING_START_TAG;
                        return false;
                    }
                    goto def4;
                case '>':
                    if (isGoodEndTag()) {
                        dataState = ParserState::DATA;
                        return emit(endTagToken());
                    }
                    goto def4;
                case ASCII_UPPER_ALPHA: {
                    name += char(*c.nextChar + 0x0020);
                    tempBuf.push_back(DOMString() + *c.nextChar);
                    return false;
                }
                case ASCII_LOWER_ALPHA: {
                    name += *c.nextChar;
                    tempBuf.push_back(DOMString() + *c.nextChar);
                    return false;
                }
            }
        case ParserState::RAWTEXT_LESS_THAN_SIGN:
            if (c.length > 1) {
                def5:
                reconsume = true;
                dataState = ParserState::RAWTEXT;
                return emit(ltToken());
            }
            switch (*c.nextChar) {
                default:
                    goto def5;
                case '/':
                    resetBuf();
                    dataState = ParserState::RAWTEXT_END_TAG_OPEN;
                    return false;
            }
        case ParserState::RAWTEXT_END_TAG_OPEN:
            if (c.length > 1) {
                def0:
                dataState = ParserState::RAWTEXT;
                reconsume = true;
                return emit(ltToken(), solidusToken());
            }
            switch (*c.nextChar) {
                case ASCII_UPPER_ALPHA:
                case ASCII_LOWER_ALPHA:
                    dataState = ParserState::RAWTEXT_END_TAG_NAME;
                    reconsume = true;
                    name.clear();
                    return false;
                default:
                    goto def0;
            }
        case ParserState::RAWTEXT_END_TAG_NAME:
            if (c.length > 1) {
                def6:
                reconsume = true;
                dataState = ParserState::RAWTEXT;
                return dumpBuf();
            }
            END_TAG_SWITCH(def6);
        case ParserState::SCRIPT_DATA_LESS_THAN_SIGN:
            if (c.length > 1) {
                def7:
                reconsume = true;
                dataState = ParserState::SCRIPT_DATA;
                return emit(ltToken());
            }
            switch (*c.nextChar) {
                default:
                    goto def7;
                case '/':
                    resetBuf();
                    dataState = ParserState::SCRIPT_DATA_END_TAG_OPEN;
                    return false;
                case '!':
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_START;
                    return emit(ltToken(), bangToken());
            }
        case ParserState::SCRIPT_DATA_END_TAG_OPEN:
            if (c.length > 1) {
                def8:
                reconsume = true;
                dataState = ParserState::SCRIPT_DATA;
                return emit(ltToken(), solidusToken());
            }
            switch (*c.nextChar) {
                default:
                    goto def8;
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    name.clear();
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA_END_TAG_NAME;
                    return false;
            }
        case ParserState::SCRIPT_DATA_END_TAG_NAME:
            if (c.length > 1) {
                def9:
                reconsume = true;
                dataState = ParserState::SCRIPT_DATA;
                return dumpBuf();
            }
            END_TAG_SWITCH(def9);
        case ParserState::SCRIPT_DATA_ESCAPE_START:
            if (c.length > 1) {
                def10:
                reconsume = true;
                dataState = ParserState::SCRIPT_DATA;
                return false;
            }
            switch (*c.nextChar) {
                case '-':
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_START_DASH;
                    return emit(dashToken());
                default:
                    goto def10;
            }
        case ParserState::SCRIPT_DATA_ESCAPE_START_DASH:
            if (c.length > 1) {
                goto def10;
            }
            switch (*c.nextChar) {
                default:
                    goto def10;
                case '-':
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_DASH_DASH;
                    return emit(dashToken());
            }
        case ParserState::SCRIPT_DATA_ESCAPE:
            if (c.length > 1) return emit(defaultToken(c));
            switch (*c.nextChar) {
                default:
                    return emit(defaultToken(c));
                case '-':
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_DASH;
                    return emit(dashToken());
                case '<':
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_LESS_THAN_SIGN;
                    return false;
                case 0:
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
            }
        case ParserState::SCRIPT_DATA_ESCAPE_DASH:
            if (c.length > 1) {
                def11:
                dataState = ParserState::SCRIPT_DATA_ESCAPE;
                return emit(defaultToken(c));
            }
            switch (*c.nextChar) {
                default:
                    goto def11;
                case 0:
                    dataState = ParserState::SCRIPT_DATA_ESCAPE;
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
                case '-':
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_DASH_DASH;
                    return emit(dashToken());
                case '<':
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_LESS_THAN_SIGN;
            }
        case ParserState::SCRIPT_DATA_ESCAPE_DASH_DASH:
            if (c.length > 1) {
                goto def11;
            }
            switch (*c.nextChar) {
                default:
                    goto def11;
                case '-':
                    return emit(dashToken());
                case 0:
                    dataState = ParserState::SCRIPT_DATA_ESCAPE;
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
                case '<':
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_LESS_THAN_SIGN;
                    return false;
                case '>':
                    dataState = ParserState::SCRIPT_DATA;
                    return emit(gtToken());
            }
        case ParserState::SCRIPT_DATA_ESCAPE_LESS_THAN_SIGN:
            if (c.length > 1) {
                def12:
                reconsume = true;
                dataState = ParserState::SCRIPT_DATA_ESCAPE;
                return emit(ltToken());
            }
            switch (*c.nextChar) {
                default:
                    goto def12;
                case '/':
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_END_TAG_OPEN;
                    resetBuf();
                    return false;
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    resetBuf();
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_START;
                    return emit(ltToken());
            }
        case ParserState::SCRIPT_DATA_ESCAPE_END_TAG_OPEN:
            if (c.length > 1) {
                def13:
                reconsume = true;
                dataState = ParserState::SCRIPT_DATA_ESCAPE;
                return emit(ltToken(), solidusToken());
            }
            switch (*c.nextChar) {
                case ASCII_UPPER_ALPHA:
                case ASCII_LOWER_ALPHA:
                    reconsume = true;
                    name.clear();
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_END_TAG_NAME;
                    return false;
                default:
                    goto def13;
            }
        case ParserState::SCRIPT_DATA_ESCAPE_END_TAG_NAME:
            if (c.length > 1) {
                def14:
                reconsume = true;
                dataState = ParserState::SCRIPT_DATA_ESCAPE;
                return dumpBuf();
            }
            END_TAG_SWITCH(def14);
        case ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_START:
            if (c.length > 1) {
                def15:
                reconsume = true;
                dataState = ParserState::SCRIPT_DATA_ESCAPE;
                return false;
            }
            switch (*c.nextChar) {
                default:
                    goto def15;
                case WHITESPACE:
                case '/':
                case '>':
                    dataState = tempBuf.size() == 6 && stringify() == "script"
                                ? ParserState::SCRIPT_DATA_ESCAPE
                                : ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(defaultToken(c));
                case ASCII_UPPER_ALPHA: {
                    char chr = char(*c.nextChar + 0x0020);
                    tempBuf.push_back(DOMString() + chr);
                    return emit(defaultToken(c));
                }
                case ASCII_LOWER_ALPHA: {
                    tempBuf.push_back(DOMString() + *c.nextChar);
                    return emit(defaultToken(c));
                }
            }
        case ParserState::SCRIPT_DATA_DOUBLE_ESCAPE:
            if (c.length > 1) emit(defaultToken(c));
            switch (*c.nextChar) {
                default:
                    emit(defaultToken(c));
                case '-':
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_DASH;
                    return emit(dashToken());
                case '<':
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_LESS_THAN_SIGN;
                    return emit(ltToken());
                case 0:
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
            }
        case ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_DASH:
            if (c.length > 1) {
                def16:
                dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                return emit(defaultToken(c));
            }
            switch (*c.nextChar) {
                default:
                    goto def16;
                case '-':
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_DASH_DASH;
                    return emit(dashToken());
                case '<':
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_LESS_THAN_SIGN;
                    return emit(ltToken());
                case 0:
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
            }
        case ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_DASH_DASH:
            if (c.length > 1) {
                goto def16;
            }
            switch (*c.nextChar) {
                default:
                    goto def16;
                case '-':
                    return emit(dashToken());
                case '<':
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_LESS_THAN_SIGN;
                    return emit(ltToken());
                case '>':
                    dataState = ParserState::SCRIPT_DATA;
                    return emit(gtToken());
                case 0:
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
            }
        case ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_LESS_THAN_SIGN:
            if (c.length > 1) {
                def17:
                reconsume = true;
                dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                return false;
            }
            switch (*c.nextChar) {
                default:
                    goto def17;
                case '/':
                    resetBuf();
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_END;
                    return emit(solidusToken());
            }
        case ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_END:
            if (c.length > 1) {
                goto def17;
            }
            switch (*c.nextChar) {
                default:
                    goto def17;
                case WHITESPACE:
                case '/':
                case '>':
                    dataState = stringify() == "script"
                                ? ParserState::SCRIPT_DATA_ESCAPE
                                : ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(defaultToken(c));
                case ASCII_UPPER_ALPHA: {
                    tempBuf.push_back(DOMString() + char(*c.nextChar + 0x0020));
                    return emit(defaultToken(c));
                }
                case ASCII_LOWER_ALPHA: {
                    tempBuf.push_back(DOMString() + *c.nextChar);
                    return emit(defaultToken(c));
                }
            }
        case ParserState::BEFORE_ATTRIBUTE_NAME:
            if (c.length > 1) {

            }
    }
}