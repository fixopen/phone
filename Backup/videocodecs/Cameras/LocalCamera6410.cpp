#include "stdafx.h"
#include "LocalCamera6410.h"

#pragma	comment(lib,"strmbase.lib")
#pragma comment(lib,"strmiids.lib")

LocalCamera6410::LocalCamera6410()
{
	CoInitialize(NULL);
}
LocalCamera6410::~LocalCamera6410(void)
{
	CoUninitialize();
}
bool LocalCamera6410::GetFirstCameraDriver(WCHAR *pwzName)
{
	HRESULT hResult = S_OK;
	HANDLE	handle = NULL;
	DEVMGR_DEVICE_INFORMATION di;
	GUID guidCamera = { 0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86 };
	if( pwzName == NULL )
	{
		return false;
	}
	di.dwSize = sizeof(di);
	handle = FindFirstDevice(DeviceSearchByGuid, &guidCamera, &di );
	if(( handle == NULL ) || ( di.hDevice == NULL ))
	{
		FindClose( handle );
		return false;
	}
	StringCchCopy( pwzName, MAX_PATH, di.szLegacyName );
	FindClose( handle );
	return true;

}
bool LocalCamera6410::SetPara(param *pParam)
{
	CameraPara.uiWidth = pParam->uiWidth;
	CameraPara.uiHeight = pParam->uiHeight;
	CameraPara.uiFrameRate = pParam->uiFrameRate;
	CameraPara.SampleCallBack = pParam->SampleCallBack;
	return true;
}
bool LocalCamera6410::OpenCamera(void)
{
	bool ret;
	HRESULT hResult = S_FALSE;
	HCRET lpfnDllRegisterServer;

	ret = GetFirstCameraDriver(wzCameraName_);
	if(!ret)
	{
		return ret;
	}
	//the first dll
	hDLL_SampleGrabber = LoadLibrary(_T("\\nandflash\\SampleGrabberFilter.dll"));
	if(hDLL_SampleGrabber == NULL)
	{
		DWORD error = GetLastError();
		TRACE1("LoadLibrary() Failed with: %i\n", error);
		return false;
	}
	lpfnDllRegisterServer = (HCRET) GetProcAddress(hDLL_SampleGrabber,_T( "DllRegisterServer"));
	if(lpfnDllRegisterServer == NULL)
	{
		DWORD error = GetLastError();
		TRACE1("GetProcAddress() Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		return false;
	}
	hResult = lpfnDllRegisterServer();
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("lpfnDllRegisterServer() Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		return false;
	}
	//the seconde dll
	hDLL_NullRender = LoadLibrary(_T("\\nandflash\\dump.dll"));
	if(hDLL_NullRender == NULL)
	{
		DWORD error = GetLastError();
		TRACE1("LoadLibrary() Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		return FALSE;
	}
	lpfnDllRegisterServer = (HCRET)GetProcAddress(hDLL_NullRender,_T( "DllRegisterServer"));//查找注册器入口
	if(lpfnDllRegisterServer == NULL)
	{
		DWORD error = GetLastError();
		TRACE1("GetProcAddress() Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		return false;
	}
	hResult = lpfnDllRegisterServer();//注册dll
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("lpfnDllRegisterServer() Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		return false;
	}
	hResult = m_pCaptureGraphBuilder.CoCreateInstance(CLSID_CaptureGraphBuilder);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pCaptureGraphBuilder.CoCreateInstance(CLSID_CaptureGraphBuilder) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		return false;
	}
	hResult = m_pGraphBuilder.CoCreateInstance(CLSID_FilterGraph);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pGraphBuilder.CoCreateInstance(CLSID_FilterGraph) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		return false;
	}
	hResult = m_pCaptureGraphBuilder->SetFiltergraph(m_pGraphBuilder);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pCaptureGraphBuilder->SetFiltergraph(m_pGraphBuilder) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		return false;
	}
	hResult = m_pGraphBuilder.QueryInterface(&m_pMediaControl);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pGraphBuilder.QueryInterface(&m_pMediaControl) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		return false;
	}
	hResult = m_pVideoCaptureFilter.CoCreateInstance(CLSID_VideoCapture);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pVideoCaptureFilter.CoCreateInstance(CLSID_VideoCapture) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		return false;
	}
	hResult = m_pVideoCaptureFilter->QueryInterface(&m_pPropertyBag);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pVideoCaptureFilter->QueryInterface(&m_pPropertyBag) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		return false;
	}
	varCamName = wzCameraName_;
	if(( varCamName.vt == VT_BSTR ) == NULL ) {
		DWORD error = GetLastError();
		TRACE1("varCamName Failed with (There is none camera): %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		return false;
	}
	hResult = PropBag.Write( L"VCapName", &varCamName);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("PropBag.Write( L\"VCapName\", &varCamName) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		return false;
	}
	hResult = m_pPropertyBag->Load( &PropBag, NULL);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pPropertyBag->Load( &PropBag, NULL) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		PropBag.Release();
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		return false;
	}
	hResult = m_pGraphBuilder->AddFilter( m_pVideoCaptureFilter, L"Video capture source");
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pGraphBuilder->AddFilter( m_pVideoCaptureFilter, L\"Video capture source\") Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		PropBag.Release();
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		return false;
	}
	hResult = m_pSampleGrabber.CoCreateInstance(CLSID_SampleGrabber);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pSampleGrabber.CoCreateInstance(CLSID_SampleGrabber) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		PropBag.Release();
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		return false;
	}
	hResult = m_pGraphBuilder->AddFilter(m_pSampleGrabber, L"Sample Filter");
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pGraphBuilder->AddFilter(m_pSampleGrabber, L\"Sample Filter\") Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		PropBag.Release();
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		m_pSampleGrabber.Release();
		m_pSampleGrabber = NULL;
		return false;
	}
	hResult = m_pSampleGrabber->QueryInterface(IID_ISampleGrabber,(void**)&m_pISample);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pSampleGrabber->QueryInterface(IID_ISampleGrabber,(void**)&m_pISample) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		PropBag.Release();
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		m_pSampleGrabber.Release();
		m_pSampleGrabber = NULL;
		return false;
	}
	hResult = m_pISample->RegisterCallback(CameraPara.SampleCallBack);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pISample->RegisterCallback(CameraPara.SampleCallBack) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		PropBag.Release();
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		m_pSampleGrabber.Release();
		m_pSampleGrabber = NULL;
		m_pISample.Release();
		m_pISample = NULL;
		return false;
	}
	hResult = m_pNullRender.CoCreateInstance(CLSID_NullRender);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pNullRender.CoCreateInstance(CLSID_NullRender) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		PropBag.Release();
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		m_pSampleGrabber.Release();
		m_pSampleGrabber = NULL;
		m_pISample.Release();
		m_pISample = NULL;
		return false;
	}
	hResult = m_pGraphBuilder->AddFilter(m_pNullRender, L"Null Render Filter");
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pGraphBuilder->AddFilter(m_pNullRender, L\"Null Render Filter\") Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		PropBag.Release();
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		m_pSampleGrabber.Release();
		m_pSampleGrabber = NULL;
		m_pISample.Release();
		m_pISample = NULL;
		m_pNullRender.Release();
		m_pNullRender = NULL;
		return false;
	}
	hResult = m_pNullRender->QueryInterface(IID_INullRender,(void**)&m_pIDump);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pNullRender->QueryInterface(IID_INullRender,(void**)&m_pIDump) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		PropBag.Release();
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		m_pSampleGrabber.Release();
		m_pSampleGrabber = NULL;
		m_pISample.Release();
		m_pISample = NULL;
		m_pNullRender.Release();
		m_pNullRender = NULL;
		return false;
	}
	hResult = m_pIDump->SetFileName(_T("test.lxk"),NULL);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pIDump->SetFileName(_T(\"test.lxk\"),NULL) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		PropBag.Release();
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		m_pSampleGrabber.Release();
		m_pSampleGrabber = NULL;
		m_pISample.Release();
		m_pISample = NULL;
		m_pNullRender.Release();
		m_pNullRender = NULL;
		return false;
	}
	hResult = m_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video, m_pVideoCaptureFilter,m_pSampleGrabber,m_pNullRender);
	if(FAILED(hResult))
	{
		DWORD error = GetLastError();
		TRACE1("m_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_CAPTURE,&MEDIATYPE_Video, m_pVideoCaptureFilter,m_pSampleGrabber,m_pNullRender) Failed with: %i\n", error);
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;
		varCamName = L"";
		PropBag.Release();
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
		m_pSampleGrabber.Release();
		m_pSampleGrabber = NULL;
		m_pISample.Release();
		m_pISample = NULL;
		m_pNullRender.Release();
		m_pNullRender = NULL;
		return false;
	}
	return true;
}
bool LocalCamera6410::CloseCamera()
{

	if (m_pMediaControl  != NULL)
	{
		m_pMediaControl->Stop();
		m_pMediaControl.Release();
		m_pMediaControl = NULL;
	}
	if (m_pPropertyBag != NULL)
	{
		m_pPropertyBag.Release();
		m_pPropertyBag = NULL;
	}
	if (m_pVideoCaptureFilter != NULL )
	{
		m_pVideoCaptureFilter.Release();
		m_pVideoCaptureFilter = NULL;

	}
	if (m_pSampleGrabber != NULL)
	{
		m_pSampleGrabber.Release();
		m_pSampleGrabber = NULL;
	}
	if (m_pISample != NULL)
	{
		m_pISample.Release();
		m_pISample = NULL;
	}
	if (m_pNullRender != NULL)
	{
		m_pNullRender.Release();
		m_pNullRender = NULL;
	}
	if (m_pIDump != NULL)
	{
		m_pIDump.Release();
		m_pIDump = NULL;
	}
	if (m_pGraphBuilder != NULL)
	{
		m_pGraphBuilder.Release();
		m_pGraphBuilder = NULL;
	}

	if (m_pCaptureGraphBuilder != NULL)
	{
		m_pCaptureGraphBuilder->SetFiltergraph(NULL);
		m_pCaptureGraphBuilder.Release();
		m_pCaptureGraphBuilder = NULL;
	}
	if (hDLL_SampleGrabber != NULL)
	{
		FreeLibrary(hDLL_SampleGrabber);
		hDLL_SampleGrabber = NULL;
	}
	if (hDLL_NullRender != NULL)
	{
		FreeLibrary(hDLL_NullRender);
		hDLL_NullRender = NULL;
	}
	return true;
}
bool LocalCamera6410::StartCapture(void)
{
	bool ret = false;
	{
	  HRESULT hr = S_FALSE;
	  IAMStreamConfig *pConfig = NULL;
	  hr = m_pCaptureGraphBuilder->FindInterface(
	  &PIN_CATEGORY_CAPTURE, // Preview pin.
	  0, // Any media type.
	  m_pVideoCaptureFilter, // Pointer to the capture filter.
	  IID_IAMStreamConfig, (void**)&pConfig);

	  if (SUCCEEDED(hr))
	  {
		  int iCount = 0, iSize = 0;
		  hr = pConfig->GetNumberOfCapabilities(&iCount, &iSize);
   		  // Check the size to make sure we pass in the correct structure.
		  if (iSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
		  {
			// Use the video capabilities structure.
			for (int iFormat = 0; iFormat < iCount; iFormat++)
			{
				VIDEO_STREAM_CONFIG_CAPS scc;
				AM_MEDIA_TYPE *pmtConfig;
				hr = pConfig->GetStreamCaps(iFormat, &pmtConfig, (BYTE*)&scc);
				if (SUCCEEDED(hr))
				{
					/* Examine the format, and possibly use it. */
					 //if ((pmtConfig->majortype == MEDIATYPE_Video) &&
					 // (pmtConfig->subtype == MEDIASUBTYPE_RGB565) &&
					 // (pmtConfig->formattype == FORMAT_VideoInfo) &&
					 // (pmtConfig->cbFormat >= sizeof (VIDEOINFOHEADER)) &&
					 // (pmtConfig->pbFormat != NULL))
					 if ((pmtConfig->majortype == MEDIATYPE_Video) &&
					  (pmtConfig->subtype == MEDIASUBTYPE_YV12) &&
					  (pmtConfig->formattype == FORMAT_VideoInfo) &&
					  (pmtConfig->cbFormat >= sizeof (VIDEOINFOHEADER)) &&
					  (pmtConfig->pbFormat != NULL))
					  {
						  VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)pmtConfig->pbFormat;
						  // pVih contains the detailed format information.
						  LONG lWidth = pVih->bmiHeader.biWidth;
						  LONG lHeight = pVih->bmiHeader.biHeight;
					 
	////调整大小					  
	//						  //pVih->bmiHeader.biWidth = 640;
	//						  //pVih->bmiHeader.biHeight = -480;
	//						  //pVih->bmiHeader.biSizeImage = DIBSIZE(pVih->bmiHeader);
						  if(((lWidth == CameraPara.uiWidth)||(lWidth == -CameraPara.uiWidth)) && ((lHeight == CameraPara.uiHeight)||((lHeight == -CameraPara.uiHeight))))
						  {
							  static int seted = 0;
							  if(!seted)
							  {
								  hr = pConfig->SetFormat(pmtConfig);//重新设置视频格式
								  if (SUCCEEDED(hr))
								  {
									  ret = true;
									  seted++;
									  DeleteMediaType(pmtConfig);
									  break;
								  }								 
							  }							  
						  }
					 }
					 // Delete the media type when you are done.
					 DeleteMediaType(pmtConfig);
				}
			}
		}
		  pConfig->Release();
	  }
	}
	if(ret)
	{
		if(FAILED(m_pMediaControl->Run()))
			ret = false;		
	}
	return ret;
}
bool LocalCamera6410::StopCapture()
{
	if(FAILED(m_pMediaControl->Stop()))
		return false;
	return true;
}
