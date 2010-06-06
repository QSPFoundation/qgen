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

#include "OptionsDialog.h"

IMPLEMENT_CLASS(OptionsDialog, wxDialog)

BEGIN_EVENT_TABLE(OptionsDialog, wxDialog)
	EVT_BUTTON(ID_COLORS_STATEMENTS, OptionsDialog::OnColorSelect)
	EVT_BUTTON(ID_COLORS_FUNCTIONS, OptionsDialog::OnColorSelect)
	EVT_BUTTON(ID_COLORS_SYSVARIABLES, OptionsDialog::OnColorSelect)
	EVT_BUTTON(ID_COLORS_STRINGS, OptionsDialog::OnColorSelect)
	EVT_BUTTON(ID_COLORS_NUMBERS, OptionsDialog::OnColorSelect)
	EVT_BUTTON(ID_COLORS_OPERATIONSBRACKETS, OptionsDialog::OnColorSelect)
	EVT_BUTTON(ID_COLORS_MARKS, OptionsDialog::OnColorSelect)
	EVT_BUTTON(ID_COLORS_COMMENTS, OptionsDialog::OnColorSelect)
	EVT_BUTTON(ID_COLORS_BASEFONT, OptionsDialog::OnColorSelect)
	EVT_BUTTON(ID_COLORS_TEXTBACK, OptionsDialog::OnColorSelect)
	EVT_BUTTON(ID_COLORS_BASEBACK, OptionsDialog::OnColorSelect)
	EVT_BUTTON(ID_FONTS_STATEMENTS, OptionsDialog::OnFontSelect)
	EVT_BUTTON(ID_FONTS_FUNCTIONS, OptionsDialog::OnFontSelect)
	EVT_BUTTON(ID_FONTS_SYSVARIABLES, OptionsDialog::OnFontSelect)
	EVT_BUTTON(ID_FONTS_STRINGS, OptionsDialog::OnFontSelect)
	EVT_BUTTON(ID_FONTS_NUMBERS, OptionsDialog::OnFontSelect)
	EVT_BUTTON(ID_FONTS_OPERATIONSBRACKETS, OptionsDialog::OnFontSelect)
	EVT_BUTTON(ID_FONTS_MARKS, OptionsDialog::OnFontSelect)
	EVT_BUTTON(ID_FONTS_COMMENTS, OptionsDialog::OnFontSelect)
	EVT_BUTTON(ID_FONTS_BASE, OptionsDialog::OnFontSelect)
	EVT_BUTTON(ID_PATH_PLAYER, OptionsDialog::OnPathSelect)
	EVT_BUTTON(ID_PATH_HELP, OptionsDialog::OnPathSelect)
	EVT_BUTTON(ID_PATH_TXT2GAM, OptionsDialog::OnPathSelect)
	EVT_BUTTON(ID_OK_SETTINGS, OptionsDialog::OnOkSettings)
	EVT_BUTTON(ID_APPLY_SETTINGS, OptionsDialog::OnApplySettings)
	EVT_BUTTON(ID_RESET_SETTINGS, OptionsDialog::OnResetSettings)
	EVT_BUTTON(ID_ADD_NEWHKEY, OptionsDialog::OnAddHotKey)
	EVT_BUTTON(ID_EDIT_HKEY, OptionsDialog::OnEditHotKey)
	EVT_BUTTON(ID_DELETE_HKEY, OptionsDialog::OnDeleteHotKey)
	EVT_TEXT(wxID_ANY, OptionsDialog::OnStateChanged)
	EVT_CHECKBOX(wxID_ANY, OptionsDialog::OnStateChanged)
	EVT_SPINCTRL(wxID_ANY, OptionsDialog::OnStateChangedSpinCtrl)
	EVT_LIST_ITEM_ACTIVATED(ID_LIST_HKEYS, OptionsDialog::OnDblClickHotKeysList)
	EVT_CLOSE(OptionsDialog::OnCloseDialog)
END_EVENT_TABLE()

