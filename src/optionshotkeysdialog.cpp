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

#include "optionshotkeysdialog.h"

IMPLEMENT_CLASS(OptionsHotkeysDialog, wxDialog)

BEGIN_EVENT_TABLE(OptionsHotkeysDialog, wxDialog)
    EVT_BUTTON(wxID_OK, OptionsHotkeysDialog::OnOkSettings)
    EVT_BUTTON(ID_HOTKEY_HELP, OptionsHotkeysDialog::OnHelpHotKeys)
END_EVENT_TABLE()

OptionsHotkeysDialog::OptionsHotkeysDialog(wxWindow *parent, const wxString& title, IControls *controls, int style) :
    wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, style)
{
    _controls = controls;
    wxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer *hotKeySizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText *stText01 = new wxStaticText(this, wxID_ANY, _("Hotkeys:"));
    _txtInputHotkey = new HotKeyTextBox(this, wxID_ANY);

    hotKeySizer->Add(stText01, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    hotKeySizer->Add(_txtInputHotkey, 0, wxALL, 5);

    wxStaticText *stText02 = new wxStaticText(this, wxID_ANY, _("Text:"));
    _txtInputText = new SyntaxTextBox(this, controls, SYNTAX_STYLE_COLORED | SYNTAX_STYLE_NOHOTKEYS |
                                                      SYNTAX_STYLE_SIMPLEMENU | SYNTAX_STYLE_NOHELPTIPS);

    wxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);
    _btnHelp = new wxButton(this, ID_HOTKEY_HELP, _("Help"));
    _btnOK = new wxButton(this, wxID_OK, _("OK"));
    _btnCancel = new wxButton(this, wxID_CANCEL, _("Cancel"));

    btnSizer->Add(_btnHelp, 0, wxLEFT|wxBOTTOM, 5);
    btnSizer->AddStretchSpacer(1);
    btnSizer->Add(_btnOK, 0, wxRIGHT|wxBOTTOM, 5);
    btnSizer->Add(_btnCancel, 0, wxRIGHT|wxBOTTOM, 5);

    topSizer->Add(hotKeySizer, 0);
    topSizer->Add(stText02, 0, wxLEFT|wxTOP, 5);
    topSizer->Add(_txtInputText, 1, wxALL|wxGROW, 5);
    topSizer->Add(btnSizer, 0, wxGROW);

    SetSizerAndFit(topSizer);
    SetAutoLayout(true);
    SetMinClientSize(wxSize(330, 230));
    SetSize(wxSize(630, 430));

    _txtInputHotkey->SetFocus();
    _btnOK->SetDefault();
}

void OptionsHotkeysDialog::OnOkSettings(wxCommandEvent &event)
{
    _hotkeyData = HotkeyData(_txtInputHotkey->GetHotKeyCode(), _txtInputHotkey->GetFlags(), _txtInputText->GetValue());
    event.Skip();
}

void OptionsHotkeysDialog::SetHotkeyData( const HotkeyData &hotKeyData )
{
    _hotkeyData = hotKeyData;
    _txtInputHotkey->SetValue(_hotkeyData.GetKeysAsString());
    _txtInputHotkey->SetFlags(_hotkeyData.Flags);
    _txtInputHotkey->SetHotKeyCode(_hotkeyData.HotKeyCode);
    _txtInputText->SetValue(_hotkeyData.CommandText);
}

void OptionsHotkeysDialog::OnHelpHotKeys( wxCommandEvent &event )
{
    #ifdef __WXMSW__
        DesktopWindow desktop;
        wxCHMHelpController *chmHelp = new wxCHMHelpController(&desktop);
    #else
        wxExtHelpController *chmHelp = new wxExtHelpController();
    #endif
    if (_controls->SearchHelpFile())
    {
        chmHelp->LoadFile(_controls->GetSettings()->GetCurrentHelpPath());
        chmHelp->KeywordSearch(wxT("Macros"));
    }
    delete chmHelp;
}
