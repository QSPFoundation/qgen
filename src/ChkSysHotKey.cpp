#include "ChkSysHotKey.h"

bool ChkSysHotKey::CheckSystemHotKeys(wxMenuBar *m, int _keyCode, int _flags)
{
	keyCode = _keyCode;
	flags = _flags;

	WalkMenuBar(m);

	return isInMenu;
}

void ChkSysHotKey::WalkMenuBar(wxMenuBar *m)
{
	for (int i = 0; i < (int)m->GetMenuCount(); i++)
	{
		WalkMenu(m->GetMenu(i));
	}
}

void ChkSysHotKey::WalkMenu(wxMenu *m)
{
	for (int j = 0; j < (int)m->GetMenuItemCount(); j++)
	{
		wxMenuItem *menuItem = m->GetMenuItems().Item(j)->GetData();
		if (menuItem->GetKind() != wxITEM_SEPARATOR && 
			menuItem->GetLabel() != wxEmptyString)
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
	if(a)
	{
		if (keyCode == a->GetKeyCode() && flags == a->GetFlags())
		{
			isInMenu = true;
		}
	}
}
