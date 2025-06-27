// Copyright (C) 2005-2025
// Vladimir Bauer (baxzzzz AT gmail DOT com)
// Nex (nex AT otaku DOT ru)
// Shchannikov Dmitry (rrock DOT ru AT gmail DOT com)
// Valeriy Argunov (byte AT qsp DOT org)
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

#include "optionsdialog.h"

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
    EVT_BUTTON(ID_COLORS_LINE_NUMBERS, OptionsDialog::OnColorSelect)
    EVT_BUTTON(ID_COLORS_BASEFONT, OptionsDialog::OnColorSelect)
    EVT_BUTTON(ID_COLORS_TEXTBACK, OptionsDialog::OnColorSelect)
    EVT_BUTTON(ID_COLORS_ALTTEXTBACK, OptionsDialog::OnColorSelect)
    EVT_BUTTON(ID_COLORS_BASEBACK, OptionsDialog::OnColorSelect)
    EVT_BUTTON(ID_FONTS_STATEMENTS, OptionsDialog::OnFontSelect)
    EVT_BUTTON(ID_FONTS_FUNCTIONS, OptionsDialog::OnFontSelect)
    EVT_BUTTON(ID_FONTS_SYSVARIABLES, OptionsDialog::OnFontSelect)
    EVT_BUTTON(ID_FONTS_STRINGS, OptionsDialog::OnFontSelect)
    EVT_BUTTON(ID_FONTS_NUMBERS, OptionsDialog::OnFontSelect)
    EVT_BUTTON(ID_FONTS_OPERATIONSBRACKETS, OptionsDialog::OnFontSelect)
    EVT_BUTTON(ID_FONTS_LABELS, OptionsDialog::OnFontSelect)
    EVT_BUTTON(ID_FONTS_COMMENTS, OptionsDialog::OnFontSelect)
    EVT_BUTTON(ID_FONTS_LINE_NUMBERS, OptionsDialog::OnFontSelect)
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
    _txtFirstLocName = new wxTextCtrl(_general, wxID_ANY, wxEmptyString);

    _autoSaveUnits  = new wxStaticText(_general, wxID_ANY, wxEmptyString);

    wxSizer *sizerAutoSave = new wxBoxSizer(wxHORIZONTAL);
    sizerAutoSave->Add(_spnAutoSaveMin, 0, wxALIGN_CENTER_VERTICAL);
    sizerAutoSave->Add(_autoSaveUnits, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 2);

    topSizerGeneral->Add(_chkAutoSave, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerGeneral->Add(sizerAutoSave, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    topSizerGeneral->Add(_chkFirstLoc, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerGeneral->Add(_txtFirstLocName, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    topSizerGeneral->Add(_chkDescOfLoc, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerGeneral->AddStretchSpacer(0);
    topSizerGeneral->Add(_chkOpeningLoc, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerGeneral->AddStretchSpacer(0);
    topSizerGeneral->Add(_chkOpeningAct, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerGeneral->AddStretchSpacer(0);
    topSizerGeneral->Add(_chkOnLocActIcons, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerGeneral->AddStretchSpacer(0);
    topSizerGeneral->Add(_chkLocDescVisible, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerGeneral->AddStretchSpacer(0);
    topSizerGeneral->Add(_chkLocActsVisible, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerGeneral->AddStretchSpacer(0);
    topSizerGeneral->Add(_chkOpenLastGame, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerGeneral->AddStretchSpacer(0);
    topSizerGeneral->Add(_stTextCmbLang, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerGeneral->Add(_cmbLang, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);

    topSizerGeneral->AddGrowableCol(0, 1);

    _general->SetSizerAndFit(topSizerGeneral);
    _general->SetAutoLayout(true);

    _editor = new wxPanel(_notebook);
    _notebook->AddPage(_editor, wxEmptyString);

    wxFlexGridSizer *topSizerEditor = new wxFlexGridSizer(2);

    _chkWrapLines = new wxCheckBox(_editor, wxID_ANY, wxEmptyString);
    _chkShowLinesNums = new wxCheckBox(_editor, wxID_ANY, wxEmptyString);
    _chkCollapseCode = new wxCheckBox(_editor, wxID_ANY, wxEmptyString);
    _chkShowHiddenChars = new wxCheckBox(_editor, wxID_ANY, wxEmptyString);

    _stTextTabSize = new wxStaticText(_editor, wxID_ANY, wxEmptyString);
    _spnTabSize = new wxSpinCtrl(_editor, wxID_ANY, wxT("8"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 2, 8, 8);

    topSizerEditor->Add(_chkWrapLines, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerEditor->AddStretchSpacer(0);
    topSizerEditor->Add(_chkShowLinesNums, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerEditor->AddStretchSpacer(0);
    topSizerEditor->Add(_chkCollapseCode, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerEditor->AddStretchSpacer(0);
    topSizerEditor->Add(_chkShowHiddenChars, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerEditor->AddStretchSpacer(0);
    topSizerEditor->Add(_stTextTabSize, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerEditor->Add(_spnTabSize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);

    topSizerEditor->AddGrowableCol(0, 1);

    _editor->SetSizerAndFit(topSizerEditor);
    _editor->SetAutoLayout(true);

    _sizes = new wxPanel(_notebook);
    _notebook->AddPage(_sizes, wxEmptyString);

    wxFlexGridSizer *topSizerSizes = new wxFlexGridSizer(2);

    _stTextHeights = new wxStaticText(_sizes, wxID_ANY, wxEmptyString);
    _stTextWidth1 = new wxStaticText(_sizes, wxID_ANY, wxEmptyString);
    _stTextWidth2 = new wxStaticText(_sizes, wxID_ANY, wxEmptyString);

    _spnHeights = new wxSpinCtrl(_sizes, wxID_ANY, wxT("100"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 1, 100, 100);
    _spnWidth1 = new wxSpinCtrl(_sizes, wxID_ANY, wxT("100"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 1, 100, 100);
    _spnWidth2 = new wxSpinCtrl(_sizes, wxID_ANY, wxT("100"), wxDefaultPosition, wxSize(50, wxDefaultCoord), wxSP_ARROW_KEYS, 1, 100, 100);

    topSizerSizes->Add(_stTextHeights, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerSizes->Add(_spnHeights, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    topSizerSizes->Add(_stTextWidth1, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerSizes->Add(_spnWidth1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    topSizerSizes->Add(_stTextWidth2, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerSizes->Add(_spnWidth2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);

    topSizerSizes->AddGrowableCol(0, 1);

    _sizes->SetSizerAndFit(topSizerSizes);
    _sizes->SetAutoLayout(true);

    _colors = new wxPanel(_notebook);
    _notebook->AddPage(_colors, wxEmptyString);

    wxFlexGridSizer *topSizerColors = new wxFlexGridSizer(3);

    _stBaseBackColor = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
    _colorBaseBack = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
    _btnBaseBackColor = new wxButton(_colors, ID_COLORS_BASEBACK, wxEmptyString);

    topSizerColors->Add(_stBaseBackColor, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerColors->Add(_colorBaseBack, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    topSizerColors->Add(_btnBaseBackColor, 0, wxALIGN_CENTER_VERTICAL);

    _stTextBackColor = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
    _colorTextBack = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
    _btnTextBackColor = new wxButton(_colors, ID_COLORS_TEXTBACK, wxEmptyString);

    topSizerColors->Add(_stTextBackColor, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerColors->Add(_colorTextBack, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    topSizerColors->Add(_btnTextBackColor, 0, wxALIGN_CENTER_VERTICAL);

    _stAltTextBackColor = new wxStaticText(_colors, wxID_ANY, wxEmptyString);
    _colorAltTextBack = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
    _btnAltTextBackColor = new wxButton(_colors, ID_COLORS_ALTTEXTBACK, wxEmptyString);

    topSizerColors->Add(_stAltTextBackColor, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerColors->Add(_colorAltTextBack, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    topSizerColors->Add(_btnAltTextBackColor, 0, wxALIGN_CENTER_VERTICAL);

    AddSyntaxColorConfig(ID_COLORS_BASEFONT, SYNTAX_BASE, wxT("Base font color:"), topSizerColors);
    AddSyntaxColorConfig(ID_COLORS_LINE_NUMBERS, SYNTAX_LINE_NUMBERS, wxT("Line numbers color:"), topSizerColors);
    AddSyntaxColorConfig(ID_COLORS_STATEMENTS, SYNTAX_STATEMENTS, wxT("Statements color:"), topSizerColors);
    AddSyntaxColorConfig(ID_COLORS_FUNCTIONS, SYNTAX_FUNCTIONS, wxT("Functions color:"), topSizerColors);
    AddSyntaxColorConfig(ID_COLORS_SYSVARIABLES, SYNTAX_SYS_VARIABLES, wxT("System variables color:"), topSizerColors);
    AddSyntaxColorConfig(ID_COLORS_STRINGS, SYNTAX_STRINGS, wxT("Strings color:"), topSizerColors);
    AddSyntaxColorConfig(ID_COLORS_NUMBERS, SYNTAX_NUMBERS, wxT("Numbers color:"), topSizerColors);
    AddSyntaxColorConfig(ID_COLORS_OPERATIONSBRACKETS, SYNTAX_OPERATIONS, wxT("Operations color:"), topSizerColors);
    AddSyntaxColorConfig(ID_COLORS_MARKS, SYNTAX_LABELS, wxT("Labels color:"), topSizerColors);
    AddSyntaxColorConfig(ID_COLORS_COMMENTS, SYNTAX_COMMENTS, wxT("Comments color:"), topSizerColors);

    topSizerColors->AddGrowableCol(0, 1);

    _colors->SetSizerAndFit(topSizerColors);
    _colors->SetAutoLayout(true);

    _fonts = new wxPanel(_notebook);
    _notebook->AddPage(_fonts, wxEmptyString);

    wxFlexGridSizer *topSizerFonts = new wxFlexGridSizer(3);

    AddSyntaxFontConfig(ID_FONTS_BASE, SYNTAX_BASE, false, wxT("Main font:"), topSizerFonts);
    AddSyntaxFontConfig(ID_FONTS_LINE_NUMBERS, SYNTAX_LINE_NUMBERS, true, wxT("Line numbers font:"), topSizerFonts);
    AddSyntaxFontConfig(ID_FONTS_STATEMENTS, SYNTAX_STATEMENTS, false, wxT("Statements font:"), topSizerFonts);
    AddSyntaxFontConfig(ID_FONTS_FUNCTIONS, SYNTAX_FUNCTIONS, false, wxT("Functions font:"), topSizerFonts);
    AddSyntaxFontConfig(ID_FONTS_SYSVARIABLES, SYNTAX_SYS_VARIABLES, false, wxT("System variables font:"), topSizerFonts);
    AddSyntaxFontConfig(ID_FONTS_STRINGS, SYNTAX_STRINGS, false, wxT("Strings font:"), topSizerFonts);
    AddSyntaxFontConfig(ID_FONTS_NUMBERS, SYNTAX_NUMBERS, false, wxT("Numbers font:"), topSizerFonts);
    AddSyntaxFontConfig(ID_FONTS_OPERATIONSBRACKETS, SYNTAX_OPERATIONS, false, wxT("Operations font:"), topSizerFonts);
    AddSyntaxFontConfig(ID_FONTS_LABELS, SYNTAX_LABELS, false, wxT("Labels font:"), topSizerFonts);
    AddSyntaxFontConfig(ID_FONTS_COMMENTS, SYNTAX_COMMENTS, false, wxT("Comments font:"), topSizerFonts);

    topSizerFonts->AddGrowableCol(1, 1);

    _fonts->SetSizerAndFit(topSizerFonts);
    _fonts->SetAutoLayout(true);

    _paths = new wxPanel(_notebook);
    _notebook->AddPage(_paths, wxEmptyString);

    wxFlexGridSizer *topSizerPaths = new wxFlexGridSizer(3);

    _stPlayerPath = new wxStaticText(_paths, wxID_ANY, wxEmptyString);
    _txtPathPlayer = new wxTextCtrl(_paths, wxID_ANY);
    _btnPathPlayer = new wxButton(_paths, ID_PATH_PLAYER, wxEmptyString);

    _stHelpPath = new wxStaticText(_paths, wxID_ANY, wxEmptyString);
    _txtPathHelp = new wxTextCtrl(_paths, wxID_ANY);
    _btnPathHelp = new wxButton(_paths, ID_PATH_HELP, wxEmptyString);

    _stTxt2GamPath = new wxStaticText(_paths, wxID_ANY, wxEmptyString);
    _txtPathTxt2Gam = new wxTextCtrl(_paths, wxID_ANY);
    _btnPathTxt2Gam = new wxButton(_paths, ID_PATH_TXT2GAM, wxEmptyString);

    topSizerPaths->Add(_stPlayerPath, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    topSizerPaths->Add(_txtPathPlayer, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 2);
    topSizerPaths->Add(_btnPathPlayer, 0, wxALIGN_CENTER_VERTICAL);
    topSizerPaths->Add(_stHelpPath, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    topSizerPaths->Add(_txtPathHelp, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 2);
    topSizerPaths->Add(_btnPathHelp, 0, wxALIGN_CENTER_VERTICAL);
    topSizerPaths->Add(_stTxt2GamPath, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    topSizerPaths->Add(_txtPathTxt2Gam, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 2);
    topSizerPaths->Add(_btnPathTxt2Gam, 0, wxALIGN_CENTER_VERTICAL);

    topSizerPaths->AddGrowableCol(1, 1);

    _paths->SetSizerAndFit(topSizerPaths);
    _paths->SetAutoLayout(true);

#ifdef __WXMSW__
    _hotkeys = new wxPanel(_notebook);
    _notebook->AddPage(_hotkeys, wxEmptyString);

    wxBoxSizer *topSizerHotKeys = new wxBoxSizer(wxVERTICAL);

    _stHotKeys = new wxStaticText(_hotkeys, wxID_ANY, wxEmptyString);
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

    topSizerHotKeys->Add(_stHotKeys, 0, wxALL, 5);
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

    SetMinClientSize(wxSize(500, 550));

    InitOptionsDialog();
    _btnOK->SetDefault();
    ReCreateGUI();
}

OptionsDialog::~OptionsDialog()
{
    _langTable.clear();
    _fontConfigs.clear();
    _colorConfigs.clear();
}

void OptionsDialog::AddSyntaxFontConfig(int componentId, SyntaxType syntaxType, bool useAltBackground, const wxString& descriptionKey, wxFlexGridSizer *topSizerFonts)
{
    FontConfig config;

    config.ComponentId = componentId;
    config.Type = syntaxType;
    config.UseAltBackground = useAltBackground;
    config.DescriptionKey = descriptionKey;
    config.Description = new wxStaticText(_fonts, wxID_ANY, wxGetTranslation(descriptionKey));
    config.TextSample = new wxTextCtrl(_fonts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTE_READONLY);
    config.SelectButton = new wxButton(_fonts, componentId, wxEmptyString);

    topSizerFonts->Add(config.Description, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    topSizerFonts->Add(config.TextSample, 1, wxALL|wxGROW, 2);
    topSizerFonts->Add(config.SelectButton, 0, wxALIGN_CENTER_VERTICAL);

    _fontConfigs[componentId] = config;
}

void OptionsDialog::UpdateSyntaxFontConfigUi()
{
    for (FontConfigTable::iterator it = _fontConfigs.begin(); it != _fontConfigs.end(); ++it)
    {
        FontConfig& config = it->second;
        config.Description->SetLabel(wxGetTranslation(config.DescriptionKey));
        config.SelectButton->SetLabel(_("Select font..."));
    }
}

void OptionsDialog::SaveSyntaxFontSettings()
{
    for (FontConfigTable::iterator it = _fontConfigs.begin(); it != _fontConfigs.end(); ++it)
    {
        FontConfig& config = it->second;
        _settings->SetFont(config.Type, config.TextSample->GetFont());
    }
}

void OptionsDialog::ApplySyntaxFontSettings()
{
    wxColour textBackColour = _settings->GetTextBackColour();
    wxColour altTextBackColour = _settings->GetAltTextBackColour();

    for (FontConfigTable::iterator it = _fontConfigs.begin(); it != _fontConfigs.end(); ++it)
    {
        FontConfig& config = it->second;
        wxTextCtrl * textSample = config.TextSample;

        textSample->SetValue(_settings->GetFont(config.Type).GetFaceName());
        textSample->SetFont(_settings->GetFont(config.Type));
        textSample->SetForegroundColour(_settings->GetColour(config.Type));
        if (config.UseAltBackground)
            textSample->SetBackgroundColour(altTextBackColour);
        else
            textSample->SetBackgroundColour(textBackColour);
    }
}

void OptionsDialog::AddSyntaxColorConfig(int componentId, SyntaxType syntaxType, const wxString& descriptionKey, wxFlexGridSizer *topSizerColors)
{
    ColorConfig config;

    config.ComponentId = componentId;
    config.Type = syntaxType;
    config.DescriptionKey = descriptionKey;
    config.Description = new wxStaticText(_colors, wxID_ANY, wxGetTranslation(descriptionKey));
    config.ColorSample = new wxWindow(_colors, wxID_ANY, wxDefaultPosition, wxSize(50, 25));
    config.SelectButton = new wxButton(_colors, componentId, wxEmptyString);

    topSizerColors->Add(config.Description, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxGROW, 5);
    topSizerColors->Add(config.ColorSample, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
    topSizerColors->Add(config.SelectButton, 0, wxALIGN_CENTER_VERTICAL);

    _colorConfigs[componentId] = config;
}

void OptionsDialog::UpdateSyntaxColorConfigUi()
{
    for (ColorConfigTable::iterator it = _colorConfigs.begin(); it != _colorConfigs.end(); ++it)
    {
        ColorConfig& config = it->second;
        config.Description->SetLabel(wxGetTranslation(config.DescriptionKey));
        config.SelectButton->SetLabel(_("Select color..."));
    }
}

void OptionsDialog::SaveSyntaxColorSettings()
{
    for (ColorConfigTable::iterator it = _colorConfigs.begin(); it != _colorConfigs.end(); ++it)
    {
        ColorConfig& config = it->second;
        _settings->SetColour(config.Type, config.ColorSample->GetBackgroundColour());
    }
}

void OptionsDialog::ApplySyntaxColorSettings()
{
    for (ColorConfigTable::iterator it = _colorConfigs.begin(); it != _colorConfigs.end(); ++it)
    {
        ColorConfig& config = it->second;
        config.ColorSample->SetBackgroundColour(_settings->GetColour(config.Type));
    }
}

void OptionsDialog::UpdateFontColor(SyntaxType syntaxType, const wxColour& color)
{
    for (FontConfigTable::iterator it = _fontConfigs.begin(); it != _fontConfigs.end(); ++it)
    {
        FontConfig& config = it->second;
        if (config.Type == syntaxType)
            config.TextSample->SetForegroundColour(color);
    }
}

void OptionsDialog::UpdateFontBackgroundColor(const wxColour& color, const wxColour& altColor)
{
    for (FontConfigTable::iterator it = _fontConfigs.begin(); it != _fontConfigs.end(); ++it)
    {
        FontConfig& config = it->second;
        if (config.UseAltBackground)
            config.TextSample->SetBackgroundColour(altColor);
        else
            config.TextSample->SetBackgroundColour(color);
    }
}

void OptionsDialog::ReCreateGUI()
{
    // Main button
    _btnOK->SetLabel(_("OK"));
    _btnCancel->SetLabel(_("Cancel"));
    _btnApply->SetLabel(_("Apply"));
    _btnReset->SetLabel(_("Reset"));
    // Page General
    _notebook->SetPageText(0, _("General"));
    _chkAutoSave->SetLabel(_("Auto save every"));
    _chkFirstLoc->SetLabel(_("Auto create first location:"));
    _chkDescOfLoc->SetLabel(_("Show short location descriptions"));
    _chkOpeningLoc->SetLabel(_("Open a location after creation"));
    _chkOpeningAct->SetLabel(_("Open an action after creation"));
    _chkOnLocActIcons->SetLabel(_("Show location icons"));
    _chkLocDescVisible->SetLabel(_("Show base description on location tab"));
    _chkLocActsVisible->SetLabel(_("Show base actions on location tab"));
    _chkOpenLastGame->SetLabel(_("Remember game file on exit"));
    _stTextCmbLang->SetLabel(_("UI language"));
    _autoSaveUnits->SetLabel(_("minutes"));
    // Page Editor
    _notebook->SetPageText(1, _("Code editor"));
    _chkWrapLines->SetLabel(_("Wrap lines by words"));
    _chkShowLinesNums->SetLabel(_("Show line numbers"));
    _chkCollapseCode->SetLabel(_("Collapse blocks of code when location gets opened"));
    _chkShowHiddenChars->SetLabel(_("Show hidden characters"));
    // Page Sizes
    _notebook->SetPageText(2, _("Sizes"));
    _stTextHeights->SetLabel(_("Relative height of description and location\ncode fields to the tab height (%):"));
    _stTextWidth1->SetLabel(_("Relative width of location description field\nto the tab width (%):"));
    _stTextWidth2->SetLabel(_("Relative width of actions list to the tab width (%):"));
    _stTextTabSize->SetLabel(_("Size of TAB:"));
    // Page Colors
    _notebook->SetPageText(3, _("Colors"));
    _stTextBackColor->SetLabel(_("Background color of location tabs:"));
    _btnTextBackColor->SetLabel(_("Select color..."));
    _stAltTextBackColor->SetLabel(_("Background color of line numbers:"));
    _btnAltTextBackColor->SetLabel(_("Select color..."));
    _stBaseBackColor->SetLabel(_("Main background color:"));
    _btnBaseBackColor->SetLabel(_("Select color..."));
    UpdateSyntaxColorConfigUi();
    // Page Fonts
    _notebook->SetPageText(4, _("Fonts"));
    UpdateSyntaxFontConfigUi();
    // Page Paths
    _notebook->SetPageText(5, _("Paths"));
    _stPlayerPath->SetLabel(_("Path to player:"));
    _stHelpPath->SetLabel(_("Path to help:"));
    _stTxt2GamPath->SetLabel(_("Path to TXT2GAM:"));
    _btnPathPlayer->SetLabel(_("Select path..."));
    _btnPathHelp->SetLabel(_("Select path..."));
    _btnPathTxt2Gam->SetLabel(_("Select path..."));
#ifdef __WXMSW__
    //Page HotKeys
    _notebook->SetPageText(6,  _("Hotkeys"));
    _stHotKeys->SetLabel(_("Hotkeys list:"));
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
    for (size_t i = 0; i < _notebook->GetPageCount(); ++i)
    {
        wxSizer *pageSizer = _notebook->GetPage(i)->GetSizer();
        if (pageSizer) pageSizer->Layout();
    }
    GetSizer()->Layout();
}

void OptionsDialog::OnColorSelect(wxCommandEvent &event)
{
    switch (event.GetId())
    {
    case ID_COLORS_TEXTBACK:
        {
            wxColourData data;
            data.SetColour(_colorTextBack->GetBackgroundColour());
            wxColourDialog dialog(this, &data);
            if (dialog.ShowModal() == wxID_OK)
            {
                wxColour color = dialog.GetColourData().GetColour();
                UpdateFontBackgroundColor(color, _colorAltTextBack->GetBackgroundColour());
                _colorTextBack->SetBackgroundColour(color);
                _colorTextBack->Refresh();
                _btnApply->Enable();
            }
            break;
        }
    case ID_COLORS_ALTTEXTBACK:
        {
            wxColourData data;
            data.SetColour(_colorAltTextBack->GetBackgroundColour());
            wxColourDialog dialog(this, &data);
            if (dialog.ShowModal() == wxID_OK)
            {
                wxColour color = dialog.GetColourData().GetColour();
                UpdateFontBackgroundColor(_colorTextBack->GetBackgroundColour(), color);
                _colorAltTextBack->SetBackgroundColour(color);
                _colorAltTextBack->Refresh();
                _btnApply->Enable();
            }
            break;
        }
    case ID_COLORS_BASEBACK:
        {
            wxColourData data;
            data.SetColour(_colorBaseBack->GetBackgroundColour());
            wxColourDialog dialog(this, &data);
            if (dialog.ShowModal() == wxID_OK)
            {
                wxColour color = dialog.GetColourData().GetColour();
                _colorBaseBack->SetBackgroundColour(color);
                _colorBaseBack->Refresh();
                _btnApply->Enable();
            }
            break;
        }
    default:
        {
            wxColourData data;
            ColorConfig& config = _colorConfigs[event.GetId()];
            wxWindow * colorSample = config.ColorSample;

            data.SetColour(colorSample->GetBackgroundColour());
            wxColourDialog dialog(this, &data);
            if (dialog.ShowModal() == wxID_OK)
            {
                wxColour color = dialog.GetColourData().GetColour();
                colorSample->SetBackgroundColour(color);
                colorSample->Refresh();
                UpdateFontColor(config.Type, color);
                _btnApply->Enable();
            }
            break;
        }
    }
}

void OptionsDialog::OnFontSelect(wxCommandEvent &event)
{
    wxFontData data;
    FontConfig& config = _fontConfigs[event.GetId()];
    wxTextCtrl * textSample = config.TextSample;

    data.SetInitialFont(textSample->GetFont());
    data.EnableEffects(false);
    wxFontDialog dialog(this, data);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxFont font = dialog.GetFontData().GetChosenFont();
        textSample->SetValue(font.GetFaceName());
        textSample->SetFont(font);
        textSample->Refresh();
        _btnApply->Enable();
    }
}

void OptionsDialog::OnPathSelect( wxCommandEvent &event)
{
    wxFileDialog dialog(this);
    switch (event.GetId())
    {
    case ID_PATH_PLAYER:
        dialog.Create(this, _("Select player file"), wxEmptyString, wxEmptyString,
            _("Player file (*.exe)|*.exe|All files (*.*)|*"), wxFD_OPEN);
        if (dialog.ShowModal() == wxID_OK)
        {
            _txtPathPlayer->SetValue(dialog.GetPath());
            _btnApply->Enable();
        }
        break;
    case ID_PATH_HELP:
        dialog.Create(this, _("Select help file"), wxEmptyString, wxEmptyString,
            _("Help file (*.chm)|*.chm|All files (*.*)|*"), wxFD_OPEN);
        if (dialog.ShowModal() == wxID_OK)
        {
            _txtPathHelp->SetValue(dialog.GetPath());
            _btnApply->Enable();
        }
        break;
    case ID_PATH_TXT2GAM:
        dialog.Create(this, _("Select converter file"), wxEmptyString, wxEmptyString,
            _("Converter file (*.exe)|*.exe|All files (*.*)|*"), wxFD_OPEN);
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
    _settings->PostInitLocaleSettings();
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
        _txtFirstLocName->Enable(event.IsChecked());
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
    int lang = _langTable[_cmbLang->GetStringSelection()];
    _controls->UpdateLocale(lang);
    _settings->SetIdLang(lang);

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
    _settings->SetShowLocsIcons(_chkOnLocActIcons->GetValue());
    _settings->SetCollapseCode(_chkCollapseCode->GetValue());
    _settings->SetShowHiddenChars(_chkShowHiddenChars->GetValue());
    _settings->SetHeightsCoeff((double)_spnHeights->GetValue() / 100);
    _settings->SetWidthsCoeff1((double)_spnWidth1->GetValue() / 100);
    _settings->SetWidthsCoeff2((double)_spnWidth2->GetValue() / 100);
    _settings->SetTabSize(_spnTabSize->GetValue());
    _settings->SetCurrentHelpPath(_txtPathHelp->GetValue());
    _settings->SetCurrentPlayerPath(_txtPathPlayer->GetValue());
    _settings->SetCurrentTxt2GamPath(_txtPathTxt2Gam->GetValue());

    _settings->SetTextBackColour(_colorTextBack->GetBackgroundColour());
    _settings->SetAltTextBackColour(_colorAltTextBack->GetBackgroundColour());
    _settings->SetBaseBackColour(_colorBaseBack->GetBackgroundColour());

    SaveSyntaxColorSettings();
    SaveSyntaxFontSettings();

#ifdef __WXMSW__
    HotkeyData hotKeyData;
    size_t count = _lstHotKeys->GetItemCount();
    HotkeysStore *hotKeysStore = _settings->GetHotKeys();
    hotKeysStore->ClearHotkeysData();
    for (size_t i = 0; i < count; ++i)
        hotKeysStore->AddHotkeyData(_hotkeysData[i]);
#endif

    if (_chkFirstLoc->GetValue())
        _settings->SetFirstLocName(_txtFirstLocName->GetValue());
    else
        _settings->SetFirstLocName(wxEmptyString);

    _settings->PostInitLocaleSettings();
    _txtFirstLocName->SetValue(_settings->GetFirstLocName());

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
    _txtFirstLocName->SetValue(_settings->GetFirstLocName());
    _chkOnLocActIcons->SetValue(_settings->GetShowLocsIcons());
    _chkCollapseCode->SetValue(_settings->GetCollapseCode());
    _chkShowHiddenChars->SetValue(_settings->GetShowHiddenChars());

    _spnHeights->SetValue(_settings->GetHeightsCoeff() * 100);
    _spnWidth1->SetValue(_settings->GetWidthsCoeff1() * 100);
    _spnWidth2->SetValue(_settings->GetWidthsCoeff2() * 100);
    _spnTabSize->SetValue(_settings->GetTabSize());

    _txtPathHelp->SetValue(_settings->GetCurrentHelpPath());
    _txtPathPlayer->SetValue(_settings->GetCurrentPlayerPath());
    _txtPathTxt2Gam->SetValue(_settings->GetCurrentTxt2GamPath());

    _colorTextBack->SetBackgroundColour(_settings->GetTextBackColour());
    _colorAltTextBack->SetBackgroundColour(_settings->GetAltTextBackColour());
    _colorBaseBack->SetBackgroundColour(_settings->GetBaseBackColour());

    ApplySyntaxColorSettings();
    ApplySyntaxFontSettings();

    _txtFirstLocName->Enable(_settings->GetCreateFirstLoc());
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
    if (index >= 0)
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

    wxDir dir(Utils::GetResourcePath(QGEN_TRANSLATIONS));
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
