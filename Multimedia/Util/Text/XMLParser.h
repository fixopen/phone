#ifndef __UTIL_TEXT_XMLPARSER_H__
#define __UTIL_TEXT_XMLPARSER_H__

#define EOF (-1)

#include <string>
#include "DocSource.h"
#include "../SmartPtr.h"
#include "../Fsm.h"
#include "../../Xml/Node.h"
#include "../../Xml/Document.h"
#include "../../Xml/ProcessingInstruction.h"
#include "../../Xml/DataSection.h"
#include "../../Xml/Comment.h"
#include "../../Xml/DocType.h"
#include "../../Xml/Text.h"

namespace Util {
    namespace Text {
        class XMLParser;

        template<typename T>
        class BaseParser : public Dfa<T> {
            friend class XMLParser;
        public:
            virtual ~BaseParser() {}
            std::wstring const& GetContent() const {
                return content_;
            }
            void SetResult(Xml::Node* r) {
                node_ = r;
            }
            void SetParentParser(XMLParser* parser) {
                parentParser_ = parser;
            }
            void SetSource(DocSource* source) {
                source_ = source;
            }
            void GiveChar(wchar_t const c) {
                fireEvent(getEventByChar_(c, getCurrentState()), 0);
            }
        protected:
            void recordPosition_(void* param) {
                pos_ = source_->GetPosition();
            }
            void recordPrevPosition_(void* param) {
                pos_ = source_->GetPosition() - 1;
            }
            void recordContent_(void* param = 0) {
                int offset = reinterpret_cast<int>(param);
                content_ = source_->GetContent(pos_, source_->GetPosition() + offset);
            }
            virtual int const getEventByChar_(wchar_t const chr, int const state) const {
                int result = 0;
                return result;
            }
            XMLParser* parentParser_;
            Xml::Node* node_;
            DocSource* source_;
        private:
            size_t pos_;
            std::wstring content_;
        };

        class PIParser : public BaseParser<PIParser> {
        public:
            enum State {
                sTarget,
                sTargetStop,
                sContent,
                sStartStop,
                sEnd,
                sError,
            };
            enum Event {
                chr,
                nonChr,
                space,
                questMark = L'?',
                gt = L'>',
                eof = EOF,
            };
        public:
            PIParser();
            Event const GetEventByChar(wchar_t const c, State const state) const;
        private:
            void recordTarget_(void* param) {
                recordContent_(reinterpret_cast<void*>(-1));
                node_->name(GetContent());
            }
            void recordValue_(void* param) {
                recordContent_(reinterpret_cast<void*>(-2));
                node_->value(GetContent());
            }
        };

        class CDataParser : public BaseParser<CDataParser> {
        public:
            enum State {
                sStart,
                sC,
                sD,
                sA1,
                sT,
                sA2,
                sContent,
                sStartStop,
                sContinueStop,
                sEnd,
                sError,
            };
            enum Event {
                chr,
                leftSquare = L'[',
                letterA = L'A',
                letterC = L'C',
                letterD = L'D',
                letterT = L'T',
                rightSquare = L']',
                gt = L'>',
                eof = EOF,
            };
        public:
            CDataParser();
            Event const GetEventByChar(wchar_t const c, State const state) const;
        private:
            void recordCDataSection_(void* param) {
                recordContent_(reinterpret_cast<void*>(-3));
                node_->value(GetContent());
            }
        };

        class CommentParser : public BaseParser<CommentParser> {
        public:
            enum State {
                sStart,
                sStartContent,
                sContent,
                sStartStop,
                sContinueStop,
                sEnd,
                sError,
            };
            enum Event {
                chr,
                hyphen = L'-',
                gt = L'>',
                eof = EOF,
            };
        public:
            CommentParser();
            Event const GetEventByChar(wchar_t const c, State const state) const;
        private:
            void recordComment_(void* param) {
                recordContent_(reinterpret_cast<void*>(-3));
                node_->value(GetContent());
            }
        };

