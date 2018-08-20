#include "xmlLoad.h"
#include "Xml/Text.h"
#include "Xml/DocType.h"
#include "Xml/DataSection.h"
#include "Xml/Comment.h"
//#include "Xml/DocType.h"

#include <exception>
#include <cctype>

namespace Util {
    xmlLoad::xmlLoad()
        : parent_(0)
        , doc_(0)
        , current_(0)
        , currentElement_(0)
        , attr_(0) {
            piFsm_.registerRule(piTarget, space, piTargetStop, recordTarget);
            piFsm_.registerRule(piTargetStop, chr, piContent, recordPrevPosition);
            piFsm_.registerRule(piContent, questMark, piStartStop);
            piFsm_.registerRule(piStartStop, gt, piEnd, recordContent);
            piFsm_.registerRule(piStartStop, chr, piContent);
            piFsm_.registerRule(piTarget, eof, piError);

            cdataFsm_.registerRule(cdataStart, letterC, cdataC);
            cdataFsm_.registerRule(cdataC, letterD, cdataD);
            cdataFsm_.registerRule(cdataD, letterA, cdataA1);
            cdataFsm_.registerRule(cdataA1, letterT, cdataT);
            cdataFsm_.registerRule(cdataT, letterA, cdataA2);
            cdataFsm_.registerRule(cdataA2, leftSquare, cdataContent, recordPosition);
            cdataFsm_.registerRule(cdataContent, rightSquare, cdataStartStop);
            cdataFsm_.registerRule(cdataStartStop, rightSquare, cdataContinueStop);
            cdataFsm_.registerRule(cdataContinueStop, gt, cdataEnd, recordCdataContent);
            cdataFsm_.registerRule(cdataStart, chr, cdataError);
            cdataFsm_.registerRule(cdataC, chr, cdataError);
            cdataFsm_.registerRule(cdataD, chr, cdataError);
            cdataFsm_.registerRule(cdataA1, chr, cdataError);
            cdataFsm_.registerRule(cdataT, chr, cdataError);
            cdataFsm_.registerRule(cdataA2, chr, cdataError);
            cdataFsm_.registerRule(cdataStartStop, chr, cdataContent);
            cdataFsm_.registerRule(cdataContinueStop, chr, cdataContent);

            commentFsm_.registerRule(commentStart, hyphen, commentStartContent);
            commentFsm_.registerRule(commentStartContent, chr, commentContent, recordPrevPosition);
            commentFsm_.registerRule(commentContent, hyphen, commentStartStop);
            commentFsm_.registerRule(commentStartStop, hyphen, commentContinueStop);
            commentFsm_.registerRule(commentContinueStop, gt, commentEnd, recordCommentContent);
            commentFsm_.registerRule(commentStartStop, chr, commentContent);
            commentFsm_.registerRule(commentContinueStop, chr, commentError);

            entityFsm_.registerRule(entityName, semicolon, entityEnd, convertEntity);
            entityFsm_.registerRule(entityName, space, entityError);

            tagFsm_.registerRule(tagTagName, space, tagTagNameStop, recordTagName);
            tagFsm_.registerRule(tagTagName, splash, tagEmptyTagStart, recordTagName);
            tagFsm_.registerRule(tagTagName, gt, tagTagEnd, recordTagName);
            tagFsm_.registerRule(tagTagNameStop, gt, tagTagEnd);
            tagFsm_.registerRule(tagTagNameStop, splash, tagEmptyTagStart);
            tagFsm_.registerRule(tagTagNameStop, chr, tagAttrName, recordPrevPosition);
            tagFsm_.registerRule(tagAttrName, space, tagAttrNameStop, recordAttrName);
            tagFsm_.registerRule(tagAttrName, eq, tagAttrEq, recordAttrName);
            tagFsm_.registerRule(tagAttrNameStop, eq, tagAttrEq);
            tagFsm_.registerRule(tagAttrEq, quot, tagAttrQuotValue, recordPosition);
            tagFsm_.registerRule(tagAttrEq, apos, tagAttrAposValue, recordPosition);
            tagFsm_.registerRule(tagAttrQuotValue, quot, tagAttrValueStop, recordAttrValue);
            tagFsm_.registerRule(tagAttrValueStop, gt, tagTagEnd);
            tagFsm_.registerRule(tagAttrValueStop, splash, tagEmptyTagStart);
            tagFsm_.registerRule(tagAttrValueStop, space, tagTagNameStop);
            tagFsm_.registerRule(tagEmptyTagStart, gt, tagEmptyTagEnd);
            tagFsm_.registerRule(tagEmptyTagStart, chr, tagError);
            tagFsm_.registerRule(tagEmptyTagStart, space, tagError);
            tagFsm_.registerRule(tagTagName, eof, tagError);
            tagFsm_.registerRule(tagTagNameStop, eof, tagError);
            tagFsm_.registerRule(tagAttrName, eof, tagError);
            tagFsm_.registerRule(tagAttrNameStop, eof, tagError);
            tagFsm_.registerRule(tagAttrEq, eof, tagError);
            tagFsm_.registerRule(tagAttrQuotValue, eof, tagError);
            tagFsm_.registerRule(tagAttrAposValue, eof, tagError);
            //tagFsm_.registerRule(tagTagEnd, eof, tagTagEnd);
            //tagFsm_.registerRule(tagEmptyTagEnd, eof, tagEmptyTagEnd);

            fsm_.registerRule(docIdle, chr, docText, textProc);
            fsm_.registerRule(docIdle, space, docIdle, textProc);
            fsm_.registerRule(docIdle, lt, docStage1);
            fsm_.registerRule(docStage1, questMark, docPI, startPiFsm);
            fsm_.registerRule(docStage1, chr, docElement, elementProc);
            fsm_.registerRule(docStage1, splash, docEndTag, validAndInsert);
            fsm_.registerRule(docStage1, exclaimMark, docStage2);
            fsm_.registerRule(docStage2, letterD, docDocType, startDocTypeFsm);
            fsm_.registerRule(docStage2, leftSquare, docCdata, startCdataFsm);
            fsm_.registerRule(docStage2, hyphen, docComment, startCommentFsm);
            fsm_.registerRule(docIdle, eof, docEnd);
            fsm_.registerRule(docText, eof, docError);
            fsm_.registerRule(docStage1, eof, docError);
            fsm_.registerRule(docPI, eof, docEnd);
            fsm_.registerRule(docElement, eof, docError);
            fsm_.registerRule(docEndTag, eof, docEnd);
            fsm_.registerRule(docStage2, eof, docError);
            fsm_.registerRule(docDocType, eof, docEnd);
            fsm_.registerRule(docCdata, eof, docError);
            fsm_.registerRule(docComment, eof, docEnd);
    }

