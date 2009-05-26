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

#include "LocationActions.h"

IMPLEMENT_CLASS(LocationActions, wxPanel)

LocationActions::LocationActions(wxWindow *owner, ILocationPage *locPage, IControls *controls ) : wxPanel( owner )
{
	_locPage = locPage;
	_controls = controls;
	_splitterv_down = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3DSASH );
	_splitterv_down->Hide();

	_actCode = new ActionCode( _splitterv_down, _locPage, _controls );
	_actList = new ActionsListBox( _splitterv_down, wxID_ANY, _locPage, _actCode, _controls );
	_addActWin = new AddActionWindow(this, wxID_ANY, _controls);
	_addActWin->Hide();

	wxSizer *sizerDown = new wxBoxSizer( wxVERTICAL );
	_splitterv_down->SetMinimumPaneSize(1);
	_splitterv_down->SplitVertically(_actList, _actCode );

	sizerDown->Add( new wxStaticText( this, wxID_ANY, wxT("Базовые действия:"),
					wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE ), 0, wxALL|wxGROW );
	sizerDown->Add( _addActWin, 1, wxALL|wxGROW );
	sizerDown->Add( _splitterv_down, 1, wxALL|wxGROW );

	SetSizerAndFit( sizerDown );
	SetAutoLayout( true );

	_splitterv_down->Show();

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
	_actList->SaveActionData();
}

void LocationActions::ShowActions()
{
	bool isAnyAction = (_actList->GetSelection() >= 0);
	_addActWin->Show(!isAnyAction);
	_splitterv_down->Show(isAnyAction);
	GetSizer()->Layout();
}

void LocationActions::LoadAllActions()
{
	_actList->LoadAllActions();
	ShowActions();
}

void LocationActions::Clear()
{
	_actList->DeleteAllActions();
	ShowActions();
}

size_t LocationActions::AddActionToList( const wxString& name )
{
	size_t idx = _actList->AddAction(name);
	ShowActions();
	return idx;
}

void LocationActions::DeleteActionFromList( size_t actIndex )
{
	_actList->DeleteAction(actIndex);
	ShowActions();
}

long LocationActions::GetSelectedAction()
{
	return _actList->GetSelection();
}

void LocationActions::RenameActionInList( size_t index, const wxString& name )
{
	_actList->SetString(index, name);
}

bool LocationActions::IsActionsListEmpty()
{
	return !_actList->GetCount();
}

void LocationActions::SelectActionInList( size_t actIndex )
{
	_actList->SetFocus();
	_actList->Select( actIndex );
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
	_actList->MoveItemTo(actIndex, moveTo);
}

void LocationActions::RefreshActions()
{
	_actList->RefreshActions();
}

void LocationActions::SetFocusOnActionCode()
{
	_actCode->SetFocusOnActionCode();
}
