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

OptionsDialog::OptionsDialog(wxFrame *parent, const wxString &title, Controls *controls, int style) :
	wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, style)
{
	_settings = controls->GetSettings();
	_controls = controls;
	_parent = parent;

	wxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	wxSizer *notebookSizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer *btnSizer = new wxBoxSizer(wxHORIZONTAL);

	_btnOK = new wxButton(this, ID_OK_SETTINGS, wxT("OK"));
	wxButton *btnCancel = new wxButton(this, wxID_CANCEL, wxT("Отмена"));
	_btnApply = new wxButton(this, ID_APPLY_SETTINGS, wxT("Применить"));
	_btnReset = new wxButton(this, ID_RESET_SETTINGS, wxT("Сброс"));

	btnSizer->Add(_btnReset, 0, wxLEFT|wxBOTTOM, 5);
	btnSizer->AddStretchSpacer(1);
	btnSizer->Add(_btnOK, 0, wxRIGHT|wxBOTTOM, 5);
	btnSizer->Add(btnCancel, 0, wxRIGHT|wxBOTTOM, 5);
	btnSizer->Add(_btnApply, 0, wxRIGHT|wxBOTTOM, 5);

	_notebook = new wxNotebook(this, wxID_ANY);

	_general = new wxPanel(_notebook);
	_notebook->AddPage(_general, wxT("Основные"));

	wxFlexGridSizer *topSizerGeneral = new wxFlexGridSizer(2);
	wxSizer *sizerAutoSave = new wxBoxSizer(wxHORIZONTAL);

	_chkAutoSave = new wxCheckBox(_general, ID_AUTO_SAVE, wxT("Автосохранение каждые"));
	_chkFirstLoc = new wxCheckBox(_general, ID_FIRST_LOC, wxT("Название начальной локации:"));
	_chkDescOfLoc = new wxCheckBox(_general, wxID_ANY, wxT("Показывать краткие описания локаций"));
	_chkOpeningLoc = new wxCheckBox(_general, wxID_ANY, wxT("Открытие локации при создании"));
	_chkOpeningAct = new wxCheckBox(_general, wxID_ANY, wxT("Открытие действия при создании"));
	_chkOnLocActIcons = new wxCheckBox(_general, wxID_ANY, wxT("Отображать иконки в списке локаций"));
	_chkLocDescVisible = new wxCheckBox(_general, wxID_ANY, wxT("Показывать базовые описания локаций"));
	_chkLocActsVisible = new wxCheckBox(_general, wxID_ANY, wxT("Показывать базовые действия локаций"));
	_chkOpenLastGame = new wxCheckBox(_general, wxID_ANY, wxT("Запоминать игру при выходе"));

	_spnAutoSaveMin = new wxSpinCtrl(_general, wxID_ANY, wxT("5"), wxDefaultPosition, wxSize(45, wxDefaultCoord),
										wxSP_ARROW_KEYS, 1, 60, 5);
	_txtNameFirsLoc = new wxTextCtrl(_general, wxID_ANY, wxT("Начало"));

	wxStaticText *autoSaveUnits  = new wxStaticText(_general, wxID_ANY, wxT("минут"));

	sizerAutoSave->Add(_spnAutoSaveMin);
	sizerAutoSave->Add(autoSaveUnits, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 2);

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
	topSizerGeneral->AddGrowableCol(1, 0);

	_general->SetSizerAndFit(topSizerGeneral);
	_general->SetAutoLayout(true);

	_editor = new wxPanel(_notebook);
	_notebook->AddPage(_editor, wxT("Редактор кода"));

	wxFlexGridSizer *topSizerEditor = new wxFlexGridSizer(2);

	_chkWrapLines = new wxCheckBox(_editor, wxID_ANY, wxT("Перенос строк по словам в коде локаций и действий"));
	_chkShowLinesNums = new wxCheckBox(_editor, wxID_ANY, wxT("Показывать номера строк"));
	_chkCollapseCode = new wxCheckBox(_editor, wxID_ANY, wxT("Сворачивать блоки кода при открытии"));

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
	_notebook->AddPage(_sizes, wxT("Размеры"));

	wxFlexGridSizer *topSizerSizes = new wxFlexGridSizer(2);

	wxStaticText *stTextHeights = new wxStaticText(_sizes, wxID_ANY, wxT("Относительная высота полей описания и кода\nлокации к высоте вкладки (%):"));
	wxStaticText *stTextWidth1 = new wxStaticText(_sizes, wxID_ANY, wxT("Относительная ширина поля описания локации к ширине\nвкладки (%):"));
	wxStaticText *stTextWidth2 = new wxStaticText(_sizes, wxID_ANY, wxT("Относительная ширина списка действий к ширине вкладки (%):"));
	wxStaticText *stTextTabSize = new wxStaticText(_sizes, wxID_ANY, wxT("Размер TAB:"));
	_spnHeights = new wxSpinCtrl(_sizes, wxID_ANY, wxT("100"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 1, 100, 100);
	_spnWidth1 = new wxSpinCtrl(_sizes, wxID_ANY, wxT("100"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 1, 100, 100);
	_spnWidth2 = new wxSpinCtrl(_sizes, wxID_ANY, wxT("100"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 1, 100, 100);
	_spnTabSize = new wxSpinCtrl(_sizes, wxID_ANY, wxT("8"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 2, 8, 8);

	topSizerSizes->Add(stTextHeights, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerSizes->Add(_spnHeights, 0, wxALL, 5);
	topSizerSizes->Add(stTextWidth1, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerSizes->Add(_spnWidth1, 0, wxALL, 5);
	topSizerSizes->Add(stTextWidth2, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerSizes->Add(_spnWidth2, 0, wxALL, 5);
	topSizerSizes->Add(stTextTabSize, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerSizes->Add(_spnTabSize, 0, wxALL, 5);

	topSizerSizes->AddGrowableCol(0, 1);

	_sizes->SetSizerAndFit(topSizerSizes);
	_sizes->SetAutoLayout(true);

	_colors = new wxPanel(_notebook);
	_notebook->AddPage(_colors, wxT("Цвета"));

	wxFlexGridSizer *topSizerColors = new wxFlexGridSizer(3);

	wxStaticText *stText1 = new wxStaticText(_colors, wxID_ANY, wxT("Цвет операторов:"));
	wxStaticText *stText2 = new wxStaticText(_colors, wxID_ANY, wxT("Цвет функций:"));
	wxStaticText *stText3 = new wxStaticText(_colors, wxID_ANY, wxT("Цвет системных переменных:"));
	wxStaticText *stText4 = new wxStaticText(_colors, wxID_ANY, wxT("Цвет строк:"));
	wxStaticText *stText5 = new wxStaticText(_colors, wxID_ANY, wxT("Цвет чисел:"));
	wxStaticText *stText6 = new wxStaticText(_colors, wxID_ANY, wxT("Цвет операций:"));
	wxStaticText *stText7 = new wxStaticText(_colors, wxID_ANY, wxT("Цвет меток:"));
	wxStaticText *stText8 = new wxStaticText(_colors, wxID_ANY, wxT("Цвет комментариев:"));
	wxStaticText *stText9 = new wxStaticText(_colors, wxID_ANY, wxT("Цвет основного шрифта:"));
	wxStaticText *stText10 = new wxStaticText(_colors, wxID_ANY, wxT("Цвет фона вкладок локаций:"));
	wxStaticText *stText11 = new wxStaticText(_colors, wxID_ANY, wxT("Цвет основного фона:"));

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

	_btnClrsStatements = new wxButton(_colors, ID_COLORS_STATEMENTS, wxT("Выбрать цвет..."));
	_btnClrsFunctions = new wxButton(_colors, ID_COLORS_FUNCTIONS, wxT("Выбрать цвет..."));
	_btnClrsSysVariables = new wxButton(_colors, ID_COLORS_SYSVARIABLES, wxT("Выбрать цвет..."));
	_btnClrsStrings = new wxButton(_colors, ID_COLORS_STRINGS, wxT("Выбрать цвет..."));
	_btnClrsNumbers = new wxButton(_colors, ID_COLORS_NUMBERS, wxT("Выбрать цвет..."));
	_btnClrsOptsBrts = new wxButton(_colors, ID_COLORS_OPERATIONSBRACKETS, wxT("Выбрать цвет..."));
	_btnClrsMarks = new wxButton(_colors, ID_COLORS_MARKS, wxT("Выбрать цвет..."));
	_btnClrsComments = new wxButton(_colors, ID_COLORS_COMMENTS, wxT("Выбрать цвет..."));
	_btnClrsBaseFont = new wxButton(_colors, ID_COLORS_BASEFONT, wxT("Выбрать цвет..."));
	_btnClrsTextBack = new wxButton(_colors, ID_COLORS_TEXTBACK, wxT("Выбрать цвет..."));
	_btnClrsBaseBack = new wxButton(_colors, ID_COLORS_BASEBACK, wxT("Выбрать цвет..."));

	topSizerColors->Add(stText11, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorBaseBack, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsBaseBack, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(stText10, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorTextBack, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsTextBack, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(stText9, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorBaseFont, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsBaseFont, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(stText1, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorStatements, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsStatements, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(stText2, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorFunctions, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsFunctions, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(stText3, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorSysVariables, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsSysVariables, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(stText4, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorStrings, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsStrings, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(stText5, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorNumbers, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsNumbers, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(stText6, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorOptsBrts, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsOptsBrts, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(stText7, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorMarks, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsMarks, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->Add(stText8, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerColors->Add(_colorComments, 0, wxALL|wxALIGN_RIGHT, 2);
	topSizerColors->Add(_btnClrsComments, 0, wxALIGN_CENTER_VERTICAL);
	topSizerColors->AddGrowableCol(0, 1);

	_colors->SetSizerAndFit(topSizerColors);
	_colors->SetAutoLayout(true);

	_fonts = new wxPanel(_notebook);
	_notebook->AddPage(_fonts, wxT("Шрифты"));

	wxFlexGridSizer *topSizerFonts = new wxFlexGridSizer(3);

	wxStaticText *stText01 = new wxStaticText(_fonts, wxID_ANY, wxT("Шрифт операторов:"));
	wxStaticText *stText02 = new wxStaticText(_fonts, wxID_ANY, wxT("Шрифт функций:"));
	wxStaticText *stText03 = new wxStaticText(_fonts, wxID_ANY, wxT("Шрифт системных переменных:"));
	wxStaticText *stText04 = new wxStaticText(_fonts, wxID_ANY, wxT("Шрифт строк:"));
	wxStaticText *stText05 = new wxStaticText(_fonts, wxID_ANY, wxT("Шрифт чисел:"));
	wxStaticText *stText06 = new wxStaticText(_fonts, wxID_ANY, wxT("Шрифт операций:"));
	wxStaticText *stText07 = new wxStaticText(_fonts, wxID_ANY, wxT("Шрифт меток:"));
	wxStaticText *stText08 = new wxStaticText(_fonts, wxID_ANY, wxT("Шрифт комментариев:"));
	wxStaticText *stText09 = new wxStaticText(_fonts, wxID_ANY, wxT("Основной шрифт:"));

	_txtFontStatements = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontFunctions = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontSysVariables = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontStrings = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontNumbers = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontOptsBrts = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontMarks = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontComments = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
	_txtFontBase = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);

	_btnFontsStatements = new wxButton(_fonts, ID_FONTS_STATEMENTS, wxT("Выбрать шрифт..."));
	_btnFontsFunctions = new wxButton(_fonts, ID_FONTS_FUNCTIONS, wxT("Выбрать шрифт..."));
	_btnFontsSysVariables = new wxButton(_fonts, ID_FONTS_SYSVARIABLES, wxT("Выбрать шрифт..."));
	_btnFontsStrings = new wxButton(_fonts, ID_FONTS_STRINGS, wxT("Выбрать шрифт..."));
	_btnFontsNumbers = new wxButton(_fonts, ID_FONTS_NUMBERS, wxT("Выбрать шрифт..."));
	_btnFontsOptsBrts = new wxButton(_fonts, ID_FONTS_OPERATIONSBRACKETS, wxT("Выбрать шрифт..."));
	_btnFontsMarks = new wxButton(_fonts, ID_FONTS_MARKS, wxT("Выбрать шрифт..."));
	_btnFontsComments = new wxButton(_fonts, ID_FONTS_COMMENTS, wxT("Выбрать шрифт..."));
	_btnFontsBase = new wxButton(_fonts, ID_FONTS_BASE, wxT("Выбрать шрифт..."));

	topSizerFonts->Add(stText09, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontBase, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsBase, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(stText01, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontStatements, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsStatements, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(stText02, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontFunctions, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsFunctions, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(stText03, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontSysVariables, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsSysVariables, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(stText04, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontStrings, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsStrings, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(stText05, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontNumbers, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsNumbers, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(stText06, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontOptsBrts, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsOptsBrts, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(stText07, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontMarks, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsMarks, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->Add(stText08, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerFonts->Add(_txtFontComments, 0, wxGROW|wxALL, 2);
	topSizerFonts->Add(_btnFontsComments, 0, wxALIGN_CENTER_VERTICAL);
	topSizerFonts->AddGrowableCol(1, 0);

	_fonts->SetSizerAndFit(topSizerFonts);
	_fonts->SetAutoLayout(true);

	_paths = new wxPanel(_notebook);
	_notebook->AddPage(_paths, wxT("Пути"));

	wxFlexGridSizer *topSizerPaths = new wxFlexGridSizer(3);

	wxStaticText *stText001 = new wxStaticText(_paths, wxID_ANY, wxT("Путь к плееру:"));
	wxStaticText *stText002 = new wxStaticText(_paths, wxID_ANY, wxT("Путь к справке:"));
	wxStaticText *stText003 = new wxStaticText(_paths, wxID_ANY, wxT("Путь к TXT2GAM:"));

	_txtPathPlayer = new wxTextCtrl(_paths, wxID_ANY);
	_txtPathHelp = new wxTextCtrl(_paths, wxID_ANY);
	_txtPathTxt2Gam = new wxTextCtrl(_paths, wxID_ANY);

	_btnPathPlayer = new wxButton(_paths, ID_PATH_PLAYER, wxT("Выбрать путь..."));
	_btnPathHelp = new wxButton(_paths, ID_PATH_HELP, wxT("Выбрать путь..."));
	_btnPathTxt2Gam = new wxButton(_paths, ID_PATH_TXT2GAM, wxT("Выбрать путь..."));

	topSizerPaths->Add(stText001, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerPaths->Add(_txtPathPlayer, 0, wxALL|wxGROW, 2);
	topSizerPaths->Add(_btnPathPlayer, 0, wxALIGN_CENTER_VERTICAL);
	topSizerPaths->Add(stText002, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerPaths->Add(_txtPathHelp, 0, wxALL|wxGROW, 2);
	topSizerPaths->Add(_btnPathHelp, 0, wxALIGN_CENTER_VERTICAL);
	topSizerPaths->Add(stText003, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5);
	topSizerPaths->Add(_txtPathTxt2Gam, 0, wxALL|wxGROW, 2);
	topSizerPaths->Add(_btnPathTxt2Gam, 0, wxALIGN_CENTER_VERTICAL);
	topSizerPaths->AddGrowableCol(1, 0);

	_paths->SetSizerAndFit(topSizerPaths);
	_paths->SetAutoLayout(true);

	#ifdef __WXMSW__
		_hotkeys = new wxPanel(_notebook);
		_notebook->AddPage(_hotkeys, wxT("Комбинации клавиш"));

		wxBoxSizer *topSizerHotKeys = new wxBoxSizer(wxVERTICAL);

		wxStaticText *stText0001 = new wxStaticText(_hotkeys, wxID_ANY, wxT("Список команд:"));
		_lstHotKeys = new wxListCtrl(_hotkeys, ID_LIST_HKEYS, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL);
		_lstHotKeys->InsertColumn(0, wxT("Комбинация клавиш"), wxLIST_FORMAT_LEFT, 150);
		_lstHotKeys->InsertColumn(1, wxT("Исполняемая команда"), wxLIST_FORMAT_LEFT, 300);

		wxBoxSizer *btnHotkKeysSizer = new wxBoxSizer(wxHORIZONTAL);
		_btnAddNewHotKey = new wxButton(_hotkeys, ID_ADD_NEWHKEY, wxT("Добавить..."));
		_btnEditHotKey = new wxButton(_hotkeys, ID_EDIT_HKEY, wxT("Редактировать..."));
		_btnDelHotKey = new wxButton(_hotkeys, ID_DELETE_HKEY, wxT("Удалить"));

		btnHotkKeysSizer->Add(_btnAddNewHotKey, wxALL, 5);
		btnHotkKeysSizer->Add(_btnEditHotKey, wxALL, 5);
		btnHotkKeysSizer->Add(_btnDelHotKey, wxALL, 5);

		topSizerHotKeys->Add(stText0001, 0, wxTOP|wxLEFT, 5);
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
		dialog.Create(this, wxT("Открыть файл"), wxEmptyString, wxEmptyString,
			wxT("Файл плеера (*.exe)|*.exe|Все файлы (*.*)|*.*"), wxFD_OPEN);
		if (dialog.ShowModal() == wxID_OK)
		{
			_txtPathPlayer->SetValue(dialog.GetPath());
			_btnApply->Enable();
		}
		break;
	case ID_PATH_HELP:
		dialog.Create(this, wxT("Открыть файл"), wxEmptyString, wxEmptyString,
			wxT("Файл справки (*.chm)|*.chm|Все файлы (*.*)|*.*"), wxFD_OPEN);
		if (dialog.ShowModal() == wxID_OK)
		{
			_txtPathHelp->SetValue(dialog.GetPath());
			_btnApply->Enable();
		}
		break;
	case ID_PATH_TXT2GAM:
		dialog.Create(this, wxT("Открыть файл"), wxEmptyString, wxEmptyString,
			wxT("Файл конвертора (*.exe)|*.exe|Все файлы (*.*)|*.*"), wxFD_OPEN);
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
}

void OptionsDialog::OnResetSettings(wxCommandEvent &event)
{
	_settings->InitSettings();
	InitOptionsDialog();
	_settings->NotifyAll();
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
		_settings->NotifyAll();
	#endif

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
		OptionsHotkeysDialog dialog(this, wxT("Редактирование команды"), _controls);
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
	OptionsHotkeysDialog dialog(this, wxT("Добавление команды"), _controls);
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
