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

#ifndef _SYNTAX_TEXTBOX_H
	#define _SYNTAX_TEXTBOX_H

	#include <wx/wx.h>
	#include <wx/stc/stc.h>
	#include "Idents.h"
	#include "IControls.h"

	enum
	{
		SYNTAX_FOLD_MARGIN = 1,
		SYNTAX_NUM_MARGIN
	};

	enum
	{
		 SYNTAX_STYLE_SIMPLE = 0,
		 SYNTAX_STYLE_COLORED = 1,
		 SYNTAX_STYLE_NOHOTKEYS = 2,
		 SYNTAX_STYLE_SIMPLEMENU = 4
	};

	class SyntaxTextBox : public wxStyledTextCtrl, public IObserver
	{
		DECLARE_CLASS(SyntaxTextBox)
		DECLARE_EVENT_TABLE()
	private:
		IControls *_controls;
		wxArrayString keywords;
		int _style;

		void FillKeywords(const wxString &str);
		wxString GetArrayAsString(const wxArrayString &arr) const;
		int GetCharPosition(int startPos, int chars);
		int GetCharIndexFromPosition(int fromPos, int pos);
		bool StartAutoComplete();
		void Expand(int &line, bool doExpand, bool force = false, int visLevels = 0, int level = -1);

		void OnKeyDown(wxKeyEvent& event);
		void OnRightClick(wxMouseEvent& event);
		void OnMarginClicked(wxStyledTextEvent &event);
		void OnCharAdded(wxStyledTextEvent &event);
	public:
		SyntaxTextBox(wxWindow *owner, IControls *controls, int style);
		~SyntaxTextBox();

		void Update(bool isFromObservable = false);
		void SetValue(const wxString &str);
		void SetSelection(long from, long to);
		void Replace(long from, long to, const wxString &str);
		long GetInsertionPoint() const;
		wxString GetStringSelection() const;
		void RemoveSelection();
		void Clear();
		void ExpandCollapseAll(bool isExpanded);
	};

#endif
