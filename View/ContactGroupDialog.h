#ifndef __VIEW_CONTACTGROUPDIALOG_H__
#define __VIEW_CONTACTGROUPDIALOG_H__

#include "../Data/ContactGroup.h"

namespace View
{
	class ContactGroupDialog : public CDialog
	{
	public:
		//record this pointer, init form control's value by this object's field
		//when this form destory, update object's field by form control's value
		void SetContactGroup(Data::ContactGroup* const contactGroup);
	};
}

#endif //__VIEW_CONTACTGROUPDIALOG_H__