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

#ifndef _QUEST_GENERATOR_OPTIONS_HOT_KEYS_DIALOG_H
	#define _QUEST_GENERATOR_OPTIONS_HOT_KEYS_DIALOG_H

	#include <wx/wx.h>
	#include "Settings.h"
	#include "HotKeyTextCtrl.h"

	enum
	{
		ID_INPUT_TEXT
	};

	class OptionsHotKeysDialog : public wxDialog
	{
		DECLARE_CLASS(OptionsHotKeysDialog)
		DECLARE_EVENT_TABLE()
	private:
		wxTextCtrl		*_txtInputHotKey;
		wxTextCtrl		*_txtInputText;
		wxButton		*_btnOK;
		wxButton		*_btnCancel;
		Settings		*_settings;
		HotKeyData		_hotKeyData;
	
		void OnOkSettings(wxCommandEvent &event);
		void OnKeyDown(wxKeyEvent& event);
	public:
		OptionsHotKeysDialog(wxWindow *parent, const wxString& title, Settings *settings, 
								int style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER);
		void SetHotKeyData(const HotKeyData &hotKeyData);
		HotKeyData GetHotKeyData() const { return _hotKeyData; }
	};
#endif