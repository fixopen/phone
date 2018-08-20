#ifndef __DEVICE_SDCARD_H__
#define __DEVICE_SDCARD_H__

#pragma once

#include "Storage.h"

namespace Device {
	class SDCard : public Storage {
	public:
		SDCard();
		~SDCard();
	};
}

#endif //__DEVICE_SDCARD_H__
