namespace vc_attributes {
    enum YesNoMaybe {
        No = 0x0fff0001,
        Maybe = 0x0fff0010,
        Yes = 0x0fff0100
    };
    typedef enum YesNoMaybe YesNoMaybe;

    enum AccessType {
        NoAccess = 0,
        Read = 1,
        Write = 2,
        ReadWrite = 3
    };
    typedef enum AccessType AccessType;

    [repeatable]
    [source_annotation_attribute(Parameter)]
    struct PreAttribute {
        PreAttribute();
        unsigned int Deref;
        YesNoMaybe Valid;
        YesNoMaybe Null;
        YesNoMaybe Tainted;
        AccessType Access;
        size_t ValidElementsConst;
        size_t ValidBytesConst;
        const wchar_t* ValidElements;
        const wchar_t* ValidBytes;
        const wchar_t* ValidElementsLength;
        const wchar_t* ValidBytesLength;
        size_t WritableElementsConst;
        size_t WritableBytesConst;
        const wchar_t* WritableElements;
        const wchar_t* WritableBytes;
        const wchar_t* WritableElementsLength;
        const wchar_t* WritableBytesLength;
        size_t ElementSizeConst;
        const wchar_t* ElementSize;
        YesNoMaybe NullTerminated;
        const wchar_t* Condition;
    };

    [repeatable]
    [source_annotation_attribute(Parameter|ReturnValue)]
    struct PostAttribute {
        PostAttribute();
        unsigned int Deref;
        YesNoMaybe Valid;
        YesNoMaybe Null;
        YesNoMaybe Tainted;
        AccessType Access;
        size_t ValidElementsConst;
        size_t ValidBytesConst;
        const wchar_t* ValidElements;
        const wchar_t* ValidBytes;
        const wchar_t* ValidElementsLength;
        const wchar_t* ValidBytesLength;
        size_t WritableElementsConst;
        size_t WritableBytesConst;
        const wchar_t* WritableElements;
        const wchar_t* WritableBytes;
        const wchar_t* WritableElementsLength;
        const wchar_t* WritableBytesLength;
        size_t ElementSizeConst;
        const wchar_t* ElementSize;
        YesNoMaybe NullTerminated;
        YesNoMaybe MustCheck;
        const wchar_t* Condition;
    };

    [source_annotation_attribute(Parameter)]
    struct FormatStringAttribute {
        FormatStringAttribute();
        const wchar_t* Style;
        const wchar_t* UnformattedAlternative;
    };

    [repeatable]
    [source_annotation_attribute(ReturnValue)]
    struct InvalidCheckAttribute {
        InvalidCheckAttribute();
        long Value;
    };

    [source_annotation_attribute(Method)]
    struct SuccessAttribute {
        SuccessAttribute();
        const wchar_t* Condition;
    };

    [repeatable]
    [source_annotation_attribute(Parameter)]
    struct PreBoundAttribute {
        PreBoundAttribute();
        unsigned int Deref;
    };

    [repeatable]
    [source_annotation_attribute(Parameter|ReturnValue)]
    struct PostBoundAttribute {
        PostBoundAttribute();
        unsigned int Deref;
    };

    [repeatable]
    [source_annotation_attribute(Parameter)]
    struct PreRangeAttribute {
        PreRangeAttribute();
        unsigned int Deref;
        const char* MinVal;
        const char* MaxVal;
    };

    [repeatable]
    [source_annotation_attribute(Parameter|ReturnValue)]
    struct PostRangeAttribute {
        PostRangeAttribute();
        unsigned int Deref;
        const char* MinVal;
        const char* MaxVal;
    };
}

typedef ::vc_attributes::YesNoMaybe SA_YesNoMaybe;
const ::vc_attributes::YesNoMaybe SA_Yes = ::vc_attributes::Yes;
const ::vc_attributes::YesNoMaybe SA_No = ::vc_attributes::No;
const ::vc_attributes::YesNoMaybe SA_Maybe = ::vc_attributes::Maybe;

typedef ::vc_attributes::AccessType SA_AccessType;
const ::vc_attributes::AccessType SA_NoAccess = ::vc_attributes::NoAccess;
const ::vc_attributes::AccessType SA_Read = ::vc_attributes::Read;
const ::vc_attributes::AccessType SA_Write = ::vc_attributes::Write;
const ::vc_attributes::AccessType SA_ReadWrite = ::vc_attributes::ReadWrite;

typedef ::vc_attributes::PreAttribute          SA_Pre;
typedef ::vc_attributes::PostAttribute         SA_Post;

typedef ::vc_attributes::FormatStringAttribute SA_FormatString;

typedef ::vc_attributes::InvalidCheckAttribute SA_InvalidCheck;

typedef ::vc_attributes::SuccessAttribute      SA_Success;

typedef ::vc_attributes::PreBoundAttribute     SA_PreBound;
typedef ::vc_attributes::PostBoundAttribute    SA_PostBound;

typedef ::vc_attributes::PreRangeAttribute     SA_PreRange;
typedef ::vc_attributes::PostRangeAttribute    SA_PostRange;
