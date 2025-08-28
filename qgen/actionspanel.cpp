// Copyright (C) 2005-2025
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

#include "actionspanel.h"
#include "mainframe.h"

#include "bitmaps/toolbar_action_new.xpm"
#include "bitmaps/toolbar_action_rename.xpm"
#include "bitmaps/toolbar_action_delete.xpm"

IMPLEMENT_CLASS(ActionsPanel, wxPanel)

BEGIN_EVENT_TABLE(ActionsPanel, wxPanel)
    EVT_BUTTON(ID_ACTION_ADD, ActionsPanel::OnAddAction)
    EVT_BUTTON(ID_ACTION_REN, ActionsPanel::OnRenAction)
    EVT_BUTTON(ID_ACTION_DEL, ActionsPanel::OnDelAction)
END_EVENT_TABLE()

ActionsPanel::ActionsPanel(wxWindow *owner, ILocationPage *locPage, ActionCode *actCode, IControls *controls) : wxPanel(owner)
{
    _controls = controls;

    _actList = new ActionsListBox(this, wxID_ANY, locPage, actCode, controls);

    _addActButton = new wxBitmapButton(this, ID_ACTION_ADD, wxBitmap(toolbar_action_new_xpm), wxDefaultPosition, wxSize(32, 32));
    _renActButton = new wxBitmapButton(this, ID_ACTION_REN, wxBitmap(toolbar_action_rename_xpm), wxDefaultPosition, wxSize(32, 32));
    _delActButton = new wxBitmapButton(this, ID_ACTION_DEL, wxBitmap(toolbar_action_delete_xpm), wxDefaultPosition, wxSize(32, 32));

    wxSizer* sizerButtons = new wxBoxSizer(wxVERTICAL);
    sizerButtons->Add(_addActButton, 0, wxALL, 2);
    sizerButtons->Add(_renActButton, 0, wxALL, 2);
    sizerButtons->Add(_delActButton, 0, wxALL, 2);

    wxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(sizerButtons);
    sizer->Add(_actList, 1, wxGROW);

    SetSizerAndFit(sizer);
    SetAutoLayout(true);
}

void ActionsPanel::OnAddAction(wxCommandEvent &WXUNUSED(event))
{
    _controls->AddActionOnSelectedLoc();
}

void ActionsPanel::OnRenAction(wxCommandEvent &WXUNUSED(event))
{
    _controls->RenameSelectedAction();
}

void ActionsPanel::OnDelAction(wxCommandEvent &WXUNUSED(event))
{
    _controls->DeleteSelectedAction();
}

void ActionsPanel::EnableButtons()
{
    bool isAnyAction = (_actList->GetCount() > 0);
    _renActButton->Enable(isAnyAction);
    _delActButton->Enable(isAnyAction);
}
