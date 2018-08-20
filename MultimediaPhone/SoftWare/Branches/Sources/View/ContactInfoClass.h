#ifndef __VIEW_CONTACTINFOCLASS_H__
#define __VIEW_CONTACTINFOCLASS_H__

#include "../Data/ContactData.h"

namespace View
{
	class ContactInfoClassDialog : public CDialog
	{
	public:
		//record this pointer, init form control's value by this object's field
		//when this form destory, update object's field by form control's value
		//void SetContactInfoClass(Data::ContactInfoClass* const contactInfoClass);
	};
}

#endif //__VIEW_CONTACT_H__