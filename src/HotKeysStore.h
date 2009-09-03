// Copyright (C) 2005-2009
// BaxZzZz (bauer_v AT mail DOT ru)
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

#ifndef _QUEST_GENERATOR_HOT_KEYS_STORE_H
	#define _QUEST_GENERATOR_HOT_KEYS_STORE_H

	#include <wx/wx.h>
	#include <wx/fileconf.h>
	#include <wx/dynarray.h>

	struct HotkeyData
	{
		wxString CommandText;
		int HotKeyCode;
		int Flags;

		HotkeyData()
		{
			HotKeyCode = 0;
			Flags = 0;
		}

		HotkeyData(int hotKeyCode, int flags, const wxString &data)
		{
			HotKeyCode = hotKeyCode;
			Flags = flags;
			CommandText = data;
		}

		wxString GetKeysAsString() const
		{
			wxString tmp;
			if (Flags & wxACCEL_ALT)
				AppendAccel(tmp, wxT("Alt"));
			if (Flags & wxACCEL_CTRL)
				AppendAccel(tmp, wxT("Ctrl"));
			if (Flags & wxACCEL_SHIFT)
				AppendAccel(tmp, wxT("Shift"));
			return wxString::Format(wxT("%s + %c"), tmp.wx_str(), HotKeyCode);
		}

	private:
		void AppendAccel(wxString &data, const wxString &key) const
		{
			if (data.IsEmpty())
				data.Append(key);
			else
				data.Append(wxString::Format(wxT(" + %s"), key.wx_str()));
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
		bool DeleteHotkeyData(const HotkeyData &hotkey);
		void ClearHotkeysData();
		size_t GetHotkeysCount();
		HotkeyData &GetHotkeyData(size_t index);
		int FindHotkeyDataIndex(const HotkeyData &hotkey);
		void SaveHotkeysData(wxConfigBase &fileConfig);
		void LoadHotkeysData(wxConfigBase &fileConfig);
	};

#endif
