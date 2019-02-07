//
// Created by prem on 1/28/2019.
//

#include "parser/html_tokenizer.h"
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
#define ASCII_UPPER_HEX 'A':case'B':case'C':case'D':case'E':case'F'
#define ASCII_LOWER_HEX 'a':case'b':case'c':case'd':case'e':case'f'

bool HTMLStateMachine::operator<<(char *c) {
    reconsume = false;
    emitted.clear();
    switch (dataState) {
        case TokenizerState::DATA:
            switch (*c) {
                case 0:
                default:
                    return emit(defaultToken(c));
                case '&':
                    returnState = TokenizerState::DATA;
                    dataState = TokenizerState::CHARACTER_REFERENCE;
                    return false;
                case '<':
                    dataState = TokenizerState::TAG_OPEN;
                    return false;
                case EOF:
                    return emit(eofToken());
            }
        case TokenizerState::RCDATA:
            switch (*c) {
                default:
                    return emit(defaultToken(c));
                case '&':
                    returnState = TokenizerState::RCDATA;
                    dataState = TokenizerState::CHARACTER_REFERENCE;
                    return false;
                case '<':
                    dataState = TokenizerState::RCDATA_LESS_THAN_SIGN;
                    return false;
                case EOF:
                    return emit(eofToken());
                case 0:
                    return emit(replacementToken());
            }
        case TokenizerState::RAWTEXT:
            switch (*c) {
                default:
                    return emit(defaultToken(c));
                case EOF:
                    return emit(eofToken());
                case '<':
                    dataState = TokenizerState::RAWTEXT_LESS_THAN_SIGN;
                    return false;
            }
        case TokenizerState::SCRIPT_DATA:
            switch (*c) {
                default:
                    return emit(defaultToken(c));
                case EOF:
                    return emit(defaultToken(c));
                case 0:
                    return emit(replacementToken());
                case '<':
                    dataState = TokenizerState::SCRIPT_DATA_LESS_THAN_SIGN;
                    return false;
            }
        case TokenizerState::PLAINTEXT:
            switch (*c) {
                default:
                    return emit(defaultToken(c));
                case EOF:
                    return emit(eofToken());
                case 0:
                    return emit(replacementToken());
            }
        case TokenizerState::TAG_OPEN:
            switch (*c) {
                case '!':
                    dataState = TokenizerState::MARKUP_DECLARATION_OPEN;
                    markup.clear();
                    return false;
                case '/':
                    dataState = TokenizerState::END_TAG_OPEN;
                    return false;
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    name.clear();
                    return reconsumeChar(TokenizerState::TAG_NAME);
                case '?':
                    name.clear();
                    return reconsumeChar(TokenizerState::BOGUS_COMMENT);
                case EOF:
                    return emit(ltToken(), eofToken());
                default:
                    return emit(ltToken());
            }
        case TokenizerState::END_TAG_OPEN:
            switch (*c) {
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    name.clear();
                    return reconsumeChar(TokenizerState::TAG_NAME);
                case '>':
                    dataState = TokenizerState::DATA;
                    return false;
                case EOF:
                    return emit(ltToken(), solidusToken());
                default:
                    return reconsumeChar(TokenizerState::BOGUS_COMMENT);
            }
        case TokenizerState::TAG_NAME:
            switch (*c) {
                case WHITESPACE:
                    dataState = TokenizerState::BEFORE_ATTRIBUTE_NAME;
                    return false;
                case '/':
                    dataState = TokenizerState::SELF_CLOSING_START_TAG;
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
        case TokenizerState::RCDATA_LESS_THAN_SIGN:
            switch (*c) {
                case '/':
                    resetBuf();
                    dataState = TokenizerState::RCDATA_END_TAG_OPEN;
                    return false;
                default:
                    reconsumeChar(TokenizerState::RCDATA);
                    return emit(ltToken());
            }
        case TokenizerState::RCDATA_END_TAG_OPEN:
            switch (*c) {
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    resetBuf();
                    return reconsumeChar(TokenizerState::RCDATA_END_TAG_NAME);
                default:
                    reconsumeChar(TokenizerState::RCDATA);
                    return emit(ltToken(), solidusToken());
            }
        case TokenizerState::RCDATA_END_TAG_NAME:
            switch (*c) {
                case WHITESPACE:
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::BEFORE_ATTRIBUTE_NAME;
                        return false;
                    }
                    [[fallthrough]];
                case '/':
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::SELF_CLOSING_START_TAG;
                        return false;
                    }
                    [[fallthrough]];
                case '>':
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::DATA;
                        return emit(endTagToken());
                    }
                    [[fallthrough]];
                default:
                    reconsumeChar(TokenizerState::RCDATA);
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
        case TokenizerState::RAWTEXT_LESS_THAN_SIGN:
            switch (*c) {
                default:
                    reconsumeChar(TokenizerState::RAWTEXT);
                    return emit(ltToken());
                case '/':
                    resetBuf();
                    dataState = TokenizerState::RAWTEXT_END_TAG_OPEN;
                    return false;
            }
        case TokenizerState::RAWTEXT_END_TAG_OPEN:
            switch (*c) {
                case ASCII_UPPER_ALPHA:
                case ASCII_LOWER_ALPHA:
                    name.clear();
                    return reconsumeChar(TokenizerState::RAWTEXT_END_TAG_NAME);
                default:
                    reconsumeChar(TokenizerState::RAWTEXT);
                    return emit(ltToken(), solidusToken());
            }
        case TokenizerState::RAWTEXT_END_TAG_NAME:
            switch (*c) {
                case WHITESPACE:
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::BEFORE_ATTRIBUTE_NAME;
                        return false;
                    }
                    [[fallthrough]];
                case '/':
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::SELF_CLOSING_START_TAG;
                        return false;
                    }
                    [[fallthrough]];
                case '>':
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::DATA;
                        return emit(endTagToken());
                    }
                    [[fallthrough]];
                default:
                    reconsumeChar(TokenizerState::RAWTEXT);
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
        case TokenizerState::SCRIPT_DATA_LESS_THAN_SIGN:
            switch (*c) {
                default:
                    reconsumeChar(TokenizerState::SCRIPT_DATA);
                    return emit(ltToken());
                case '/':
                    resetBuf();
                    dataState = TokenizerState::SCRIPT_DATA_END_TAG_OPEN;
                    return false;
                case '!':
                    dataState = TokenizerState::SCRIPT_DATA_ESCAPE_START;
                    return emit(ltToken(), bangToken());
            }
        case TokenizerState::SCRIPT_DATA_END_TAG_OPEN:
            switch (*c) {
                default:
                    reconsumeChar(TokenizerState::SCRIPT_DATA);
                    return emit(ltToken(), solidusToken());
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    name.clear();
                    return reconsumeChar(TokenizerState::SCRIPT_DATA_END_TAG_NAME);
            }
        case TokenizerState::SCRIPT_DATA_END_TAG_NAME:
            switch (*c) {
                case WHITESPACE:
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::BEFORE_ATTRIBUTE_NAME;
                        return false;
                    }
                    [[fallthrough]];
                case '/':
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::SELF_CLOSING_START_TAG;
                        return false;
                    }
                    [[fallthrough]];
                case '>':
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::DATA;
                        return emit(endTagToken());
                    }
                    [[fallthrough]];
                default:
                    reconsumeChar(TokenizerState::SCRIPT_DATA);
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
        case TokenizerState::SCRIPT_DATA_ESCAPE_START:
        case TokenizerState::SCRIPT_DATA_ESCAPE_START_DASH:
            switch (*c) {
                case '-':
                    dataState = dataState == TokenizerState::SCRIPT_DATA_ESCAPE_START
                                ? TokenizerState::SCRIPT_DATA_ESCAPE_START_DASH
                                : TokenizerState::SCRIPT_DATA_ESCAPE_DASH_DASH;
                    return emit(dashToken());
                default:
                    return reconsumeChar(TokenizerState::SCRIPT_DATA);
            }
        case TokenizerState::SCRIPT_DATA_ESCAPE:
            switch (*c) {
                default:
                    return emit(defaultToken(c));
                case '-':
                    dataState = TokenizerState::SCRIPT_DATA_ESCAPE_DASH;
                    return emit(dashToken());
                case '<':
                    dataState = TokenizerState::SCRIPT_DATA_ESCAPE_LESS_THAN_SIGN;
                    return false;
                case 0:
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
            }
        case TokenizerState::SCRIPT_DATA_ESCAPE_DASH:
            switch (*c) {
                default:
                    dataState = TokenizerState::SCRIPT_DATA_ESCAPE;
                    return emit(defaultToken(c));
                case 0:
                    dataState = TokenizerState::SCRIPT_DATA_ESCAPE;
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
                case '-':
                    dataState = TokenizerState::SCRIPT_DATA_ESCAPE_DASH_DASH;
                    return emit(dashToken());
                case '<':
                    dataState = TokenizerState::SCRIPT_DATA_ESCAPE_LESS_THAN_SIGN;
                    return false;
            }
        case TokenizerState::SCRIPT_DATA_ESCAPE_DASH_DASH:
            switch (*c) {
                default:
                    dataState = TokenizerState::SCRIPT_DATA_ESCAPE;
                    return emit(defaultToken(c));
                case '-':
                    return emit(dashToken());
                case 0:
                    dataState = TokenizerState::SCRIPT_DATA_ESCAPE;
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
                case '<':
                    dataState = TokenizerState::SCRIPT_DATA_ESCAPE_LESS_THAN_SIGN;
                    return false;
                case '>':
                    dataState = TokenizerState::SCRIPT_DATA;
                    return emit(gtToken());
            }
        case TokenizerState::SCRIPT_DATA_ESCAPE_LESS_THAN_SIGN:
            switch (*c) {
                default:
                    reconsumeChar(TokenizerState::SCRIPT_DATA_ESCAPE);
                    return emit(ltToken());
                case '/':
                    dataState = TokenizerState::SCRIPT_DATA_ESCAPE_END_TAG_OPEN;
                    resetBuf();
                    return false;
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                    resetBuf();
                    reconsumeChar(TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_START);
                    return emit(ltToken());
            }
        case TokenizerState::SCRIPT_DATA_ESCAPE_END_TAG_OPEN:
            switch (*c) {
                case ASCII_UPPER_ALPHA:
                case ASCII_LOWER_ALPHA:
                    name.clear();
                    return reconsumeChar(TokenizerState::SCRIPT_DATA_ESCAPE_END_TAG_NAME);
                default:
                    reconsumeChar(TokenizerState::SCRIPT_DATA_ESCAPE);
                    return emit(ltToken(), solidusToken());
            }
        case TokenizerState::SCRIPT_DATA_ESCAPE_END_TAG_NAME:
            switch (*c) {
                case WHITESPACE:
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::BEFORE_ATTRIBUTE_NAME;
                        return false;
                    }
                    [[fallthrough]];
                case '/':
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::SELF_CLOSING_START_TAG;
                        return false;
                    }
                    [[fallthrough]];
                case '>':
                    if (isGoodEndTag()) {
                        dataState = TokenizerState::DATA;
                        return emit(endTagToken());
                    }
                    [[fallthrough]];
                default:
                    reconsumeChar(TokenizerState::SCRIPT_DATA_ESCAPE);
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
        case TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_START:
            switch (*c) {
                default:
                    return reconsumeChar(TokenizerState::SCRIPT_DATA_ESCAPE);
                case WHITESPACE:
                case '/':
                case '>':
                    dataState = tempBuf.size() == 6 && tempBuf == "script"
                                ? TokenizerState::SCRIPT_DATA_ESCAPE
                                : TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE;
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
        case TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE:
            switch (*c) {
                default:
                    return emit(defaultToken(c));
                case '-':
                    dataState = TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_DASH;
                    return emit(dashToken());
                case '<':
                    dataState = TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_LESS_THAN_SIGN;
                    return emit(ltToken());
                case 0:
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
            }
        case TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_DASH:
            switch (*c) {
                default:
                    dataState = TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(defaultToken(c));
                case '-':
                    dataState = TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_DASH_DASH;
                    return emit(dashToken());
                case '<':
                    dataState = TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_LESS_THAN_SIGN;
                    return emit(ltToken());
                case 0:
                    dataState = TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
            }
        case TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_DASH_DASH:
            switch (*c) {
                default:
                    dataState = TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(defaultToken(c));
                case '-':
                    return emit(dashToken());
                case '<':
                    dataState = TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_LESS_THAN_SIGN;
                    return emit(ltToken());
                case '>':
                    dataState = TokenizerState::SCRIPT_DATA;
                    return emit(gtToken());
                case 0:
                    dataState = TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE;
                    return emit(replacementToken());
                case EOF:
                    return emit(eofToken());
            }
        case TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_LESS_THAN_SIGN:
            switch (*c) {
                default:
                    return reconsumeChar(TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE);
                case '/':
                    resetBuf();
                    dataState = TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_END;
                    return emit(solidusToken());
            }
        case TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE_END:
            switch (*c) {
                default:
                    return reconsumeChar(TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE);
                case WHITESPACE:
                case '/':
                case '>':
                    dataState = tempBuf == "script"
                                ? TokenizerState::SCRIPT_DATA_ESCAPE
                                : TokenizerState::SCRIPT_DATA_DOUBLE_ESCAPE;
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
        case TokenizerState::BEFORE_ATTRIBUTE_NAME:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '/':
                case '>':
                    return reconsumeChar(TokenizerState::AFTER_ATTRIBUTE_NAME);
                case '=':
                    resetAttr();
                    attr = '=';
                    dataState = TokenizerState::ATTRIBUTE_NAME;
                    return false;
                default:
                    resetAttr();
                    return reconsumeChar(TokenizerState::ATTRIBUTE_NAME);
            }
        case TokenizerState::ATTRIBUTE_NAME:
            switch (*c) {
                case WHITESPACE:
                case '/':
                case '>':
                case EOF:
                    return reconsumeChar(TokenizerState::AFTER_ATTRIBUTE_NAME);
                case '=':
                    dataState = TokenizerState::BEFORE_ATTRIBUTE_VALUE;
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
        case TokenizerState::AFTER_ATTRIBUTE_NAME:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '/':
                    dataState = TokenizerState::SELF_CLOSING_START_TAG;
                    return false;
                case '=':
                    dataState = TokenizerState::BEFORE_ATTRIBUTE_VALUE;
                    return false;
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(startTagToken());
                case EOF:
                    return emit(eofToken());
                default:
                    resetAttr();
                    return reconsumeChar(TokenizerState::ATTRIBUTE_NAME);
            }
        case TokenizerState::BEFORE_ATTRIBUTE_VALUE:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '"':
                    dataState = TokenizerState::DQUOTE_ATTRIBUTE_VALUE;
                    return false;
                case '\'':
                    dataState = TokenizerState::SQUOTE_ATTRIBUTE_VALUE;
                    return false;
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(startTagToken());
                default:
                    return reconsumeChar(TokenizerState::UQUOTE_ATTRIBUTE_VALUE);
            }
        case TokenizerState::DQUOTE_ATTRIBUTE_VALUE:
            switch (*c) {
                case '"':
                    dataState = TokenizerState::QUOTE_AFTER_ATTRIBUTE_VALUE;
                    return false;
                case '&':
                    returnState = TokenizerState::DQUOTE_ATTRIBUTE_VALUE;
                    dataState = TokenizerState::CHARACTER_REFERENCE;
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
        case TokenizerState::SQUOTE_ATTRIBUTE_VALUE:
            switch (*c) {
                case '\'':
                    dataState = TokenizerState::QUOTE_AFTER_ATTRIBUTE_VALUE;
                    return false;
                case '&':
                    returnState = TokenizerState::SQUOTE_ATTRIBUTE_VALUE;
                    dataState = TokenizerState::CHARACTER_REFERENCE;
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
        case TokenizerState::UQUOTE_ATTRIBUTE_VALUE:
            switch (*c) {
                case WHITESPACE:
                    dataState = TokenizerState::BEFORE_ATTRIBUTE_NAME;
                    return false;
                case '&':
                    returnState = TokenizerState::UQUOTE_ATTRIBUTE_VALUE;
                    dataState = TokenizerState::CHARACTER_REFERENCE;
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
        case TokenizerState::QUOTE_AFTER_ATTRIBUTE_VALUE:
            switch (*c) {
                case WHITESPACE:
                    dataState = TokenizerState::BEFORE_ATTRIBUTE_NAME;
                    return false;
                case '/':
                    dataState = TokenizerState::SELF_CLOSING_START_TAG;
                    return false;
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(startTagToken());
                case EOF:
                    return emit(eofToken());
                default:
                    return reconsumeChar(TokenizerState::BEFORE_ATTRIBUTE_NAME);
            }
        case TokenizerState::SELF_CLOSING_START_TAG:
            switch (*c) {
                case '>':
                    selfClosing = true;
                    dataState = TokenizerState::DATA;
                    return emit(startTagToken());
                case EOF:
                    return emit(eofToken());
                default:
                    return reconsumeChar(TokenizerState::BEFORE_ATTRIBUTE_NAME);
            }
        case TokenizerState::BOGUS_COMMENT:
            switch (*c) {
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(commentToken());
                case EOF:
                    return emit(commentToken(), eofToken());
                case 0:
                    utf8::appendCodePoint(name, UNICODE_REPL);
                    return false;
                default:
                    utf8::appendCodePoint(name, c);
                    return false;
            }
        case TokenizerState::MARKUP_DECLARATION_OPEN:
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
                        dataState = TokenizerState::BOGUS_COMMENT;
                        break;
                    } else {
                        name.clear();
                        dataState = TokenizerState::COMMENT_START;
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
                            dataState = TokenizerState::DOCTYPE;
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
                            dataState = TokenizerState::CDATA_SECTION;
                            break;
                    }
                    break;
            }
            return false;
        failMarkup:
            reconsumeMarkup = true;
            dataState = TokenizerState::BOGUS_COMMENT;
            return false;
        case TokenizerState::COMMENT_START:
            switch (*c) {
                case '-':
                    dataState = TokenizerState::COMMENT_START_DASH;
                    return false;
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(commentToken());
                default:
                    return reconsumeChar(TokenizerState::COMMENT);
            }
        case TokenizerState::COMMENT_START_DASH:
            switch (*c) {
                case '-':
                    dataState = TokenizerState::COMMENT_END;
                    return false;
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(commentToken());
                case EOF:
                    return emit(commentToken(), eofToken());
                default:
                    name += '-';
                    return reconsumeChar(TokenizerState::COMMENT);
            }
        case TokenizerState::COMMENT:
            switch (*c) {
                case '<':
                    name += '<';
                    dataState = TokenizerState::COMMENT_LESS_THAN_SIGN;
                    return false;
                case '-':
                    dataState = TokenizerState::COMMENT_END_DASH;
                    return false;
                case 0:
                    utf8::appendCodePoint(name, UNICODE_REPL);
                    return false;
                case EOF:
                    return emit(commentToken(), eofToken());
                default:
                    utf8::appendCodePoint(name, c);
                    return false;
            }
        case TokenizerState::COMMENT_LESS_THAN_SIGN:
            switch (*c) {
                case '!':
                    name += '!';
                    dataState = TokenizerState::COMMENT_LESS_THAN_SIGN_BANG;
                    return false;
                case '<':
                    name += '<';
                    return false;
                default:
                    return reconsumeChar(TokenizerState::COMMENT);
            }
        case TokenizerState::COMMENT_LESS_THAN_SIGN_BANG:
            switch (*c) {
                case '-':
                    dataState = TokenizerState::COMMENT_LESS_THAN_SIGN_BANG_DASH;
                    return false;
                default:
                    return reconsumeChar(TokenizerState::COMMENT);
            }
        case TokenizerState::COMMENT_LESS_THAN_SIGN_BANG_DASH:
            switch (*c) {
                case '-':
                    dataState = TokenizerState::COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH;
                    return false;
                default:
                    return reconsumeChar(TokenizerState::COMMENT_END_DASH);
            }
        case TokenizerState::COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH:
            switch (*c) {
                case '>':
                case EOF:
                default:
                    return reconsumeChar(TokenizerState::COMMENT_END);
            }
        case TokenizerState::COMMENT_END_DASH:
            switch (*c) {
                case '-':
                    dataState = TokenizerState::COMMENT_END;
                    return false;
                case EOF:
                    return emit(commentToken(), eofToken());
                default:
                    name += '-';
                    return reconsumeChar(TokenizerState::COMMENT);
            }
        case TokenizerState::COMMENT_END:
            switch (*c) {
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(commentToken());
                case '!':
                    dataState = TokenizerState::COMMENT_END_BANG;
                    return false;
                case '-':
                    name += '-';
                    return false;
                case EOF:
                    return emit(commentToken(), eofToken());
                default:
                    name += "--";
                    return reconsumeChar(TokenizerState::COMMENT);
            }
        case TokenizerState::COMMENT_END_BANG:
            switch (*c) {
                case '-':
                    name += "--!";
                    dataState = TokenizerState::COMMENT_END_DASH;
                    return false;
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(commentToken());
                case EOF:
                    return emit(commentToken(), eofToken());
                default:
                    name += "--!";
                    return reconsumeChar(TokenizerState::COMMENT);
            }
#define edoc case EOF: forceQuirks = true; return emit(doctypeToken(), eofToken())
#define gdoc case '>': forceQuirks = true; dataState = TokenizerState::DATA; return emit(doctypeToken())
#define ddoc default: forceQuirks = true; return reconsumeChar(TokenizerState::BOGUS_DOCTYPE)
        case TokenizerState::DOCTYPE:
            switch (*c) {
                case WHITESPACE:
                    dataState = TokenizerState::BEFORE_DOCTYPE_NAME;
                    return false;
                case '>':
                default:
                    return reconsumeChar(TokenizerState::BEFORE_DOCTYPE_NAME);
                edoc;
            }
        case TokenizerState::BEFORE_DOCTYPE_NAME:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case ASCII_UPPER_ALPHA:
                    resetDoctype();
                    name += char(*c + 0x0020);
                    nameMissing = false;
                    dataState = TokenizerState::DOCTYPE_NAME;
                    return false;
                case 0:
                    resetDoctype();
                    utf8::appendCodePoint(name, UNICODE_REPL);
                    nameMissing = false;
                    dataState = TokenizerState::DOCTYPE_NAME;
                    return false;
                case '>':
                    resetDoctype();
                    forceQuirks = true;
                    dataState = TokenizerState::DATA;
                    return emit(doctypeToken());
                case EOF:
                    resetDoctype();
                    forceQuirks = true;
                    return emit(doctypeToken(), eofToken());
                default:
                    resetDoctype();
                    utf8::appendCodePoint(name, c);
                    nameMissing = false;
                    dataState = TokenizerState::DOCTYPE_NAME;
                    return false;
            }
        case TokenizerState::DOCTYPE_NAME:
            switch (*c) {
                case WHITESPACE:
                    dataState = TokenizerState::AFTER_DOCTYPE_NAME;
                    return false;
                case '>':
                    dataState = TokenizerState::DATA;
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
        case TokenizerState::AFTER_DOCTYPE_NAME:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '>':
                    dataState = TokenizerState::DATA;
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
                                    dataState = TokenizerState::AFTER_DOCTYPE_PUBLIC_KEYWORD;
                                } else if (*c == 'm' || *c == 'M') {
                                    markup.clear();
                                    dataState = TokenizerState::AFTER_DOCTYPE_SYSTEM_KEYWORD;
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
                    dataState = TokenizerState::BOGUS_DOCTYPE;
                    return false;
            }
        case TokenizerState::AFTER_DOCTYPE_PUBLIC_KEYWORD:
            switch (*c) {
                case WHITESPACE:
                    dataState = TokenizerState::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                case '"':
                    publicIDMissing = false;
                    dataState = TokenizerState::DQUOTE_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                case '\'':
                    publicIDMissing = false;
                    dataState = TokenizerState::SQUOTE_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                gdoc;
                edoc;
                ddoc;
            }
        case TokenizerState::BEFORE_DOCTYPE_PUBLIC_IDENTIFIER:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '"':
                    publicIDMissing = false;
                    dataState = TokenizerState::DQUOTE_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                case '\'':
                    publicIDMissing = false;
                    dataState = TokenizerState::SQUOTE_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                gdoc;
                edoc;
                ddoc;
            }
        case TokenizerState::DQUOTE_DOCTYPE_PUBLIC_IDENTIFIER:
            switch (*c) {
                case '"':
                    dataState = TokenizerState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                case 0:
                    utf8::appendCodePoint(attr, UNICODE_REPL);
                    return false;
                gdoc;
                edoc;
                default:
                    utf8::appendCodePoint(attr, c);
                    return false;
            }
        case TokenizerState::SQUOTE_DOCTYPE_PUBLIC_IDENTIFIER:
            switch (*c) {
                case '\'':
                    dataState = TokenizerState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
                    return false;
                case 0:
                    utf8::appendCodePoint(attr, UNICODE_REPL);
                    return false;
                gdoc;
                edoc;
                default:
                    utf8::appendCodePoint(attr, c);
                    return false;
            }
        case TokenizerState::AFTER_DOCTYPE_PUBLIC_IDENTIFIER:
            switch (*c) {
                case WHITESPACE:
                    dataState = TokenizerState::BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS;
                    return false;
                case '"':
                    systemIDMissing = false;
                    dataState = TokenizerState::DQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '\'':
                    systemIDMissing = false;
                    dataState = TokenizerState::SQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(doctypeToken());
                edoc;
                ddoc;
            }
        case TokenizerState::BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(doctypeToken());
                case '"':
                    systemIDMissing = false;
                    dataState = TokenizerState::DQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '\'':
                    systemIDMissing = false;
                    dataState = TokenizerState::SQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                edoc;
                ddoc;
            }
        case TokenizerState::AFTER_DOCTYPE_SYSTEM_KEYWORD:
            switch (*c) {
                case WHITESPACE:
                    dataState = TokenizerState::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '"':
                    systemIDMissing = false;
                    dataState = TokenizerState::DQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '\'':
                    systemIDMissing = false;
                    dataState = TokenizerState::SQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                gdoc;
                edoc;
                ddoc;
            }
        case TokenizerState::BEFORE_DOCTYPE_SYSTEM_IDENTIFIER:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '"':
                    systemIDMissing = false;
                    dataState = TokenizerState::DQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case '\'':
                    systemIDMissing = false;
                    dataState = TokenizerState::SQUOTE_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                gdoc;
                edoc;
                ddoc;
            }
        case TokenizerState::DQUOTE_DOCTYPE_SYSTEM_IDENTIFIER:
            switch (*c) {
                case '"':
                    dataState = TokenizerState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case 0:
                    utf8::appendCodePoint(val, UNICODE_REPL);
                    return false;
                gdoc;
                edoc;
                default:
                    utf8::appendCodePoint(val, c);
                    return false;
            }
        case TokenizerState::SQUOTE_DOCTYPE_SYSTEM_IDENTIFIER:
            switch (*c) {
                case '\'':
                    dataState = TokenizerState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
                    return false;
                case 0:
                    utf8::appendCodePoint(val, UNICODE_REPL);
                    return false;
                gdoc;
                edoc;
                default:
                    utf8::appendCodePoint(val, c);
                    return false;
            }
        case TokenizerState::AFTER_DOCTYPE_SYSTEM_IDENTIFIER:
            switch (*c) {
                case WHITESPACE:
                    return false;
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(doctypeToken());
                edoc;
                default:
                    return reconsumeChar(TokenizerState::BOGUS_DOCTYPE);
            }
        case TokenizerState::BOGUS_DOCTYPE:
            switch (*c) {
                case '>':
                    dataState = TokenizerState::DATA;
                    return emit(doctypeToken());
                case 0:
                default:
                    return false;
                case EOF:
                    return emit(doctypeToken(), eofToken());
            }
        case TokenizerState::CDATA_SECTION:
            switch (*c) {
                case ']':
                    dataState = TokenizerState::CDATA_SECTION_BRACKET;
                    return false;
                case EOF:
                    return emit(eofToken());
                default:
                    return emit(defaultToken(c));
            }
        case TokenizerState::CDATA_SECTION_BRACKET:
            switch (*c) {
                case ']':
                    dataState = TokenizerState::CDATA_SECTION_END;
                    return false;
                default:
                    reconsumeChar(TokenizerState::CDATA_SECTION);
                    return emit(rbToken());
            }
        case TokenizerState::CDATA_SECTION_END:
            switch (*c) {
                case ']':
                    return emit(rbToken());
                case '>':
                    dataState = TokenizerState::DATA;
                    return false;
                default:
                    reconsumeChar(TokenizerState::CDATA_SECTION);
                    return emit(rbToken(), rbToken());
            }
        case TokenizerState::CHARACTER_REFERENCE:
            tempBuf.clear();
            tempBuf += '&';
            switch (*c) {
                case ASCII_UPPER_ALPHA:
                case ASCII_LOWER_ALPHA:
                case ASCII_NUMERALS:
                    return reconsumeChar(TokenizerState::NAMED_CHARACTER_REFERENCE);
                case '#':
                    tempBuf += '#';
                    dataState = TokenizerState::NUMERIC_CHARACTER_REFERENCE;
                    return false;
                default:
                    switch (returnState) {
                        case TokenizerState::DATA:
                        case TokenizerState::RCDATA:
                            return dumpBuf();
                        default:
                            val += tempBuf;
                            tempBuf.clear();
                            return false;
                    }
            }
        case TokenizerState::NAMED_CHARACTER_REFERENCE: {
            if (utf8::isAscii(*c)) {
                if (tempBuf.empty()) {
                    markup.clear();
                    traverser = EscapeCodeTrieTraverser(
                            &getHeadNodes()[(*c > 'Z' ? *c - ('a' - ('Z' - 'A')) + 1 : *c) - 'A']
                    );
                    tempBuf += *c;
                    return false;
                } else {
                    if (traverser << *c) {
                        if (traverser) markup = tempBuf;
                        return false;
                    } else if (!markup.empty()) goto successCRef;
                }
            } else if (!markup.empty()) goto successCRef;
            markup.clear();
            markup.swap(tempBuf);
            markup += *c;
            reconsumeMarkup = true;
            return false;
            successCRef:
            if (skipCode(*c)) {
                dataState = returnState;
                return flushChars();
            } else {
                DOMString ref;
                ref.reserve(tempBuf.length());
                utf8::appendCodePoint(ref, getEscapeCodeMap().at(markup));
                tempBuf = tempBuf.substr(markup.length());
                markup.swap(ref);
                markup.swap(tempBuf);
                markup += *c;
                reconsumeMarkup = true;
                dataState = returnState;
                return flushChars();
            }
        }
        case TokenizerState::AMBIGUOUS_AMPERSAND:
            switch (*c) {
                case ASCII_LOWER_ALPHA:
                case ASCII_UPPER_ALPHA:
                case ASCII_NUMERALS:
                    if (isAttributeState(returnState)) {
                        utf8::appendCodePoint(val, c);
                        return false;
                    }
                    return emit(defaultToken(c));
                case ';':
                default:
                    return reconsumeChar(returnState);
            }
        case TokenizerState::NUMERIC_CHARACTER_REFERENCE:
            crc = 0;
            switch (*c) {
                case 'x':
                case 'X':
                    tempBuf += *c;
                    dataState = TokenizerState::HEXADECIMAL_CHARACTER_REFERENCE_START;
                    return false;
                default:
                    reconsume = true;
                    dataState = TokenizerState::DECIMAL_CHARACTER_REFERENCE_START;
                    return false;
            }
        case TokenizerState::HEXADECIMAL_CHARACTER_REFERENCE_START:
            switch (*c) {
                case ASCII_NUMERALS:
                case ASCII_LOWER_HEX:
                case ASCII_UPPER_HEX:
                    return reconsumeChar(TokenizerState::HEXADECIMAL_CHARACTER_REFERENCE);
                default:
                    return fakeFlushChars(c);
            }
        case TokenizerState::DECIMAL_CHARACTER_REFERENCE_START:
            switch (*c) {
                case ASCII_NUMERALS:
                    return reconsumeChar(TokenizerState::DECIMAL_CHARACTER_REFERENCE);
                default:
                    return fakeFlushChars(c);
            }
        case TokenizerState::HEXADECIMAL_CHARACTER_REFERENCE:
            switch (*c) {
                case ASCII_NUMERALS:
                    crc = crc * 16 + (*c - 0x0030);
                    return false;
                case ASCII_UPPER_HEX:
                    crc = crc * 16 + (*c - 0x0037);
                    return false;
                case ASCII_LOWER_HEX:
                    crc = crc * 16 + (*c - 0x0057);
                    return false;
                case ';':
                    dataState = TokenizerState::NUMERIC_CHARACTER_REFERENCE_END;
                    return false;
                default:
                    return reconsumeChar(TokenizerState::NUMERIC_CHARACTER_REFERENCE_END);
            }
        case TokenizerState::DECIMAL_CHARACTER_REFERENCE:
            switch (*c) {
                case ASCII_NUMERALS:
                    crc = crc * 10 + (*c - 0x0030);
                    return false;
                case ';':
                    dataState = TokenizerState::NUMERIC_CHARACTER_REFERENCE_END;
                    return false;
                default:
                    return reconsumeChar(TokenizerState::NUMERIC_CHARACTER_REFERENCE_END);
            }
        case TokenizerState::NUMERIC_CHARACTER_REFERENCE_END:
            if (crc > 0x10ffff || (crc >= 0xd800 && crc <= 0xdfff) || !crc) crc = UNICODE_REPL;
            else if (crc >= 0x80 && crc <= 0x9f) crc = UInt(getNumericEscapeCodes()[crc - 0x80]);
            tempBuf.clear();
            utf8::appendCodePoint(tempBuf, crc);
            dataState = returnState;
            return flushChars();
        default:
            return false;
    }
}