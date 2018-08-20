#ifndef __UTIL_VIDEO_PROPERTYBAG_H__
#define __UTIL_VIDEO_PROPERTYBAG_H__

#include <oaidl.h> //for VARIANT BSTR HRESULT ULONG ...
#include <ocidl.h> //for IPropertyBag

struct VAR_LIST {
    VARIANT var;
    VAR_LIST* pNext;
    BSTR pBSTRName;
};

class CPropertyBag : public IPropertyBag {
public:
    CPropertyBag();
    ~CPropertyBag();

    HRESULT STDMETHODCALLTYPE
    Read(LPCOLESTR pszPropName, VARIANT* pVar, IErrorLog* pErrorLog);

    HRESULT STDMETHODCALLTYPE
    Write(LPCOLESTR pszPropName, VARIANT* pVar);

    ULONG STDMETHODCALLTYPE AddRef();        
    ULONG STDMETHODCALLTYPE Release();        
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);   
private:
    ULONG _refCount;
    VAR_LIST* pVar;
};

#endif //__UTIL_VIDEO_PROPERTYBAG_H__
