#ifndef __VIEW_LISTTASKPROGRESS_H__
#define __VIEW_LISTTASKPROGRESS_H__

#pragma once

namespace View {
	class ListTaskProgress : public CWnd { // ListTaskProgress
		DECLARE_DYNAMIC(ListTaskProgress)

	public:
		ListTaskProgress();
		virtual ~ListTaskProgress();

	protected:
		DECLARE_MESSAGE_MAP()
	};
}

#endif //__VIEW_LISTTASKPROGRESS_H__
