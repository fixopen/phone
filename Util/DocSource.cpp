#include "DocSource.h"

namespace Util {
    MemDocSource::MemDocSource(std::wstring const& memory) : memory_(memory), index_(0) {
    }

    MemDocSource::~MemDocSource() {
    }

    int MemDocSource::GetChar() const {
        char ch;
        if (index_ >= memory_.length()) {
            ch = EOF;
        } else {
            ch = memory_[index_];
        }
        ++index_;
        return ch;
    }

    int MemDocSource::GetPosition() const {
        return index_;
    }

    std::wstring const MemDocSource::GetContent(int const start, int const stop) const {
        return memory_.substr(start, stop - start);
    }

    bool const MemDocSource::IsEnd() const {
        return index_ >= memory_.length();
    }

    FileDocSource::FileDocSource(std::string const& filename) {
        file_ = /*std::*/fopen(filename.c_str(), "rb");
    }

    FileDocSource::~FileDocSource() {
        /*std::*/fclose(file_);
    }

    int FileDocSource::GetChar() const {
        char ch;
        int r = /*std::*/fscanf(file_, "%c", &ch);
        if (r == EOF)
            ch = EOF;
        return ch;
    }

    int FileDocSource::GetPosition() const {
        return /*std::*/ftell(file_);
    }

    //void FileDocSource::MarkStart(int const offset) {
    //}

    //void FileDocSource::MarkStop(int const offset) {
    //}

    //std::wstring const FileDocSource::GetContent() {
    //    std::wstring result;
    //    return result;
    //}

    std::wstring const FileDocSource::GetContent(int const start, int const stop) const {
        std::wstring result;
        size_t org = /*std::*/ftell(file_);
        /*std::*/fseek(file_, start, SEEK_SET);
        int count = stop - start;
        char* buffer = static_cast<char*>(malloc(count + 1));
        /*std::*/fread(buffer, 1, count, file_);
        /*std::*/fseek(file_, org, SEEK_SET);
        for (int i = 0; i < count; ++i) {
            result += buffer[i];
        }
        free(buffer);
        return result;
    }

    bool const FileDocSource::IsEnd() const {
        return !!/*std::*/feof(file_);
    }
}