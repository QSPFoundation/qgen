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

#include "searchdialog.h"

IMPLEMENT_CLASS(SearchDialog, wxDialog)

BEGIN_EVENT_TABLE(SearchDialog, wxDialog)
    EVT_BUTTON(ID_FIND_NEXT, SearchDialog::OnFindNext)
    EVT_BUTTON(ID_FIND_ANEW, SearchDialog::OnFindAgain)
    EVT_BUTTON(ID_FIND_REPL, SearchDialog::OnFindRepl)
    EVT_BUTTON(ID_FIND_REPLALL, SearchDialog::OnFindReplAll)
    EVT_BUTTON(ID_FIND_SKIPLOC, SearchDialog::OnSkipLoc)
    EVT_TEXT(ID_TEXT_FIND, SearchDialog::OnUpdFindText)
    EVT_TEXT_ENTER(ID_TEXT_FIND, SearchDialog::OnFindNext)
    EVT_TEXT_ENTER(ID_TEXT_REPL, SearchDialog::OnFindRepl)
END_EVENT_TABLE()

SearchDialog::SearchDialog(wxWindow *parent, const wxString &title, IControls *controls, int style) :
    wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | style)
{
    _controls = controls;
    _parent = parent;
    wxCommandEvent dummy;
    Settings *settings = _controls->GetSettings();
    _searchDataStore = settings->GetSearchDataStore();

    wxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);

    _textFind = new wxComboBox(this, ID_TEXT_FIND, wxEmptyString, wxDefaultPosition,
                        wxDefaultSize, _searchDataStore->GetSearchStrings());
    _textRepl = new wxComboBox(this, ID_TEXT_REPL, wxEmptyString, wxDefaultPosition,
                        wxDefaultSize, _searchDataStore->GetReplaceStrings());
    if (_textFind->GetCount()) _textFind->Select(0);
    if (_textRepl->GetCount()) _textRepl->Select(0);
    _chkMatchCase = new wxCheckBox(this, wxID_ANY, _("Match case"));
    _chkWholeWord = new wxCheckBox(this, wxID_ANY, _("Search only word"));
    _chkRegEx = new wxCheckBox(this, wxID_ANY, _("Regular expression"));

    leftSizer->Add(_textFind, 1, wxDOWN|wxGROW, 2);
    leftSizer->Add(_textRepl, 1, wxUP|wxGROW, 2);
    leftSizer->Add(_chkMatchCase, 1, wxUP|wxGROW, 6);
    leftSizer->Add(_chkWholeWord, 1, wxUP|wxGROW, 6);
    leftSizer->Add(_chkRegEx, 1, wxUP|wxGROW, 6);

    _btnSearchAgain = new wxButton(this, ID_FIND_ANEW, _("New search"));
    _btnNextSearch = new wxButton(this, ID_FIND_NEXT, _("Continue search"));
    _btnSkipLoc = new wxButton(this, ID_FIND_SKIPLOC, _("Skip location"));
    _btnReplace = new wxButton(this, ID_FIND_REPL, _("Replace"));
    _btnReplaceAll = new wxButton(this, ID_FIND_REPLALL, _("Replace all"));
    _btnClose = new wxButton(this, wxID_CANCEL, _("Cancel"));

    rightSizer->Add(_btnNextSearch, 1, wxALL|wxGROW, 1);
    rightSizer->Add(_btnSearchAgain, 1, wxALL|wxGROW, 1);
    rightSizer->Add(_btnReplace, 1, wxALL|wxGROW, 1);
    rightSizer->Add(_btnReplaceAll, 1, wxALL|wxGROW, 1);
    rightSizer->Add(_btnSkipLoc, 1, wxALL|wxGROW, 1);
    rightSizer->AddSpacer(10);
    rightSizer->Add(_btnClose, 1, wxALL|wxGROW, 1);

    topSizer->Add(leftSizer, 1, wxALL, 2);
    topSizer->Add(rightSizer);

    SetSizerAndFit(topSizer);
    SetAutoLayout(true);

    int maxHeight = GetClientSize().GetHeight();
    SetMaxClientSize(wxSize(900, maxHeight));
    SetClientSize(350, maxHeight);

    OnUpdFindText(dummy);
    _btnNextSearch->SetDefault();
}

