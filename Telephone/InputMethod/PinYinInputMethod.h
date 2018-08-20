#ifndef __INPUTMETHOD_PINYININPUTMETHOD_H__
#define __INPUTMETHOD_PINYININPUTMETHOD_H__

#pragma once

#include "InputMethod.h"
#include <utility> //for std::pair
#include <string> //for std::string

namespace InputMethod {
	class PinYinInputMethod : public InputMethod {
	public:
		PinYinInputMethod();
		std::pair<int, std::string> const GetHanzi(std::string const& pinyin);
		std::string const GetLegend(std::string const& hanzi);
		~PinYinInputMethod();
	private:
		static std::string const PinyinTableFilename_;
		char* pinyinTable_;
		static size_t const PinyinBaseOffset_ = 1664; //拼音数据在拼音表中的偏移地址
		static size_t const PinyinTotalCount_ = 416; //拼音总个数
		static std::string const FirstCharTableFilename_;
		char* firstCharTable_;
	};
}

#endif //__INPUTMETHOD_PINYININPUTMETHOD_H__
