#include "XMLParser.h"
#include "../../Xml/Text.h"

#include <exception>
#include <cstdlib>
#include <cctype>
//#include <locale>

namespace Util {
    namespace Text {
        PIParser::PIParser()
            : BaseParser<PIParser>() {
            RegisterRule(sTarget, space, sTargetStop, &PIParser::recordTarget_);
            RegisterRule(sTarget, nonChr, sError);
            RegisterRule(sTarget, eof, sError);
            RegisterRule(sTargetStop, chr, sContent, &PIParser::recordPrevPosition_);
            RegisterRule(sTargetStop, eof, sError);
            RegisterRule(sTargetStop, questMark, sStartStop);
            RegisterRule(sContent, questMark, sStartStop);
            RegisterRule(sContent, eof, sError);
            RegisterRule(sStartStop, gt, sEnd, &PIParser::recordValue_);
            RegisterRule(sStartStop, chr, sContent);
            RegisterRule(sStartStop, eof, sError);
        }

        PIParser::Event const PIParser::GetEventByChar(wchar_t const c, PIParser::State const state) const {
            Event result;

            switch (state) {
                case sTarget:
                    result = chr;

                    if (c == EOF) {
                        result = eof;
                    } else if (iswspace(c)) {
                        result = space;
                    } else if (!iswalnum(c)) {
                        result = nonChr;
                    }

                    break;
                case sTargetStop:
                    result = chr;

                    if (iswspace(c)) {
                        result = space;
                    } else if (c == L'?') {
                        result = questMark;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sContent:
                    result = chr;

                    if (c == L'?') {
                        result = questMark;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sStartStop:
                    result = chr;

                    if (c == L'>') {
                        result = gt;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                default:
                    result = static_cast<Event>(c);
                    break;
            }

            return result;
        }

        CDataParser::CDataParser()
            : BaseParser<CDataParser>() {
            RegisterRule(sStart, letterC, sC);
            RegisterRule(sStart, chr, sError);
            RegisterRule(sStart, eof, sError);
            RegisterRule(sC, letterD, sD);
            RegisterRule(sC, chr, sError);
            RegisterRule(sC, eof, sError);
            RegisterRule(sD, letterA, sA1);
            RegisterRule(sD, chr, sError);
            RegisterRule(sD, eof, sError);
            RegisterRule(sA1, letterT, sT);
            RegisterRule(sA1, chr, sError);
            RegisterRule(sA1, eof, sError);
            RegisterRule(sT, letterA, sA2);
            RegisterRule(sT, chr, sError);
            RegisterRule(sT, eof, sError);
            RegisterRule(sA2, leftSquare, sContent, &CDataParser::recordPosition_);
            RegisterRule(sA2, chr, sError);
            RegisterRule(sA2, eof, sError);
            RegisterRule(sContent, rightSquare, sStartStop);
            RegisterRule(sContent, eof, sError);
            RegisterRule(sStartStop, rightSquare, sContinueStop);
            RegisterRule(sStartStop, chr, sContent);
            RegisterRule(sStartStop, eof, sContent);
            RegisterRule(sContinueStop, gt, sEnd, &CDataParser::recordCDataSection_);
            RegisterRule(sContinueStop, chr, sContent);
            RegisterRule(sContinueStop, eof, sContent);
        }

        CDataParser::Event const CDataParser::GetEventByChar(wchar_t const c, CDataParser::State const state) const {
            Event result;

            switch (state) {
                case sStart:
                    result = chr;

                    if (c == L'C') {
                        result = letterC;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sC:
                    result = chr;

                    if (c == L'D') {
                        result = letterD;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sD:
                    result = chr;

                    if (c == L'A') {
                        result = letterA;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sA1:
                    result = chr;

                    if (c == L'T') {
                        result = letterT;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sT:
                    result = chr;

                    if (c == L'A') {
                        result = letterA;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sA2:
                    result = chr;

                    if (c == L'[') {
                        result = leftSquare;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sContent:
                    result = chr;

                    if (c == L']') {
                        result = rightSquare;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sStartStop:
                    result = chr;

                    if (c == L']') {
                        result = rightSquare;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sContinueStop:
                    result = chr;

                    if (c == L'>') {
                        result = gt;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                default:
                    result = static_cast<Event>(c);
                    break;
            }

            return result;
        }

        CommentParser::CommentParser()
            : BaseParser<CommentParser>() {
            RegisterRule(sStart, hyphen, sStartContent);
            RegisterRule(sStartContent, chr, sContent, &CommentParser::recordPrevPosition_);
            RegisterRule(sContent, hyphen, sStartStop);
            RegisterRule(sStartStop, hyphen, sContinueStop);
            RegisterRule(sContinueStop, gt, sEnd, &CommentParser::recordComment_);
            RegisterRule(sStartStop, chr, sContent);
            RegisterRule(sContinueStop, chr, sError);
        }

        CommentParser::Event const CommentParser::GetEventByChar(wchar_t const c, CommentParser::State const state) const {
            Event result = chr;

            if (c == L'-') {
                result = hyphen;
            } else if (c == L'>') {
                result = gt;
            } else if (c == EOF) {
                result = eof;
            }

            return result;
        }

        EntityParser::EntityParser()
            : BaseParser<EntityParser>() {
            RegisterRule(sName, semicolon, sEnd, &EntityParser::convertEntity_);
            RegisterRule(sName, space, sError);
            RegisterRule(sName, eof, sError);
        }

        EntityParser::Event const EntityParser::GetEventByChar(wchar_t const c, EntityParser::State const state) const {
            Event result = chr; //static_cast<Event>(c);

            if (iswspace(c)) {
                result = space;
            } else if (c == EOF) {
                result = eof;
            } else if (c == L';') {
                result = semicolon;
            }

            return result;
        }

        void EntityParser::convertEntity_(void* param) {
            //recordContent_();
            //std::wstring entityName = GetContent();
            //replace name by content
        }

        TagParser::TagParser()
            : BaseParser<TagParser>() {
            RegisterRule(sTagName, space, sTagNameStop, &TagParser::recordTagName_);
            RegisterRule(sTagName, splash, sEmptyTagStart, &TagParser::recordTagName_);
            RegisterRule(sTagName, gt, sTagEnd, &TagParser::recordTagName_);
            RegisterRule(sTagName, nonChr, sError);
            RegisterRule(sTagName, eof, sError);
            RegisterRule(sTagNameStop, gt, sTagEnd);
            RegisterRule(sTagNameStop, splash, sEmptyTagStart);
            RegisterRule(sTagNameStop, chr, sAttrName, &TagParser::recordPrevPosition_);
            RegisterRule(sTagNameStop, eof, sError);
            RegisterRule(sAttrName, space, sAttrNameStop, &TagParser::recordAttrName_);
            RegisterRule(sAttrName, eq, sAttrEq, &TagParser::recordAttrName_);
            RegisterRule(sAttrName, eof, sError);
            RegisterRule(sAttrNameStop, eq, sAttrEq);
            RegisterRule(sAttrNameStop, nonEq, sError);
            RegisterRule(sAttrNameStop, eof, sError);
            RegisterRule(sAttrEq, quot, sAttrQuotValue, &TagParser::recordPosition_);
            RegisterRule(sAttrEq, apos, sAttrAposValue, &TagParser::recordPosition_);
            RegisterRule(sAttrEq, nonValueMark, sError);
            RegisterRule(sAttrEq, eof, sError);
            RegisterRule(sAttrQuotValue, quot, sAttrValueStop, &TagParser::recordAttrValue_);
            RegisterRule(sAttrQuotValue, eof, sError);
            RegisterRule(sAttrAposValue, apos, sAttrValueStop, &TagParser::recordAttrValue_);
            RegisterRule(sAttrAposValue, eof, sError);
            RegisterRule(sAttrValueStop, gt, sTagEnd);
            RegisterRule(sAttrValueStop, splash, sEmptyTagStart);
            RegisterRule(sAttrValueStop, space, sTagNameStop);
            RegisterRule(sAttrValueStop, chr, sError);
            RegisterRule(sAttrValueStop, eof, sError);
            RegisterRule(sEmptyTagStart, gt, sEmptyTagEnd);
            RegisterRule(sEmptyTagStart, chr, sError);
            RegisterRule(sEmptyTagStart, space, sError);
            RegisterRule(sEmptyTagStart, eof, sError);
        }

        TagParser::Event const TagParser::GetEventByChar(wchar_t const c, TagParser::State const state) const {
            Event result;

            switch (state) {
                case sTagName:
                    result = nonChr;

                    if (iswspace(c)) {
                        result = space;
                    } else if (iswalnum(c) || (c == L':')) {
                        result = chr;
                    } else if (c == L'/') {
                        result = splash;
                    } else if (c == L'>') {
                        result = gt;
                    } else if (c == L'=') {
                        result = eq;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sTagNameStop:
                    result = chr;

                    if (iswspace(c)) {
                        result = space;
                    } else if (c == L'/') {
                        result = splash;
                    } else if (c == L'>') {
                        result = gt;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sAttrName:
                    result = chr;

                    if (c == L'=') {
                        result = eq;
                    } else if (iswspace(c)) {
                        result = space;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sAttrNameStop:
                    result = nonEq;

                    if (iswspace(c)) {
                        result = space;
                    } else if (c == L'=') {
                        result = eq;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sAttrEq:
                    result = nonValueMark;

                    if (iswspace(c)) {
                        result = space;
                    } else if (c == L'"') {
                        result = quot;
                    } else if (c == L'\'') {
                        result = apos;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sAttrQuotValue:
                    result = chr;

                    if (c == L'"') {
                        result = quot;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sAttrAposValue:
                    result = chr;

                    if (c == L'\'') {
                        result = apos;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sAttrValueStop:
                    result = chr;

                    if (c == L'/') {
                        result = splash;
                    } else if (c == L'>') {
                        result = gt;
                    } else if (iswspace(c)) {
                        result = space;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                case sEmptyTagStart:
                    result = chr;

                    if (c == L'>') {
                        result = gt;
                    } else if (iswspace(c)) {
                        result = space;
                    } else if (c == EOF) {
                        result = eof;
                    }

                    break;
                default:
                    result = static_cast<Event>(c);
                    break;
            }

            return result;
        }

        DocTypeParser::DocTypeParser()
            : BaseParser<DocTypeParser>() {
        }

        DocTypeParser::Event const DocTypeParser::GetEventByChar(wchar_t const c, DocTypeParser::State const state) const {
            Event result = static_cast<Event>(c);

            if (iswspace(c) || iswalnum(c)) {
                result = chr;
            } else if (c == EOF) {
                result = eof;
            }

            return result;
        }

        //TextParser::TextParser()
        //: BaseParser<TextParser>() {
        //}

        //ElementParser::ElementParser()
        //: BaseParser<ElementParser>() {
        //}

        XMLParser::XMLParser()
            : BaseParser<XMLParser>()
            , parent_(0)
            , doc_(0)
            , current_(0) {
            RegisterRule(sIdle, chr, sText, &XMLParser::startText_);
            RegisterRule(sIdle, lt, sStage1);
            RegisterRule(sIdle, eof, sEnd);
            RegisterRule(sText, eof, sError);
            RegisterRule(sText, lt, sStage1, &XMLParser::recordText_);
            RegisterRule(sStage1, questMark, sPI, &XMLParser::startPIParser_);
            RegisterRule(sStage1, chr, sTag, &XMLParser::startTagParser_);
            RegisterRule(sStage1, splash, sEndTag, &XMLParser::recordPosition_);
            RegisterRule(sStage1, exclaimMark, sStage2);
            RegisterRule(sStage1, eof, sError);
            RegisterRule(sPI, eof, sEnd);
            RegisterRule(sPI, end, sIdle, &XMLParser::stopPIParser_);
            RegisterRule(sTag, eof, sError);
            RegisterRule(sTag, end, sIdle, &XMLParser::stopTagParser_);
            RegisterRule(sEndTag, eof, sError);
            RegisterRule(sEndTag, nonChr, sError);
            RegisterRule(sEndTag, gt, sIdle, &XMLParser::validAndInsert_);
            RegisterRule(sStage2, letterD, sDocType, &XMLParser::startDocTypeParser_);
            RegisterRule(sStage2, leftSquare, sCData, &XMLParser::startCDataParser_);
            RegisterRule(sStage2, hyphen, sComment, &XMLParser::startCommentParser_);
            RegisterRule(sStage2, eof, sError);
            RegisterRule(sDocType, eof, sEnd);
            RegisterRule(sDocType, end, sIdle, &XMLParser::stopDocTypeParser_);
            RegisterRule(sCData, eof, sError);
            RegisterRule(sCData, end, sIdle, &XMLParser::stopCDataParser_);
            RegisterRule(sComment, eof, sEnd);
            RegisterRule(sComment, end, sIdle, &XMLParser::stopCommentParser_);
            SetStartState(sIdle);
            piParser_ = new PIParser();
            std::vector </*PIParser::State*/int > piEndStates;
            piEndStates.push_back(PIParser::sEnd);
            piParser_->SetEndStates(piEndStates);
            piParser_->SetParentParser(this);
            tagParser_ = new TagParser();
            std::vector </*TagParser::State*/int > tagEndStates;
            tagEndStates.push_back(TagParser::sEmptyTagEnd);
            tagEndStates.push_back(TagParser::sTagEnd);
            tagParser_->SetEndStates(tagEndStates);
            tagParser_->SetParentParser(this);
            docTypeParser_ = new DocTypeParser();
            std::vector </*DocTypeParser::State*/int > docTypeEndStates;
            docTypeEndStates.push_back(DocTypeParser::sEnd);
            docTypeParser_->SetEndStates(docTypeEndStates);
            docTypeParser_->SetParentParser(this);
            cdataParser_ = new CDataParser();
            std::vector </*CDataParser::State*/int > cdataEndStates;
            cdataEndStates.push_back(CDataParser::sEnd);
            cdataParser_->SetEndStates(cdataEndStates);
            cdataParser_->SetParentParser(this);
            commentParser_ = new CommentParser();
            std::vector </*CommentParser::State*/int > commentEndStates;
            commentEndStates.push_back(CommentParser::sEnd);
            commentParser_->SetEndStates(commentEndStates);
            commentParser_->SetParentParser(this);
        }

        Util::shared_ptr<Xml::Document> const XMLParser::Parse(DocSource* const source) {
            SetSource(source);
            piParser_->SetSource(source);
            tagParser_->SetSource(source);
            docTypeParser_->SetSource(source);
            cdataParser_->SetSource(source);
            commentParser_->SetSource(source);
            doc_ = new Xml::Document();
            current_ = doc_;

            do {
                wchar_t chr = source_->GetChar();
                FireEvent(GetEventByChar(chr, static_cast<State>(GetCurrentState())), 0);
            } while ((!source_->IsEnd()) && !IsEnd());

            return Util::shared_ptr<Xml::Document>(doc_);
        }

        XMLParser::Event const XMLParser::GetEventByChar(wchar_t const c, XMLParser::State const state) const {
            Event result;

            switch (state) {
                case sIdle:
                    result = lt;

                    if (c == EOF) {
                        result = eof;
                    } else if (c != L'<') {
                        result = chr;
                    }

                    break;
                case sText:
                    result = chr;

                    if (c == EOF) {
                        result = eof;
                    } else if (c == L'<') {
                        result = lt;
                    }

                    break;
                case sStage1:
                    result = chr;

                    if (c == EOF) {
                        result = eof;
                    } else if (c == L'?') {
                        result = questMark;
                    } else if (c == L'/') {
                        result = splash;
                    } else if (c == L'!') {
                        result = exclaimMark;
                    }

                    break;
                case sStage2:
                    result = eof;

                    if (c == L'D') {
                        result = letterD;
                    } else if (c == L'[') {
                        result = leftSquare;
                    } else if (c == L'-') {
                        result = hyphen;
                    }

                    break;
                case sEndTag:
                    result = nonChr;

                    if (c == L'>') {
                        result = gt;
                    }

                    if (c == EOF) {
                        result = eof;
                    } else if (iswalnum(c) || (c == L':')) {
                        result = chr;
                    }

                    break;
                default:
                    break;
            }

            return result;
        }

        void XMLParser::startText_(void* param) {
            recordPrevPosition_(param);
            text_ = new Xml::Text(current_);
        }

        void XMLParser::recordText_(void* param) {
            recordContent_(reinterpret_cast<void*>(-1));
            std::wstring content = GetContent();

            if (current_ != doc_) {
                text_->value(content);
                current_->appendChild(text_);
            }
        }

        void XMLParser::startPIParser_(void* param) {
            piParser_->SetStartState(PIParser::sTarget);
            parent_ = current_;
            current_ = new Xml::ProcessingInstruction(parent_);
            piParser_->SetResult(current_);
            piParser_->recordPosition_(param);

            do {
                wchar_t chr = source_->GetChar();
                piParser_->FireEvent(piParser_->GetEventByChar(chr, static_cast<PIParser::State>(piParser_->GetCurrentState())), 0);
            } while (!source_->IsEnd() && !piParser_->IsEnd());

            if (piParser_->IsEnd()) {
                FireEvent(end, 0);
            }
        }

        void XMLParser::startTagParser_(void* param) {
            tagParser_->recordPrevPosition_(param);
            tagParser_->SetStartState(TagParser::sTagName);
            parent_ = current_;
            current_ = new Xml::Element(parent_);
            tagParser_->SetResult(current_);

            do {
                wchar_t chr = source_->GetChar();
                tagParser_->FireEvent(tagParser_->GetEventByChar(chr, static_cast<TagParser::State>(tagParser_->GetCurrentState())), 0);
            } while (!source_->IsEnd() && !tagParser_->IsEnd());

            if (tagParser_->IsEnd()) {
                FireEvent(end, 0);
            }
        }

        namespace {
            std::wstring const findValue(std::wstring const& content, std::wstring const& name) {
                std::wstring result;
                size_t pos = content.find(name);

                if (pos != std::wstring::npos) {
                    size_t startPos = content.find(L"\"", pos);
                    bool startWithQuot = true;

                    if (startPos == std::wstring::npos) {
                        startPos = content.find(L"'");

                        if (startPos == std::wstring::npos) {
                            throw std::exception("profile format error!");
                        }

                        startWithQuot = false;
                    }

                    std::wstring endWith = startWithQuot ? L"\"" : L"'";
                    ++startPos;
                    size_t endPos = content.find(endWith, startPos);
                    result = content.substr(startPos, endPos - startPos);
                }

                return result;
            }
        }

        void XMLParser::stopPIParser_(void* param) {
            if (parent_) {
                if (current_->name() == L"xml") {
                    //parse content
                    std::wstring content = current_->value();
                    std::wstring version = L"version";
                    std::wstring encoding = L"encoding";
                    std::wstring standalone = L"standalone";
                    doc_->version(findValue(content, version));
                    doc_->encoding(findValue(content, encoding));
                    doc_->standalone(findValue(content, version) == L"yes" ? true : false);
                    delete current_;
                    current_ = doc_;
                    parent_ = 0;
                } else {
                    parent_->appendChild(current_);
                    //delete current_;
                    current_ = parent_;
                    parent_ = current_->Parent();
                }
            }
        }

        void XMLParser::stopTagParser_(void* param) {
            switch (tagParser_->GetCurrentState()) {
                case TagParser::sEmptyTagEnd:
                    parent_->appendChild(current_);
                    //delete current_;
                    current_ = parent_;
                    parent_ = current_->Parent();
                    break;
                case TagParser::sTagEnd:
                    parent_->appendChild(current_);
                    break;
                default:
                    break;
            }
        }

        void XMLParser::validAndInsert_(void* param) {
            recordContent_(reinterpret_cast<void*>(-1));
            std::wstring tagName = GetContent();

            if (tagName == current_->name()) {
                //parent_->appendChild(current_);
                //delete current_;
                current_ = parent_;
                parent_ = current_->Parent();
            } else {
                throw std::exception("tag name not match!");
            }
        }

        void XMLParser::startDocTypeParser_(void* param) {
            docTypeParser_->SetStartState(DocTypeParser::sStart);
            parent_ = current_;
            current_ = new Xml::DocType(parent_);
            docTypeParser_->SetResult(current_);

            do {
                wchar_t chr = source_->GetChar();
                docTypeParser_->FireEvent(docTypeParser_->GetEventByChar(chr, static_cast<DocTypeParser::State>(docTypeParser_->GetCurrentState())), 0);
            } while (!source_->IsEnd() && !docTypeParser_->IsEnd());

            if (docTypeParser_->IsEnd()) {
                FireEvent(end, 0);
            }
        }

        void XMLParser::startCDataParser_(void* param) {
            cdataParser_->SetStartState(CDataParser::sStart);
            parent_ = current_;
            current_ = new Xml::CDataSection(parent_);
            cdataParser_->SetResult(current_);

            do {
                wchar_t chr = source_->GetChar();
                cdataParser_->FireEvent(cdataParser_->GetEventByChar(chr, static_cast<CDataParser::State>(cdataParser_->GetCurrentState())), 0);
            } while (!source_->IsEnd() && !cdataParser_->IsEnd());

            if (cdataParser_->IsEnd()) {
                FireEvent(end, 0);
            }
        }

        void XMLParser::startCommentParser_(void* param) {
            commentParser_->SetStartState(CommentParser::sStart);
            parent_ = current_;
            current_ = new Xml::Comment(parent_);
            commentParser_->SetResult(current_);

            do {
                wchar_t chr = source_->GetChar();
                commentParser_->FireEvent(commentParser_->GetEventByChar(chr, static_cast<CommentParser::State>(commentParser_->GetCurrentState())), 0);
            } while (!source_->IsEnd() && !commentParser_->IsEnd());

            if (commentParser_->IsEnd()) {
                FireEvent(end, 0);
            }
        }

        void XMLParser::stopDocTypeParser_(void* param) {
            if (parent_) {
                parent_->appendChild(current_);
                //delete current_;
                current_ = parent_;
                parent_ = current_->Parent();
            }
        }

        void XMLParser::stopCDataParser_(void* param) {
            if (parent_) {
                parent_->appendChild(current_);
                //delete current_;
                current_ = parent_;
                parent_ = current_->Parent();
            }
        }


        void XMLParser::stopCommentParser_(void* param) {
            if (parent_) {
                parent_->appendChild(current_);
                //delete current_;
                current_ = parent_;
                parent_ = current_->Parent();
            }
        }
    }
}