    xmlLoad::~xmlLoad() {
    }

    void xmlLoad::SetStartPosition(int const pos) {
        startPos_ = pos;
    }

    void xmlLoad::SetStopPosition(int const pos) {
        stopPos_ = pos;
    }

    std::wstring const xmlLoad::GetContent() const {
        return source_->GetContent(startPos_, stopPos_);
    }

    Util::shared_ptr<Xml::Document> const xmlLoad::parse(Util::shared_ptr<DocSource>& source) {
        source_ = source;
        doc_ = new Xml::Document();
        current_ = doc_;
        getEventByChar_ = getCommonEventByChar;
        chr_ = source_->GetChar();
        fsm_.setStartState(docIdle);
        do {
            fsm_.fireEvent(getEventByChar_(chr_), this);
            chr_ = source_->GetChar();
        } while (!source_->IsEnd());
        return Util::shared_ptr<Xml::Document>(doc_);
    }

    void xmlLoad::parseToElement_() {
        //parse tagName and attributes
        TagState ts = parseTag_();
        switch (ts) {
    case tagEmptyTagEnd:
        fsm_.setStartState(docIdle);
        //rewind to uplevel
        parent_->appendChild(current_);
        //parent_->appendChild(currentElement_);
        current_ = parent_;
        currentElement_ = dynamic_cast<Xml::Element*>(current_);
        parent_ = current_->Parent();
        break;
    case tagTagEnd:
        fsm_.setStartState(docIdle);
        do {
            fsm_.fireEvent(getEventByChar_(chr_), this);
            chr_ = source_->GetChar();
        } while (!source_->IsEnd());
        break;
        }
    }

    TagState const xmlLoad::parseTag_() {
        chr_ = source_->GetChar();
        tagFsm_.setStartState(tagTagName);
        std::vector<int> tagEndStates;
        tagEndStates.push_back(tagEmptyTagEnd);
        tagEndStates.push_back(tagTagEnd);
        tagFsm_.setEndNotify(tagEndStates);
        do {
            tagFsm_.fireEvent(getEventByChar_(chr_), this);
            chr_ = source_->GetChar();
        } while (!tagFsm_.isEnd() && !source_->IsEnd());
        return static_cast<TagState>(tagFsm_.getCurrentState());
    }

    void textProc(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //record start position
        load->SetStartPosition(load->source_->GetPosition() - 1);
        //create node
        Xml::Text* t = new Xml::Text(load->current_);
        //fill node field
        t->name(L"#text");
        do {
            load->chr_ = load->source_->GetChar();
        } while ((load->chr_ != L'<') && (load->chr_ != EOF));
        load->SetStopPosition(load->source_->GetPosition() - 1);
        t->value(load->GetContent());
        //append node to parent
        if (load->current_ != load->doc_) {
            load->current_->appendChild(t);
        }
        //reset state machine
        load->fsm_.setStartState(docStage1);
    }

