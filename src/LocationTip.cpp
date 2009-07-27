// Copyright (C) 2005-2009
// BaxZzZz (bauer_v AT mail DOT ru)
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

#include "LocationTip.h"

IMPLEMENT_CLASS(LocationTip, wxFrame);

LocationTip::LocationTip(wxWindow *parent, IControls *controls) : wxFrame(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(TIP_SIZE_X, TIP_SIZE_Y), wxFRAME_TOOL_WINDOW)
{
	_mainFrame = parent;
	_controls = controls;

	SetBackgroundColour(wxColour(255,255,190));
	wxBoxSizer *_sizer= new wxBoxSizer(wxVERTICAL);
	_sizer->SetMinSize(TIP_SIZE_X, TIP_SIZE_Y);

	_desc = new wxTextCtrl(this, ID_DESC_TEXT, wxT("Описание:"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE);
	_desc->SetBackgroundColour(wxColour(255,255,190));
	_locDesc = new SyntaxTextBox(this, _controls, NULL, SYNTAX_STYLE_NOSCROLLBARS | SYNTAX_STYLE_SIMPLE | SYNTAX_STYLE_NOHOTKEYS | SYNTAX_STYLE_SIMPLEMENU);
	wxTextCtrl *_code = new wxTextCtrl(this, ID_CODE_TEXT, wxT("Код локации:"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxBORDER_NONE);
	_code->SetBackgroundColour(wxColour(255,255,190));
	_locCode = new SyntaxTextBox(this, _controls, NULL, SYNTAX_STYLE_NOSCROLLBARS | SYNTAX_STYLE_COLORED | SYNTAX_STYLE_NOHOTKEYS | SYNTAX_STYLE_SIMPLEMENU);

	_sizer->Add(_desc, 0, wxTOP|wxLEFT|wxRIGHT|wxGROW, 5);
	_sizer->Add(_locDesc, 1, wxTOP|wxLEFT|wxRIGHT|wxGROW, 5);
	_sizer->Add(_code, 0, wxTOP|wxLEFT|wxRIGHT|wxGROW, 5);
	_sizer->Add(_locCode, 1, wxALL|wxGROW, 5);

	SetSizer(_sizer);
	SetAutoLayout( true );
	Layout();
}

LocationTip::~LocationTip()
{

}

void LocationTip::MoveTip(wxPoint pos, wxString locationName)
{
	if (locationName != _prevLocName)
	{
		LoadTip(locationName);
		_prevLocName = locationName;

		wxRect displaySize = wxGetClientDisplayRect();
		if (displaySize.GetHeight() <= (pos.y + TIP_SIZE_Y))
			Move( pos.x, pos.y -  TIP_SIZE_Y);
		else
			Move( pos.x + 5, pos.y + 20 );
		if (!IsShown())
		{
			Show();
		}
	}	
}

void LocationTip::HideTip()
{
	if (IsShown())
	{
		Show(false);
	}
}

void LocationTip::LoadTip(wxString locationName)
{
	wxSizer *_sizer = GetSizer();
	size_t locIndex = _controls->GetContainer()->FindLocationIndex(locationName);
	wxString str = _controls->GetContainer()->GetLocationDesc(locIndex);
	if (str.IsEmpty())
	{
		if (_sizer->IsShown(_desc))
		{
			_sizer->Hide(_desc);
			_sizer->Hide(_locDesc);
		}		
	} else {
		if (!_sizer->IsShown(_desc))
		{
			_sizer->Show(_desc);
			_sizer->Show(_locDesc);
		}
		_locDesc->SetValue(str);
	}

	str = _controls->GetContainer()->GetLocationCode(locIndex);
	_locCode->SetValue(str);
	
	Layout();
}
