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

#include "SearchDialog.h"

IMPLEMENT_CLASS(SearchDialog, wxDialog)

BEGIN_EVENT_TABLE(SearchDialog, wxDialog)
	EVT_BUTTON(FIND_NEXT, SearchDialog::OnFindNext)
	EVT_BUTTON(FIND_ANEW, SearchDialog::OnFindAgain)
	EVT_BUTTON(FIND_REPL, SearchDialog::OnFindRepl)
	EVT_BUTTON(FIND_SKIPLOC, SearchDialog::OnSkipLoc)
	EVT_UPDATE_UI(FIND_TEXT, SearchDialog::OnUpdFindText)
	EVT_UPDATE_UI(REPL_TEXT, SearchDialog::OnUpdReplText)
	EVT_TEXT_ENTER(FIND_TEXT, SearchDialog::OnFindNext)
	EVT_TEXT_ENTER(REPL_TEXT, SearchDialog::OnFindRepl)
END_EVENT_TABLE()

SearchDialog::SearchDialog(wxWindow *parent, const wxString &title, Controls *controls, int style) :
	wxDialog( parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | style )
{
	_controls = controls;

	wxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
	wxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
	wxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);

	_textFind = new wxTextCtrl(this, FIND_TEXT, wxEmptyString, wxDefaultPosition, wxSize(150, wxDefaultCoord), wxTE_PROCESS_ENTER );
	_textRepl = new wxTextCtrl(this, REPL_TEXT, wxEmptyString, wxDefaultPosition, wxSize(150, wxDefaultCoord), wxTE_PROCESS_ENTER );
	_chkMatchCase = new wxCheckBox(this, wxID_ANY, wxT("Точное совпадение"));

	leftSizer->Add(_textFind, 1, wxALL|wxGROW, 2);
	leftSizer->Add(_textRepl, 1, wxALL|wxGROW, 2);
	leftSizer->Add(_chkMatchCase, 1, wxALL|wxGROW, 2);

	_btnSearchAgain = new wxButton(this, FIND_ANEW, wxT("Начать заново"));
	_btnNextSearch = new wxButton(this, FIND_NEXT, wxT("Продолжить поиск"));
	_btnSkipLoc = new wxButton(this, FIND_SKIPLOC, wxT("Пропустить локацию"));
	_btnReplace = new wxButton(this, FIND_REPL, wxT("Заменить"));
	_btnClose = new wxButton(this, wxID_CANCEL, wxT("Закрыть"));

	rightSizer->Add(_btnSearchAgain, 1, wxALL|wxGROW, 1);
	rightSizer->Add(_btnNextSearch, 1, wxALL|wxGROW, 1);
	rightSizer->Add(_btnReplace, 1, wxALL|wxGROW, 1);
	rightSizer->Add(_btnSkipLoc, 1, wxALL|wxGROW, 1);
	rightSizer->Add(_btnClose, 1, wxALL|wxGROW, 1);

	topSizer->Add(leftSizer, 1);
	topSizer->Add(rightSizer);

	SetSizerAndFit(topSizer);
	SetAutoLayout(true);

	int maxHeight = GetClientSize().GetHeight();
	SetMaxClientSize(wxSize(2048, maxHeight));
	SetClientSize(350, maxHeight);
}

void SearchDialog::OnFindNext( wxCommandEvent &event )
{
	_controls->SearchString( _textFind->GetValue(), false, _chkMatchCase->GetValue() );
}

void SearchDialog::OnFindAgain( wxCommandEvent &event )
{
	_controls->SearchString( _textFind->GetValue(), true, _chkMatchCase->GetValue() );
}

void SearchDialog::OnFindRepl( wxCommandEvent &event )
{
	_controls->ReplaceSearchString(_textRepl->GetValue());
	OnFindNext(wxCommandEvent());
}

void SearchDialog::OnUpdFindText( wxUpdateUIEvent& event )
{
	bool status = !_textFind->IsEmpty();
	_btnNextSearch->Enable(status);	
	_btnSearchAgain->Enable(status);
	_btnSkipLoc->Enable(status);
}

void SearchDialog::OnUpdReplText( wxUpdateUIEvent& event )
{
	_btnReplace->Enable(!_textRepl->IsEmpty());
}

void SearchDialog::OnSkipLoc( wxCommandEvent &event )
{
	if (_controls->SearchNextLoc())
		_controls->SearchString( _textFind->GetValue(), false, _chkMatchCase->GetValue() );
}
