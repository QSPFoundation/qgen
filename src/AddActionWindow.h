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

#ifndef _QUEST_GENERATOR_ADD_ACTION_
	#define _QUEST_GENERATOR_ADD_ACTION_

	#include <wx/wx.h>
	#include "Idents.h"
	#include "IControls.h"
	#include "bitmaps/add_action.xpm"
	#include "bitmaps/add_action_up.xpm"

	class AddActionWindow : public wxPanel, public IObserver
	{
		DECLARE_CLASS(AddActionWindow)
		DECLARE_EVENT_TABLE()
	private:
		wxBitmapButton *_bmpAddAction;
		wxStaticText *_label;
		IControls *_controls;

		void OnAddActionClick(wxCommandEvent &event);
	public:
		AddActionWindow(wxWindow *owner, wxWindowID id, IControls *controls);
		~AddActionWindow();

		void Update(bool isFromObservable = false);
	};

#endif
