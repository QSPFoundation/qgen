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

#ifndef	_QGEN_SEARCH_REPLACE_DIALOG_
	#define _QGEN_SEARCH_REPLACE_DIALOG_

	#include <wx/wx.h>
	#include "Controls.h"
	#include "SearchDataStore.h"

	class SearchDialog : public wxDialog
	{
		DECLARE_CLASS(SearchDialog)
		DECLARE_EVENT_TABLE()

		wxComboBox	*_textFind,
					*_textRepl;
		wxCheckBox  *_chkMatchCase,
					*_chkWholeWord;
		wxButton	*_btnClose,
					*_btnNextSearch,
					*_btnSearchAgain,
					*_btnReplace,
					*_btnSkipLoc;
		Controls	*_controls;
		SearchDataStore *_searchDataStore;

		void OnFindNext(wxCommandEvent &event);
		void OnFindAgain(wxCommandEvent &event);
		void OnFindRepl(wxCommandEvent &event);
		void OnSkipLoc(wxCommandEvent &event);
		void OnUpdFindText(wxCommandEvent& event);

		void AddSearchText(const wxString &text);
		void AddReplaceText(const wxString &text);
	public:
		SearchDialog(wxWindow *parent, const wxString& title, Controls *controls, int style = 0);
	};

#endif
