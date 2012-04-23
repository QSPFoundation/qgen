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

#ifndef _QGEN_OPTIONS_DIALOG_H_
	#define _QGEN_OPTIONS_DIALOG_H_

	#include <wx/wx.h>
	#include <wx/notebook.h>
	#include <wx/spinctrl.h>
	#include <wx/colordlg.h>
	#include <wx/fontdlg.h>
	#include <wx/notebook.h>
	#include <wx/sizer.h>
	#include <wx/listctrl.h>
	#include <wx/dir.h>
	#include "settings.h"
	#include "icontrols.h"
	#include "optionshotkeysdialog.h"
	#include "chksyshotkey.h"

	enum
	{
		ID_COLORS_STATEMENTS = 11200,
		ID_COLORS_FUNCTIONS,
		ID_COLORS_SYSVARIABLES,
		ID_COLORS_NUMBERS,
		ID_COLORS_STRINGS,
		ID_COLORS_OPERATIONSBRACKETS,
		ID_COLORS_MARKS,
		ID_COLORS_COMMENTS,
		ID_COLORS_BASEFONT,
		ID_COLORS_TEXTBACK,
		ID_COLORS_BASEBACK,
		ID_FONTS_STATEMENTS,
		ID_FONTS_FUNCTIONS,
		ID_FONTS_SYSVARIABLES,
		ID_FONTS_NUMBERS,
		ID_FONTS_STRINGS,
		ID_FONTS_OPERATIONSBRACKETS,
		ID_FONTS_MARKS,
		ID_FONTS_COMMENTS,
		ID_FONTS_BASE,
		ID_PATH_PLAYER,
		ID_PATH_HELP,
		ID_PATH_TXT2GAM,
		ID_OK_SETTINGS,
		ID_RESET_SETTINGS,
		ID_APPLY_SETTINGS,
		ID_AUTO_SAVE,
		ID_FIRST_LOC,
		ID_ADD_NEWHKEY,
		ID_EDIT_HKEY,
		ID_DELETE_HKEY,
		ID_LIST_HKEYS,
		ID_COMB_LANG
	};

	WX_DECLARE_STRING_HASH_MAP(int, LangTable);

	class OptionsDialog : public wxDialog
	{
		DECLARE_CLASS(OptionsDialog)
		DECLARE_EVENT_TABLE()
	private:
		wxNotebook	*_notebook;
		Settings	*_settings;
		IControls	*_controls;
		wxFrame		*_parent;

		wxNotebookPage *_general;
		wxNotebookPage *_editor;
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
		wxCheckBox	*_chkCollapseCode;

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
		wxButton	*_btnCancel;

		wxStaticText *_stTextHeights;
		wxStaticText *_stTextWidth1;
		wxStaticText *_stTextWidth2;
		wxStaticText *_stTextTabSize;
		wxStaticText *_stTextCmbLang;
		wxStaticText *_autoSaveUnits;

		wxStaticText *_stText1;
		wxStaticText *_stText2;
		wxStaticText *_stText3;
		wxStaticText *_stText4;
		wxStaticText *_stText5;
		wxStaticText *_stText6;
		wxStaticText *_stText7;
		wxStaticText *_stText8;
		wxStaticText *_stText9;
		wxStaticText *_stText10;
		wxStaticText *_stText11;

		wxStaticText *_stText01;
		wxStaticText *_stText02;
		wxStaticText *_stText03;
		wxStaticText *_stText04;
		wxStaticText *_stText05;
		wxStaticText *_stText06;
		wxStaticText *_stText07;
		wxStaticText *_stText08;
		wxStaticText *_stText09;

		wxStaticText *_stText001;
		wxStaticText *_stText002;
		wxStaticText *_stText003;

		wxStaticText *_stText0001;

		HotkeyDataArray _hotkeysData;

		wxComboBox	*_cmbLang;
		LangTable	_langTable;

		void OnColorSelect(wxCommandEvent &event);
		void OnFontSelect(wxCommandEvent &event);
		void OnPathSelect(wxCommandEvent &event);
		void OnApplySettings(wxCommandEvent &event);
		void OnOkSettings(wxCommandEvent &event);
		void OnResetSettings(wxCommandEvent &event);
		void OnStateChanged(wxCommandEvent &event);
		void OnStateChangedSpinCtrl(wxSpinEvent &event);
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
		void UpdateLanguagesList();
		void ReCreateGUI();
	public:
		OptionsDialog(wxFrame *parent, const wxString& title, IControls *controls, int style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER);
		~OptionsDialog();
	};

#endif
