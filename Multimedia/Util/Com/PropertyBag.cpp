#include "PropertyBag.h"

namespace Util {
    namespace Com {
        //构造函数
        CPropertyBag::CPropertyBag()
            : refCount_(1)
            , varList_(0) {
        }

        //析构函数
        CPropertyBag::~CPropertyBag() {
            VAR_LIST* temp = varList_;
            HRESULT hr = S_OK;

            while (temp) {
                VAR_LIST* del = temp;
                SysFreeString(temp->name);
                VariantClear(&temp->value);
                temp = temp->next;
                delete del;
            }
        }

        //属性读方法
        HRESULT STDMETHODCALLTYPE
        CPropertyBag::Read(LPCOLESTR name, VARIANT* value, IErrorLog* errorLog) {
            VAR_LIST* temp = varList_;
            HRESULT hr = S_OK;

            while (temp) {
                if (0 == wcscmp(name, temp->name)) {
                    hr = VariantCopy(value, &temp->value);
                    break;
                }

                temp = temp->next;
            }

            return hr;
        }

        //属性写方法
        HRESULT STDMETHODCALLTYPE
        CPropertyBag::Write(LPCOLESTR name, VARIANT* value) {
            HRESULT hr = S_OK;
            VAR_LIST* temp = new VAR_LIST();
            //ASSERT(temp);

            if (!temp) {
                return E_OUTOFMEMORY;
            }

            temp->name = SysAllocString(name);
            VariantInit(&temp->value);
            temp->next = varList_;
            varList_ = temp;
            return VariantCopy(&temp->value, value);
        }

        ULONG STDMETHODCALLTYPE
        CPropertyBag::AddRef() {
            return InterlockedIncrement((LONG*)&refCount_);
        }

        ULONG STDMETHODCALLTYPE
        CPropertyBag::Release() {
            //ASSERT(refCount_ != 0xFFFFFFFF);
            ULONG ret = InterlockedDecrement((LONG*)&refCount_);

            if (!ret) {
                delete this;
            }

            return ret;
        }

        HRESULT STDMETHODCALLTYPE
        CPropertyBag::QueryInterface(REFIID riid, void** ppv) {
            if (!ppv) {
                return E_POINTER;
            }

            if (riid == IID_IPropertyBag) {
                *ppv = static_cast<IPropertyBag*>(this);
            } else {
                return *ppv = 0, E_NOINTERFACE;
            }

            return AddRef(), S_OK;
        }
    }
}
