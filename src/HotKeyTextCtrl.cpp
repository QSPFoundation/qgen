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

#include "HotKeyTextCtrl.h"

IMPLEMENT_CLASS(HotKeyTextCtrl, wxTextCtrl)

BEGIN_EVENT_TABLE(HotKeyTextCtrl, wxTextCtrl)
	EVT_KEY_DOWN(HotKeyTextCtrl::OnKeyDown)
	EVT_KEY_UP(HotKeyTextCtrl::OnKeyUp)
END_EVENT_TABLE()

HotKeyTextCtrl::HotKeyTextCtrl( wxWindow *parent, wxWindowID id, const wxString &value/*=wxEmptyString*/ ) : wxTextCtrl( parent, id, value)
{
	_isKeyDown = false;
	_isAltDown = false;
	_isCtrlDown = false;
	_isShiftDown = false;
	_isSymbolDown = false;
}

void HotKeyTextCtrl::OnKeyDown( wxKeyEvent& event )
{
	int keyCode = event.GetKeyCode();
	if(!_isKeyDown)	
		_hotKey.Clear();
	if (event.GetModifiers() != wxMOD_NONE)
	{
		if (event.AltDown() && !_isAltDown)
		{
			(_isKeyDown) ? _hotKey.Append(wxT(" + Alt")) : _hotKey.Append(wxT("Alt"));
			_isAltDown = true;
			_isKeyDown = true;
		}
		if (event.ControlDown() && !_isCtrlDown)
		{
			(_isKeyDown) ? _hotKey.Append(wxT(" + Ctrl")) : _hotKey.Append(wxT("Ctrl"));
			_isCtrlDown = true;
			_isKeyDown = true;
		}
		if (event.ShiftDown() && !_isShiftDown)
		{
			(_isKeyDown) ? _hotKey.Append(wxT(" + Shift")) : _hotKey.Append(wxT("Shift"));
			_isShiftDown = true;
			_isKeyDown = true;
		}
		if (keyCode >= wxT('A') && keyCode <= wxT('Z') && !_isSymbolDown)
		{
			_hotKey = wxString::Format(wxT("%s + %c"), _hotKey.wx_str(), keyCode);
			_isSymbolDown = true;
		}
		SetValue(_hotKey);
	}
}

void HotKeyTextCtrl::OnKeyUp( wxKeyEvent& event )
{
	if(!_isSymbolDown && _isKeyDown)
	{
		_hotKey.Clear();
		SetValue(_hotKey);
	}
	_isKeyDown = false;
	_isAltDown = false;
	_isCtrlDown = false;
	_isShiftDown = false;
	_isSymbolDown = false;
}
