#include "PinYinInputMethod.h"
#include "../Util/ByteOperator.h"
#include <algorithm> //for std::transform

namespace InputMethod {
	std::string const PinYinInputMethod::PinyinTableFilename_ = "\\HIVE\\RES_DAT\\pinyin.dat";
	std::string const PinYinInputMethod::FirstCharTableFilename_ = "\\HIVE\\RES_DAT\\firstchar.txt";

	PinYinInputMethod::PinYinInputMethod() {
		pinyinTable_ = new char[23000];
		FILE* pinyinFile = fopen(PinyinTableFilename_.c_str(), "rb");
		fread(pinyinTable_, 1, 23000, pinyinFile);
		fclose(pinyinFile);

		firstCharTable_ = new char[6800];
		FILE* firstCharFile = fopen(FirstCharTableFilename_.c_str(), "rb");
		fread(firstCharTable_, 1, 6800, firstCharFile);
		fclose(firstCharFile);
	}

	namespace {
		char const toLower(char const chr) {
			static int const lowerUpperDistance = 'a' - 'A';
			char result = chr;
			if (chr >= 'A' && chr <= 'Z') {
				result = chr + lowerUpperDistance;
			}
			return result;
		}
	}

	std::pair<int, std::string> const PinYinInputMethod::GetHanzi(std::string const& pinyin) {
		std::pair<int, std::string> result;
		result.first = 0;
		std::string normalizePinyin = pinyin;
		std::transform(pinyin.begin(), pinyin.end(), normalizePinyin.begin(), toLower);
		size_t begin = 0, end = PinyinTotalCount_ - 1;
		char* findPinyin = 0;

		for (; begin <= end; ) {
			size_t pos = (begin + end) / 2;
			unsigned long offset = *((unsigned long*)(pinyinTable_ + pos * 4));
			offset = Util::ByteOperator::ReverseByte(offset);
			findPinyin = (char*)(pinyinTable_ + PinyinBaseOffset_ + offset);
			int pinyinCompareResult = strcmp(normalizePinyin.c_str(), findPinyin);

			if (pinyinCompareResult == 0) {
				break;
			} else {
				if (pinyinCompareResult < 0) {
					end = pos - 1;
				} else {
					begin = pos + 1;
				}
			}
		}

		if (begin <= end) { // 查到
			// 定位汉字串
			while (*findPinyin != 0x00) {
				findPinyin++;
			}

			++findPinyin;
			result.second = findPinyin;
			result.first = result.second.length() / 2;
		} else { // 没有查到
			if ((begin == -1) || (begin == PinyinTotalCount_)) { // 输入拼音在有效拼音字符串范围之外，返回 -1
				result.first = -1;
			} else { // 输入拼音是有效字符串的子集，返回 0
				unsigned long offset = *((long*)(pinyinTable_ + begin * 4));
				offset = Util::ByteOperator::ReverseByte(offset);
				findPinyin = (char*)(pinyinTable_ + PinyinBaseOffset_ + offset);

				if (strstr(findPinyin, normalizePinyin.c_str())) {
					result.first = 0;
				} else { // 输入拼音在有效拼音字符串范围之外，返回 -1
					result.first = -1;
				}
			}
		}

		return result;
	}

	std::string const PinYinInputMethod::GetLegend(std::string const& hanzi) {
		std::string result;
		return result;
	}

	PinYinInputMethod::~PinYinInputMethod() {
		delete[] pinyinTable_;
		delete[] firstCharTable_;
	}
}
