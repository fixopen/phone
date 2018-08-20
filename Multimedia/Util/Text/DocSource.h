#ifndef __UTIL_TEXT_DOCSOURCE_H__
#define __UTIL_TEXT_DOCSOURCE_H__

#include <string>
#include <cstdio>

namespace Util {
    namespace Text {
        class DocSource {
        public:
            virtual ~DocSource() = 0 {};
            virtual int GetChar() const = 0;
            virtual int GetPosition() const = 0;
            virtual std::wstring const GetContent(int const start, int const stop) const = 0;
            virtual bool const IsEnd() const = 0;
        };
        class MemDocSource : public DocSource {
        public:
            MemDocSource(std::wstring const& memory);
            virtual ~MemDocSource();
            virtual int GetChar() const;
            virtual int GetPosition() const;
            virtual std::wstring const GetContent(int const start, int const stop) const;
            virtual bool const IsEnd() const;
        private:
            std::wstring memory_;
            mutable size_t index_;
        };
        class FileDocSource : public DocSource {
        public:
            FileDocSource(std::string const& filename);
            virtual ~FileDocSource();
            virtual int GetChar() const;
            virtual int GetPosition() const;
            //virtual void MarkStart(int const offset = 0);
            //virtual void MarkStop(int const offset = 0);
            //virtual std::wstring const GetContent();
            virtual std::wstring const GetContent(int const start, int const stop) const;
            virtual bool const IsEnd() const;
        private:
            std::FILE* file_;
        };
    }
}

#endif //__UTIL_TEXT_DOCSOURCE_H__