    void endTextProc(void* param) {
    }

    void startPiFsm(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //record start position
        load->SetStartPosition(load->source_->GetPosition());
        //create node
        load->parent_ = load->current_;
        load->current_ = new Xml::ProcessingInstruction(load->parent_);
        //setup sub state machine
        load->piFsm_.setStartState(piTarget);
        std::vector<int> piEndStates;
        piEndStates.push_back(piEnd);
        load->piFsm_.setEndNotify(piEndStates, endPiHandle);
        load->fsm_.activeChild(&load->piFsm_);
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

    void endPiHandle(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //teardown sub state machine
        load->fsm_.clearChild();
        //append node to parent
        if (load->parent_) {
            if (load->current_->name() == L"xml") {
                //parse content
                std::wstring content = load->current_->value();
                std::wstring version = L"version";
                std::wstring encoding = L"encoding";
                std::wstring standalone = L"standalone";
                load->doc_->version(findValue(content, version));
                load->doc_->encoding(findValue(content, encoding));
                load->doc_->standalone(findValue(content, version) == L"yes" ? true : false);
                delete load->current_;
                load->current_ = load->doc_;
                load->parent_ = 0;
            } else {
                load->parent_->appendChild(load->current_);
                load->current_ = load->parent_;
                load->parent_ = load->current_->Parent();
            }
        }
        //reset state machine
        load->fsm_.setStartState(docIdle);
    }

    void elementProc(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //record start position
        load->SetStartPosition(load->source_->GetPosition() - 1);
        //create node and record context
        load->parent_ = load->current_;
        Xml::Element* newElement = new Xml::Element(load->parent_);
        load->currentElement_ = newElement;
        load->current_ = load->currentElement_;

        //start parse
        load->parseToElement_();
    }

    void validAndInsert(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //get content
        std::wstring tagName;
        load->SetStartPosition(load->source_->GetPosition());
        Event e = chr;
        do {
            load->chr_ = load->source_->GetChar();
            e = load->getEventByChar_(load->chr_);
            if (e == space) {
                load->SetStopPosition(load->source_->GetPosition() - 1);
                tagName = load->GetContent();
            }
        } while (load->chr_ != L'>');
        if (tagName == L"") {
            load->SetStopPosition(load->source_->GetPosition() - 1);
            tagName = load->GetContent();
        }
        //valid node
        if (tagName != load->current_->name()) {
            throw std::exception("tag name not match!");
        } else {
            //rewind to uplevel
            load->parent_->appendChild(load->current_);
            load->current_ = load->parent_;
            load->parent_ = load->current_->Parent();
        }
        //reset state machine
        load->fsm_.setStartState(docIdle);
    }

    void startDocTypeFsm(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //record start position
        load->SetStartPosition(load->source_->GetPosition());
        //create node
        load->parent_ = load->current_;
        load->current_ = new Xml::DocType(load->parent_);
        //setup sub state machine
        load->docTypeFsm_.setStartState(docTypeStart);
        std::vector<int> docTypeEndStates;
        docTypeEndStates.push_back(docTypeEnd);
        load->docTypeFsm_.setEndNotify(docTypeEndStates, endDocTypeHandle);
        load->fsm_.activeChild(&load->docTypeFsm_);
    }

    void endDocTypeHandle(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //teardown sub state machine
        load->fsm_.clearChild();
        //append node to parent
        //load->doc_->DocType();
        //reset state machine
        load->fsm_.setStartState(docIdle);
    }

    void startCdataFsm(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //record start position
        load->SetStartPosition(load->source_->GetPosition());
        //create node
        load->parent_ = load->current_;
        load->current_ = new Xml::CDataSection(load->parent_);
        //setup sub state machine
        load->cdataFsm_.setStartState(cdataStart);
        std::vector<int> cdataEndStates;
        cdataEndStates.push_back(cdataEnd);
        load->cdataFsm_.setEndNotify(cdataEndStates, endCdataHandle);
        load->fsm_.activeChild(&load->cdataFsm_);
        load->getEventByChar_ = getCDataEventByChar;
    }

    void endCdataHandle(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //teardown sub state machine
        load->fsm_.clearChild();
        load->getEventByChar_ = getCommonEventByChar;
        //rewind to uplevel
        load->parent_->appendChild(load->current_);
        load->current_ = load->parent_;
        load->parent_ = load->parent_->Parent();
        //reset state machine
        load->fsm_.setStartState(docIdle);
    }

    void startCommentFsm(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //record start position
        load->SetStartPosition(load->source_->GetPosition());
        //setup sub state machine
        load->commentFsm_.setStartState(commentStart);
        std::vector<int> commentEndStates;
        commentEndStates.push_back(commentEnd);
        load->commentFsm_.setEndNotify(commentEndStates, endCommentHandle);
        load->fsm_.activeChild(&load->commentFsm_);
        //create node
        load->parent_ = load->current_;
        load->current_ = new Xml::Comment(load->parent_);
        load->current_->name(L"#comment");
    }

    void endCommentHandle(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //teardown sub state machine
        load->fsm_.clearChild();
        //reset state machine
        load->fsm_.setStartState(docIdle);
        //rewind to uplevel
        load->parent_->appendChild(load->current_);
        load->current_ = load->parent_;
        load->parent_ = load->parent_->Parent();
    }

    void recordTagName(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        load->SetStopPosition(load->source_->GetPosition() - 1);
        load->current_->name(load->GetContent());
    }

    //void startAttrFsm(void* param) {
    //	xmlLoad* load = static_cast<xmlLoad*>(param);
    //	//record start position
    //	load->SetStartPosition(load->source_->GetPosition() - 1);
    //	//setup sub state machine
    //	load->attrFsm_.setStartState(attrName);
    //	std::vector<int> attrEndStates;
    //	attrEndStates.push_back(attrValueStop);
    //	load->attrFsm_.setEndNotify(attrEndStates, endAttrHandle);
    //	load->tagFsm_.activeChild(&load->attrFsm_);
    //	//create node
    //}

    //void endAttrHandle(void* param) {
    //	xmlLoad* load = static_cast<xmlLoad*>(param);
    //	//teardown sub state machine
    //	load->tagFsm_.clearChild();
    //	//append to parent
    //  //set last event to parent state machine
    //  load->tagFsm_.fireEvent(load->getEventByChar_(load->chr_), load);
    //}

    void recordAttrName(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        Xml::Attr* attr = new Xml::Attr();
        load->attr_ = attr;
        load->SetStopPosition(load->source_->GetPosition() - 1);
        load->attr_->name(load->GetContent());
    }

    void recordAttrValue(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        load->SetStopPosition(load->source_->GetPosition() - 1);
        load->attr_->value(load->GetContent());
        load->currentElement_->appendAttr(load->attr_);
    }

    void startEntityFsm(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //setup sub state machine
        load->entityFsm_.setStartState(entityName);
        std::vector<int> entityEndStates;
        entityEndStates.push_back(entityEnd);
        load->entityFsm_.setEndNotify(entityEndStates, endEntityHandle);
        //load->attrFsm_.activeChild(&load->entityFsm_);
    }

    void convertEntity(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        load->SetStopPosition(load->source_->GetPosition() - 1);
        //std::wstring entityName = load->GetContent();
        //replace name by content
    }

    void endEntityHandle(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        //teardown sub state machine
        //load->attrFsm_.clearChild();
    }

    void recordCommentContent(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        load->SetStopPosition(load->source_->GetPosition() - 3);
        load->current_->value(load->GetContent());
    }

    void recordCdataContent(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        load->SetStopPosition(load->source_->GetPosition() - 3);
        load->current_->value(load->GetContent());
    }

    void recordTarget(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        load->SetStopPosition(load->source_->GetPosition() - 1);
        load->current_->name(load->GetContent());
    }

    void recordContent(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        load->SetStopPosition(load->source_->GetPosition() - 2);
        load->current_->value(load->GetContent());
    }

    void recordPosition(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        load->SetStartPosition(load->source_->GetPosition());
    }

    void recordPrevPosition(void* param) {
        xmlLoad* load = static_cast<xmlLoad*>(param);
        load->SetStartPosition(load->source_->GetPosition() - 1);
    }

    Event const getCommonEventByChar(wchar_t const c) {
        Event result = static_cast<Event>(c);
        if (std::use_facet<std::ctype<wchar_t> >(std::locale::classic()).is(std::ctype<wchar_t>::alnum, c) || c == L':') {
            result = chr;
        } else if (std::use_facet<std::ctype<wchar_t> >(std::locale::classic()).is(std::ctype<wchar_t>::space, c)) {
            result = space;
        } else if (c == EOF) {
            result = eof;
        }
        return result;
    }

    Event const getCDataEventByChar(wchar_t const c) {
        Event result = static_cast<Event>(c);
        if ((c != L'C')
            && (c != L'D')
            && (c != L'A')
            && (c != L'T')
            && (c != L'[')
            && (c != L']')
            && (c != L'>')) {
                result = chr;
        }
        return result;
    }
}
