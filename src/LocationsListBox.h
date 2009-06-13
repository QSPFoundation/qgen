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

#ifndef _QUEST_GENERATOR_LOCATION_LIST_H
	#define _QUEST_GENERATOR_LOCATION_LIST_H

	#include <wx/wx.h>
	#include <wx/treectrl.h>
	#include "Idents.h"
	#include "IControls.h"
	#include "LocationPage.h"

	#include "bitmaps/location_closed.xpm"
	#include "bitmaps/location.xpm"
	#include "bitmaps/action.xpm"

	enum
	{
		ICON_NOTACTIVELOCATION,
		ICON_ACTIVELOCATION,
		ICON_ACTION
	};

	class LocationsListBox : public wxTreeCtrl, public IObserver
	{
		DECLARE_CLASS(LocationsListBox)
		DECLARE_EVENT_TABLE()
	private:
		IControls *_controls;
		wxImageList _statesImageList;
		long _draggedLocIndex;
		long _draggedActIndex;

		void OnRightClick(wxMouseEvent &event);
		void OnDoubleClick(wxMouseEvent &event);
		void OnEndLabelEdit(wxTreeEvent &event);
		void OnBeginDrag(wxTreeEvent &event);
		void OnEndDrag(wxTreeEvent &event);
		bool IsItemOk(wxTreeItemId id, int flags);
		wxTreeItemId GetItemByPos(const wxTreeItemId &parent, size_t index);
	public:
		LocationsListBox(wxWindow *parent, wxWindowID id, IControls *controls,
						 long style = wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|
									  wxTR_HIDE_ROOT|wxTR_NO_LINES|
									  wxTR_ROW_LINES|wxTR_EDIT_LABELS|
									  wxTR_FULL_ROW_HIGHLIGHT);
		~LocationsListBox();

		void Update(bool isFromObservable = false);
		void Insert(const wxString &text, size_t pos);
		void Delete(size_t index);
		void Select(size_t index);
		wxString GetString(size_t index);
		void SetString(size_t index, const wxString & text);
		long GetStringIndex(const wxString &text);
		wxString GetStringSelection();
		void Clear();
		size_t GetCount();
		void ExpandItems();
		void CollapseItems();
		void ApplyStatesImageList();

		void UpdateLocationActions(size_t locIndex, const wxArrayString & actions);
		void MoveItemTo(size_t locIndex, size_t moveTo);
		void SetLocStatus(size_t locIndex, bool isOpened);
	};

#endif
