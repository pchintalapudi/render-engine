//
// Created by prem on 1/28/2019.
//

#include "parser/html_parser.h"
#include "parser/html_escape_codes.h"

using namespace feather::parser;

#define ASCII_LOWER_ALPHA 'a':case 'b':case 'c': case 'd':case 'e':case 'f':case'g':case 'h':case 'i':case 'j':\
case 'k':case 'l':case 'm':case'n':case'o':case'p':case'r':case's':case't':case'u':case'v':case'w':case'x':\
case'y':case'z'
#define ASCII_UPPER_ALPHA 'A':case'B':case'C':case'D':case'E':case'F':case'G':case'H':case'I':case'J':case'K':case'L':\
case'M':case'N':case'O':case'P':case'Q':case'R':case'S':case'T':case'U':case'V':case'W':case'X':case'Y':case'Z'
#define WHITESPACE '\t':case'\n':case'\f':case' '
#define ASCII_NUMERALS '0':case '1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9'
#define le(lowercase, uppercase, fail) if (*c != lowercase && *c != uppercase) goto fail; markup += lowercase; break;

bool HTMLStateMachine::operator<<(char *c) {
    reconsume = false;
    emitted.clear();
    switch (dataState) {
        case ParserState::DATA:
            switch (*c) {
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
            switch (*c) {
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
            switch (*c) {
                default:
                    return emit(defaultToken(c));
                case EOF:
                    return emit(eofToken());
                case '<':
                    dataState = ParserState::RAWTEXT_LESS_THAN_SIGN;
                    return false;
            }
        case ParserState::SCRIPT_DATA:
            switch (*c) {
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
            switch (*c) {
                default:
                    return emit(defaultToken(c));
                case EOF:
                    return emit(eofToken());
                case 0:
                    return emit(replacementToken());
            }
        case ParserState::TAG_OPEN:
            switch (*c) {
                case '!':
                    dataState = ParserState::MARKUP_DECLARATION_OPEN;
                    markup.clear();
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
            switch (*c) {
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
                    dataState = ParserState::BOGUS_COMMENT;
                    reconsume = true;
                    return false;
            }
        case ParserState::TAG_NAME:
            switch (*c) {
                case WHITESPACE:
                    dataState = ParserState::BEFORE_ATTRIBUTE_NAME;
                    return false;
                case '/':
                    dataState = ParserState::SELF_CLOSING_START_TAG;
                    return false;
                case ASCII_UPPER_ALPHA:
                    name += char(*c + 0x0020);
                    return false;
                case 0:
                    utf8::appendCodePoint(name, UNICODE_REPL);
                    return false;
                case EOF:
                    return emit(eofToken());
                default:
                    utf8::appendCodePoint(name, c);
                    return false;
            }
        case ParserState::RCDATA_LESS_THAN_SIGN:
            switch (*c) {
                case '/':
                    resetBuf();
                    dataState = ParserState::RCDATA_END_TAG_OPEN;
                    return false;
                default:
                    reconsume = true;
                    dataState = ParserState::RCDATA;
                    return emit(ltToken());
            }
        case ParserState::RCDATA_END_TAG_OPEN:
            switch (*c) {
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    resetBuf();
                    reconsume = true;
                    dataState = ParserState::RCDATA_END_TAG_NAME;
                    return false;
                default:
                    reconsume = true;
                    dataState = ParserState::RCDATA;
                    return emit(ltToken(), solidusToken());
            }
        case ParserState::RCDATA_END_TAG_NAME:
            switch (*c) {
                case WHITESPACE:
                    if (isGoodEndTag()) {
                        dataState = ParserState::BEFORE_ATTRIBUTE_NAME;
                        return false;
                    }
                    [[fallthrough]];
                case '/':
                    if (isGoodEndTag()) {
                        dataState = ParserState::SELF_CLOSING_START_TAG;
                        return false;
                    }
                    [[fallthrough]];
                case '>':
                    if (isGoodEndTag()) {
                        dataState = ParserState::DATA;
                        return emit(endTagToken());
                    }
                    [[fallthrough]];
                default:
                    dataState = ParserState::RCDATA;
                    reconsume = true;
                    emit(ltToken(), solidusToken());
                    return dumpBuf();
                case ASCII_UPPER_ALPHA: {
                    name += char(*c + 0x0020);
                    tempBuf += *c;
                    return false;
                }
                case ASCII_LOWER_ALPHA: {
                    name += *c;
                    tempBuf += *c;
                    return false;
                }
            }
        case ParserState::RAWTEXT_LESS_THAN_SIGN:
            switch (*c) {
                default:
                    reconsume = true;
                    dataState = ParserState::RAWTEXT;
                    return emit(ltToken());
                case '/':
                    resetBuf();
                    dataState = ParserState::RAWTEXT_END_TAG_OPEN;
                    return false;
            }
        case ParserState::RAWTEXT_END_TAG_OPEN:
            switch (*c) {
                case ASCII_UPPER_ALPHA:
                case ASCII_LOWER_ALPHA:
                    dataState = ParserState::RAWTEXT_END_TAG_NAME;
                    reconsume = true;
                    name.clear();
                    return false;
                default:
                    dataState = ParserState::RAWTEXT;
                    reconsume = true;
                    return emit(ltToken(), solidusToken());
            }
        case ParserState::RAWTEXT_END_TAG_NAME:
            switch (*c) {
                case WHITESPACE:
                    if (isGoodEndTag()) {
                        dataState = ParserState::BEFORE_ATTRIBUTE_NAME;
                        return false;
                    }
                    [[fallthrough]];
                case '/':
                    if (isGoodEndTag()) {
                        dataState = ParserState::SELF_CLOSING_START_TAG;
                        return false;
                    }
                    [[fallthrough]];
                case '>':
                    if (isGoodEndTag()) {
                        dataState = ParserState::DATA;
                        return emit(endTagToken());
                    }
                    [[fallthrough]];
                default:
                    reconsume = true;
                    dataState = ParserState::RAWTEXT;
                    emit(ltToken(), solidusToken());
                    return dumpBuf();
                case ASCII_UPPER_ALPHA: {
                    name += char(*c + 0x0020);
                    tempBuf += *c;
                    return false;
                }
                case ASCII_LOWER_ALPHA: {
                    char chr = *c;
                    name += chr;
                    tempBuf += chr;
                    return false;
                }
            }
        case ParserState::SCRIPT_DATA_LESS_THAN_SIGN:
            switch (*c) {
                default:
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA;
                    return emit(ltToken());
                case '/':
                    resetBuf();
                    dataState = ParserState::SCRIPT_DATA_END_TAG_OPEN;
                    return false;
                case '!':
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_START;
                    return emit(ltToken(), bangToken());
            }
        case ParserState::SCRIPT_DATA_END_TAG_OPEN:
            switch (*c) {
                default:
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA;
                    return emit(ltToken(), solidusToken());
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    name.clear();
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA_END_TAG_NAME;
                    return false;
            }
        case ParserState::SCRIPT_DATA_END_TAG_NAME:
            switch (*c) {
                case WHITESPACE:
                    if (isGoodEndTag()) {
                        dataState = ParserState::BEFORE_ATTRIBUTE_NAME;
                        return false;
                    }
                    [[fallthrough]];
                case '/':
                    if (isGoodEndTag()) {
                        dataState = ParserState::SELF_CLOSING_START_TAG;
                        return false;
                    }
                    [[fallthrough]];
                case '>':
                    if (isGoodEndTag()) {
                        dataState = ParserState::DATA;
                        return emit(endTagToken());
                    }
                    [[fallthrough]];
                default:
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA;
                    emit(ltToken(), solidusToken());
                    return dumpBuf();
                case ASCII_UPPER_ALPHA: {
                    name += char(*c + 0x0020);
                    tempBuf += *c;
                    return false;
                }
                case ASCII_LOWER_ALPHA: {
                    name += *c;
                    tempBuf += *c;
                    return false;
                }
            }
        case ParserState::SCRIPT_DATA_ESCAPE_START:
        case ParserState::SCRIPT_DATA_ESCAPE_START_DASH:
            switch (*c) {
                case '-':
                    dataState = dataState == ParserState::SCRIPT_DATA_ESCAPE_START
                                ? ParserState::SCRIPT_DATA_ESCAPE_START_DASH
                                : ParserState::SCRIPT_DATA_ESCAPE_DASH_DASH;
                    return emit(dashToken());
                default:
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA;
                    return false;
            }
        case ParserState::SCRIPT_DATA_ESCAPE:
            switch (*c) {
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
            switch (*c) {
                default:
                    dataState = ParserState::SCRIPT_DATA_ESCAPE;
                    return emit(defaultToken(c));
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
                    return false;
            }
        case ParserState::SCRIPT_DATA_ESCAPE_DASH_DASH:
            switch (*c) {
                default:
                    dataState = ParserState::SCRIPT_DATA_ESCAPE;
                    return emit(defaultToken(c));
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
            switch (*c) {
                default:
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA_ESCAPE;
                    return emit(ltToken());
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
            reconsume = true;
            switch (*c) {
                case ASCII_UPPER_ALPHA:
                case ASCII_LOWER_ALPHA:
                    name.clear();
                    dataState = ParserState::SCRIPT_DATA_ESCAPE_END_TAG_NAME;
                    return false;
                default:
                    dataState = ParserState::SCRIPT_DATA_ESCAPE;
                    return emit(ltToken(), solidusToken());
            }
        case ParserState::SCRIPT_DATA_ESCAPE_END_TAG_NAME:
            switch (*c) {
                case WHITESPACE:
                    if (isGoodEndTag()) {
                        dataState = ParserState::BEFORE_ATTRIBUTE_NAME;
                        return false;
                    }
                    [[fallthrough]];
                case '/':
                    if (isGoodEndTag()) {
                        dataState = ParserState::SELF_CLOSING_START_TAG;
                        return false;
                    }
                    [[fallthrough]];
                case '>':
                    if (isGoodEndTag()) {
                        dataState = ParserState::DATA;
                        return emit(endTagToken());
                    }
                    [[fallthrough]];
                default:
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA_ESCAPE;
                    emit(ltToken(), solidusToken());
                    return dumpBuf();
                case ASCII_UPPER_ALPHA: {
                    name += char(*c + 0x0020);
                    tempBuf += *c;
                    return false;
                }
                case ASCII_LOWER_ALPHA: {
                    char chr = *c;
                    name += chr;
                    tempBuf += chr;
                    return false;
                }
            }
        case ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_START:
            switch (*c) {
                default:
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA_ESCAPE;
                    return false;
                case WHITESPACE:
                case '/':
                case '>':
                    dataState = tempBuf.size() == 6 && tempBuf == "script"
                                ? ParserState::SCRIPT_DATA_ESCAPE
                                : ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(defaultToken(c));
                case ASCII_UPPER_ALPHA: {
                    char chr = char(*c + 0x0020);
                    tempBuf += chr;
                    return emit(defaultToken(c));
                }
                case ASCII_LOWER_ALPHA: {
                    tempBuf += *c;
                    return emit(defaultToken(c));
                }
            }
        case ParserState::SCRIPT_DATA_DOUBLE_ESCAPE:
            switch (*c) {
                default:
                    return emit(defaultToken(c));
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
            switch (*c) {
                default:
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(defaultToken(c));
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
            switch (*c) {
                default:
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(defaultToken(c));
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
            switch (*c) {
                default:
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return false;
                case '/':
                    resetBuf();
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_END;
                    return emit(solidusToken());
            }
        case ParserState::SCRIPT_DATA_DOUBLE_ESCAPE_END:
            switch (*c) {
                default:
                    reconsume = true;
                    dataState = ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return false;
                case WHITESPACE:
                case '/':
                case '>':
                    dataState = tempBuf == "script"
                                ? ParserState::SCRIPT_DATA_ESCAPE
                                : ParserState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(defaultToken(c));
                case ASCII_UPPER_ALPHA: {
                    tempBuf += char(*c + 0x0020);
                    return emit(defaultToken(c));
                }
                case ASCII_LOWER_ALPHA: {
                    tempBuf += *c;
                    return emit(defaultToken(c));
                }
            }
        case ParserState::BEFORE_ATTRIBUTE_NAME:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '/':
                case '>':
                    reconsume = true;
                    dataState = ParserState::AFTER_ATTRIBUTE_NAME;
                    return false;
                case '=':
                    resetAttr();
                    attr = '=';
                    dataState = ParserState::ATTRIBUTE_NAME;
                    return false;
                default:
                    resetAttr();
                    reconsume = true;
                    dataState = ParserState::ATTRIBUTE_NAME;
                    return false;
            }
        case ParserState::ATTRIBUTE_NAME:
            switch (*c) {
                case WHITESPACE:
                case '/':
                case '>':
                case EOF:
                    reconsume = true;
                    dataState = ParserState::AFTER_ATTRIBUTE_NAME;
                    return false;
                case '=':
                    dataState = ParserState::BEFORE_ATTRIBUTE_VALUE;
                    return false;
                case 0:
                    utf8::appendCodePoint(attr, UNICODE_REPL);
                    return false;
                case ASCII_UPPER_ALPHA:
                    attr += char(*c + 0x0020);
                    return false;
                case '"':
                case '\'':
                case '<':
                default:
                    utf8::appendCodePoint(attr, c);
                    return false;
            }
        case ParserState::AFTER_ATTRIBUTE_NAME:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '/':
                    dataState = ParserState::SELF_CLOSING_START_TAG;
                    return false;
                case '=':
                    dataState = ParserState::BEFORE_ATTRIBUTE_VALUE;
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return emit(startTagToken());
                case EOF:
                    return emit(eofToken());
                default:
                    resetAttr();
                    reconsume = true;
                    dataState = ParserState::ATTRIBUTE_NAME;
                    return false;
            }
        case ParserState::BEFORE_ATTRIBUTE_VALUE:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '"':
                    dataState = ParserState::DQUOTE_ATTRIBUTE_VALUE;
                    return false;
                case '\'':
                    dataState = ParserState::SQUOTE_ATTRIBUTE_VALUE;
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return emit(startTagToken());
                default:
                    reconsume = true;
                    dataState = ParserState::UQUOTE_ATTRIBUTE_VALUE;
                    return false;
            }
        case ParserState::DQUOTE_ATTRIBUTE_VALUE:
            switch (*c) {
                case '"':
                    dataState = ParserState::QUOTE_AFTER_ATTRIBUTE_VALUE;
                    return false;
                case '&':
                    returnState = ParserState::DQUOTE_ATTRIBUTE_VALUE;
                    dataState = ParserState::CHARACTER_REFERENCE;
                    return false;
                case 0:
                    utf8::appendCodePoint(val, UNICODE_REPL);
                    return false;
                case EOF:
                    return emit(eofToken());
                default:
                    utf8::appendCodePoint(val, c);
                    return false;
            }
        case ParserState::SQUOTE_ATTRIBUTE_VALUE:
            switch (*c) {
                case '\'':
                    dataState = ParserState::QUOTE_AFTER_ATTRIBUTE_VALUE;
                    return false;
                case '&':
                    returnState = ParserState::SQUOTE_ATTRIBUTE_VALUE;
                    dataState = ParserState::CHARACTER_REFERENCE;
                    return false;
                case 0:
                    utf8::appendCodePoint(val, UNICODE_REPL);
                    return false;
                case EOF:
                    return emit(eofToken());
                default:
                    utf8::appendCodePoint(val, c);
                    return false;
            }
        case ParserState::UQUOTE_ATTRIBUTE_VALUE:
            switch (*c) {
                case WHITESPACE:
                    dataState = ParserState::BEFORE_ATTRIBUTE_NAME;
                    return false;
                case '&':
                    returnState = ParserState::UQUOTE_ATTRIBUTE_VALUE;
                    dataState = ParserState::CHARACTER_REFERENCE;
                    return false;
                case 0:
                    utf8::appendCodePoint(val, UNICODE_REPL);
                    return false;
                case EOF:
                    return emit(eofToken());
                case '"':
                case '\'':
                case '<':
                case '=':
                case '`':
                    val += *c;
                    return false;
                default:
                    utf8::appendCodePoint(val, c);
                    return false;
            }
        case ParserState::QUOTE_AFTER_ATTRIBUTE_VALUE:
            switch (*c) {
                case WHITESPACE:
                    dataState = ParserState::BEFORE_ATTRIBUTE_NAME;
                    return false;
                case '/':
                    dataState = ParserState::SELF_CLOSING_START_TAG;
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return emit(startTagToken());
                case EOF:
                    return emit(eofToken());
                default:
                    reconsume = true;
                    dataState = ParserState::BEFORE_ATTRIBUTE_NAME;
                    return false;
            }
        case ParserState::SELF_CLOSING_START_TAG:
            switch (*c) {
                case '>':
                    selfClosing = true;
                    dataState = ParserState::DATA;
                    return emit(startTagToken());
                case EOF:
                    return emit(eofToken());
                default:
                    reconsume = true;
                    dataState = ParserState::BEFORE_ATTRIBUTE_NAME;
                    return false;
            }
        case ParserState::BOGUS_COMMENT:
            switch (*c) {
                case '>':
                    dataState = ParserState::DATA;
                    return emit(commentToken());
                case EOF:
                    return emit(commentToken(), eofToken());
                case 0:
                    utf8::appendCodePoint(comment, UNICODE_REPL);
                    return false;
                default:
                    utf8::appendCodePoint(comment, c);
                    return false;
            }
        case ParserState::MARKUP_DECLARATION_OPEN:
            if (markup.empty()) {
                switch (*c) {
                    case '-':
                        markup += '-';
                        markupType = TriValue::AUTO;
                        break;
                    case 'D':
                    case 'd':
                        markup += 'd';
                        markupType = TriValue::YES;
                        break;
                    case '[':
                        markup += '[';
                        markupType = TriValue::NO;
                        break;
                    default:
                        goto failMarkup;
                }
                return false;
            }
            switch (markupType) {
                case TriValue::AUTO:
                    if (*c != '-') {
                        markup += '-';
                        reconsumeMarkup = true;
                        dataState = ParserState::BOGUS_COMMENT;
                        break;
                    } else {
                        comment.clear();
                        dataState = ParserState::COMMENT_START;
                        break;
                    }
                    break;
                case TriValue::YES:
                    switch (markup.length()) {
                        case 1:
                        le('o', 'O', failMarkup);
                        case 2:
                        le('c', 'C', failMarkup);
                        case 3:
                        le('t', 'T', failMarkup);
                        case 4:
                        le('y', 'Y', failMarkup);
                        case 5:
                        le('p', 'P', failMarkup);
                        case 6:
                            if (*c != 'y' && *c != 'Y') goto failMarkup;
                            markup.clear();
                            dataState = ParserState::DOCTYPE;
                            break;
                    }
                    break;
                case TriValue::NO:
                    switch (markup.length()) {
                        case 1:
                            if (*c != 'C') goto failMarkup;
                            markup += 'C';
                            break;
                        case 2:
                            if (*c != 'D') goto failMarkup;
                            markup += 'D';
                            break;
                        case 3:
                            if (*c != 'A') goto failMarkup;
                            markup += 'A';
                            break;
                        case 4:
                            if (*c != 'T') goto failMarkup;
                            markup += 'T';
                            break;
                        case 5:
                            if (*c != 'A') goto failMarkup;
                            markup += 'A';
                            break;
                        case 6:
                            if (*c != '[') goto failMarkup;
                            markup.clear();
                            dataState = ParserState::CDATA_SECTION;
                            break;
                    }
                    break;
            }
            return false;
        failMarkup:
            reconsumeMarkup = true;
            dataState = ParserState::BOGUS_COMMENT;
            return false;
        case ParserState::COMMENT_START:
            switch (*c) {
                case '-':
                    dataState = ParserState::COMMENT_START_DASH;
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return emit(commentToken());
                default:
                    reconsume = true;
                    dataState = ParserState::COMMENT;
                    return false;
            }
        case ParserState::COMMENT_START_DASH:
            switch (*c) {
                case '-':
                    dataState = ParserState::COMMENT_END;
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return emit(commentToken());
                case EOF:
                    return emit(commentToken(), eofToken());
                default:
                    comment += '-';
                    reconsume = true;
                    dataState = ParserState::COMMENT;
                    return false;
            }
        case ParserState::COMMENT:
            switch (*c) {
                case '<':
                    comment += '<';
                    dataState = ParserState::COMMENT_LESS_THAN_SIGN;
                    return false;
                case '-':
                    dataState = ParserState::COMMENT_END_DASH;
                    return false;
                case 0:
                    utf8::appendCodePoint(comment, UNICODE_REPL);
                    return false;
                case EOF:
                    return emit(commentToken(), eofToken());
                default:
                    utf8::appendCodePoint(comment, c);
                    return false;
            }
        case ParserState::COMMENT_LESS_THAN_SIGN:
            switch (*c) {
                case '!':
                    comment += '!';
                    dataState = ParserState::COMMENT_LESS_THAN_SIGN_BANG;
                    return false;
                case '<':
                    comment += '<';
                    return false;
                default:
                    reconsume = true;
                    dataState = ParserState::COMMENT;
                    return false;
            }
        case ParserState::COMMENT_LESS_THAN_SIGN_BANG:
            switch (*c) {
                case '-':
                    dataState = ParserState::COMMENT_LESS_THAN_SIGN_BANG_DASH;
                    return false;
                default:
                    reconsume = true;
                    dataState = ParserState::COMMENT;
                    return false;
            }
        case ParserState::COMMENT_LESS_THAN_SIGN_BANG_DASH:
            switch (*c) {
                case '-':
                    dataState = ParserState::COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH;
                    return false;
                default:
                    reconsume = true;
                    dataState = ParserState::COMMENT_END_DASH;
                    return false;
            }
        case ParserState::COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH:
            switch (*c) {
                case '>':
                case EOF:
                default:
                    reconsume = true;
                    dataState = ParserState::COMMENT_END;
                    return false;
            }
        case ParserState::COMMENT_END_DASH:
            switch (*c) {
                case '-':
                    dataState = ParserState::COMMENT_END;
                    return false;
                case EOF:
                    return emit(commentToken(), eofToken());
                default:
                    comment += '-';
                    reconsume = true;
                    dataState = ParserState::COMMENT;
                    return false;
            }
        case ParserState::COMMENT_END:
            switch (*c) {
                case '>':
                    dataState = ParserState::DATA;
                    return emit(commentToken());
                case '!':
                    dataState = ParserState::COMMENT_END_BANG;
                    return false;
                case '-':
                    comment += '-';
                    return false;
                case EOF:
                    return emit(commentToken(), eofToken());
                default:
                    comment += '-';
                    comment += '-';
                    reconsume = true;
                    dataState = ParserState::COMMENT;
                    return false;
            }
        case ParserState::COMMENT_END_BANG:
            switch (*c) {
                case '-':
                    comment += "--!";
                    dataState = ParserState::COMMENT_END_DASH;
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return emit(commentToken());
                case EOF:
                    return emit(commentToken(), eofToken());
                default:
                    comment += "--!";
                    reconsume = true;
                    dataState = ParserState::COMMENT;
                    return false;
            }
#define edoc case EOF: forceQuirks = true; return emit(doctypeToken(), eofToken())
#define gdoc case '>': forceQuirks = true; dataState = ParserState::DATA; return emit(doctypeToken())
#define ddoc default: forceQuirks = true; reconsume = true; dataState = ParserState::BOGUS_DOCTYPE; return false
        case ParserState::DOCTYPE:
            switch (*c) {
                case WHITESPACE:
                    dataState = ParserState::BEFORE_DOCTYPE_NAME;
                    return false;
                case '>':
                default:
                    reconsume = true;
                    dataState = ParserState::BEFORE_DOCTYPE_NAME;
                    return false;
                edoc;
            }
        case ParserState::BEFORE_DOCTYPE_NAME:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case ASCII_UPPER_ALPHA:
                    resetDoctype();
                    name += char(*c + 0x0020);
                    nameMissing = false;
                    dataState = ParserState::DOCTYPE_NAME;
                    return false;
                case 0:
                    resetDoctype();
                    utf8::appendCodePoint(name, UNICODE_REPL);
                    nameMissing = false;
                    dataState = ParserState::DOCTYPE_NAME;
                    return false;
                case '>':
                    resetDoctype();
                    forceQuirks = true;
                    dataState = ParserState::DATA;
                    return emit(doctypeToken());
                case EOF:
                    resetDoctype();
                    forceQuirks = true;
                    return emit(doctypeToken(), eofToken());
                default:
                    resetDoctype();
                    utf8::appendCodePoint(name, c);
                    nameMissing = false;
                    dataState = ParserState::DOCTYPE_NAME;
                    return false;
            }
        case ParserState::DOCTYPE_NAME:
            switch (*c) {
                case WHITESPACE:
                    dataState = ParserState::AFTER_DOCTYPE_NAME;
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return emit(doctypeToken());
                case ASCII_UPPER_ALPHA:
                    name += char(*c + 0x0020);
                    return false;
                case 0:
                    utf8::appendCodePoint(name, UNICODE_REPL);
                    return false;
                edoc;
                default:
                    utf8::appendCodePoint(name, c);
                    return false;
            }
        case ParserState::AFTER_DOCTYPE_NAME:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return emit(doctypeToken());
                case EOF:
                    forceQuirks = true;
                    return emit(doctypeToken(), eofToken());
                default:
                    if (markup.empty()) {
                        if (*c == 'p' || *c == 'P')markup += 'p';
                        else if (*c == 's' || *c == 'S')markup += 's';
                        else goto failName;
                    } else {
                        switch (markup.length()) {
                            case 1:
                                if (markup.back() == 'p' && (*c == 'u' || *c == 'U')) markup += 'u';
                                else if (*c == 'y' || *c == 'Y') markup += 'y';
                                else goto failName;
                                break;
                            case 2:
                                if (markup.back() == 'u' && (*c == 'b' || *c == 'B')) markup += 'b';
                                else if (*c == 's' || *c == 'S') markup += 's';
                                else goto failName;
                                break;
                            case 3:
                                if (markup.back() == 'b' && (*c == 'l' || *c == 'L')) markup += 'l';
                                else if (*c == 't' || *c == 'T') markup += 't';
                                else goto failName;
                                break;
                            case 4:
                                if (markup.back() == 'l' && (*c == 'i' || *c == 'I')) markup += 'i';
                                else if (*c == 'e' || *c == 'E') markup += 'e';
                                else goto failName;
                                break;
                            case 5:
                                if (markup.back() == 'i' && (*c == 'c' || *c == 'C')) {
                                    markup.clear();
                                    dataState = ParserState::AFTER_DOCTYPE_PUBLIC_KEYWORD;
                                } else if (*c == 'm' || *c == 'M') {
                                    markup.clear();
                                    dataState = ParserState::AFTER_DOCTYPE_SYSTEM_KEYWORD;
                                } else goto failName;
                                break;
                            default:
                                goto failName;
                        }
                    }
                    return false;
                failName:
                    forceQuirks = true;
                    reconsumeMarkup = true;
                    dataState = ParserState::BOGUS_DOCTYPE;
                    return false;
            }
        case ParserState::AFTER_DOCTYPE_PUBLIC_KEYWORD:
            switch (*c) {
                case WHITESPACE:
                    dataState = ParserState::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                case '"':
                    publicIDMissing = false;
                    dataState = ParserState::DQUOTE_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                case '\'':
                    publicIDMissing = false;
                    dataState = ParserState::SQUOTE_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                gdoc;
                edoc;
                ddoc;
            }
        case ParserState::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '"':
                    publicIDMissing = false;
                    dataState = ParserState::DQUOTE_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                case '\'':
                    publicIDMissing = false;
                    dataState = ParserState::SQUOTE_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                gdoc;
                edoc;
                ddoc;
            }
        case ParserState::DQUOTE_DOCTYPE_PUBLIC_IDENTIFIER:
            switch (*c) {
                case '"':
                    dataState = ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                case 0:
                    utf8::appendCodePoint(publicID, UNICODE_REPL);
                    return false;
                gdoc;
                edoc;
                default:
                    utf8::appendCodePoint(publicID, c);
                    return false;
            }
        case ParserState::SQUOTE_DOCTYPE_PUBLIC_IDENTIFIER:
            switch (*c) {
                case '\'':
                    dataState = ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                case 0:
                    utf8::appendCodePoint(publicID, UNICODE_REPL);
                    return false;
                gdoc;
                edoc;
                default:
                    utf8::appendCodePoint(publicID, c);
                    return false;
            }
        case ParserState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER:
            switch (*c) {
                case WHITESPACE:
                    dataState = ParserState::BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS;
                    return false;
                case '"':
                    systemIDMissing = false;
                    dataState = ParserState::DQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '\'':
                    systemIDMissing = false;
                    dataState = ParserState::SQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return emit(doctypeToken());
                edoc;
                ddoc;
            }
        case ParserState::BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return emit(doctypeToken());
                case '"':
                    systemIDMissing = false;
                    dataState = ParserState::DQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '\'':
                    systemIDMissing = false;
                    dataState = ParserState::SQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                edoc;
                ddoc;
            }
        case ParserState::AFTER_DOCTYPE_SYSTEM_KEYWORD:
            switch (*c) {
                case WHITESPACE:
                    dataState = ParserState::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '"':
                    systemIDMissing = false;
                    dataState = ParserState::DQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '\'':
                    systemIDMissing = false;
                    dataState = ParserState::SQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                gdoc;
                edoc;
                ddoc;
            }
        case ParserState::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '"':
                    systemIDMissing = false;
                    dataState = ParserState::DQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '\'':
                    systemIDMissing = false;
                    dataState = ParserState::SQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                gdoc;
                edoc;
                ddoc;
            }
        case ParserState::DQUOTE_DOCTYPE_SYSTEM_IDENTIFIER:
            switch (*c) {
                case '"':
                    dataState = ParserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case 0:
                    utf8::appendCodePoint(systemID, UNICODE_REPL);
                    return false;
                gdoc;
                edoc;
                default:
                    utf8::appendCodePoint(systemID, c);
                    return false;
            }
        case ParserState::SQUOTE_DOCTYPE_SYSTEM_IDENTIFIER:
            switch (*c) {
                case '\'':
                    dataState = ParserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case 0:
                    utf8::appendCodePoint(systemID, UNICODE_REPL);
                    return false;
                gdoc;
                edoc;
                default:
                    utf8::appendCodePoint(systemID, c);
                    return false;
            }
        case ParserState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '>':
                    dataState = ParserState::DATA;
                    return emit(doctypeToken());
                edoc;
                default:
                    reconsume = true;
                    dataState = ParserState::BOGUS_DOCTYPE;
                    return false;
            }
        case ParserState::BOGUS_DOCTYPE:
            switch (*c) {
                case '>':
                    dataState = ParserState::DATA;
                    return emit(doctypeToken());
                case 0:
                default:
                    return false;
                case EOF:
                    return emit(doctypeToken(), eofToken());
            }
        case ParserState::CDATA_SECTION:
            switch (*c) {
                case ']':
                    dataState = ParserState::CDATA_SECTION_BRACKET;
                    return false;
                case EOF:
                    return emit(eofToken());
                default:
                    return emit(defaultToken(c));
            }
        case ParserState::CDATA_SECTION_BRACKET:
            switch (*c) {
                case ']':
                    dataState = ParserState::CDATA_SECTION_END;
                    return false;
                default:
                    reconsume = true;
                    dataState = ParserState::CDATA_SECTION;
                    return emit(rbToken());
            }
        case ParserState::CDATA_SECTION_END:
            switch (*c) {
                case ']':
                    return emit(rbToken());
                case '>':
                    dataState = ParserState::DATA;
                    return false;
                default:
                    reconsume = true;
                    dataState = ParserState::CDATA_SECTION;
                    return emit(rbToken(), rbToken());
            }
        case ParserState::CHARACTER_REFERENCE:
            tempBuf.clear();
            tempBuf += '&';
            switch (*c) {
                case ASCII_UPPER_ALPHA:
                case ASCII_LOWER_ALPHA:
                case ASCII_NUMERALS:
                    reconsume = true;
                    dataState = ParserState::NAMED_CHARACTER_REFERENCE;
                    return false;
                case '#':
                    tempBuf += '#';
                    dataState = ParserState::NUMERIC_CHARACTER_REFERENCE;
                    return false;
                default:
                    switch (returnState) {
                        case ParserState::DATA:
                        case ParserState::RCDATA:
                            return dumpBuf();
                        default:
                            val += tempBuf;
                            tempBuf.clear();
                            return false;
                    }
            }
        case ParserState::NAMED_CHARACTER_REFERENCE: {
            bool valid = false;
            if (utf8::isAscii(*c)) {
                if (tempBuf.empty()) {
                    markup.clear();
                    traverser = EscapeCodeTrieTraverser(
                            &headNodes[(*c > 'Z' ? *c - ('a' - ('Z' - 'A')) + 1 : *c) - 'A']
                    );
                    tempBuf += *c;
                    return false;
                } else {
                    if (traverser << *c) {
                        if (traverser) markup = tempBuf;
                        return false;
                    } else if (!markup.empty()) {
                        goto successCRef;
                    }
                }
            } else utf8::appendCodePoint(tempBuf, c);
            failCRef:
            successCRef:
            if (markup.back() != ';' && isAttributeState(returnState)) {
                dataState = returnState;
                return dumpBuf();
            } else {
                DOMString ref;
                ref.reserve(tempBuf.length());
                utf8::appendCodePoint(ref, getEscapeCodeMap().at(markup));
                tempBuf = tempBuf.substr(markup.length());
                markup.swap(ref);
                markup.swap(tempBuf);
                dataState = returnState;
                return dumpBuf();
            }
        }
    }
}