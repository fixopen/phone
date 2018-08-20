#include "DocSource.h"

namespace Util {
    DocSource::DocSource(std::string const& filename) {
        file_ = std::fopen(filename.c_str(), "rb");
    }

    DocSource::~DocSource() {
        std::fclose(file_);
    }

    int DocSource::GetChar() {
        char ch;
        int r = std::fscanf(file_, "%c", &ch);
        if (r == EOF)
            ch = EOF;
        return ch;
    }

    int DocSource::GetPosition() {
        return std::ftell(file_);
    }

    std::wstring const DocSource::GetContent(int const start, int const stop) {
        std::wstring result;
        size_t org = std::ftell(file_);
        std::fseek(file_, start, SEEK_SET);
        int count = stop - start;
        char* buffer = static_cast<char*>(malloc(count + 1));
        std::fread(buffer, 1, count, file_);
        std::fseek(file_, org, SEEK_SET);
        for (int i = 0; i < count; ++i) {
            result += buffer[i];
        }
        free(buffer);
        return result;
    }

    bool const DocSource::IsEnd() const {
        return std::feof(file_);
    }
}