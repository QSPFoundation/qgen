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

#include "AddActionWindow.h"

IMPLEMENT_CLASS(AddActionWindow, wxPanel)

BEGIN_EVENT_TABLE(AddActionWindow, wxPanel)
	EVT_BUTTON(wxID_ANY, AddActionWindow::OnAddActionClick)
END_EVENT_TABLE()

AddActionWindow::AddActionWindow( wxWindow *owner, wxWindowID id, IControls *controls ) : wxPanel(owner, id)
{
	_controls = controls;

	_bmpAddAction = new wxBitmapButton(this, wxID_ANY, wxBitmap(add_action_xpm), wxDefaultPosition, wxDefaultSize, 0);
	_bmpAddAction->SetBitmapHover(wxBitmap(add_action_up_xpm));
	_bmpAddAction->SetBitmapSelected(wxBitmap(add_action_xpm));
	_label = new wxStaticText(this, wxID_ANY, wxT("Добавить действие"));
	wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddStretchSpacer();
	sizer->Add(_bmpAddAction, 0, wxALIGN_CENTRE_HORIZONTAL);
	sizer->Add(_label, 0, wxALIGN_CENTRE_HORIZONTAL);
	sizer->AddStretchSpacer();

	SetSizerAndFit(sizer);
	SetAutoLayout(true);

	Update();
	_controls->GetSettings()->AddObserver(this);
}

AddActionWindow::~AddActionWindow()
{
	_controls->GetSettings()->RemoveObserver(this);
}

void AddActionWindow::Update( bool isFromObservable )
{
	Settings *settings = _controls->GetSettings();
	wxColour backColour = settings->GetBackColour();
	SetBackgroundColour(backColour);
	_bmpAddAction->SetBackgroundColour(backColour);
	_label->SetForegroundColour((backColour.Blue() << 16 | backColour.Green() << 8 | backColour.Red()) ^ 0xFFFFFF);
	Refresh();
}

void AddActionWindow::OnAddActionClick( wxCommandEvent &event )
{
	_controls->AddActionOnSelectedLoc();
}
