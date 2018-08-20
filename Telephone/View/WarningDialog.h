#ifndef __VIEW_WARNINGDIALOG_H__
#define __VIEW_WARNINGDIALOG_H__

#pragma once

namespace View {
	class WarningDialog : public CWnd { // WarningDialog
		DECLARE_DYNAMIC(WarningDialog)

	public:
		WarningDialog();
		virtual ~WarningDialog();

	protected:
		DECLARE_MESSAGE_MAP()
	};
}

#endif //__VIEW_WARNINGDIALOG_H__