void SearchDialog::OnFindNext(wxCommandEvent &event)
{
    wxString searchString = _textFind->GetValue();
    AddSearchText(searchString);

    _controls->SearchString(searchString, false,
        _chkMatchCase->GetValue(),
        _chkWholeWord->GetValue(),
        _chkRegEx->GetValue());
    _textFind->SetFocus();
    _btnNextSearch->SetDefault();
}

void SearchDialog::OnFindAgain(wxCommandEvent &event)
{
    wxString searchString = _textFind->GetValue();
    AddSearchText(searchString);

    _controls->SearchString(searchString, true,
        _chkMatchCase->GetValue(),
        _chkWholeWord->GetValue(),
        _chkRegEx->GetValue());
    _textFind->SetFocus();
    _btnNextSearch->SetDefault();
}

void SearchDialog::OnFindRepl(wxCommandEvent &event)
{
    wxCommandEvent dummy;
    wxString replaceWithString = _textRepl->GetValue();
    AddReplaceText(replaceWithString);

    if (_controls->ReplaceSearchString(replaceWithString,
        _chkMatchCase->GetValue(),
        _chkRegEx->GetValue()))
    {
        OnFindNext(dummy);
    }
    _textRepl->SetFocus();
    _btnNextSearch->SetDefault();
}

void SearchDialog::OnFindReplAll(wxCommandEvent &event)
{
    wxString searchString = _textFind->GetValue();
    wxString replaceWithString = _textRepl->GetValue();
    AddSearchText(searchString);
    AddReplaceText(replaceWithString);

    bool toFindAgain = true;
    while (true)
    {
        if (!_controls->ReplaceSearchString(replaceWithString,
            _chkMatchCase->GetValue(),
            _chkRegEx->GetValue()))
            break;

        if (!_controls->SearchString(searchString, toFindAgain,
                _chkMatchCase->GetValue(),
                _chkWholeWord->GetValue(),
                _chkRegEx->GetValue()))
            break;

        toFindAgain = false;
    }
    _textRepl->SetFocus();
    _btnNextSearch->SetDefault();
}

void SearchDialog::OnUpdFindText(wxCommandEvent& event)
{
    bool hasSearchString = !_textFind->GetValue().IsEmpty();
    _btnNextSearch->Enable(hasSearchString);
    _btnSearchAgain->Enable(hasSearchString);
    _btnSkipLoc->Enable(hasSearchString);
    _btnReplace->Enable(hasSearchString);
    _btnReplaceAll->Enable(hasSearchString);
}

void SearchDialog::OnSkipLoc(wxCommandEvent &event)
{
    wxString searchString = _textFind->GetValue();
    AddSearchText(searchString);

    if (_controls->SearchNextLoc())
    {
        _controls->SearchString(searchString, false,
            _chkMatchCase->GetValue(),
            _chkWholeWord->GetValue(),
            _chkRegEx->GetValue());
    }
    _textFind->SetFocus();
    _btnNextSearch->SetDefault();
}

void SearchDialog::AddSearchText(const wxString &text)
{
    _searchDataStore->AddSearchString(text);

    int itemIndex = _textFind->FindString(text, true);
    if (itemIndex == wxNOT_FOUND)
    {
        int count = _textFind->GetCount();
        if (count >= 15) _textFind->Delete(count - 1);
    }
    else
        _textFind->Delete(itemIndex);
    _textFind->Insert(text, 0);
}

void SearchDialog::AddReplaceText(const wxString &text)
{
    _searchDataStore->AddReplaceString(text);

    int itemIndex = _textRepl->FindString(text, true);
    if (itemIndex == wxNOT_FOUND)
    {
        int count = _textRepl->GetCount();
        if (count >= 15) _textRepl->Delete(count - 1);
    }
    else
        _textRepl->Delete(itemIndex);
    _textRepl->Insert(text, 0);
}

bool SearchDialog::Show(bool show)
{
    if (wxDialog::Show(show))
    {
        if (show)
            _textFind->SetFocus();
        else
            _parent->SetFocus();
    }
    return true;
}

void SearchDialog::SetTextToSearch(const wxString &text)
{
    _textFind->SetValue(text);
}
