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

#ifndef _ACTION_CODE_H
	#define _ACTION_CODE_H

	#include "SyntaxTextBox.h"
	#include "ImagePathTextBox.h"
	#include "Idents.h"

	class ActionCode : public wxPanel, public IObserver
	{
		DECLARE_CLASS(ActionCode)
		DECLARE_EVENT_TABLE()
	private:
		ImagePathTextBox	*_pathPicTxtCtrl;
		SyntaxTextBox		*_actCodeTxt;
		wxButton			*_button;
		IControls			*_controls;
		wxStatusBar			*_statusBar;
		ILocationPage		*_locPage;

		void OnOpenPicture(wxCommandEvent &event);

		bool Enable(bool status = true);
	public:
		ActionCode(wxWindow *owner, ILocationPage *locPage, IControls *controls, wxStatusBar *statusBar );
		~ActionCode();

		void Update(bool isFromObservable = false);
		void LoadAction( size_t actIndex );
		void SaveAction( size_t actIndex );
		void ClearAction();

		void SelectPicturePathString(long startPos, long lastPos);
		void SelectCodeString(long startPos, long lastPos );
		void ReplacePicturePathString(long start, long end, const wxString & str);
		void ReplaceCodeString(long start, long end, const wxString & str);
		void SetFocusOnActionCode();
		void ExpandCollapseAll(bool isExpanded);
	};

#endif
