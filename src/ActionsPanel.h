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

#ifndef _ACTIONS_PANEL_H
	#define _ACTIONS_PANEL_H

	#include "wx/wx.h"
	#include "IControls.h"
	#include "ActionCode.h"
	#include "ActionsListBox.h"

	#include "bitmaps/new_action.xpm"
	#include "bitmaps/rename_action.xpm"
	#include "bitmaps/delete_action.xpm"

	enum
	{
		ID_ADD_ACTION,
		ID_REN_ACTION,
		ID_DEL_ACTION
	};

	class ActionsPanel : public wxPanel
	{
		DECLARE_CLASS(ActionsPanel)
		DECLARE_EVENT_TABLE()

	private:
		ActionsListBox		*_actList;
		wxBitmapButton		*_addActButton;
		wxBitmapButton		*_renActButton;
		wxBitmapButton		*_delActButton;
		IControls			*_controls;

		void OnAddAction( wxCommandEvent &event );
		void OnRenAction( wxCommandEvent &event );
		void OnDelAction( wxCommandEvent &event );

	public:
		ActionsPanel( wxWindow *owner, ILocationPage *locPage, ActionCode *_actCode, IControls *controls );

		void EnableButtons();
		ActionsListBox *GetActionsListBox() { return _actList; };
	};

#endif // _ACTION_PANEL_H
