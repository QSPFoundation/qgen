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

#include "locationdesc.h"

IMPLEMENT_CLASS(LocationDesc, wxPanel)

LocationDesc::LocationDesc(wxWindow *owner, ILocationPage *locPage, IControls *controls) :
    wxPanel(owner, wxID_ANY)
{
    _locPage  = locPage;
    _controls = controls;

    _text = new SyntaxTextBox(this, _controls, SYNTAX_STYLE_SIMPLE | SYNTAX_STYLE_NOMARGINS | SYNTAX_STYLE_NOHELPTIPS);
    _stTextDesc = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);

    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(_stTextDesc, 0, wxGROW);
    sizer->Add(_text, 1, wxGROW);

    SetSizerAndFit(sizer);
    SetAutoLayout(true);
    Update();
    _controls->GetSettings()->AddObserver(this);
}

LocationDesc::~LocationDesc()
{
    _controls->GetSettings()->RemoveObserver(this);
}

void LocationDesc::SaveDesc()
{
    if (_text->IsModified())
    {
        _controls->GetContainer()->SetLocationDesc(_locPage->GetLocationIndex(), _text->GetValue());
        _text->SetModified(false);
    }
}

void LocationDesc::LoadDesc()
{
    _text->SetValue(_controls->GetContainer()->GetLocationDesc(_locPage->GetLocationIndex()));
}

void LocationDesc::Clear()
{
    _text->Clear();
}

void LocationDesc::SelectString(long startPos, long lastPos)
{
    _text->SetSelection(startPos, lastPos);
}

void LocationDesc::ReplaceString(long start, long end, const wxString& str)
{
    _text->Replace(start, end, str);
}

void LocationDesc::Update(bool isFromObservable /*= false*/)
{
    _stTextDesc->SetLabel(_("Base description:"));
    GetSizer()->Layout();
}
