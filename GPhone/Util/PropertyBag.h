#ifndef __UTIL_PROPERTYBAG_H__
#define __UTIL_PROPERTYBAG_H__

#include <oaidl.h> //for VARIANT BSTR HRESULT ULONG ...
#include <ocidl.h> //for IPropertyBag

namespace Util {
    struct VariantList {
        VARIANT value;
        VariantList* next;
        BSTR name;
    };

    class PropertyBag : public IPropertyBag {
    public:
        PropertyBag();
        ~PropertyBag();

        HRESULT STDMETHODCALLTYPE
            Read(LPCOLESTR propName, VARIANT* propValue, IErrorLog* errorLog);

        HRESULT STDMETHODCALLTYPE
            Write(LPCOLESTR propName, VARIANT* propValue);

        ULONG STDMETHODCALLTYPE AddRef();
        ULONG STDMETHODCALLTYPE Release();
        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID interfaceId, void** returnInterface);
    private:
        ULONG refCount_;
        VariantList* values_;
    };
}

#endif //__UTIL_PROPERTYBAG_H__
