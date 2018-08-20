#ifndef __DEVICE_DIALPAD_H__
#define __DEVICE_DIALPAD_H__

#pragma once

#include "Keypad.h"

namespace Device {
	class Dialpad : public Keypad {
	public:
		Dialpad();
		~Dialpad();
	};
}

#endif //__DEVICE_DIALPAD_H__
