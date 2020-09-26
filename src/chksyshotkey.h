// Copyright (C) 2005-2012
// Vladimir Bauer (baxzzzz AT gmail DOT com)
// Nex (nex AT otaku DOT ru)
// Shchannikov Dmitry (rrock DOT ru AT gmail DOT com)
// Valeriy Argunov (nporep AT mail DOT ru)
/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#ifndef _QGEN_CHECK_SYSTEM_HOTKEY_H_
    #define _QGEN_CHECK_SYSTEM_HOTKEY_H_

    #include <wx/wx.h>

    class ChkSysHotKey
    {
    private:
		bool    _isInMenu;
		int		_keyCode;
		int		_flags;

		void WalkMenuBar(wxMenuBar *menuBar);
		void WalkMenu(wxMenu *menu);
		void WalkMenuItem(wxMenuItem *menuItem);
    public:
		bool CheckSystemHotKeys(wxMenuBar *menuBar, int keyCode, int flags);
    };

#endif
