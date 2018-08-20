#ifndef __VIEW_CONTACTINFO_H__
#define __VIEW_CONTACTINFO_H__

#include "../Data/ContactData.h"

namespace View
{
	class ContactInfoDialog : public CDialog
	{
	public:
		//record this pointer, init form control's value by this object's field
		//when this form destory, update object's field by form control's value
		void SetContactInfo(Data::ContactInfo* const contactInfo);
	};
}

#endif //__VIEW_CONTACT_H__