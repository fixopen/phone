#ifndef __VIEW_NOTIFYDIALOG_H__
#define __VIEW_NOTIFYDIALOG_H__

#pragma once

namespace View {
	class NotifyDialog : public CWnd { // NotifyDialog
		DECLARE_DYNAMIC(NotifyDialog)
	public:
		NotifyDialog();
		virtual ~NotifyDialog();

	protected:
		DECLARE_MESSAGE_MAP()
	};
}

#endif //__VIEW_NOTIFYDIALOG_H__
