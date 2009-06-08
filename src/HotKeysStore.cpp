// Copyright (C) 2005-2009 Valeriy Argunov (nporep AT mail DOT ru)
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

#include "HotKeysStore.h"
#include "wx/arrimpl.cpp"

WX_DEFINE_OBJARRAY(HotkeyDataArray);

HotkeysStore::HotkeysStore()
{

}

size_t HotkeysStore::AddHotkeyData( const HotkeyData &hotKeyData )
{
	int index = FindHotkeyDataIndex(hotKeyData.Hotkey);
	if (index < 0)
	{
		_hotkeysData.Add(hotKeyData);
		return _hotkeysData.GetCount() - 1;
	}
	_hotkeysData[index] = hotKeyData;
	return (size_t)index;
}

bool HotkeysStore::DeleteHotkeyData( const wxString &hotKey )
{
	int index = FindHotkeyDataIndex(hotKey);
	if ( index < 0) return false;
	_hotkeysData.RemoveAt(index);
	return true;
}

void HotkeysStore::ClearHotkeysData()
{
	_hotkeysData.Clear();
}

HotkeyData & HotkeysStore::GetHotkeyData( size_t index )
{
	return _hotkeysData[index];
}

int HotkeysStore::FindHotkeyDataIndex(const wxString &hotKey)
{
	wxString stHotKey(hotKey.Lower());
	size_t i, count = _hotkeysData.GetCount();
	for (i = 0; i < count; i++)
		if (stHotKey == _hotkeysData[i].Hotkey.Lower()) return (int)i;
	return wxNOT_FOUND;
}

size_t HotkeysStore::GetHotkeysCount()
{
	return _hotkeysData.GetCount();
}

void HotkeysStore::SaveHotkeysData(wxConfigBase &fileConfig)
{
	wxString str;
	size_t count = _hotkeysData.GetCount();
	fileConfig.DeleteGroup(wxT("HotKeys"));
	for (size_t i = 0; i < count; i++)
	{
		str = wxString::Format(wxT("HotKeys/Hotkey%d_Keys"), i);
		fileConfig.Write(str, _hotkeysData[i].Hotkey);
		str = wxString::Format(wxT("HotKeys/Hotkey%d_Exec"), i);
		fileConfig.Write(str, _hotkeysData[i].CommandText);
	}
}

void HotkeysStore::LoadHotkeysData(wxConfigBase &fileConfig)
{
	wxString str, keyVal, dataVal;
	size_t i = 0;
	_hotkeysData.Clear();
	while (1)
	{
		str = wxString::Format(wxT("HotKeys/Hotkey%d_Keys"), i);
		if (!fileConfig.Read(str, &keyVal)) break;
		str = wxString::Format(wxT("HotKeys/Hotkey%d_Exec"), i);
		if (!fileConfig.Read(str, &dataVal)) break;
		AddHotkeyData(HotkeyData(keyVal, dataVal));
		++i;
	}
}