        class EntityParser : public BaseParser<EntityParser> {
        public:
            enum State {
                sName,
                sEnd,
                sError,
            };
            enum Event {
                chr,
                space,
                semicolon = L';',
                eof = EOF,
            };
        public:
            EntityParser();
            Event const GetEventByChar(wchar_t const chr, State const state) const;
        private:
            void convertEntity_(void* param);
        };

        class TagParser : public BaseParser<TagParser> {
        public:
            enum State {
                sTagName,
                sTagNameStop,
                sAttrName,
                sAttrNameStop,
                sAttrEq,
                sAttrQuotValue,
                sAttrAposValue,
                sAttrValueStop,
                sEmptyTagStart,
                sEmptyTagEnd,
                sTagEnd,
                sError,
            };
            enum Event {
                chr,
                nonChr,
                nonValueMark,
                nonEq,
                space,
                splash = L'/',
                colon = L':',
                eq = L'=',
                amp = L'&',
                quot = L'"',
                apos = L'\'',
                gt = L'>',
                eof = EOF,
            };
        public:
            TagParser();
            Event const GetEventByChar(wchar_t const chr, State const state) const;
        private:
            void recordTagName_(void* param) {
                recordContent_(reinterpret_cast<void*>(-1));
                node_->name(GetContent());
            }

            void recordAttrName_(void* param) {
                attr_ = new Xml::Attr();
                recordContent_(reinterpret_cast<void*>(-1));
                attr_->name(GetContent());
            }

            void recordAttrValue_(void* param) {
                recordContent_(reinterpret_cast<void*>(-1));
                attr_->value(GetContent());
                static_cast<Xml::Element*>(node_)->appendAttr(attr_);
                //delete attr_;
                attr_ = 0;
            }

            void startEntityParser_(void* param) {
                entityParser_->SetStartState(EntityParser::sName);
            }

            void stopEntityParser_(void* param) {
            }

            EntityParser* entityParser_;
            Xml::Attr* attr_;
        };

        class DocTypeParser : public BaseParser<DocTypeParser> {
        public:
            enum State {
                sStart,
                sEnd,
                sError,
            };
            enum Event {
                chr,
                gt = L'>',
                eof = EOF,
            };
        public:
            DocTypeParser();
            Event const GetEventByChar(wchar_t const chr, State const state) const;
        private:
        };

        //class TextParser : public BaseParser<TextParser> {
        //};

        //class ElementParser : public BaseParser<ElementParser> {
        //};

        class XMLParser : public BaseParser<XMLParser> {
        public:
            enum State {
                sIdle,
                sText,
                sStage1,
                sPI,
                sTag,
                sEndTag,
                sStage2,
                sDocType,
                sCData,
                sComment,
                sEnd,
                sError,
            };

            enum Event {
                end,
                chr,
                nonChr,
                splash = L'/',
                lt = L'<',
                gt = L'>',
                hyphen = L'-',
                questMark = L'?',
                exclaimMark = L'!',
                leftSquare = L'[',
                letterD = L'D',
                eof = EOF,
            };
        public:
            XMLParser();
            Util::shared_ptr<Xml::Document> const Parse(DocSource* const source);
            Event const GetEventByChar(wchar_t const chr, State const state) const;
        private:
            void startText_(void* param);
            void recordText_(void* param);
            void startPIParser_(void* param);
            void startTagParser_(void* param);
            void stopPIParser_(void* param);
            void stopTagParser_(void* param);
            void validAndInsert_(void* param);
            void startDocTypeParser_(void* param);
            void startCDataParser_(void* param);
            void startCommentParser_(void* param);
            void stopDocTypeParser_(void* param);
            void stopCDataParser_(void* param);
            void stopCommentParser_(void* param);
        private:
        private:
            Xml::Document* doc_;
            Xml::Node* parent_;
            Xml::Node* current_;
            Xml::Text* text_;

            PIParser* piParser_;
            TagParser* tagParser_;
            DocTypeParser* docTypeParser_;
            CDataParser* cdataParser_;
            CommentParser* commentParser_;
        };
    }
}

#endif // __UTIL_TEXT_XMLPARSER_H__
