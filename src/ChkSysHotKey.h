#ifndef _QUEST_GENERATOR_CHECK_SYSTEM_HOTKEYS_H
	#define _QUEST_GENERATOR_CHECK_SYSTEM_HOTKEYS_H

	#include <wx/wx.h>

	class ChkSysHotKey
	{
	private:
		bool _isInMenu;
		int _keyCode;
		int _flags;
		void WalkMenuBar(wxMenuBar *m);
		void WalkMenu(wxMenu *m);
		void WalkMenuItem(wxMenuItem *m);

	public:
		bool CheckSystemHotKeys(wxMenuBar *m, int keyCode, int flags);
	};

#endif //_QUEST_GENERATOR_CHECK_SYSTEM_HOTKEYS_H
