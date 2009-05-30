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

#ifndef  _QUEST_GENERATOR_PAGE_LOCATION_H
	#define _QUEST_GENERATOR_PAGE_LOCATION_H

	#include <wx/wx.h>
	#include <wx/aui/aui.h>
	#include <wx/splitter.h>
	#include <wx/panel.h>

	#include "LocationActions.h"
	#include "LocationCode.h"
	#include "LocationDesc.h"
	#include "ILocationPage.h"

	class LocationPage : public wxPanel, public ILocationPage, public IObserver
	{
		DECLARE_CLASS(LocationPage)
	private:
		IControls		*_controls;
		LocationCode	*_locCode;
		LocationDesc	*_locDesc;
		LocationActions *_locActs;
		size_t			_locIndex;
		wxSplitterWindow	*_splitterh;
		wxSplitterWindow	*_splitterv_up;
		Settings		*_settings;
		int				_descWidth;
		int				_actsHeight;

	public:
		LocationPage( wxAuiNotebook *owner, IControls *controls );
		~LocationPage();

		void Update(bool isFromObservable = false);
		void LocDescVisible(bool isVisible);
		void LocActsVisible(bool isVisible);
		void SavePage();
		void LoadPage();
		void ExpandCollapseAll(bool isExpanded);
		size_t GetLocationIndex();
		long GetSelectedAction();
		void SetLocationIndex(size_t locIndex);
		size_t AddAction(const wxString& name);
		void DeleteAction(size_t actIndex);
		void RenameAction(size_t actIndex, const wxString& name);
		void MoveActionTo(size_t actIndex, size_t moveTo);
		void DeleteAllActions();
		void Clear();
		bool IsActionsEmpty();
		bool IsActsShown() const { return _splitterh->IsSplit(); }
		bool IsDescShown() const { return _splitterv_up->IsSplit(); }
		void RefreshActions();
		void SelectLocDescString( long startPos, long lastPos );
		void SelectLocCodeString( long startPos, long lastPos );
		void SelectAction( size_t actIndex );
		void SetFocusOnActionCode();
		void SelectPicturePathString( long startPos, long lastPos );
		void SelectActionCodeString( long startPos, long lastPos);

		void ReplaceLocDescString(long start, long end, const wxString & str);
		void ReplaceLocCodeString(long start, long end, const wxString & str);
		void ReplacePicturePathString(long start, long end, const wxString & str);
		void ReplaceActionCodeString(long start, long end, const wxString & str);
	};

#endif
