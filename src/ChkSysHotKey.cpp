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
