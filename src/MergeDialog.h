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

#ifndef	_QGEN_MERGE_DIALOG_
	#define _QGEN_MERGE_DIALOG_

	#include <wx/wx.h>
	#include <wx/artprov.h>

	enum
	{
		ID_REPLACE_BUTTON,
		ID_SKIP_BUTTON,
		ID_CANCEL_BUTTON
	};

	enum
	{
		MergeAll = 1 << 0,
		MergeReplace = 1 << 1,
		MergeSkip = 1 << 2,
		MergeCancel = 1 << 30
	};

	class MergeDialog : public wxDialog
	{
		DECLARE_CLASS(MergeDialog)
		DECLARE_EVENT_TABLE()

		wxButton	*_replace;
		wxButton	*_skip;
		wxButton	*_cancel;
		wxCheckBox	*_chkToAll;

		void OnButton(wxCommandEvent &event);

	public:
		MergeDialog(wxWindow *parent, const wxString& title, const wxString &message, int style = 0);
	};

#endif
