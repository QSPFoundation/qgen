// Copyright (C) 2005-2010
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

#ifndef _QGEN_LOCATION_ACTIONS_H_
	#define _QGEN_LOCATION_ACTIONS_H_

	#include <wx/wx.h>
	#include <wx/splitter.h>
	#include "ilocationpage.h"
	#include "icontrols.h"
	#include "actionspanel.h"
	#include "actioncode.h"

	class LocationActions : public wxPanel, public IObserver
	{
		DECLARE_CLASS(LocationActions)
	private:
		ILocationPage		*_locPage;
		IControls			*_controls;
		ActionsPanel		*_actPanel;
		ActionCode			*_actCode;
		wxSplitterWindow	*_splitterv_down;
		wxStaticText		*_stTextBaseActions;

		void ShowActions();
	public:
		LocationActions(wxWindow *owner, ILocationPage *locPage, IControls *controls);
		~LocationActions();

		void Update(bool isFromObservable = false);
		void SaveAction();
		void LoadAllActions();
		long GetSelectedAction();
		void RefreshActions();
		size_t AddActionToList(const wxString& name);
		void DeleteActionFromList(size_t actIndex);
		void RenameActionInList(size_t index, const wxString& name);
		void MoveActionTo(size_t actIndex, size_t moveTo);
		void Clear();
		bool IsActionsListEmpty();
		void SelectActionInList(size_t actIndex);
		void SelectPicturePathString(long startPos, long lastPos);
		void SelectActionCodeString(long startPos, long lastPos);
		void ReplacePicturePathString(long start, long end, const wxString & str);
		void ReplaceActionCodeString(long start, long end, const wxString & str);
		void SetFocusOnActionCode();
	};

#endif
