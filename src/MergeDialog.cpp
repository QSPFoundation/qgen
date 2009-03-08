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

#include "MergeDialog.h"

IMPLEMENT_CLASS(MergeDialog, wxDialog)

BEGIN_EVENT_TABLE(MergeDialog, wxDialog)
	EVT_BUTTON(ID_REPLACE_BUTTON, MergeDialog::OnButton)
	EVT_BUTTON(ID_SKIP_BUTTON, MergeDialog::OnButton)
	EVT_BUTTON(ID_CANCEL_BUTTON, MergeDialog::OnButton)
END_EVENT_TABLE()

MergeDialog::MergeDialog(wxWindow *parent, const wxString& title, const wxString &message, int style /* = 0 */) :
			wxDialog( parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE |style|wxICON_INFORMATION )
{
	wxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	wxSizer *msgSizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

	wxBitmap bitmap = wxArtProvider::GetIcon(wxART_WARNING, wxART_OTHER);
	wxStaticBitmap *info_icon = new wxStaticBitmap(this, wxID_ANY, bitmap);

	wxStaticText *msgText = new wxStaticText(this, wxID_ANY, message);

	_replace = new wxButton(this, ID_REPLACE_BUTTON, wxT("Заменить"));
	_skip = new wxButton(this, ID_SKIP_BUTTON, wxT("Пропустить"));
	_cancel = new wxButton(this, ID_CANCEL_BUTTON, wxT("Отмена"));

	_chkToAll= new wxCheckBox(this, wxID_ANY, wxT("для всех"));
	
	msgSizer->Add(info_icon, 0, wxALIGN_LEFT);
	msgSizer->AddSpacer(8);
	msgSizer->Add(msgText, 0, wxGROW);
	buttonSizer->Add(_replace, 1, wxALL|wxGROW, 5);
	buttonSizer->Add(_skip, 1, wxALL|wxGROW, 5);
	buttonSizer->Add(_cancel, 1, wxALL|wxGROW, 5);
	buttonSizer->Add(_chkToAll, 1, wxALL|wxGROW, 5);

	topSizer->Add(msgSizer,1, wxGROW|wxALL, 26);
	topSizer->Add(buttonSizer,1, wxGROW);

	SetSizerAndFit(topSizer);
	SetAutoLayout(true);
}

void MergeDialog::OnButton(wxCommandEvent &event)
{
	int ret;
	switch (event.GetId())
	{
	case ID_REPLACE_BUTTON:
		ret = MergeReplace;
		break;
	case ID_SKIP_BUTTON:
		ret = MergeSkip;
		break;
	case ID_CANCEL_BUTTON:
		ret = MergeCancel;
		break;
	}
	if (_chkToAll->IsChecked()) ret |= MergeAll;
	EndModal(ret);
}
