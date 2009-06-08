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

#ifndef _QUEST_GENERATOR_HOT_KEYS_STORE_H
	#define _QUEST_GENERATOR_HOT_KEYS_STORE_H

	#include <wx/wx.h>
	#include <wx/fileconf.h>
	#include <wx/dynarray.h> 

	struct HotkeyData
	{
		wxString Hotkey;
		wxString CommandText;
		HotkeyData(){ }
		HotkeyData(const wxString &keys, const wxString &data)
		{
			Hotkey = keys;
			CommandText = data;
		}
	};

	WX_DECLARE_OBJARRAY(HotkeyData, HotkeyDataArray);

	class HotkeysStore
	{
	private:
		HotkeyDataArray _hotkeysData;
	public:
		HotkeysStore();
		size_t AddHotkeyData(const HotkeyData &hotkeyData);
		bool DeleteHotkeyData(const wxString &hotkey);
		void ClearHotkeysData();
		size_t GetHotkeysCount();
		HotkeyData &GetHotkeyData(size_t index);
		int FindHotkeyDataIndex(const wxString &hotkey);
		void SaveHotkeysData(wxConfigBase &fileConfig);
		void LoadHotkeysData(wxConfigBase &fileConfig);
	};

#endif
