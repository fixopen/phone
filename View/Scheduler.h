#ifndef __VIEW_SCHEDULER_H__
#define __VIEW_SCHEDULER_H__

#include "../Data/SchedulerData.h"

namespace View
{
	class SchedulerDialog : public CDialog
	{
	public:
		//record this pointer, init form control's value by this object's field
		//when this form destory, update object's field by form control's value
		void SetScheduler(Data::Scheduler* const scheduler);
	};
}

#endif //__VIEW_SCHEDULER_H__