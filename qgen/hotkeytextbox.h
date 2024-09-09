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

#ifndef _QGEN_HOTKEY_TEXT_BOX_H_
    #define _QGEN_HOTKEY_TEXT_BOX_H_

    #include <wx/wx.h>

    class HotKeyTextBox : public wxTextCtrl
    {
        DECLARE_CLASS(HotKeyTextBox)
        DECLARE_EVENT_TABLE()
    private:
        wxString _hotKey;
        int _hotKeyCode;
        int _flags;

        void OnKeyDown(wxKeyEvent& event);
        void OnKeyUp(wxKeyEvent& event);
        void AppendAccel(wxString &data, const wxString &key) const;
    public:
        HotKeyTextBox(wxWindow *parent, wxWindowID id, const wxString &value = wxEmptyString);

        void SetFlags(int flags) { _flags = flags; }
        int GetFlags() const { return _flags; }
        void SetHotKeyCode(int hotKeyCode) { _hotKeyCode = hotKeyCode; }
        int GetHotKeyCode() const { return _hotKeyCode; }
    };

#endif
