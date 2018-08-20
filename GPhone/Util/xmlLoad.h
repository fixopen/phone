#ifndef __UTIL_XMLLOAD_H__
#define __UTIL_XMLLOAD_H__

#include "DocSource.h"
#include "SmartPtr.h"
#include "fsm.h"
#include "../Xml/Document.h"
#include "../Xml/ProcessingInstruction.h"

namespace Util {
    enum State {
        docIdle,
        docText,
        docStage1,
        docPI,
        docElement,
        docEndTag,
        docStage2,
        docDocType,
        docCdata,
        docComment,
        docEnd,
        docError,
    };

    enum PIState {
        piTarget,
        piTargetStop,
        piContent,
        piStartStop,
        piEnd,
        piError,
    };

    enum TagState {
        tagTagName,
        tagTagNameStop,
        tagAttrName,
        tagAttrNameStop,
        tagAttrEq,
        tagAttrQuotValue,
        tagAttrAposValue,
        tagAttrValueStop,
        tagEmptyTagStart,
        tagEmptyTagEnd,
        tagTagEnd,
        tagError,
    };

    enum EntityState {
        entityName,
        entityEnd,
        entityError,
    };

    enum CommentState {
        commentStart,
        commentStartContent,
        commentContent,
        commentStartStop,
        commentContinueStop,
        commentEnd,
        commentError,
    };

    enum CdataState {
        cdataStart,
        cdataC,
        cdataD,
        cdataA1,
        cdataT,
        cdataA2,
        cdataContent,
        cdataStartStop,
        cdataContinueStop,
        cdataEnd,
        cdataError,
    };

    enum DocTypeState {
        docTypeStart,
        docTypeEnd,
        docTypeError,
    };

    enum Event {
        chr,
        space,
        amp = L'&',
        quot = L'"',
        apos = L'\'',
        lt = L'<',
        gt = L'>',
        splash = L'/',
        eq = L'=',
        hyphen = L'-',
        semicolon = L':',
        questMark = L'?',
        exclaimMark = L'!',
        leftSquare = L'[',
        rightSquare = L']',
        letterA = L'A',
        letterC = L'C',
        letterD = L'D',
        letterT = L'T',
        eof = EOF,
    };

    void textProc(void* param);
    void endTextProc(void* param);
    void startPiFsm(void* param);
    void endPiHandle(void* param);
    void elementProc(void* param);
    void validAndInsert(void* param);
    void startDocTypeFsm(void* param);
    void endDocTypeHandle(void* param);
    void startCdataFsm(void* param);
    void endCdataHandle(void* param);
    void startCommentFsm(void* param);
    void endCommentHandle(void* param);

    void recordTagName(void* param);
    void recordAttrName(void* param);
    void recordAttrValue(void* param);
    void startEntityFsm(void* param);

    void convertEntity(void* param);
    void endEntityHandle(void* param);

    void recordCommentContent(void* param);

    void recordCdataContent(void* param);

    void recordTarget(void* param);
    void recordContent(void* param);

    void recordPrevPosition(void* param);
    void recordPosition(void* param);

    class xmlLoad {
        friend void textProc(void* param);
        friend void endTextProc(void* param);
        friend void startPiFsm(void* param);
        friend void endPiHandle(void* param);
        friend void elementProc(void* param);
        friend void validAndInsert(void* param);
        friend void startDocTypeFsm(void* param);
        friend void endDocTypeHandle(void* param);
        friend void startCdataFsm(void* param);
        friend void endCdataHandle(void* param);
        friend void startCommentFsm(void* param);
        friend void endCommentHandle(void* param);

        friend void recordTagName(void* param);
        friend void recordAttrName(void* param);
        friend void recordAttrValue(void* param);
        friend void startEntityFsm(void* param);

        friend void convertEntity(void* param);
        friend void endEntityHandle(void* param);

        friend void recordCommentContent(void* param);

        friend void recordCdataContent(void* param);

        friend void recordTarget(void* param);
        friend void recordContent(void* param);

        friend void recordPrevPosition(void* param);
        friend void recordPosition(void* param);
    public:
        xmlLoad(void);
        ~xmlLoad(void);
        void SetStartPosition(int const pos);
        void SetStopPosition(int const pos);
        std::wstring const GetContent() const;
        Util::shared_ptr<Xml::Document> const parse(Util::shared_ptr<DocSource>& source);
    private:
        //public:
        void parseToElement_();
        TagState const parseTag_();
    private:
        //public:
        Event const (*getEventByChar_)(wchar_t const chr);
        wchar_t chr_;
        int startPos_;
        int stopPos_;
        Xml::Node* parent_;
        Xml::Document* doc_;
        Xml::Node* current_;
        Xml::Element* currentElement_;
        Xml::Attr* attr_;

        Util::shared_ptr<DocSource> source_;

        Util::FSM piFsm_;
        Util::FSM cdataFsm_;
        Util::FSM commentFsm_;
        Util::FSM entityFsm_;
        Util::FSM tagFsm_;
        Util::FSM docTypeFsm_;
        //Util::FSM textFsm_;
        //Util::FSM elementFsm_;
        Util::FSM fsm_;
    };

    Event const getCommonEventByChar(wchar_t const chr);
    Event const getCDataEventByChar(wchar_t const c);
    //Event const getCommentEventByChar(wchar_t const c);
    //Event const getPIEventByChar(wchar_t const c);
}

#endif //__UTIL_XMLLOAD_H__