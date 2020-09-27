// Copyright (C) 2005-2012
// Vladimir Bauer (baxzzzz AT gmail DOT com)
// Nex (nex AT otaku DOT ru)
// Shchannikov Dmitry (rrock DOT ru AT gmail DOT com)
// Valeriy Argunov (nporep AT mail DOT ru)
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

#include "locationcode.h"

IMPLEMENT_CLASS(LocationCode, wxPanel)

LocationCode::LocationCode( wxWindow *owner, ILocationPage *locPage, IControls *controls ) :
    wxPanel(owner, wxID_ANY)
{
    _locPage  = locPage;
    _controls = controls;

    _text = new SyntaxTextBox(this, _controls, SYNTAX_STYLE_COLORED);
    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    _stTextExec = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    sizer->Add(_stTextExec, 0, wxALL|wxGROW, 1);
    sizer->Add(_text, 1, wxALL|wxGROW, 1);
    SetSizerAndFit(sizer);
    SetAutoLayout(true);
    Update();
    _controls->GetSettings()->AddObserver(this);
}

LocationCode::~LocationCode()
{
    _controls->GetSettings()->RemoveObserver(this);
}

void LocationCode::SaveCode()
{
    if (_text->IsModified())
    {
        _controls->GetContainer()->SetLocationCode( _locPage->GetLocationIndex(), _text->GetValue() );
        _text->SetModified(false);
    }
}

void LocationCode::LoadCode()
{
    _text->SetValue(_controls->GetContainer()->GetLocationCode(_locPage->GetLocationIndex()));
}

void LocationCode::Clear()
{
    _text->Clear();
}

void LocationCode::SelectString( long startPos, long lastPos )
{
    _text->SetSelection( startPos, lastPos );
}

void LocationCode::ReplaceString( long start, long end, const wxString & str )
{
    _text->Replace(start, end, str);
}

void LocationCode::ExpandCollapseAll( bool isExpanded )
{
    _text->ExpandCollapseAll(isExpanded);
}

void LocationCode::Update( bool isFromObservable /*= false*/ )
{
    _stTextExec->SetLabel(_("Execute on visit:"));
    GetSizer()->Layout();
}
