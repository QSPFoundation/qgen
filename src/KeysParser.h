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

#ifndef _QUEST_GENERATOR_KEYS_PARSER_H
	#define _QUEST_GENERATOR_KEYS_PARSER_H
	
	#include <wx/wx.h>
	#include <wx/dynarray.h>
	#include <map>

	#ifdef __WXMSW__
		#include <Windows.h>
	#endif
	
	#include "HotKeysStore.h"

	WX_DECLARE_OBJARRAY(INPUT, KeysArray);

	class KeysParser
	{
		std::map<wxString, int> _keysTable;
		HotkeysStore *_hotKeysStore;

		void InitKeysTable();
		void OnKeyPress(int keyCode);
		void OnKeysPress(const wxString &text);
		bool IsHotkeyMatches(int keyCode, int modifiers, const wxString &hotkey);
	public:
		KeysParser(HotkeysStore *hotKeysStore);
		bool ExecuteHotkeyAction(int keyCode, int modifiers);	
		void ParseText(const wxString &text);

	};
#endif