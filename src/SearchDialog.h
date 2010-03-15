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

#ifndef	_QGEN_SEARCH_REPLACE_DIALOG_H_
	#define _QGEN_SEARCH_REPLACE_DIALOG_H_

	#include <wx/wx.h>
	#include "Controls.h"
	#include "SearchDataStore.h"

	enum
	{
		ID_FIND_NEXT = 10400,
		ID_FIND_REPL,
		ID_FIND_REPLALL,
		ID_FIND_ANEW,
		ID_FIND_SKIPLOC,
		ID_TEXT_FIND,
		ID_TEXT_REPL
	};

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
					*_btnReplaceAll,
					*_btnSkipLoc;
		wxWindow	*_parent;
		Controls	*_controls;
		SearchDataStore *_searchDataStore;

		void OnFindNext(wxCommandEvent &event);
		void OnFindAgain(wxCommandEvent &event);
		void OnFindRepl(wxCommandEvent &event);
		void OnFindReplAll(wxCommandEvent &event);
		void OnSkipLoc(wxCommandEvent &event);
		void OnUpdFindText(wxCommandEvent& event);

		void AddSearchText(const wxString &text);
		void AddReplaceText(const wxString &text);
	public:
		SearchDialog(wxWindow *parent, const wxString& title, Controls *controls, int style = 0);

		bool Show(bool show = true);
	};

#endif
