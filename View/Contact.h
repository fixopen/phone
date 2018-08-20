#ifndef __VIEW_CONTACT_H__
#define __VIEW_CONTACT_H__

#include "../Data/Contact.h"

#if 0
Contact* p = new Contact;
p->number = xxx;
ContactDialog->SetContact(p);
ContactDialog->show();
//...
ContactDialog->OnOk();
{
	p->telephone = contactDialog->telcphone;
	//..
}
p->insert();
delete p;

Setting* p = Setting::GetFromDatabase("alias = 'zs'");
SettingDialog->SetSetting(p);
SettingDialog->show();
//...
SettingDialog->OnOk();
{
	p->telephone = SettingDialog->telcphone;
	//..
}
p->update();
delete p;
#endif

namespace View
{
	class ContactDialog : public CDialog
	{
	public:
		//record this pointer, init form control's value by this object's field
		//when this form destory, update object's field by form control's value
		void SetContact(Data::Contact* const contact);
	};
}

#endif //__VIEW_CONTACT_H__