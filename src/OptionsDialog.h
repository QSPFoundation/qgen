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

#ifndef _QUEST_GENERATOR_OPTIONS_DIALOG_H
	#define _QUEST_GENERATOR_OPTIONS_DIALOG_H

	#include <wx/wx.h>
	#include <wx/notebook.h>
	#include <wx/spinctrl.h>
	#include <wx/colordlg.h>
	#include <wx/fontdlg.h>
	#include <wx/notebook.h>
	#include <wx/sizer.h>
	#include <wx/listctrl.h> 
	#include "Settings.h"
	#include "Controls.h"
	#include "OptionsHotKeysDialog.h"

	enum
	{
		ID_COLORS_STATEMENTS,
		ID_COLORS_FUNCTIONS,
		ID_COLORS_SYS_VARIABLES,
		ID_COLORS_NUMBERS,
		ID_COLORS_STRINGS,
		ID_COLORS_OPERATIONS_BRACKETS,
		ID_COLORS_MARKS,
		ID_COLORS_COMMENTS,
		ID_COLORS_BASE_FONT,
		ID_COLORS_TEXT_BACK,
		ID_COLORS_BASE_BACK,
		ID_FONTS_STATEMENTS,
		ID_FONTS_FUNCTIONS,
		ID_FONTS_SYS_VARIABLES,
		ID_FONTS_NUMBERS,
		ID_FONTS_STRINGS,
		ID_FONTS_OPERATIONS_BRACKETS,
		ID_FONTS_MARKS,
		ID_FONTS_COMMENTS,
		ID_FONTS_BASE,
		ID_PATH_PLAYER,
		ID_PATH_HELP,
		ID_PATH_TXT2GAM,
		ID_OK_SETTINGS,
		ID_CANCEL_SETTINGS,
		ID_RESET_SETTINGS,
		ID_APPLY_SETTINGS,
		ID_AUTO_SAVE,
		ID_FIRST_LOC,
		ID_ADD_NEW_HKEY,
		ID_EDIT_HKEY,
		ID_DELETE_HKEY,
		ID_LIST_HKEYS
	};

	class OptionsDialog : public wxDialog
	{
		DECLARE_CLASS(OptionsDialog)
		DECLARE_EVENT_TABLE()
	private:
		wxNotebook	*_notebook;
		Settings	*_settings;
		Controls	*_controls;

		wxNotebookPage *_general;
		wxNotebookPage *_sizes;
		wxNotebookPage *_colors;
		wxNotebookPage *_fonts;
		wxNotebookPage *_paths;
		wxNotebookPage *_hotkeys;

		wxCheckBox	*_chkAutoSave;
		wxCheckBox	*_chkDescOfLoc;
		wxCheckBox	*_chkOpeningLoc;
		wxCheckBox	*_chkOpeningAct;
		wxCheckBox	*_chkFirstLoc;
		wxCheckBox	*_chkOnLocActIcons;
		wxCheckBox	*_chkLocDescVisible;
		wxCheckBox	*_chkLocActsVisible;
		wxCheckBox	*_chkWrapLines;
		wxCheckBox	*_chkOpenLastGame;
		wxCheckBox	*_chkShowLinesNums;

		wxSpinCtrl	*_spnAutoSaveMin;
		wxSpinCtrl	*_spnHeights;
		wxSpinCtrl	*_spnWidth1;
		wxSpinCtrl	*_spnWidth2;
		wxSpinCtrl	*_spnTabSize;
		wxTextCtrl	*_txtNameFirsLoc;

		wxWindow	*_colorStatements;
		wxWindow	*_colorFunctions;
		wxWindow	*_colorSysVariables;
		wxWindow	*_colorStrings;
		wxWindow	*_colorNumbers;
		wxWindow	*_colorOptsBrts;
		wxWindow	*_colorMarks;
		wxWindow	*_colorComments;
		wxWindow	*_colorBaseFont;
		wxWindow	*_colorTextBack;
		wxWindow	*_colorBaseBack;

		wxButton	*_btnClrsStatements;
		wxButton	*_btnClrsFunctions;
		wxButton	*_btnClrsSysVariables;
		wxButton	*_btnClrsStrings;
		wxButton	*_btnClrsNumbers;
		wxButton	*_btnClrsOptsBrts;
		wxButton	*_btnClrsMarks;
		wxButton	*_btnClrsComments;
		wxButton	*_btnClrsBaseFont;
		wxButton	*_btnClrsTextBack;
		wxButton	*_btnClrsBaseBack;

		wxButton	*_btnFontsStatements;
		wxButton	*_btnFontsFunctions;
		wxButton	*_btnFontsSysVariables;
		wxButton	*_btnFontsStrings;
		wxButton	*_btnFontsNumbers;
		wxButton	*_btnFontsOptsBrts;
		wxButton	*_btnFontsMarks;
		wxButton	*_btnFontsComments;
		wxButton	*_btnFontsBase;

		wxTextCtrl	*_txtFontStatements;
		wxTextCtrl	*_txtFontFunctions;
		wxTextCtrl	*_txtFontSysVariables;
		wxTextCtrl	*_txtFontStrings;
		wxTextCtrl	*_txtFontNumbers;
		wxTextCtrl	*_txtFontOptsBrts;
		wxTextCtrl	*_txtFontMarks;
		wxTextCtrl	*_txtFontComments;
		wxTextCtrl	*_txtFontBase;

		wxTextCtrl	*_txtPathPlayer;
		wxTextCtrl	*_txtPathHelp;
		wxTextCtrl	*_txtPathTxt2Gam;

		wxButton	*_btnPathPlayer;
		wxButton	*_btnPathHelp;
		wxButton	*_btnPathTxt2Gam;

		wxListCtrl	*_lstHotKeys;

		wxButton	*_btnAddNewHotKey;
		wxButton	*_btnEditHotKey;
		wxButton	*_btnDelHotKey;
		
		wxButton	*_btnOK;
		wxButton	*_btnApply;
		wxButton	*_btnReset;
		
		void OnColorSelect(wxCommandEvent &event);
		void OnFontSelect(wxCommandEvent &event);
		void OnPathSelect(wxCommandEvent &event);
		void OnApplySettings(wxCommandEvent &event);
		void OnOkSettings(wxCommandEvent &event);
		void OnCancelSettings(wxCommandEvent &event);
		void OnResetSettings(wxCommandEvent &event);
		void OnStateChanged(wxCommandEvent &event);
		void OnStateChanged(wxSpinEvent &event);
		void OnAddHotKey(wxCommandEvent &event);
		void OnEditHotKey(wxCommandEvent &event);
		void OnDeleteHotKey(wxCommandEvent &event);
		void OnDblClickHotKeysList(wxListEvent &event);
		void OnCloseDialog(wxCloseEvent &event);

		void InitFontsDialog(wxFontDialog &dialog, const wxFont &font);
		void InitColoursDialog(wxColourDialog &dialog, const wxColour &col);
		void InitOptionsDialog();
		void ApplySettings();
		void EditHotKey();
		void AddHotKey();
		void DeleteHotKey();
	public:
		OptionsDialog(wxWindow *parent, const wxString& title, Controls *controls, int style = wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxRESIZE_BORDER);
	};

#endif
