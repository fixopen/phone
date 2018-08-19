#ifndef	_LocalCamera6410_
#define	_LocalCamera6410_

#include <streams.h>
#include "PropertyBag.h"
#include "sampleuids.h"
#include "dumpuids.h"

typedef HRESULT (CALLBACK *HCRET)(void);

struct param
{
	unsigned int uiWidth;
	unsigned int uiHeight;
	unsigned int uiFrameRate;
	MANAGEDCALLBACKPROC SampleCallBack;
};

class LocalCamera6410
{
public:
	LocalCamera6410();
	~LocalCamera6410(void);

	bool SetPara(param *pParam);
	bool OpenCamera(void);
	bool CloseCamera(void);
	bool StartCapture(void);
	bool StopCapture();
private:
	bool GetFirstCameraDriver( WCHAR *pwzName );
	WCHAR			wzCameraName_[MAX_PATH+1];
	HINSTANCE		hDLL_SampleGrabber;
	HINSTANCE		hDLL_NullRender;
	CComPtr<ICaptureGraphBuilder2>	m_pCaptureGraphBuilder;
	CComPtr<IGraphBuilder>			m_pGraphBuilder;
	CComPtr<IMediaControl>			m_pMediaControl;
	CComPtr<IBaseFilter>			m_pVideoCaptureFilter;
	CComPtr<IPersistPropertyBag>    m_pPropertyBag;
	CComPtr<IBaseFilter>			m_pSampleGrabber;
	CComPtr<ISampleGrabber>			m_pISample;
	CComPtr<IBaseFilter>			m_pNullRender;
	CComPtr<INullRender>			m_pIDump;
	CComVariant   varCamName;  
	CPropertyBag  PropBag;
	param CameraPara;
};
#endif