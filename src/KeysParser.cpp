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

#include "KeysParser.h"
#include "wx/arrimpl.cpp"

WX_DEFINE_OBJARRAY(KeysArray);

KeysParser::KeysParser(HotkeysStore *hotKeysStore)
{
	_hotKeysStore = hotKeysStore;
	InitKeysTable();
}

void KeysParser::InitKeysTable()
{
	_keysTable["EVENT"] = 0xFF;
	_keysTable["LEFT"] = VK_LEFT;
	_keysTable["RIGHT"] = VK_RIGHT;
	_keysTable["UP"] = VK_UP;
	_keysTable["DOWN"] = VK_DOWN;
	_keysTable["CTRL"] = VK_CONTROL;
	_keysTable["SHIFT"] = VK_SHIFT;
	_keysTable["ALT"] = VK_MENU;
	_keysTable["TAB"] = VK_TAB;
	_keysTable["SPACE"] = VK_SPACE;
	_keysTable["ESC"] = VK_ESCAPE;
	_keysTable["ENTER"] = VK_RETURN;
	_keysTable["BACKSPACE"] = VK_BACK;
	_keysTable["INS"] = VK_INSERT;
	_keysTable["HOME"] = VK_HOME;
	_keysTable["DEL"] = VK_DELETE;
	_keysTable["END"] = VK_END;
	_keysTable["PAGEUP"] = VK_PRIOR;
	_keysTable["PAGEDOWN"] = VK_NEXT;
	_keysTable["F1"] = VK_F1;
	_keysTable["F2"] = VK_F2;
	_keysTable["F3"] = VK_F3;
	_keysTable["F4"] = VK_F4;
	_keysTable["F5"] = VK_F5;
	_keysTable["F6"] = VK_F6;
	_keysTable["F7"] = VK_F7;
	_keysTable["F8"] = VK_F8;
	_keysTable["F9"] = VK_F9;
	_keysTable["F10"] = VK_F10;
	_keysTable["F11"] = VK_F11;
	_keysTable["F12"] = VK_F12;
}

void KeysParser::ReleaseAlt()
{
	INPUT input;
	memset(&input, 0, sizeof(input));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VK_MENU;
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	::SendInput(1, &input, sizeof(INPUT));
}

void KeysParser::OnKeyPress(int keyCode)
{
	INPUT input;
	memset(&input, 0, sizeof(input));
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = keyCode;
	input.ki.dwFlags = KEYEVENTF_UNICODE;
	::SendInput(1, &input, sizeof(INPUT));
	input.ki.dwFlags |= KEYEVENTF_KEYUP;
	::SendInput(1, &input, sizeof(INPUT));
}

void KeysParser::ParseText(const wxString &text)
{
	wxString keys;
	wxString::const_iterator i = text.begin();
	BYTE oldStates[256];
	::GetKeyboardState(oldStates);
	if (oldStates[VK_MENU]) ReleaseAlt();
	for (int key = 0; key < 256; ++key)
		oldStates[key] &= 0x7F;
	::SetKeyboardState(oldStates);
	while (i != text.end())
	{
		if (*i == wxT('{'))
		{
			keys.clear();
			while (++i != text.end())
			{
				if (*i == wxT('}'))
				{
					OnKeysPress(keys);
					break;
				}
				keys.append(*i);
			}
			if (i == text.end()) break;
		}
		else
			OnKeyPress(*i);
		++i;
	}
	wxYieldIfNeeded();
}

void KeysParser::OnKeysPress(const wxString &text)
{
	INPUT inputs[9];
	memset(inputs, 0, sizeof(inputs));
	wxArrayString strs = wxSplit(text.Upper(), wxT('+'));
	int length, code, count = 0;
	wxString str;
	bool hasEvent = false;
	for (size_t i = 0; i < strs.GetCount(); ++i)
	{
		if (count >= 9) break;
		str = strs[i].Trim().Trim(false);
		length = str.length();
		if (length == 0) continue;
		code = (length > 1 ? _keysTable[str] : str[0]);
		switch (code)
		{
		case 0xFF:
			hasEvent = true;
		case 0:
			continue;
		}
		inputs[count].type = INPUT_KEYBOARD;
		inputs[count].ki.wVk = code;
		++count;
	}
	if (count > 0)
	{
		::SendInput(count, inputs, sizeof(INPUT));
		for (size_t i = 0; i < count; ++i)
			inputs[i].ki.dwFlags |= KEYEVENTF_KEYUP;
		::SendInput(count, inputs, sizeof(INPUT));
	}
	if (hasEvent) wxYieldIfNeeded();
}

bool KeysParser::ExecuteHotkeyAction( int keyCode, int modifiers )
{
	size_t countHotKeys = _hotKeysStore->GetHotkeysCount();
	for (size_t i = 0; i < countHotKeys; ++i)
	{
		const HotkeyData &hotKey = _hotKeysStore->GetHotkeyData(i);
		if (keyCode == hotKey.HotKeyCode && modifiers == hotKey.Flags)
		{
			ParseText(hotKey.CommandText);
			return true;
		}
	}
	return false;
}
