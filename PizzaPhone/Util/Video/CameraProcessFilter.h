#ifndef __UTIL_VIDEO_SAMPLEUIDS_H__
#define __UTIL_VIDEO_SAMPLEUIDS_H__

#include <windef.h> //for CALLBACK BYTE
#include <objbase.h> //for DEFINE_GUID DECLARE_INTERFACE_

// callback definition
typedef void (CALLBACK *MANAGEDCALLBACKPROC)(BYTE* pdata, long len);

// {AD5DB5B4-D1AB-4f37-A60D-215154B4ECC1}
DEFINE_GUID(CLSID_SampleGrabber, 0xad5db5b4, 0xd1ab, 0x4f37, 0xa6, 0xd, 0x21, 0x51, 0x54, 0xb4, 0xec, 0xc1);

// ISampleGrabber interface definition
#ifdef __cplusplus
extern "C" {
#endif 
	// {04951BFF-696A-4ade-828D-42A5F1EDB631}
	DEFINE_GUID(IID_ISampleGrabber, 0x4951bff, 0x696a, 0x4ade, 0x82, 0x8d, 0x42, 0xa5, 0xf1, 0xed, 0xb6, 0x31);

	DECLARE_INTERFACE_(ISampleGrabber, IUnknown) {
		STDMETHOD(RegisterCallback)(MANAGEDCALLBACKPROC callback) PURE;
	};
#ifdef __cplusplus
}
#endif

#endif //__UTIL_VIDEO_SAMPLEUIDS_H__