OptionsDialog::OptionsDialog(wxFrame *parent, const wxString &title, IControls *controls, int style) :
	wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, style)
{
	_settings = controls->GetSettings();
	_controls = controls;
	_parent = parent;

	wxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	wxSizer *notebookSizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);

	_btnOK = new wxButton(this, ID_OK_SETTINGS, wxEmptyString);
	_btnCancel = new wxButton(this, wxID_CANCEL, wxEmptyString);
	_btnApply = new wxButton(this, ID_APPLY_SETTINGS, wxEmptyString);
	_btnReset = new wxButton(this, ID_RESET_SETTINGS, wxEmptyString);

	btnSizer->Add(_btnReset, 0, wxLEFT|wxBOTTOM, 5);
	btnSizer->AddStretchSpacer(1);
	btnSizer->Add(_btnOK, 0, wxRIGHT|wxBOTTOM, 5);
	btnSizer->Add(_btnCancel, 0, wxRIGHT|wxBOTTOM, 5);
	btnSizer->Add(_btnApply, 0, wxRIGHT|wxBOTTOM, 5);

	_notebook = new wxNotebook(this, wxID_ANY);

	_general = new wxPanel(_notebook);
	_notebook->AddPage(_general, wxEmptyString);

	wxFlexGridSizer *topSizerGeneral = new wxFlexGridSizer(2);
	wxSizer *sizerAutoSave = new wxBoxSizer(wxHORIZONTAL);

	_chkAutoSave = new wxCheckBox(_general, ID_AUTO_SAVE, wxEmptyString);
	_chkFirstLoc = new wxCheckBox(_general, ID_FIRST_LOC, wxEmptyString);
	_chkDescOfLoc = new wxCheckBox(_general, wxID_ANY, wxEmptyString);
	_chkOpeningLoc = new wxCheckBox(_general, wxID_ANY, wxEmptyString);
	_chkOpeningAct = new wxCheckBox(_general, wxID_ANY, wxEmptyString);
	_chkOnLocActIcons = new wxCheckBox(_general, wxID_ANY, wxEmptyString);
	_chkLocDescVisible = new wxCheckBox(_general, wxID_ANY, wxEmptyString);
	_chkLocActsVisible = new wxCheckBox(_general, wxID_ANY, wxEmptyString);
	_chkOpenLastGame = new wxCheckBox(_general, wxID_ANY, wxEmptyString);

	_stTextCmbLang = new wxStaticText(_general, wxID_ANY, wxEmptyString);
	
	_cmbLang = new wxComboBox(_general, ID_COMB_LANG, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, 0, wxCB_READONLY);

	_spnAutoSaveMin = new wxSpinCtrl(_general, wxID_ANY, wxT("5"), wxDefaultPosition, wxSize(45, wxDefaultCoord),
										wxSP_ARROW_KEYS, 1, 60, 5);
	_txtNameFirsLoc = new wxTextCtrl(_general, wxID_ANY, wxEmptyString);

	_autoSaveUnits  = new wxStaticText(_general, wxID_ANY, wxEmptyString);

	sizerAutoSave->Add(_spnAutoSaveMin);
	sizerAutoSave->Add(_autoSaveUnits, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 2);

	topSizerGeneral->Add(_chkAutoSave, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	topSizerGeneral->Add(sizerAutoSave, 0, wxALL, 2);
	topSizerGeneral->Add(_chkFirstLoc, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerGeneral->Add(_txtNameFirsLoc, 0, wxALL|wxGROW, 2);
	topSizerGeneral->Add(_chkDescOfLoc, 0, wxALL, 5);
	topSizerGeneral->Add(NULL, 0);
	topSizerGeneral->Add(_chkOpeningLoc, 0, wxALL, 5);
	topSizerGeneral->Add(NULL, 0);
	topSizerGeneral->Add(_chkOpeningAct, 0, wxALL, 5);
	topSizerGeneral->Add(NULL, 0);
	topSizerGeneral->Add(_chkOnLocActIcons, 0, wxALL, 5);
	topSizerGeneral->Add(NULL, 0);
	topSizerGeneral->Add(_chkLocDescVisible, 0, wxALL, 5);
	topSizerGeneral->Add(NULL, 0);
	topSizerGeneral->Add(_chkLocActsVisible, 0, wxALL, 5);
	topSizerGeneral->Add(NULL, 0);
	topSizerGeneral->Add(_chkOpenLastGame, 0, wxALL, 5);
	topSizerGeneral->Add(NULL, 0);
	topSizerGeneral->Add(_stTextCmbLang, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerGeneral->Add(_cmbLang, 0, wxALL, 5);
	topSizerGeneral->AddGrowableCol(1, 0);

	_general->SetSizerAndFit(topSizerGeneral);
	_general->SetAutoLayout(true);

	_editor = new wxPanel(_notebook);
	_notebook->AddPage(_editor, wxEmptyString);

	wxFlexGridSizer *topSizerEditor = new wxFlexGridSizer(2);

	_chkWrapLines = new wxCheckBox(_editor, wxID_ANY, wxEmptyString);
	_chkShowLinesNums = new wxCheckBox(_editor, wxID_ANY, wxEmptyString);
	_chkCollapseCode = new wxCheckBox(_editor, wxID_ANY, wxEmptyString);

	topSizerEditor->Add(_chkWrapLines, 0, wxALL, 5);
	topSizerEditor->Add(NULL, 0);
	topSizerEditor->Add(_chkShowLinesNums, 0, wxALL, 5);
	topSizerEditor->Add(NULL, 0);
	topSizerEditor->Add(_chkCollapseCode, 0, wxALL, 5);
	topSizerEditor->Add(NULL, 0);
	topSizerEditor->AddGrowableCol(1, 0);

	_editor->SetSizerAndFit(topSizerEditor);
	_editor->SetAutoLayout(true);

	_sizes = new wxPanel(_notebook);
	_notebook->AddPage(_sizes, wxEmptyString);

	wxFlexGridSizer *topSizerSizes = new wxFlexGridSizer(2);

	_stTextHeights = new wxStaticText(_sizes, wxID_ANY, wxEmptyString);
	_stTextWidth1 = new wxStaticText(_sizes, wxID_ANY, wxEmptyString);
	_stTextWidth2 = new wxStaticText(_sizes, wxID_ANY, wxEmptyString);
	_stTextTabSize = new wxStaticText(_sizes, wxID_ANY, wxEmptyString);

	_spnHeights = new wxSpinCtrl(_sizes, wxID_ANY, wxT("100"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 1, 100, 100);
	_spnWidth1 = new wxSpinCtrl(_sizes, wxID_ANY, wxT("100"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 1, 100, 100);
	_spnWidth2 = new wxSpinCtrl(_sizes, wxID_ANY, wxT("100"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 1, 100, 100);
	_spnTabSize = new wxSpinCtrl(_sizes, wxID_ANY, wxT("8"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 2, 8, 8);

	topSizerSizes->Add(_stTextHeights, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerSizes->Add(_spnHeights, 0, wxALL, 5);
	topSizerSizes->Add(_stTextWidth1, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerSizes->Add(_spnWidth1, 0, wxALL, 5);
	topSizerSizes->Add(_stTextWidth2, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerSizes->Add(_spnWidth2, 0, wxALL, 5);
	topSizerSizes->Add(_stTextTabSize, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerSizes->Add(_spnTabSize, 0, wxALL, 5);

	topSizerSizes->AddGrowableCol(0, 1);

	_sizes->SetSizerAndFit(topSizerSizes);
	_sizes->SetAutoLayout(true);

	_colors = new wxPanel(_notebook);
	_notebook->AddPage(_colors, wxEmptyString);

	wxFlexGridSizer *topSizerColors = new wxFlexGridSizer(3);

	_stText1 = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
	_stText2 = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
	_stText3 = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
	_stText4 = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
	_stText5 = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
	_stText6 = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
	_stText7 = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
	_stText8 = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
	_stText9 = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
	_stText10 = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
	_stText11 = new wxStaticText(_colors, wxID_ANY, wxEmptyString);

	_colorStatements = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
	_colorFunctions = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
	_colorSysVariables = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
	_colorStrings = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
	_colorNumbers = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
	_colorOptsBrts = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
	_colorMarks = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
	_colorComments = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
	_colorBaseFont = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
	_colorTextBack = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
	_colorBaseBack = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));

	_btnClrsStatements = new wxButton(_colors, ID_COLORS_STATEMENTS, wxEmptyString);
	_btnClrsFunctions = new wxButton(_colors, ID_COLORS_FUNCTIONS, wxEmptyString);
	_btnClrsSysVariables = new wxButton(_colors, ID_COLORS_SYSVARIABLES, wxEmptyString);
	_btnClrsStrings = new wxButton(_colors, ID_COLORS_STRINGS, wxEmptyString);
	_btnClrsNumbers = new wxButton(_colors, ID_COLORS_NUMBERS, wxEmptyString);
	_btnClrsOptsBrts = new wxButton(_colors, ID_COLORS_OPERATIONSBRACKETS, wxEmptyString);
	_btnClrsMarks = new wxButton(_colors, ID_COLORS_MARKS, wxEmptyString);
	_btnClrsComments = new wxButton(_colors, ID_COLORS_COMMENTS, wxEmptyString);
	_btnClrsBaseFont = new wxButton(_colors, ID_COLORS_BASEFONT, wxEmptyString);
	_btnClrsTextBack = new wxButton(_colors, ID_COLORS_TEXTBACK, wxEmptyString);
	_btnClrsBaseBack = new wxButton(_colors, ID_COLORS_BASEBACK, wxEmptyString);

	topSizerColors->Add(_stText11, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorBaseBack, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsBaseBack, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(_stText10, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorTextBack, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsTextBack, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(_stText9, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorBaseFont, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsBaseFont, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(_stText1, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorStatements, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsStatements, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(_stText2, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorFunctions, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsFunctions, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(_stText3, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorSysVariables, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsSysVariables, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(_stText4, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorStrings, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsStrings, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(_stText5, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorNumbers, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsNumbers, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(_stText6, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorOptsBrts, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsOptsBrts, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(_stText7, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorMarks, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsMarks, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(_stText8, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorComments, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsComments, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->AddGrowableCol(0, 1);

	_colors->SetSizerAndFit(topSizerColors);
	_colors->SetAutoLayout(true);

	_fonts = new wxPanel(_notebook);
	_notebook->AddPage(_fonts, wxEmptyString);

	wxFlexGridSizer *topSizerFonts = new wxFlexGridSizer(3);

	_stText01 = new wxStaticText(_fonts, wxID_ANY, wxEmptyString);
	_stText02 = new wxStaticText(_fonts, wxID_ANY, wxEmptyString);
	_stText03 = new wxStaticText(_fonts, wxID_ANY, wxEmptyString);
	_stText04 = new wxStaticText(_fonts, wxID_ANY, wxEmptyString);
	_stText05 = new wxStaticText(_fonts, wxID_ANY, wxEmptyString);
	_stText06 = new wxStaticText(_fonts, wxID_ANY, wxEmptyString);
	_stText07 = new wxStaticText(_fonts, wxID_ANY, wxEmptyString);
	_stText08 = new wxStaticText(_fonts, wxID_ANY, wxEmptyString);
	_stText09 = new wxStaticText(_fonts, wxID_ANY, wxEmptyString);

	_txtFontStatements = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontFunctions = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontSysVariables = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontStrings = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontNumbers = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontOptsBrts = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontMarks = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontComments = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontBase = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);

	_btnFontsStatements = new wxButton(_fonts, ID_FONTS_STATEMENTS, wxEmptyString);
	_btnFontsFunctions = new wxButton(_fonts, ID_FONTS_FUNCTIONS, wxEmptyString);
	_btnFontsSysVariables = new wxButton(_fonts, ID_FONTS_SYSVARIABLES, wxEmptyString);
	_btnFontsStrings = new wxButton(_fonts, ID_FONTS_STRINGS, wxEmptyString);
	_btnFontsNumbers = new wxButton(_fonts, ID_FONTS_NUMBERS, wxEmptyString);
	_btnFontsOptsBrts = new wxButton(_fonts, ID_FONTS_OPERATIONSBRACKETS, wxEmptyString);
	_btnFontsMarks = new wxButton(_fonts, ID_FONTS_MARKS, wxEmptyString);
	_btnFontsComments = new wxButton(_fonts, ID_FONTS_COMMENTS, wxEmptyString);
	_btnFontsBase = new wxButton(_fonts, ID_FONTS_BASE, wxEmptyString);

	topSizerFonts->Add(_stText09, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontBase, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsBase, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(_stText01, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontStatements, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsStatements, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(_stText02, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontFunctions, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsFunctions, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(_stText03, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontSysVariables, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsSysVariables, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(_stText04, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontStrings, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsStrings, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(_stText05, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontNumbers, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsNumbers, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(_stText06, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontOptsBrts, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsOptsBrts, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(_stText07, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontMarks, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsMarks, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(_stText08, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontComments, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsComments, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->AddGrowableCol(1, 0);

	_fonts->SetSizerAndFit(topSizerFonts);
	_fonts->SetAutoLayout(true);

	_paths = new wxPanel(_notebook);
	_notebook->AddPage(_paths, wxEmptyString);

	wxFlexGridSizer *topSizerPaths = new wxFlexGridSizer(3);

	_stText001 = new wxStaticText(_paths, wxID_ANY, wxEmptyString);
	_stText002 = new wxStaticText(_paths, wxID_ANY, wxEmptyString);
	_stText003 = new wxStaticText(_paths, wxID_ANY, wxEmptyString);

	_txtPathPlayer = new wxTextCtrl(_paths, wxID_ANY);
	_txtPathHelp = new wxTextCtrl(_paths, wxID_ANY);
	_txtPathTxt2Gam = new wxTextCtrl(_paths, wxID_ANY);

	_btnPathPlayer = new wxButton(_paths, ID_PATH_PLAYER, wxEmptyString);
	_btnPathHelp = new wxButton(_paths, ID_PATH_HELP, wxEmptyString);
	_btnPathTxt2Gam = new wxButton(_paths, ID_PATH_TXT2GAM, wxEmptyString);

	topSizerPaths->Add(_stText001, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerPaths->Add(_txtPathPlayer, 0, wxALL|wxGROW, 2);
	topSizerPaths->Add(_btnPathPlayer, 0, wxALIGN_CENTER_VERTICAL);
	topSizerPaths->Add(_stText002, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerPaths->Add(_txtPathHelp, 0, wxALL|wxGROW, 2);
	topSizerPaths->Add(_btnPathHelp, 0, wxALIGN_CENTER_VERTICAL);
	topSizerPaths->Add(_stText003, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerPaths->Add(_txtPathTxt2Gam, 0, wxALL|wxGROW, 2);
	topSizerPaths->Add(_btnPathTxt2Gam, 0, wxALIGN_CENTER_VERTICAL);
	topSizerPaths->AddGrowableCol(1, 0);

	_paths->SetSizerAndFit(topSizerPaths);
	_paths->SetAutoLayout(true);

	#ifdef __WXMSW__
		_hotkeys = new wxPanel(_notebook);
		_notebook->AddPage(_hotkeys, wxEmptyString);

		wxBoxSizer *topSizerHotKeys = new wxBoxSizer(wxVERTICAL);

		_stText0001 = new wxStaticText(_hotkeys, wxID_ANY, wxEmptyString);
		_lstHotKeys = new wxListCtrl(_hotkeys, ID_LIST_HKEYS, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL);
		_lstHotKeys->InsertColumn(0, wxEmptyString, wxLIST_FORMAT_LEFT, 150);
		_lstHotKeys->InsertColumn(1, wxEmptyString, wxLIST_FORMAT_LEFT, 300);

		wxBoxSizer *btnHotkKeysSizer = new wxBoxSizer(wxHORIZONTAL);
		_btnAddNewHotKey = new wxButton(_hotkeys, ID_ADD_NEWHKEY, wxEmptyString);
		_btnEditHotKey = new wxButton(_hotkeys, ID_EDIT_HKEY, wxEmptyString);
		_btnDelHotKey = new wxButton(_hotkeys, ID_DELETE_HKEY, wxEmptyString);

		btnHotkKeysSizer->Add(_btnAddNewHotKey, wxALL, 5);
		btnHotkKeysSizer->Add(_btnEditHotKey, wxALL, 5);
		btnHotkKeysSizer->Add(_btnDelHotKey, wxALL, 5);

		topSizerHotKeys->Add(_stText0001, 0, wxTOP|wxLEFT, 5);
		topSizerHotKeys->Add(_lstHotKeys, 1, wxALL|wxGROW, 5);
		topSizerHotKeys->Add(btnHotkKeysSizer, 0, wxGROW);

		_hotkeys->SetSizerAndFit(topSizerHotKeys);
		_hotkeys->SetAutoLayout(true);
	#endif

	notebookSizer->Add(_notebook, 1, wxALL|wxGROW, 3);

	topSizer->Add(notebookSizer, 1, wxGROW);
	topSizer->Add(btnSizer, 0, wxGROW);

	SetSizerAndFit(topSizer);
	SetAutoLayout(true);
	SetMinClientSize(wxSize(500, 400));
	InitOptionsDialog();
	_btnOK->SetDefault();
	ReCreateGUI();
}

OptionsDialog::~OptionsDialog()
{
	_langTable.clear();
}

void OptionsDialog::ReCreateGUI()
{
	//Main button
	_btnOK->SetLabel(_("OK"));
	_btnCancel->SetLabel(_("Cancel"));
	_btnApply->SetLabel(_("Apply"));
	_btnReset->SetLabel(_("Reset"));
	//Page General
	_notebook->SetPageText(0, _("General"));
	_chkAutoSave->SetLabel(_("Auto save every"));
	_chkFirstLoc->SetLabel(_("Auto create first location:"));
	_chkDescOfLoc->SetLabel(_("Show short locations descriptions"));
	_chkOpeningLoc->SetLabel(_("Open location after creation"));
	_chkOpeningAct->SetLabel(_("Open action after creation"));
	_chkOnLocActIcons->SetLabel(_("Show locations icons"));
	_chkLocDescVisible->SetLabel(_("Show base description on location's tab"));
	_chkLocActsVisible->SetLabel(_("Show base actions on location's tab"));
	_chkOpenLastGame->SetLabel(_("Remember game file on exit"));
	_stTextCmbLang->SetLabel(_("UI language"));
	_txtNameFirsLoc->SetLabel(_("Start"));
	_autoSaveUnits->SetLabel(_("minutes"));
	//Page Editor
	_notebook->SetPageText(1, _("Code editor"));
	_chkWrapLines->SetLabel(_("Wrap lines by words"));
	_chkShowLinesNums->SetLabel(_("Show lines numbers"));
	_chkCollapseCode->SetLabel(_("Collapse blocks of code when location is opened"));
	//Page Sizes
	_notebook->SetPageText(2, _("Sizes"));
	_stTextHeights->SetLabel(_("Relative height of description and location's\ncode fields to the tab's height (%):"));
	_stTextWidth1->SetLabel(_("Relative width of location's description field\nto the tab's height (%):"));
	_stTextWidth2->SetLabel(_("Relative width of actions list to the tab's width (%):"));
	_stTextTabSize->SetLabel(_("Size of TAB:"));
	//Page Colors
	_notebook->SetPageText(3, _("Colors"));
	_stText1->SetLabel(_("Statements color:"));
	_stText2->SetLabel(_("Functions color:"));
	_stText3->SetLabel(_("System variables color:"));
	_stText4->SetLabel(_("Strings color:"));
	_stText5->SetLabel(_("Numbers color:"));
	_stText6->SetLabel(_("Operations color:"));
	_stText7->SetLabel(_("Labels color:"));
	_stText8->SetLabel(_("Comments color:"));
	_stText9->SetLabel(_("Base font's color:"));
	_stText10->SetLabel(_("Tabs background color:"));
	_stText11->SetLabel(_("Main background color:"));
	_btnClrsStatements->SetLabel(_("Select color..."));
	_btnClrsFunctions->SetLabel(_("Select color..."));
	_btnClrsSysVariables->SetLabel(_("Select color..."));
	_btnClrsStrings->SetLabel(_("Select color..."));
	_btnClrsNumbers->SetLabel(_("Select color..."));
	_btnClrsOptsBrts->SetLabel(_("Select color..."));
	_btnClrsMarks->SetLabel(_("Select color..."));
	_btnClrsComments->SetLabel(_("Select color..."));
	_btnClrsBaseFont->SetLabel(_("Select color..."));
	_btnClrsTextBack->SetLabel(_("Select color..."));
	_btnClrsBaseBack->SetLabel(_("Select color..."));
	//Page Fonts
	_notebook->SetPageText(4, _("Fonts"));
	_stText01->SetLabel(_("Statements font:"));
	_stText02->SetLabel(_("Functions font:"));
	_stText03->SetLabel(_("System variables font:"));
	_stText04->SetLabel(_("Strings font:"));
	_stText05->SetLabel(_("Numbers font:"));
	_stText06->SetLabel(_("Operations font:"));
	_stText07->SetLabel(_("Labels font:"));
	_stText08->SetLabel(_("Comments font:"));
	_stText09->SetLabel(_("Main font:"));
	_btnFontsStatements->SetLabel(_("Select font..."));
	_btnFontsFunctions->SetLabel(_("Select font..."));
	_btnFontsSysVariables->SetLabel(_("Select font..."));
	_btnFontsStrings->SetLabel(_("Select font..."));
	_btnFontsNumbers->SetLabel(_("Select font..."));
	_btnFontsOptsBrts->SetLabel(_("Select font..."));
	_btnFontsMarks->SetLabel(_("Select font..."));
	_btnFontsComments->SetLabel(_("Select font..."));
	_btnFontsBase->SetLabel(_("Select font..."));
	//Page Paths
	_notebook->SetPageText(5, _("Paths"));
	_stText001->SetLabel(_("Path to player:"));
	_stText002->SetLabel(_("Path to help:"));
	_stText003->SetLabel(_("Path to TXT2GAM:"));
	_btnPathPlayer->SetLabel(_("Select path..."));
	_btnPathHelp->SetLabel(_("Select path..."));
	_btnPathTxt2Gam->SetLabel(_("Select path..."));
	#ifdef __WXMSW__
		//Page HotKeys
		_notebook->SetPageText(6,  _("Hotkeys"));
		_stText0001->SetLabel(_("Hotkeys list:"));
		wxListItem header; 
		_lstHotKeys->GetColumn(0, header);
		header.SetText(_("Hotkey"));
		_lstHotKeys->SetColumn(0, header);
		_lstHotKeys->GetColumn(1, header);
		header.SetText(_("Execute"));
		_lstHotKeys->SetColumn(1, header);
		_btnAddNewHotKey->SetLabel(_("Add..."));
		_btnEditHotKey->SetLabel(_("Edit..."));
		_btnDelHotKey->SetLabel(_("Delete"));
	#endif
	GetSizer()->SetSizeHints(this);
}

void OptionsDialog::InitColoursDialog(wxColourDialog &dialog, const wxColour &col)
{
	wxColourData data;
	data.SetColour(col);
	dialog.Create(this, &data);
}

void OptionsDialog::InitFontsDialog(wxFontDialog &dialog, const wxFont &font)
{
	wxFontData data;
	data.SetInitialFont(font);
	data.EnableEffects(false);
	dialog.Create(this, data);
}

void OptionsDialog::OnColorSelect( wxCommandEvent &event )
{
	wxColourDialog dialog(this);
	switch(event.GetId())
	{
	case ID_COLORS_STATEMENTS:
		InitColoursDialog(dialog, _colorStatements->GetBackgroundColour());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColour col = dialog.GetColourData().GetColour();
			_colorStatements->SetBackgroundColour(col);
			_colorStatements->Refresh();
			_txtFontStatements->SetForegroundColour(col);
			_btnApply->Enable();
		}
		break;
	case ID_COLORS_FUNCTIONS:
		InitColoursDialog(dialog, _colorFunctions->GetBackgroundColour());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColour col = dialog.GetColourData().GetColour();
			_colorFunctions->SetBackgroundColour(col);
			_colorFunctions->Refresh();
			_txtFontFunctions->SetForegroundColour(col);
			_btnApply->Enable();
		}
		break;
	case ID_COLORS_SYSVARIABLES:
		InitColoursDialog(dialog, _colorSysVariables->GetBackgroundColour());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColour col = dialog.GetColourData().GetColour();
			_colorSysVariables->SetBackgroundColour(col);
			_colorSysVariables->Refresh();
			_txtFontSysVariables->SetForegroundColour(col);
			_btnApply->Enable();
		}
		break;
	case ID_COLORS_NUMBERS:
		InitColoursDialog(dialog, _colorNumbers->GetBackgroundColour());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColour col = dialog.GetColourData().GetColour();
			_colorNumbers->SetBackgroundColour(col);
			_colorNumbers->Refresh();
			_txtFontNumbers->SetForegroundColour(col);
			_btnApply->Enable();
		}
		break;
	case ID_COLORS_STRINGS:
		InitColoursDialog(dialog, _colorStrings->GetBackgroundColour());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColour col = dialog.GetColourData().GetColour();
			_colorStrings->SetBackgroundColour(col);
			_colorStrings->Refresh();
			_txtFontStrings->SetForegroundColour(col);
			_btnApply->Enable();
		}
		break;
	case ID_COLORS_OPERATIONSBRACKETS:
		InitColoursDialog(dialog, _colorOptsBrts->GetBackgroundColour());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColour col = dialog.GetColourData().GetColour();
			_colorOptsBrts->SetBackgroundColour(col);
			_colorOptsBrts->Refresh();
			_txtFontOptsBrts->SetForegroundColour(col);
			_btnApply->Enable();
		}
		break;
	case ID_COLORS_MARKS:
		InitColoursDialog(dialog, _colorMarks->GetBackgroundColour());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColour col = dialog.GetColourData().GetColour();
			_colorMarks->SetBackgroundColour(col);
			_colorMarks->Refresh();
			_txtFontMarks->SetForegroundColour(col);
			_btnApply->Enable();
		}
		break;
	case ID_COLORS_COMMENTS:
		InitColoursDialog(dialog, _colorComments->GetBackgroundColour());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColour col = dialog.GetColourData().GetColour();
			_colorComments->SetBackgroundColour(col);
			_colorComments->Refresh();
			_txtFontComments->SetForegroundColour(col);
			_btnApply->Enable();
		}
		break;
	case ID_COLORS_BASEFONT:
		InitColoursDialog(dialog, _colorBaseFont->GetBackgroundColour());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColour col = dialog.GetColourData().GetColour();
			_colorBaseFont->SetBackgroundColour(col);
			_colorBaseFont->Refresh();
			_txtFontBase->SetForegroundColour(col);
			_btnApply->Enable();
		}
		break;
	case ID_COLORS_TEXTBACK:
		InitColoursDialog(dialog, _colorTextBack->GetBackgroundColour());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColour col = dialog.GetColourData().GetColour();
			_colorTextBack->SetBackgroundColour(col);
			_colorTextBack->Refresh();
			_txtFontStatements->SetBackgroundColour(col);
			_txtFontFunctions->SetBackgroundColour(col);
			_txtFontSysVariables->SetBackgroundColour(col);
			_txtFontStrings->SetBackgroundColour(col);
			_txtFontNumbers->SetBackgroundColour(col);
			_txtFontOptsBrts->SetBackgroundColour(col);
			_txtFontMarks->SetBackgroundColour(col);
			_txtFontComments->SetBackgroundColour(col);
			_txtFontBase->SetBackgroundColour(col);
			_btnApply->Enable();

		}
		break;
	case ID_COLORS_BASEBACK:
		InitColoursDialog(dialog, _colorBaseBack->GetBackgroundColour());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxColour col = dialog.GetColourData().GetColour();
			_colorBaseBack->SetBackgroundColour(col);
			_colorBaseBack->Refresh();
			_btnApply->Enable();
		}
		break;
	}
}

void OptionsDialog::OnFontSelect( wxCommandEvent &event )
{
	wxFontDialog dialog;
	switch(event.GetId())
	{
	case ID_FONTS_STATEMENTS:
		InitFontsDialog(dialog, _txtFontStatements->GetFont());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxFont font = dialog.GetFontData().GetChosenFont();
			_txtFontStatements->SetValue(font.GetFaceName());
			_txtFontStatements->SetFont(font);
			_txtFontStatements->Refresh();
			_btnApply->Enable();
		}
		break;
	case ID_FONTS_FUNCTIONS:
		InitFontsDialog(dialog, _txtFontFunctions->GetFont());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxFont font = dialog.GetFontData().GetChosenFont();
			_txtFontFunctions->SetValue(font.GetFaceName());
			_txtFontFunctions->SetFont(font);
			_txtFontFunctions->Refresh();
			_btnApply->Enable();
		}
		break;
	case ID_FONTS_SYSVARIABLES:
		InitFontsDialog(dialog, _txtFontSysVariables->GetFont());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxFont font = dialog.GetFontData().GetChosenFont();
			_txtFontSysVariables->SetValue(font.GetFaceName());
			_txtFontSysVariables->SetFont(font);
			_txtFontSysVariables->Refresh();
			_btnApply->Enable();
		}
		break;
	case ID_FONTS_NUMBERS:
		InitFontsDialog(dialog, _txtFontNumbers->GetFont());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxFont font = dialog.GetFontData().GetChosenFont();
			_txtFontNumbers->SetValue(font.GetFaceName());
			_txtFontNumbers->SetFont(font);
			_txtFontNumbers->Refresh();
			_btnApply->Enable();
		}
		break;
	case ID_FONTS_STRINGS:
		InitFontsDialog(dialog, _txtFontStrings->GetFont());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxFont font = dialog.GetFontData().GetChosenFont();
			_txtFontStrings->SetValue(font.GetFaceName());
			_txtFontStrings->SetFont(font);
			_txtFontStrings->Refresh();
			_btnApply->Enable();
		}
		break;
	case ID_FONTS_OPERATIONSBRACKETS:
		InitFontsDialog(dialog, _txtFontOptsBrts->GetFont());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxFont font = dialog.GetFontData().GetChosenFont();
			_txtFontOptsBrts->SetValue(font.GetFaceName());
			_txtFontOptsBrts->SetFont(font);
			_txtFontOptsBrts->Refresh();
			_btnApply->Enable();
		}
		break;
	case ID_FONTS_MARKS:
		InitFontsDialog(dialog, _txtFontMarks->GetFont());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxFont font = dialog.GetFontData().GetChosenFont();
			_txtFontMarks->SetValue(font.GetFaceName());
			_txtFontMarks->SetFont(font);
			_txtFontMarks->Refresh();
			_btnApply->Enable();
		}
		break;
	case ID_FONTS_COMMENTS:
		InitFontsDialog(dialog, _txtFontComments->GetFont());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxFont font = dialog.GetFontData().GetChosenFont();
			_txtFontComments->SetValue(font.GetFaceName());
			_txtFontComments->SetFont(font);
			_txtFontComments->Refresh();
			_btnApply->Enable();
		}
		break;
	case ID_FONTS_BASE:
		InitFontsDialog(dialog, _txtFontBase->GetFont());
		if (dialog.ShowModal() == wxID_OK)
		{
			wxFont font = dialog.GetFontData().GetChosenFont();
			_txtFontBase->SetValue(font.GetFaceName());
			_txtFontBase->SetFont(font);
			_txtFontBase->Refresh();
			_btnApply->Enable();
		}
		break;
	}
}

void OptionsDialog::OnPathSelect( wxCommandEvent &event )
{
	wxFileDialog dialog(this);
	switch (event.GetId())
	{
	case ID_PATH_PLAYER:
		dialog.Create(this, _("Select player's file"), wxEmptyString, wxEmptyString,
			_("Player's file (*.exe)|*.exe|All files (*.*)|*.*"), wxFD_OPEN);
		if (dialog.ShowModal() == wxID_OK)
		{
			_txtPathPlayer->SetValue(dialog.GetPath());
			_btnApply->Enable();
		}
		break;
	case ID_PATH_HELP:
		dialog.Create(this, _("Select help file"), wxEmptyString, wxEmptyString,
			_("Help file (*.chm)|*.chm|All files (*.*)|*.*"), wxFD_OPEN);
		if (dialog.ShowModal() == wxID_OK)
		{
			_txtPathHelp->SetValue(dialog.GetPath());
			_btnApply->Enable();
		}
		break;
	case ID_PATH_TXT2GAM:
		dialog.Create(this, _("Select converter's file"), wxEmptyString, wxEmptyString,
			_("Converter's file (*.exe)|*.exe|All files (*.*)|*.*"), wxFD_OPEN);
		if (dialog.ShowModal() == wxID_OK)
		{
			_txtPathTxt2Gam->SetValue(dialog.GetPath());
			_btnApply->Enable();
		}
		break;
	}
}

void OptionsDialog::OnOkSettings(wxCommandEvent &event)
{
	ApplySettings();
	Close();
}

void OptionsDialog::OnApplySettings(wxCommandEvent &event)
{
	ApplySettings();
	ReCreateGUI();
}

void OptionsDialog::OnResetSettings(wxCommandEvent &event)
{
	_settings->InitSettings();
	_controls->UpdateLocale(_settings->GetLangId());
	InitOptionsDialog();
	_settings->NotifyAll();
	ReCreateGUI();
}

void OptionsDialog::OnStateChanged(wxCommandEvent &event)
{
	switch (event.GetId())
	{
	case ID_AUTO_SAVE:
		_spnAutoSaveMin->Enable(event.IsChecked());
		break;
	case ID_FIRST_LOC:
		_txtNameFirsLoc->Enable(event.IsChecked());
		break;
	}
	_btnApply->Enable(true);
}

void OptionsDialog::OnStateChangedSpinCtrl(wxSpinEvent &event)
{
	_btnApply->Enable(true);
}

void OptionsDialog::OnAddHotKey(wxCommandEvent &event)
{
	AddHotKey();
}

void OptionsDialog::OnEditHotKey(wxCommandEvent &event)
{
	EditHotKey();
}

void OptionsDialog::OnDeleteHotKey(wxCommandEvent &event)
{
	DeleteHotKey();
}

void OptionsDialog::OnDblClickHotKeysList(wxListEvent &event)
{
	EditHotKey();
}

void OptionsDialog::OnCloseDialog(wxCloseEvent &event)
{
	_settings->SetOptionsDialogWidth(GetSize().GetWidth());
	_settings->SetOptionsDialogHeight(GetSize().GetHeight());
	event.Skip();
}

void OptionsDialog::ApplySettings()
{
	_settings->SetAutoSave(_chkAutoSave->GetValue());
	_settings->SetAutoSaveInterval(_spnAutoSaveMin->GetValue());
	_settings->SetShowShortLocsDescs(_chkDescOfLoc->GetValue());
	_settings->SetLocDescVisible(_chkLocDescVisible->GetValue());
	_settings->SetLocActsVisible(_chkLocActsVisible->GetValue());
	_settings->SetWrapLines(_chkWrapLines->GetValue());
	_settings->SetOpenNewLoc(_chkOpeningLoc->GetValue());
	_settings->SetOpenNewAct(_chkOpeningAct->GetValue());
	_settings->SetOpenLastGame(_chkOpenLastGame->GetValue());
	_settings->SetShowLinesNums(_chkShowLinesNums->GetValue());
	_settings->SetCreateFirstLoc(_chkFirstLoc->GetValue());
	_settings->SetFirstLocName(_txtNameFirsLoc->GetValue());
	_settings->SetShowLocsIcons(_chkOnLocActIcons->GetValue());
	_settings->SetCollapseCode(_chkCollapseCode->GetValue());
	_settings->SetHeightsCoeff((double)_spnHeights->GetValue()/100);
	_settings->SetWidthsCoeff1((double)_spnWidth1->GetValue()/100);
	_settings->SetWidthsCoeff2((double)_spnWidth2->GetValue()/100);
	_settings->SetTabSize(_spnTabSize->GetValue());
	_settings->SetCurrentHelpPath(_txtPathHelp->GetValue());
	_settings->SetCurrentPlayerPath(_txtPathPlayer->GetValue());
	_settings->SetCurrentTxt2GamPath(_txtPathTxt2Gam->GetValue());

	_settings->SetColour(SYNTAX_STATEMENTS, _colorStatements->GetBackgroundColour());
	_settings->SetColour(SYNTAX_FUNCTIONS, _colorFunctions->GetBackgroundColour());
	_settings->SetColour(SYNTAX_SYS_VARIABLES, _colorSysVariables->GetBackgroundColour());
	_settings->SetColour(SYNTAX_STRINGS, _colorStrings->GetBackgroundColour());
	_settings->SetColour(SYNTAX_NUMBERS, _colorNumbers->GetBackgroundColour());
	_settings->SetColour(SYNTAX_OPERATIONS, _colorOptsBrts->GetBackgroundColour());
	_settings->SetColour(SYNTAX_LABELS, _colorMarks->GetBackgroundColour());
	_settings->SetColour(SYNTAX_COMMENTS, _colorComments->GetBackgroundColour());
	_settings->SetColour(SYNTAX_BASE, _colorBaseFont->GetBackgroundColour());
	_settings->SetTextBackColour(_colorTextBack->GetBackgroundColour());
	_settings->SetBaseBackColour(_colorBaseBack->GetBackgroundColour());

	_settings->SetFont(SYNTAX_STATEMENTS, _txtFontStatements->GetFont());
	_settings->SetFont(SYNTAX_FUNCTIONS, _txtFontFunctions->GetFont());
	_settings->SetFont(SYNTAX_SYS_VARIABLES, _txtFontSysVariables->GetFont());
	_settings->SetFont(SYNTAX_STRINGS, _txtFontStrings->GetFont());
	_settings->SetFont(SYNTAX_NUMBERS, _txtFontNumbers->GetFont());
	_settings->SetFont(SYNTAX_OPERATIONS, _txtFontOptsBrts->GetFont());
	_settings->SetFont(SYNTAX_LABELS, _txtFontMarks->GetFont());
	_settings->SetFont(SYNTAX_COMMENTS, _txtFontComments->GetFont());
	_settings->SetFont(SYNTAX_BASE, _txtFontBase->GetFont());

	#ifdef __WXMSW__
		HotkeyData hotKeyData;
		size_t count = _lstHotKeys->GetItemCount();
		HotkeysStore *hotKeysStore = _settings->GetHotKeys();
		hotKeysStore->ClearHotkeysData();
		for (size_t i = 0; i < count; ++i)
			hotKeysStore->AddHotkeyData(_hotkeysData[i]);
	#endif
	int lang = _langTable[_cmbLang->GetStringSelection()];
 	_controls->UpdateLocale(lang);
	_settings->SetIdLang(lang);
	_settings->NotifyAll();
	_btnApply->Enable(false);
}

void OptionsDialog::InitOptionsDialog()
{
	_chkAutoSave->SetValue(_settings->GetAutoSave());
	_spnAutoSaveMin->SetValue(_settings->GetAutoSaveInterval());
	_chkDescOfLoc->SetValue(_settings->GetShowShortLocsDescs());
	_chkLocDescVisible->SetValue(_settings->GetLocDescVisible());
	_chkLocActsVisible->SetValue(_settings->GetLocActsVisible());
	_chkWrapLines->SetValue(_settings->GetWrapLines());
	_chkOpeningLoc->SetValue(_settings->GetOpenNewLoc());
	_chkOpeningAct->SetValue(_settings->GetOpenNewAct());
	_chkOpenLastGame->SetValue(_settings->GetOpenLastGame());
	_chkShowLinesNums->SetValue(_settings->GetShowLinesNums());
	_chkFirstLoc->SetValue(_settings->GetCreateFirstLoc());
	_txtNameFirsLoc->SetValue(_settings->GetFirstLocName());
	_chkOnLocActIcons->SetValue(_settings->GetShowLocsIcons());
	_chkCollapseCode->SetValue(_settings->GetCollapseCode());

	_spnHeights->SetValue(_settings->GetHeightsCoeff()*100);
	_spnWidth1->SetValue(_settings->GetWidthsCoeff1()*100);
	_spnWidth2->SetValue(_settings->GetWidthsCoeff2()*100);
	_spnTabSize->SetValue(_settings->GetTabSize());

	_txtPathHelp->SetValue(_settings->GetCurrentHelpPath());
	_txtPathPlayer->SetValue(_settings->GetCurrentPlayerPath());
	_txtPathTxt2Gam->SetValue(_settings->GetCurrentTxt2GamPath());

	_colorStatements->SetBackgroundColour(_settings->GetColour(SYNTAX_STATEMENTS));
	_colorFunctions->SetBackgroundColour(_settings->GetColour(SYNTAX_FUNCTIONS));
	_colorSysVariables->SetBackgroundColour(_settings->GetColour(SYNTAX_SYS_VARIABLES));
	_colorStrings->SetBackgroundColour(_settings->GetColour(SYNTAX_STRINGS));
	_colorNumbers->SetBackgroundColour(_settings->GetColour(SYNTAX_NUMBERS));
	_colorOptsBrts->SetBackgroundColour(_settings->GetColour(SYNTAX_OPERATIONS));
	_colorMarks->SetBackgroundColour(_settings->GetColour(SYNTAX_LABELS));
	_colorComments->SetBackgroundColour(_settings->GetColour(SYNTAX_COMMENTS));
	_colorBaseFont->SetBackgroundColour(_settings->GetColour(SYNTAX_BASE));
	_colorTextBack->SetBackgroundColour(_settings->GetTextBackColour());
	_colorBaseBack->SetBackgroundColour(_settings->GetBaseBackColour());

	wxColour textBackColour = _settings->GetTextBackColour();

	_txtFontStatements->SetValue(_settings->GetFont(SYNTAX_STATEMENTS).GetFaceName());
	_txtFontStatements->SetFont(_settings->GetFont(SYNTAX_STATEMENTS));
	_txtFontStatements->SetForegroundColour(_settings->GetColour(SYNTAX_STATEMENTS));
	_txtFontStatements->SetBackgroundColour(textBackColour);

	_txtFontFunctions->SetValue(_settings->GetFont(SYNTAX_FUNCTIONS).GetFaceName());
	_txtFontFunctions->SetFont(_settings->GetFont(SYNTAX_FUNCTIONS));
	_txtFontFunctions->SetForegroundColour(_settings->GetColour(SYNTAX_FUNCTIONS));
	_txtFontFunctions->SetBackgroundColour(textBackColour);

	_txtFontSysVariables->SetValue(_settings->GetFont(SYNTAX_SYS_VARIABLES).GetFaceName());
	_txtFontSysVariables->SetFont(_settings->GetFont(SYNTAX_SYS_VARIABLES));
	_txtFontSysVariables->SetForegroundColour(_settings->GetColour(SYNTAX_SYS_VARIABLES));
	_txtFontSysVariables->SetBackgroundColour(textBackColour);

	_txtFontStrings->SetValue(_settings->GetFont(SYNTAX_STRINGS).GetFaceName());
	_txtFontStrings->SetFont(_settings->GetFont(SYNTAX_STRINGS));
	_txtFontStrings->SetForegroundColour(_settings->GetColour(SYNTAX_STRINGS));
	_txtFontStrings->SetBackgroundColour(textBackColour);

	_txtFontNumbers->SetValue(_settings->GetFont(SYNTAX_NUMBERS).GetFaceName());
	_txtFontNumbers->SetFont(_settings->GetFont(SYNTAX_NUMBERS));
	_txtFontNumbers->SetForegroundColour(_settings->GetColour(SYNTAX_NUMBERS));
	_txtFontNumbers->SetBackgroundColour(textBackColour);

	_txtFontOptsBrts->SetValue(_settings->GetFont(SYNTAX_OPERATIONS).GetFaceName());
	_txtFontOptsBrts->SetFont(_settings->GetFont(SYNTAX_OPERATIONS));
	_txtFontOptsBrts->SetForegroundColour(_settings->GetColour(SYNTAX_OPERATIONS));
	_txtFontOptsBrts->SetBackgroundColour(textBackColour);

	_txtFontMarks->SetValue(_settings->GetFont(SYNTAX_LABELS).GetFaceName());
	_txtFontMarks->SetFont(_settings->GetFont(SYNTAX_LABELS));
	_txtFontMarks->SetForegroundColour(_settings->GetColour(SYNTAX_LABELS));
	_txtFontMarks->SetBackgroundColour(textBackColour);

	_txtFontComments->SetValue(_settings->GetFont(SYNTAX_COMMENTS).GetFaceName());
	_txtFontComments->SetFont(_settings->GetFont(SYNTAX_COMMENTS));
	_txtFontComments->SetForegroundColour(_settings->GetColour(SYNTAX_COMMENTS));
	_txtFontComments->SetBackgroundColour(textBackColour);

	_txtFontBase->SetValue(_settings->GetFont(SYNTAX_BASE).GetFaceName());
	_txtFontBase->SetFont(_settings->GetFont(SYNTAX_BASE));
	_txtFontBase->SetForegroundColour(_settings->GetColour(SYNTAX_BASE));
	_txtFontBase->SetBackgroundColour(textBackColour);

	_txtNameFirsLoc->Enable(_settings->GetCreateFirstLoc());
	_spnAutoSaveMin->Enable(_settings->GetAutoSave());

	#ifdef __WXMSW__
		HotkeysStore *hotKeysStore = _settings->GetHotKeys();
		size_t count = hotKeysStore->GetHotkeysCount();
		_lstHotKeys->DeleteAllItems();
		_hotkeysData.Clear();
		for (size_t i = 0; i < count; ++i)
		{
			const HotkeyData &hotKeyData = hotKeysStore->GetHotkeyData(i);
			_lstHotKeys->InsertItem(i, hotKeyData.GetKeysAsString());
			_lstHotKeys->SetItem(i, 1, hotKeyData.CommandText);
			_hotkeysData.Add(hotKeyData);
		}
	#endif
	UpdateLanguagesList();
	SetSize(_settings->GetOptionsDialogWidth(), _settings->GetOptionsDialogHeight());
	_btnApply->Enable(false);
	Refresh();
}

void OptionsDialog::EditHotKey()
{
	HotkeyData hotKeyData;
	bool isError = true;
	long index = _lstHotKeys->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (index != wxNOT_FOUND)
	{
		OptionsHotkeysDialog dialog(this, _("Edit macro"), _controls);
		dialog.SetHotkeyData(_hotkeysData[index]);
		dialog.CenterOnParent();
		do
		{
			if (dialog.ShowModal() == wxID_OK)
			{
				hotKeyData = dialog.GetHotkeyData();
				if (hotKeyData.HotKeyCode && !hotKeyData.CommandText.IsEmpty())
				{
					ChkSysHotKey chkHKey;
					if (!chkHKey.CheckSystemHotKeys(_parent->GetMenuBar(), hotKeyData.HotKeyCode, hotKeyData.Flags))
					{
						long idx = _lstHotKeys->FindItem(-1, hotKeyData.GetKeysAsString());
						if (idx == wxNOT_FOUND || idx == index)
						{
							_lstHotKeys->DeleteItem(index);
							_lstHotKeys->InsertItem(index, hotKeyData.GetKeysAsString());
							_lstHotKeys->SetItem(index, 1, hotKeyData.CommandText);
							_hotkeysData[index] = hotKeyData;
							_btnApply->Enable(true);
							isError = false;
						}
						else
							_controls->ShowMessage(QGEN_MSG_EXISTS_HKEY);
					}
					else
						_controls->ShowMessage(QGEN_MSG_EXISTS_S_HKEY);
				}
				else
					_controls->ShowMessage(QGEN_MSG_EMPTYDATA);
			}
			else
				isError = false;
		} while (isError);
	}
}

void OptionsDialog::AddHotKey()
{
	HotkeyData hotKeyData;
	long index;
	bool isError = true;
	OptionsHotkeysDialog dialog(this, _("Add macro"), _controls);
	dialog.CenterOnParent();
	do
	{
		if (dialog.ShowModal() == wxID_OK)
		{
			hotKeyData = dialog.GetHotkeyData();
			if (hotKeyData.HotKeyCode && !hotKeyData.CommandText.IsEmpty())
			{
				ChkSysHotKey chkHKey;
				if (!chkHKey.CheckSystemHotKeys(_parent->GetMenuBar(), hotKeyData.HotKeyCode, hotKeyData.Flags))
				{
					if (_lstHotKeys->FindItem(-1, hotKeyData.GetKeysAsString()) == wxNOT_FOUND)
					{
						index = _lstHotKeys->GetItemCount();
						_lstHotKeys->InsertItem(index, hotKeyData.GetKeysAsString());
						_lstHotKeys->SetItem(index, 1, hotKeyData.CommandText);
						_hotkeysData.Add(hotKeyData);
						_btnApply->Enable(true);
						isError = false;
					}
					else
						_controls->ShowMessage(QGEN_MSG_EXISTS_HKEY);
				}
				else
					_controls->ShowMessage(QGEN_MSG_EXISTS_S_HKEY);
			}
			else
				_controls->ShowMessage(QGEN_MSG_EMPTYDATA);
		}
		else
			isError = false;
	} while (isError);
}

void OptionsDialog::DeleteHotKey()
{
	long index = _lstHotKeys->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (index >= 0)
	{
		_lstHotKeys->DeleteItem(index);
		_hotkeysData.RemoveAt(index);
		if (_lstHotKeys->GetItemCount() == index)
			--index;
		if (index >= 0)
		{
			_lstHotKeys->SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
			_lstHotKeys->SetFocus();
		}
		_btnApply->Enable(true);
	}
}

void OptionsDialog::UpdateLanguagesList()
{
	wxString filename;
	const wxLanguageInfo *langinfo;
	_cmbLang->Clear();
	_langTable.clear();
	_langTable[_("Default")] = wxLANGUAGE_DEFAULT;
	_cmbLang->Append(_("Default"));
	wxDir dir(_settings->GetPath() + wxT("langs"));
	if (dir.IsOpened())
	{
		for (bool cont = dir.GetFirst(&filename, wxT("*"), wxDIR_DEFAULT); cont; cont = dir.GetNext(&filename))
		{
			if (langinfo = wxLocale::FindLanguageInfo(filename))
			{
				_langTable[langinfo->Description] = langinfo->Language;
				_cmbLang->Append(langinfo->Description);
			}
		}
	}
	wxString name(wxLocale::GetLanguageName(_settings->GetLangId()));
	if (name.IsEmpty())
		name = _("Default");
	_cmbLang->SetStringSelection(name);
}
