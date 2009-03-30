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

IMPLEMENT_CLASS(OptionsHotKeysDialog, wxDialog)

BEGIN_EVENT_TABLE(OptionsHotKeysDialog, wxDialog)
	EVT_BUTTON(wxID_OK, OptionsHotKeysDialog::OnOkSettings)
	EVT_KEY_DOWN(OptionsHotKeysDialog::OnKeyDown)
END_EVENT_TABLE()

OptionsHotKeysDialog::OptionsHotKeysDialog(wxWindow *parent, const wxString& title, Settings *settings, int style) : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, style)
{
	_settings = settings;

	wxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	wxSizer *hotKeySizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxStaticText *stText01 = new wxStaticText(this, wxID_ANY, wxT("Комбинация клавиш:"));
	_txtInputHotKey = new HotKeyTextCtrl(this, wxID_ANY);

	hotKeySizer->Add(stText01, 0, wxALL, 5);
	hotKeySizer->Add(_txtInputHotKey, 0, wxALL, 5);
	
	wxStaticText *stText02 = new wxStaticText(this, wxID_ANY, wxT("Текст:"));
	_txtInputText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_TAB|wxTE_MULTILINE);

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
}

void OptionsHotKeysDialog::OnOkSettings(wxCommandEvent &event)
{
	_hotKeyData = HotKeyData(_txtInputHotKey->GetValue(), _txtInputText->GetValue());
	event.Skip();
}

void OptionsHotKeysDialog::SetHotKeyData( const HotKeyData &hotKeyData )
{
	_hotKeyData = hotKeyData;
	_txtInputHotKey->SetValue(_hotKeyData.HotKey);
	_txtInputText->SetValue(_hotKeyData.CommandText);
}


