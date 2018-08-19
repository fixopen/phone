#ifndef __UTIL_CONTROLCODE_H__
#define __UTIL_CONTROLCODE_H__

#define CTL_CODE(DeviceType, Function, Method, Access) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))

//device type
#define FILE_DEVICE_STREAMS 0x0000001E
#define FILE_DEVICE_NETWORK 0x00000012
//method
#define METHOD_BUFFERED 0
#define METHOD_IN_DIRECT 1
#define METHOD_OUT_DIRECT 2
#define METHOD_NEITHER 3

//access
#define FILE_ANY_ACCESS 0
#define FILE_READ_ACCESS (0x0001)
#define FILE_WRITE_ACCESS (0x0002)

#endif //__UTIL_CONTROLCODE_H__
