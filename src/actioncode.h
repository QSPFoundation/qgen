// Copyright (C) 2005-2012
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

#ifndef _QGEN_ACTION_CODE_H_
	#define _QGEN_ACTION_CODE_H_

	#include "syntaxtextbox.h"
	#include "imagepathtextbox.h"

	enum
	{
		ID_PICT_OPEN = 11000
	};

	class ActionCode : public wxPanel, public IObserver
	{
		DECLARE_CLASS(ActionCode)
		DECLARE_EVENT_TABLE()
	private:
		ImagePathTextBox	*_pathPicTxtCtrl;
		SyntaxTextBox		*_actCodeTxt;
		wxButton			*_button;
		IControls			*_controls;
		ILocationPage		*_locPage;

		void OnOpenPicture(wxCommandEvent &event);
		bool Enable(bool status = true);
	public:
		ActionCode(wxWindow *owner, ILocationPage *locPage, IControls *controls);
		~ActionCode();

		void Update(bool isFromObservable = false);
		void LoadAction(size_t actIndex);
		void SaveAction(size_t actIndex);
		void ClearAction();
		void SelectPicturePathString(long startPos, long lastPos);
		void SelectCodeString(long startPos, long lastPos );
		void ReplacePicturePathString(long start, long end, const wxString & str);
		void ReplaceCodeString(long start, long end, const wxString & str);
		void SetFocusOnActionCode();
		void ExpandCollapseAll(bool isExpanded);
	};

#endif
