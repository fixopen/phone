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
                registerRule(sTarget, space, sTargetStop, &PIParser::recordTarget_);
                registerRule(sTarget, nonChr, sError);
                registerRule(sTarget, eof, sError);

                registerRule(sTargetStop, chr, sContent, &PIParser::recordPrevPosition_);
                registerRule(sTargetStop, eof, sError);
                registerRule(sTargetStop, questMark, sStartStop);

                registerRule(sContent, questMark, sStartStop);
                registerRule(sContent, eof, sError);

                registerRule(sStartStop, gt, sEnd, &PIParser::recordValue_);
                registerRule(sStartStop, chr, sContent);
                registerRule(sStartStop, eof, sError);
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
                registerRule(sStart, letterC, sC);
                registerRule(sStart, chr, sError);
                registerRule(sStart, eof, sError);

                registerRule(sC, letterD, sD);
                registerRule(sC, chr, sError);
                registerRule(sC, eof, sError);

                registerRule(sD, letterA, sA1);
                registerRule(sD, chr, sError);
                registerRule(sD, eof, sError);

                registerRule(sA1, letterT, sT);
                registerRule(sA1, chr, sError);
                registerRule(sA1, eof, sError);

                registerRule(sT, letterA, sA2);
                registerRule(sT, chr, sError);
                registerRule(sT, eof, sError);

                registerRule(sA2, leftSquare, sContent, &CDataParser::recordPosition_);
                registerRule(sA2, chr, sError);
                registerRule(sA2, eof, sError);

                registerRule(sContent, rightSquare, sStartStop);
                registerRule(sContent, eof, sError);

                registerRule(sStartStop, rightSquare, sContinueStop);
                registerRule(sStartStop, chr, sContent);
                registerRule(sStartStop, eof, sContent);

                registerRule(sContinueStop, gt, sEnd, &CDataParser::recordCDataSection_);
                registerRule(sContinueStop, chr, sContent);
                registerRule(sContinueStop, eof, sContent);
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
                registerRule(sStart, hyphen, sStartContent);
                registerRule(sStartContent, chr, sContent, &CommentParser::recordPrevPosition_);
                registerRule(sContent, hyphen, sStartStop);
                registerRule(sStartStop, hyphen, sContinueStop);
                registerRule(sContinueStop, gt, sEnd, &CommentParser::recordComment_);
                registerRule(sStartStop, chr, sContent);
                registerRule(sContinueStop, chr, sError);
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
                registerRule(sName, semicolon, sEnd, &EntityParser::convertEntity_);
                registerRule(sName, space, sError);
                registerRule(sName, eof, sError);
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
                registerRule(sTagName, space, sTagNameStop, &TagParser::recordTagName_);
                registerRule(sTagName, splash, sEmptyTagStart, &TagParser::recordTagName_);
                registerRule(sTagName, gt, sTagEnd, &TagParser::recordTagName_);
                registerRule(sTagName, nonChr, sError);
                registerRule(sTagName, eof, sError);

                registerRule(sTagNameStop, gt, sTagEnd);
                registerRule(sTagNameStop, splash, sEmptyTagStart);
                registerRule(sTagNameStop, chr, sAttrName, &TagParser::recordPrevPosition_);
                registerRule(sTagNameStop, eof, sError);

                registerRule(sAttrName, space, sAttrNameStop, &TagParser::recordAttrName_);
                registerRule(sAttrName, eq, sAttrEq, &TagParser::recordAttrName_);
                registerRule(sAttrName, eof, sError);

                registerRule(sAttrNameStop, eq, sAttrEq);
                registerRule(sAttrNameStop, nonEq, sError);
                registerRule(sAttrNameStop, eof, sError);

                registerRule(sAttrEq, quot, sAttrQuotValue, &TagParser::recordPosition_);
                registerRule(sAttrEq, apos, sAttrAposValue, &TagParser::recordPosition_);
                registerRule(sAttrEq, nonValueMark, sError);
                registerRule(sAttrEq, eof, sError);

                registerRule(sAttrQuotValue, quot, sAttrValueStop, &TagParser::recordAttrValue_);
                registerRule(sAttrQuotValue, eof, sError);

                registerRule(sAttrAposValue, apos, sAttrValueStop, &TagParser::recordAttrValue_);
                registerRule(sAttrAposValue, eof, sError);

                registerRule(sAttrValueStop, gt, sTagEnd);
                registerRule(sAttrValueStop, splash, sEmptyTagStart);
                registerRule(sAttrValueStop, space, sTagNameStop);
                registerRule(sAttrValueStop, chr, sError);
                registerRule(sAttrValueStop, eof, sError);

                registerRule(sEmptyTagStart, gt, sEmptyTagEnd);
                registerRule(sEmptyTagStart, chr, sError);
                registerRule(sEmptyTagStart, space, sError);
                registerRule(sEmptyTagStart, eof, sError);
        }

        TagParser::Event const TagParser::GetEventByChar(wchar_t const c, TagParser::State const state) const {
            Event result;
            switch (state) {
            case sTagName:
                result = nonChr;
                if (iswspace(c)) {
                    result = space;
                } else if(iswalnum(c) || (c == L':')) {
                    result = chr;
                } else if (c == L'/') {
                    result = splash;
                } else if(c == L'>') {
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
                registerRule(sIdle, chr, sText, &XMLParser::startText_);
                registerRule(sIdle, lt, sStage1);
                registerRule(sIdle, eof, sEnd);

                registerRule(sText, eof, sError);
                registerRule(sText, lt, sStage1, &XMLParser::recordText_);

                registerRule(sStage1, questMark, sPI, &XMLParser::startPIParser_);
                registerRule(sStage1, chr, sTag, &XMLParser::startTagParser_);
                registerRule(sStage1, splash, sEndTag, &XMLParser::recordPosition_);
                registerRule(sStage1, exclaimMark, sStage2);
                registerRule(sStage1, eof, sError);

                registerRule(sPI, eof, sEnd);
                registerRule(sPI, end, sIdle, &XMLParser::stopPIParser_);

                registerRule(sTag, eof, sError);
                registerRule(sTag, end, sIdle, &XMLParser::stopTagParser_);

                registerRule(sEndTag, eof, sError);
                registerRule(sEndTag, nonChr, sError);
                registerRule(sEndTag, gt, sIdle, &XMLParser::validAndInsert_);

                registerRule(sStage2, letterD, sDocType, &XMLParser::startDocTypeParser_);
                registerRule(sStage2, leftSquare, sCData, &XMLParser::startCDataParser_);
                registerRule(sStage2, hyphen, sComment, &XMLParser::startCommentParser_);
                registerRule(sStage2, eof, sError);

                registerRule(sDocType, eof, sEnd);
                registerRule(sDocType, end, sIdle, &XMLParser::stopDocTypeParser_);

                registerRule(sCData, eof, sError);
                registerRule(sCData, end, sIdle, &XMLParser::stopCDataParser_);

                registerRule(sComment, eof, sEnd);
                registerRule(sComment, end, sIdle, &XMLParser::stopCommentParser_);

                setStartState(sIdle);

                piParser_ = new PIParser();
                std::vector</*PIParser::State*/int> piEndStates;
                piEndStates.push_back(PIParser::sEnd);
                piParser_->setEndStates(piEndStates);
                piParser_->SetParentParser(this);

                tagParser_ = new TagParser();
                std::vector</*TagParser::State*/int> tagEndStates;
                tagEndStates.push_back(TagParser::sEmptyTagEnd);
                tagEndStates.push_back(TagParser::sTagEnd);
                tagParser_->setEndStates(tagEndStates);
                tagParser_->SetParentParser(this);

                docTypeParser_ = new DocTypeParser();
                std::vector</*DocTypeParser::State*/int> docTypeEndStates;
                docTypeEndStates.push_back(DocTypeParser::sEnd);
                docTypeParser_->setEndStates(docTypeEndStates);
                docTypeParser_->SetParentParser(this);

                cdataParser_ = new CDataParser();
                std::vector</*CDataParser::State*/int> cdataEndStates;
                cdataEndStates.push_back(CDataParser::sEnd);
                cdataParser_->setEndStates(cdataEndStates);
                cdataParser_->SetParentParser(this);

                commentParser_ = new CommentParser();
                std::vector</*CommentParser::State*/int> commentEndStates;
                commentEndStates.push_back(CommentParser::sEnd);
                commentParser_->setEndStates(commentEndStates);
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
                fireEvent(GetEventByChar(chr, static_cast<State>(getCurrentState())), 0);
            } while ((!source_->IsEnd()) && !isEnd());
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
                } if (c == EOF) {
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
            piParser_->setStartState(PIParser::sTarget);
            parent_ = current_;
            current_ = new Xml::ProcessingInstruction(parent_);
            piParser_->SetResult(current_);
            piParser_->recordPosition_(param);
            do {
                wchar_t chr = source_->GetChar();
                piParser_->fireEvent(piParser_->GetEventByChar(chr, static_cast<PIParser::State>(piParser_->getCurrentState())), 0);
            } while (!source_->IsEnd() && !piParser_->isEnd());
            if (piParser_->isEnd()) {
                fireEvent(end, 0);
            }
        }

        void XMLParser::startTagParser_(void* param) {
            tagParser_->recordPrevPosition_(param);
            tagParser_->setStartState(TagParser::sTagName);
            parent_ = current_;
            current_ = new Xml::Element(parent_);
            tagParser_->SetResult(current_);
            do {
                wchar_t chr = source_->GetChar();
                tagParser_->fireEvent(tagParser_->GetEventByChar(chr, static_cast<TagParser::State>(tagParser_->getCurrentState())), 0);
            } while (!source_->IsEnd() && !tagParser_->isEnd());
            if (tagParser_->isEnd()) {
                fireEvent(end, 0);
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
            switch (tagParser_->getCurrentState()) {
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
            docTypeParser_->setStartState(DocTypeParser::sStart);
            parent_ = current_;
            current_ = new Xml::DocType(parent_);
            docTypeParser_->SetResult(current_);
            do {
                wchar_t chr = source_->GetChar();
                docTypeParser_->fireEvent(docTypeParser_->GetEventByChar(chr, static_cast<DocTypeParser::State>(docTypeParser_->getCurrentState())), 0);
            } while (!source_->IsEnd() && !docTypeParser_->isEnd());
            if (docTypeParser_->isEnd()) {
                fireEvent(end, 0);
            }
        }

        void XMLParser::startCDataParser_(void* param) {
            cdataParser_->setStartState(CDataParser::sStart);
            parent_ = current_;
            current_ = new Xml::CDataSection(parent_);
            cdataParser_->SetResult(current_);
            do {
                wchar_t chr = source_->GetChar();
                cdataParser_->fireEvent(cdataParser_->GetEventByChar(chr, static_cast<CDataParser::State>(cdataParser_->getCurrentState())), 0);
            } while (!source_->IsEnd() && !cdataParser_->isEnd());
            if (cdataParser_->isEnd()) {
                fireEvent(end, 0);
            }
        }

        void XMLParser::startCommentParser_(void* param) {
            commentParser_->setStartState(CommentParser::sStart);
            parent_ = current_;
            current_ = new Xml::Comment(parent_);
            commentParser_->SetResult(current_);
            do {
                wchar_t chr = source_->GetChar();
                commentParser_->fireEvent(commentParser_->GetEventByChar(chr, static_cast<CommentParser::State>(commentParser_->getCurrentState())), 0);
            } while (!source_->IsEnd() && !commentParser_->isEnd());
            if (commentParser_->isEnd()) {
                fireEvent(end, 0);
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
