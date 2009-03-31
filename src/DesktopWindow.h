#ifndef _DESKTOP_WINDOW_H_
	#define _DESKTOP_WINDOW_H_

	class DesktopWindow : public wxWindow
	{
	public:
		HWND GetHWND() const { return ::GetDesktopWindow(); }
	};

#endif
