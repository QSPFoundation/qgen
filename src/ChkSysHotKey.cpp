// Copyright (C) 2005-2009
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

#include "ChkSysHotKey.h"

bool ChkSysHotKey::CheckSystemHotKeys(wxMenuBar *m, int keyCode, int flags)
{
	_isInMenu = false;
	_keyCode = keyCode;
	_flags = flags;

	WalkMenuBar(m);

	return _isInMenu;
}

void ChkSysHotKey::WalkMenuBar(wxMenuBar *m)
{
	for (size_t i = 0; i < m->GetMenuCount(); ++i)
		WalkMenu(m->GetMenu(i));
}

void ChkSysHotKey::WalkMenu(wxMenu *m)
{
	wxMenuItemList &list = m->GetMenuItems();
	for (int i = 0; i < list.GetCount(); ++i)
	{
		wxMenuItem *menuItem = list.Item(i)->GetData();
		if (menuItem->GetKind() != wxITEM_SEPARATOR)
			WalkMenuItem(menuItem);
	}
}

void ChkSysHotKey::WalkMenuItem(wxMenuItem *m)
{
	if (m->GetSubMenu())
	{
		WalkMenu(m->GetSubMenu());
		return;
	}
	wxAcceleratorEntry *a = m->GetAccel();
	if (a && _keyCode == a->GetKeyCode() && _flags == a->GetFlags())
		_isInMenu = true;
}
