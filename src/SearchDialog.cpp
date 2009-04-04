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
	Settings *settings = _controls->GetSettings();
	_searchDataStore = settings->GetSearchDataStore();

	wxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
	wxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
	wxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);

	_textFind = new wxComboBox(this, FIND_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, _searchDataStore->GetSearchStrings());
	_textRepl = new wxComboBox(this, REPL_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, _searchDataStore->GetReplaceStrings());
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
	wxString str = _textFind->GetValue();
	_controls->SearchString( str, false, _chkMatchCase->GetValue() );
	AddSearchText(str);
	_textFind->SetFocus();
}

void SearchDialog::OnFindAgain( wxCommandEvent &event )
{
	wxString str = _textFind->GetValue();
	_controls->SearchString( str, true, _chkMatchCase->GetValue() );
	AddSearchText(str);
	_textFind->SetFocus();
}

void SearchDialog::OnFindRepl( wxCommandEvent &event )
{
	wxString str = _textRepl->GetValue();
	_controls->ReplaceSearchString(str);
	OnFindNext(wxCommandEvent());
	AddReplaceText(str);
	_textRepl->SetFocus();
}

void SearchDialog::OnUpdFindText( wxUpdateUIEvent& event )
{
	bool status = !_textFind->GetValue().IsEmpty();
	_btnNextSearch->Enable(status);	
	_btnSearchAgain->Enable(status);
	_btnSkipLoc->Enable(status);
}

void SearchDialog::OnUpdReplText( wxUpdateUIEvent& event )
{
	_btnReplace->Enable(!_textRepl->GetValue().IsEmpty() && !_textFind->GetValue().IsEmpty());
}

void SearchDialog::OnSkipLoc( wxCommandEvent &event )
{
	wxString str = _textFind->GetValue();
	if (_controls->SearchNextLoc())
		_controls->SearchString( str, false, _chkMatchCase->GetValue() );
	AddSearchText(str);
	_textFind->SetFocus();
}

void SearchDialog::AddSearchText(const wxString &text)
{
	if (_searchDataStore->AddSearchString(text))
	{
		size_t count = _textFind->GetCount();
		if (count >= 10)
			_textFind->Delete(count - 1);
		_textFind->Insert(text, 0);
	}
}

void SearchDialog::AddReplaceText(const wxString &text)
{
	if (_searchDataStore->AddReplaceString(text))
	{
		size_t count = _textRepl->GetCount();
		if (count >= 10)
			_textRepl->Delete(count - 1);
		_textRepl->Insert(text, 0);
	}
}
