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

	struct HotKeyData
	{
		wxString HotKey;
		wxString CommandText;
		HotKeyData(){ }
		HotKeyData(const wxString &keys, const wxString &data)
		{
			HotKey = keys;
			CommandText = data;
		}
	};

	WX_DECLARE_OBJARRAY(HotKeyData, HotKeyDataArray);

	class HotKeysStore
	{
	private:
		HotKeyDataArray _hotKeysData;
	public:
		HotKeysStore();
		size_t SetHotKeyData(const HotKeyData &hotKeyData);
		bool DeleteHotKeyData(const wxString &hotKey);
		void ClearHotKeysData();
		size_t GetCountHotKeys();
		HotKeyData &GetHotKeyData(size_t index);
		int FindHotKeyDataIndex(const wxString &hotKey);
		void SaveHotKeysData(wxConfigBase &fileConfig);
		void LoadHotKeysData(wxConfigBase &fileConfig);
	};

#endif
