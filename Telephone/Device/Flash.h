#ifndef __DEVICE_FLASH_H__
#define __DEVICE_FLASH_H__

#pragma once

#include "Storage.h"

namespace Device {
	class Flash : public Storage {
	public:
		Flash();
		~Flash();
	};
}

#endif //__DEVICE_FLASH_H__
