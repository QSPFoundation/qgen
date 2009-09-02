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
		int hotKeyCode;
		int flags;
		HotkeyData(){ }
		HotkeyData(const int &_hotKeyCode, const int &_flags, const wxString &data)
		{
			wxString tmp;
			bool first = true;

			hotKeyCode = _hotKeyCode;
			flags = _flags;

			if (flags & wxACCEL_ALT) 
			{
				tmp.Append(wxT("Alt"));
				first = false;
			}

			if (flags & wxACCEL_CTRL) 
				if (first) 
				{
					tmp.Append(wxT("Ctrl"));
					first = false;
				}
				else
					tmp.Append(wxT(" + Ctrl"));

			if (flags & wxACCEL_SHIFT)
				if (first)
				{
					tmp.Append(wxT("Shift"));
					first = false;
				}
				else
					tmp.Append(wxT(" + Shift"));

			Hotkey = wxString::Format(wxT("%s + %c"), tmp, hotKeyCode);

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
