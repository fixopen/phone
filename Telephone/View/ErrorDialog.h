#ifndef __VIEW_ERRORDIALOG_H__
#define __VIEW_ERRORDIALOG_H__

#pragma once

namespace View {
	class ErrorDialog : public CWnd { // ErrorDialog
		DECLARE_DYNAMIC(ErrorDialog)
	public:
		ErrorDialog();
		virtual ~ErrorDialog();

	protected:
		DECLARE_MESSAGE_MAP()
	};
}

#endif //__VIEW_ERRORDIALOG_H__
