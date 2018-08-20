#ifndef __VIEW_SOFTWAREKEYBOARD_H__
#define __VIEW_SOFTWAREKEYBOARD_H__

#pragma once

#include "../Device/Keypad.h"

namespace View {
	class SoftwareKeyboard : public Device::Keypad {
		//switcher
		//layout, event, canvas, handler(register and notify)
	public:
		SoftwareKeyboard();
		~SoftwareKeyboard();
	};
}

#endif //__VIEW_SOFTWAREKEYBOARD_H__
