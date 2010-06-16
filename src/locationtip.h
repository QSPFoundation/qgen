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

#ifndef _QGEN_LOCATION_TIP_H_
	#define _QGEN_LOCATION_TIP_H_

	#include <wx/wx.h>
	#include "icontrols.h"
	#include "syntaxtextbox.h"

	#define TIP_SIZE_X 300
	#define TIP_SIZE_Y 350
	#define TIP_EMPTY_SIZE_X 300
	#define TIP_EMPTY_SIZE_Y 150
	#define TIP_X_OFFSET 5
	#define TIP_Y_OFFSET 20

	class LocationTip :	public wxFrame
	{
		DECLARE_CLASS(LocationTip)
	private:
		wxWindow		*_mainFrame;
		IControls		*_controls;
		wxString		_locName;
		wxString		_actName;
		wxStaticText	*_title;
		wxStaticText	*_desc;
		wxStaticText	*_code;
		SyntaxTextBox	*_locDesc;
		SyntaxTextBox	*_locCode;
		wxButton		*_emptyLabel;

		void LoadTip();
	public:
		LocationTip(wxWindow *parent, IControls *controls);

		void MoveTip(const wxPoint &pos);
		void HideTip();
		void SetLocName(const wxString &name);
		wxString GetLocName() const { return _locName; }
		void SetActName(const wxString &name);
		wxString GetActName() const { return _actName; }
	};

#endif
