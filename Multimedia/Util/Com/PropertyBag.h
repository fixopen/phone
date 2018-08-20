#ifndef __UTIL_COM_PROPERTYBAG_H__
#define __UTIL_COM_PROPERTYBAG_H__

//#include <WinDef.h> //for ULONG
//#include <Guiddef.h> //for REFIID
//#include <WinNT.h> //for STDMETHODCALLTYPE, HRESULT
//#include <WTypes.h> //for LPCOLESTR, BSTR
#include <OAIdl.h> //for VARIANT, IErrorLog, IPropertyBag

namespace Util {
    namespace Com {
        struct VAR_LIST {
            BSTR name;
            VARIANT value;
            VAR_LIST* next;
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
            ULONG refCount_;
            VAR_LIST* varList_;
        };
    }
}

#endif //__UTIL_COM_PROPERTYBAG_H__
