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

#include "LocationDesc.h"

IMPLEMENT_CLASS( LocationDesc, wxPanel )

LocationDesc::LocationDesc( wxWindow *owner, ILocationPage *locPage, IControls *controls ) :
	wxPanel(owner, wxID_ANY)
{
	_locPage  = locPage;
	_controls = controls;

	_text = new SyntaxTextBox(this, _controls, SYNTAX_STYLE_SIMPLE);
	wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Описание:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE), 0, wxALL|wxGROW, 1);
	sizer->Add(_text, 1, wxALL|wxGROW, 1);
	SetSizerAndFit(sizer);
	SetAutoLayout(true);
}

void LocationDesc::SaveDesc()
{
	if (_text->IsModified())
	{
		_controls->GetContainer()->SetLocationDesc( _locPage->GetLocationIndex(), _text->GetValue() );
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
	_text->SetSelection( startPos, lastPos );
}

void LocationDesc::ReplaceString( long start, long end, const wxString & str )
{
	_text->Replace(start, end, str);
}
