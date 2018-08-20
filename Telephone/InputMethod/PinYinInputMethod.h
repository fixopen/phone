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
		static size_t const PinyinBaseOffset_ = 1664; //ƴ��������ƴ�����е�ƫ�Ƶ�ַ
		static size_t const PinyinTotalCount_ = 416; //ƴ���ܸ���
		static std::string const FirstCharTableFilename_;
		char* firstCharTable_;
	};
}

#endif //__INPUTMETHOD_PINYININPUTMETHOD_H__
