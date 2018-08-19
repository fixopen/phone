#include "PropertyBag.h"

namespace Util {
    PropertyBag::PropertyBag()
    : refCount_(1)
    , values_(0) {
    }

    PropertyBag::~PropertyBag() {
        VariantList* temp = values_;
        HRESULT hr = S_OK;

        while (temp) {
            VariantList* deleteItem = temp;
            VariantClear(&temp->value);
            SysFreeString(temp->name);
            temp = temp->next;
            delete deleteItem;
        }
    }

    HRESULT STDMETHODCALLTYPE
    PropertyBag::Read(LPCOLESTR propName, VARIANT* propValue, IErrorLog* errorLog) {
        VariantList* temp = values_;
        HRESULT hr = S_OK;

        while (temp) {
            if (0 == wcscmp(propName, temp->name)) {
                hr = VariantCopy(propValue, &temp->value);
                break;
            }
            temp = temp->next;
        }
        return hr;
    }

    HRESULT STDMETHODCALLTYPE
    PropertyBag::Write(LPCOLESTR propName, VARIANT* propValue) {
        HRESULT hr = S_OK;
        VariantList* temp = new VariantList();
        ASSERT(temp);

        if (!temp) {
            return E_OUTOFMEMORY;
        }

        VariantInit(&temp->value);
        temp->name = SysAllocString(propName);
        temp->next = values_;
        values_ = temp;
        return VariantCopy(&temp->value, propValue);
    }

    ULONG STDMETHODCALLTYPE 
    PropertyBag::AddRef() {
        return InterlockedIncrement((LONG *)&refCount_);
    }

    ULONG STDMETHODCALLTYPE 
    PropertyBag::Release() {
        ASSERT(refCount_ != 0xFFFFFFFF);
        ULONG ret = InterlockedDecrement((LONG*)&refCount_);    
        if (!ret) {
            delete this;
        } 
        return ret;
    }

    HRESULT STDMETHODCALLTYPE 
    PropertyBag::QueryInterface(REFIID interfaceId, void** returnInterface) {
        if (!returnInterface) {
            return E_POINTER;
        }
        if (interfaceId == IID_IPropertyBag) {
            *returnInterface = static_cast<IPropertyBag*>(this);
        } else {
            return *returnInterface = 0, E_NOINTERFACE;
        }

        return AddRef(), S_OK;
    }
}