// Copyright (C) 2005-2009
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

#include "ActionsPanel.h"
#include "QGen.h"

IMPLEMENT_CLASS(ActionsPanel, wxPanel)

BEGIN_EVENT_TABLE(ActionsPanel, wxPanel)
	EVT_BUTTON(ID_ACTION_ADD, ActionsPanel::OnAddAction)
	EVT_BUTTON(ID_ACTION_REN, ActionsPanel::OnRenAction)
	EVT_BUTTON(ID_ACTION_DEL, ActionsPanel::OnDelAction)
END_EVENT_TABLE()

ActionsPanel::ActionsPanel( wxWindow *owner, ILocationPage *locPage, ActionCode *actCode, IControls *controls ) : wxPanel( owner )
{
	_controls = controls;

	_actList = new ActionsListBox( this, wxID_ANY, locPage, actCode, controls );
	wxSizer *sizer2 = new wxBoxSizer ( wxHORIZONTAL );

	_addActButton = new wxBitmapButton( this, ID_ACTION_ADD, wxBitmap(toolbar_action_new_xpm), wxDefaultPosition, wxSize(32,26) );
	_renActButton = new wxBitmapButton( this, ID_ACTION_REN, wxBitmap(toolbar_action_rename_xpm), wxDefaultPosition, wxSize(32,26) );
	_delActButton = new wxBitmapButton( this, ID_ACTION_DEL, wxBitmap(toolbar_action_delete_xpm), wxDefaultPosition, wxSize(32,26) );

	sizer2->Add( _addActButton, 0, wxALL, 1 );
	sizer2->Add( _renActButton, 0, wxALL, 1 );
	sizer2->Add( _delActButton, 0, wxALL, 1 );

	wxSizer *sizer1 = new wxBoxSizer( wxVERTICAL );
	sizer1->Add( sizer2 );
	sizer1->Add( _actList, 1, wxALL|wxGROW );

	SetSizerAndFit( sizer1 );
	SetAutoLayout( true );
}

void ActionsPanel::OnAddAction( wxCommandEvent &event )
{
	_controls->AddActionOnSelectedLoc();
}

void ActionsPanel::OnRenAction( wxCommandEvent &event )
{
	_controls->RenameSelectedAction();
}

void ActionsPanel::OnDelAction( wxCommandEvent &event )
{
	_controls->DeleteSelectedAction();
}

void ActionsPanel::EnableButtons()
{
	bool isAnyAction = (_actList->GetCount() > 0);
	_renActButton->Enable(isAnyAction);
	_delActButton->Enable(isAnyAction);
}
