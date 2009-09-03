// Copyright (C) 2005-2009
// BaxZzZz (bauer_v AT mail DOT ru)
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

#ifndef _QUEST_GENERATOR_OPTIONS_HOT_KEYS_DIALOG_H
	#define _QUEST_GENERATOR_OPTIONS_HOT_KEYS_DIALOG_H

	#include <wx/wx.h>
	#include <wx/help.h> 
	#include "Settings.h"
	#include "HotKeyTextCtrl.h"
	#include "SyntaxTextBox.h"
	#include "Controls.h"
	#ifdef __WXMSW__
		#include "DesktopWindow.h"
	#endif

	enum
	{
		ID_INPUT_TEXT,
		ID_HELP_BUTTON
	};

	class OptionsHotkeysDialog : public wxDialog
	{
		DECLARE_CLASS(OptionsHotkeysDialog)
		DECLARE_EVENT_TABLE()
	private:
		HotKeyTextCtrl	*_txtInputHotkey;
		SyntaxTextBox	*_txtInputText;
		wxButton		*_btnOK;
		wxButton		*_btnCancel;
		wxButton		*_btnHelp;
		HotkeyData		_hotkeyData;
		Controls		*_controls;

		void OnOkSettings(wxCommandEvent &event);
		void OnHelpHotKeys(wxCommandEvent &event);
	public:
		OptionsHotkeysDialog(wxWindow *parent, const wxString& title, Controls *controls,
							 int style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER);

		void SetHotkeyData(const HotkeyData &hotkeyData);
		HotkeyData GetHotkeyData() const { return _hotkeyData; }
	};

#endif
