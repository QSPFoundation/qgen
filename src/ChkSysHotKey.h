#ifndef _QUEST_GENERATOR_CHECK_SYSTEM_HOTKEYS_H
	#define _QUEST_GENERATOR_CHECK_SYSTEM_HOTKEYS_H

	#include <wx/wx.h>

	class ChkSysHotKey
	{
		bool isInMenu;
		int keyCode;
		int flags;
		void WalkMenuBar(wxMenuBar *m);
		void WalkMenu(wxMenu *m);
		void WalkMenuItem(wxMenuItem *m);

	public:
		ChkSysHotKey(void) { isInMenu = false; }
		~ChkSysHotKey(void) {}

		bool CheckSystemHotKeys(wxMenuBar *m, int _keyCode, int _flags);
	};

#endif //_QUEST_GENERATOR_CHECK_SYSTEM_HOTKEYS_H
