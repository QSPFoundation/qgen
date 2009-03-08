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

WX_DEFINE_OBJARRAY(HotKeyDataArray);

HotKeysStore::HotKeysStore()
{

}

size_t HotKeysStore::SetHotKeyData( const HotKeyData &hotKeyData )
{
	int index = FindHotKeyDataIndex(hotKeyData.HotKey);
	if (index < 0)
	{
		_hotKeysData.Add(hotKeyData);
		return _hotKeysData.GetCount() - 1;
	}
	_hotKeysData[index] = hotKeyData;
	return (size_t)index;
}

bool HotKeysStore::DeleteHotKeyData( const wxString &hotKey )
{
	int index = FindHotKeyDataIndex(hotKey);
	if ( index < 0) return false;
	_hotKeysData.RemoveAt(index);
	return true;
}

void HotKeysStore::ClearHotKeysData()
{
	_hotKeysData.Clear();
}

HotKeyData & HotKeysStore::GetHotKeyData( size_t index )
{
	return _hotKeysData[index];
}

int HotKeysStore::FindHotKeyDataIndex(const wxString &hotKey)
{
	wxString stHotKey(hotKey.Lower());
	size_t i, count = _hotKeysData.GetCount();
	for (i = 0; i < count; i++)
		if (stHotKey == _hotKeysData[i].HotKey.Lower()) return (int)i;
	return wxNOT_FOUND;
}

size_t HotKeysStore::GetCountHotKeys()
{
	return _hotKeysData.GetCount();
}

void HotKeysStore::SaveHotKeysData(wxConfigBase &fileConfig)
{
	wxString str;
	size_t count = _hotKeysData.GetCount();
	fileConfig.DeleteGroup(wxT("HotKeys"));
	for (size_t i = 0; i < count; i++)
	{
		str = wxString::Format(wxT("HotKeys/HotKey%d_Keys"), i);
		fileConfig.Write(str, _hotKeysData[i].HotKey);
		str = wxString::Format(wxT("HotKeys/HotKey%d_Exec"), i);
		fileConfig.Write(str, _hotKeysData[i].CommandText);
	}
}

void HotKeysStore::LoadHotKeysData(wxConfigBase &fileConfig)
{
	wxString str, keyVal, dataVal;
	size_t i = 0;
	_hotKeysData.Clear();
	while (1)
	{
		str = wxString::Format(wxT("HotKeys/HotKey%d_Keys"), i);
		if (!fileConfig.Read(str, &keyVal)) break;
		str = wxString::Format(wxT("HotKeys/HotKey%d_Exec"), i);
		if (!fileConfig.Read(str, &dataVal)) break;
		SetHotKeyData(HotKeyData(keyVal, dataVal));
		++i;
	}
}
