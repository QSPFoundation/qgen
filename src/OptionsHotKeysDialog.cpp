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

#include "OptionsHotKeysDialog.h"

IMPLEMENT_CLASS(OptionsHotkeysDialog, wxDialog)

BEGIN_EVENT_TABLE(OptionsHotkeysDialog, wxDialog)
	EVT_BUTTON(wxID_OK, OptionsHotkeysDialog::OnOkSettings)
END_EVENT_TABLE()

OptionsHotkeysDialog::OptionsHotkeysDialog(wxWindow *parent, const wxString& title, Controls *controls, int style) : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, style)
{

	wxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	wxSizer *hotKeySizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxStaticText *stText01 = new wxStaticText(this, wxID_ANY, wxT("Комбинация клавиш:"));
	_txtInputHotkey = new HotKeyTextCtrl(this, wxID_ANY);

	hotKeySizer->Add(stText01, 0, wxALL, 5);
	hotKeySizer->Add(_txtInputHotkey, 0, wxALL, 5);

	wxStaticText *stText02 = new wxStaticText(this, wxID_ANY, wxT("Текст:"));
	_txtInputText = new SyntaxTextBox(this, controls, NULL, SYNTAX_STYLE_COLORED | SYNTAX_STYLE_NOHOTKEYS | SYNTAX_STYLE_SIMPLEMENU);

	wxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);
	_btnOK = new wxButton(this, wxID_OK, wxT("OK"));
	_btnCancel = new wxButton(this, wxID_CANCEL, wxT("Отмена"));

	btnSizer->Add(_btnOK, 0, wxALL, 5);
	btnSizer->Add(_btnCancel, 0, wxALL, 5);

	topSizer->Add(hotKeySizer, 0, wxALL);
	topSizer->Add(stText02, 0, wxLEFT|wxTOP, 5);
	topSizer->Add(_txtInputText, 1, wxALL|wxGROW, 5);
	topSizer->Add(btnSizer, 0, wxALL|wxALIGN_RIGHT);

	SetSizerAndFit(topSizer);
	SetAutoLayout(true);
	SetMinClientSize(wxSize(230, 230));
	SetSize(wxSize(630, 430));
}

void OptionsHotkeysDialog::OnOkSettings(wxCommandEvent &event)
{	
	_hotkeyData = HotkeyData(_txtInputHotkey->GetValue(), _txtInputText->GetValue());
	event.Skip();
}

void OptionsHotkeysDialog::SetHotkeyData( const HotkeyData &hotKeyData )
{
	_hotkeyData = hotKeyData;
	_txtInputHotkey->SetValue(_hotkeyData.Hotkey);
	_txtInputText->SetValue(_hotkeyData.CommandText);
}
