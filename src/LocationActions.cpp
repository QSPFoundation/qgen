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

#include "LocationActions.h"

IMPLEMENT_CLASS(LocationActions, wxPanel)

LocationActions::LocationActions(wxWindow *owner, ILocationPage *locPage, IControls *controls, wxStatusBar *statusBar ) : wxPanel( owner )
{
	_locPage = locPage;
	_controls = controls;
	_statusBar = statusBar;
	_splitterv_down = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3DSASH );

	_actCode = new ActionCode( _splitterv_down, _locPage, _controls, _statusBar );
	_actPanel = new ActionsPanel( _splitterv_down, _locPage, _actCode, _controls );

	wxSizer *sizerDown = new wxBoxSizer( wxVERTICAL );
	_splitterv_down->SetMinimumPaneSize(1);
	_splitterv_down->SplitVertically( _actPanel, _actCode );

	sizerDown->Add( new wxStaticText( this, wxID_ANY, wxT("Базовые действия:"),
					wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE ), 0, wxALL|wxGROW );
	sizerDown->Add( _splitterv_down, 1, wxALL|wxGROW );

	SetSizerAndFit( sizerDown );
	SetAutoLayout( true );

	Update();
	_controls->GetSettings()->AddObserver(this);
}

LocationActions::~LocationActions()
{
	_controls->GetSettings()->RemoveObserver(this);
}

void LocationActions::Update(bool isFromObservable)
{
	Settings *settings = _controls->GetSettings();
	_splitterv_down->SetSashGravity(settings->GetWidthsCoeff2());
}

void LocationActions::SaveAction()
{
	_actPanel->GetActionsListBox()->SaveActionData();
}

void LocationActions::LoadAllActions()
{
	_actPanel->GetActionsListBox()->LoadAllActions();
	_actPanel->EnableButtons();
}

void LocationActions::Clear()
{
	_actPanel->GetActionsListBox()->DeleteAllActions();
	_actPanel->EnableButtons();
}

size_t LocationActions::AddActionToList( const wxString& name )
{
	size_t index = _actPanel->GetActionsListBox()->AddAction(name);
	_actPanel->EnableButtons();
	return index;
}

void LocationActions::DeleteActionFromList( size_t actIndex )
{
	_actPanel->GetActionsListBox()->DeleteAction(actIndex);
	_actPanel->EnableButtons();
}

long LocationActions::GetSelectedAction()
{
	return _actPanel->GetActionsListBox()->GetSelection();
}

void LocationActions::RenameActionInList( size_t index, const wxString& name )
{
	_actPanel->GetActionsListBox()->SetString(index, name);
}

bool LocationActions::IsActionsListEmpty()
{
	return !_actPanel->GetActionsListBox()->GetCount();
}

void LocationActions::SelectActionInList( size_t actIndex )
{
	_actPanel->GetActionsListBox()->SetFocus();
	_actPanel->GetActionsListBox()->Select( actIndex );
}

void LocationActions::SelectPicturePathString( long startPos, long lastPos )
{
	_actCode->SelectPicturePathString( startPos, lastPos );
}

void LocationActions::SelectActionCodeString( long startPos, long lastPos )
{
	_actCode->SelectCodeString( startPos, lastPos );
}

void LocationActions::ReplacePicturePathString( long start, long end, const wxString & str )
{
	_actCode->ReplacePicturePathString(start, end, str);
}

void LocationActions::ReplaceActionCodeString( long start, long end, const wxString & str )
{
	_actCode->ReplaceCodeString(start, end, str);
}

void LocationActions::MoveActionTo( size_t actIndex, size_t moveTo )
{
	_actPanel->GetActionsListBox()->MoveItemTo(actIndex, moveTo);
}

void LocationActions::RefreshActions()
{
	_actPanel->GetActionsListBox()->RefreshActions();
}

void LocationActions::SetFocusOnActionCode()
{
	_actCode->SetFocusOnActionCode();
}
