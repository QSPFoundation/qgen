// Copyright (C) 2005-2012
// Vladimir Bauer (baxzzzz AT gmail DOT com)
// Nex (nex AT otaku DOT ru)
// Shchannikov Dmitry (rrock DOT ru AT gmail DOT com)
// Valeriy Argunov (byte AT qsp DOT org)
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

#include "hotkeytextbox.h"

IMPLEMENT_CLASS(HotKeyTextBox, wxTextCtrl)

BEGIN_EVENT_TABLE(HotKeyTextBox, wxTextCtrl)
    EVT_KEY_DOWN(HotKeyTextBox::OnKeyDown)
    EVT_KEY_UP(HotKeyTextBox::OnKeyUp)
END_EVENT_TABLE()

HotKeyTextBox::HotKeyTextBox( wxWindow *parent, wxWindowID id, const wxString &value ) :
    wxTextCtrl( parent, id, value, wxDefaultPosition, wxSize(150, -1), wxTE_READONLY)
{
    _hotKeyCode = 0;
    _flags = 0;
}

void HotKeyTextBox::AppendAccel(wxString &data, const wxString &key) const
{
    if (data.IsEmpty())
        data.Append(key);
    else
        data.Append(wxString::Format(wxT("+%s"), key.wx_str()));
}

void HotKeyTextBox::OnKeyDown( wxKeyEvent& event )
{
    int keyCode = event.GetKeyCode();
    if (_hotKeyCode)
    {
        _hotKey.Clear();
        _flags = 0;
        _hotKeyCode = 0;
    }
    if (event.GetModifiers() != wxMOD_NONE)
    {
        if (event.AltDown() && !(_flags & wxACCEL_ALT))
        {
            AppendAccel(_hotKey, wxT("Alt"));
            _flags |= wxACCEL_ALT;
        }
        if (event.ControlDown() && !(_flags & wxACCEL_CTRL))
        {
            AppendAccel(_hotKey, wxT("Ctrl"));
            _flags |= wxACCEL_CTRL;
        }
        if (event.ShiftDown() && !(_flags & wxACCEL_SHIFT))
        {
            AppendAccel(_hotKey, wxT("Shift"));
            _flags |= wxACCEL_SHIFT;
        }
        if (((keyCode >= wxT('A') && keyCode <= wxT('Z')) ||
            (keyCode >= wxT('0') && keyCode <= wxT('9'))) &&
            !_hotKeyCode)
        {
            _hotKey = wxString::Format(wxT("%s+%c"), _hotKey.wx_str(), keyCode);
            _hotKeyCode = keyCode;
        }
        SetValue(_hotKey);
    }
}

void HotKeyTextBox::OnKeyUp( wxKeyEvent& event )
{
    if (!_hotKeyCode)
    {
        Clear();
        _hotKey.Clear();
        _flags = 0;
    }
}
