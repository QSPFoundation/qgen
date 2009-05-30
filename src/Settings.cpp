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

#include "Settings.h"

Settings::Settings(const wxString &path)
{
	_path = path;
	InitSettings();
	LoadSettings();
}

void Settings::InitSettings()
{
	#ifdef __WXMSW__
		_currentConfigPath = _path;
	#else
		_currentConfigPath = wxGetHomeDir();
	#endif
	_currentConfigPath = wxFileName(_currentConfigPath, wxT("qgen.cfg")).GetFullPath();
	_currentPlayerPath = wxFileName(_path, wxT("qspgui.exe")).GetFullPath();
	_currentHelpPath = wxFileName(_path, wxT("qsp.chm")).GetFullPath();
	_currentTxt2GamPath = wxFileName(_path, wxT("txt2gam.exe")).GetFullPath();
	_leftFramePos = 10;
	_topFramePos = 10;
	_frameWidth = 700;
	_frameHeight = 550;
	_optionsDialogWidth = 420;
	_optionsDialogHeight = 320;
	_isFrameMaximized = false;
	_isShowStatusBar = true;

	_panelsPos = wxT("layout2|")\
		wxT("name=Properties Location;state=768;dir=5;layer=0;row=0;pos=0;prop=100000|")\
		wxT("name=Standard Toolbar;caption=Панель инструментов;state=2106096;dir=1;layer=10;row=0;pos=0;prop=100000;bestw=540;besth=34|")\
		wxT("name=Locations;caption=Локации;state=31459324;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=166|")\
		wxT("dock_size(5,0,0)=186|dock_size(4,1,0)=168|dock_size(1,10,0)=28|");

	_isAutoSave = false;
	_autoSaveInterval = 5;
	_isShowShortLocsDescs = false;
	_isLocDescVisible = true;
	_isLocActsVisible = true;
	_isWrapLines = false;
	_isOpenNewLoc = true;
	_isOpenNewAct = true;
	_isOpenLastGame = true;
	_isShowLinesNums = true;
	_isCreateFirstLoc = false;
	_firstLocName = wxT("Начало");
	_isShowLocsIcons = true;
	_isCollapseCode = false;
	_heightsCoeff = 0.6;
	_widthsCoeff1 = 0.3;
	_widthsCoeff2 = 0.25;
	_tabSize = 4;

	_font[SYNTAX_BASE] = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New"));
	_font[SYNTAX_STATEMENTS] = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
	_font[SYNTAX_FUNCTIONS] = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
	_font[SYNTAX_SYS_VARIABLES] = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
	_font[SYNTAX_STRINGS] = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New"));
	_font[SYNTAX_NUMBERS] = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
	_font[SYNTAX_OPERATIONS] = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
	_font[SYNTAX_LABELS] = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
	_font[SYNTAX_COMMENTS] = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New"));

	_textBackColour = wxColour(220, 220, 220);
	_baseBackColour = wxColour(220, 220, 220);
	_colour[SYNTAX_BASE] = *wxBLACK;
	_colour[SYNTAX_STATEMENTS] = *wxBLUE;
	_colour[SYNTAX_FUNCTIONS] = *wxBLUE;
	_colour[SYNTAX_SYS_VARIABLES] = *wxBLUE;
	_colour[SYNTAX_STRINGS] = wxColour(0, 120, 25);
	_colour[SYNTAX_NUMBERS] = wxColour(150, 0, 150);
	_colour[SYNTAX_OPERATIONS] = wxColour(0, 0, 140);
	_colour[SYNTAX_LABELS] = *wxRED;
	_colour[SYNTAX_COMMENTS] = wxColour(130, 130, 130);
}

void Settings::LoadSettings()
{
	if (!wxFileExists(_currentConfigPath)) return;

	wxFileConfig cfg(wxEmptyString, wxEmptyString, _currentConfigPath);
	cfg.Read(wxT("Paths/Player"), &_currentPlayerPath);
	cfg.Read(wxT("Paths/Help"), &_currentHelpPath);
	cfg.Read(wxT("Paths/Txt2Gam"), &_currentTxt2GamPath);
	cfg.Read(wxT("Paths/LastGame"), &_lastGamePath);
	cfg.Read(wxT("Pos/Left"), &_leftFramePos);
	cfg.Read(wxT("Pos/Top"), &_topFramePos);
	cfg.Read(wxT("Pos/Width"), &_frameWidth);
	cfg.Read(wxT("Pos/Height"), &_frameHeight);
	cfg.Read(wxT("Pos/Maximize"), &_isFrameMaximized);
	cfg.Read(wxT("Pos/ShowStatusBar"), &_isShowStatusBar);
	cfg.Read(wxT("Pos/HeightsCoef"), &_heightsCoeff);
	cfg.Read(wxT("Pos/WidthsCoef1"), &_widthsCoeff1);
	cfg.Read(wxT("Pos/WidthsCoef2"), &_widthsCoeff2);
	cfg.Read(wxT("Pos/Panels"), &_panelsPos);
	cfg.Read(wxT("OptionsDialog/Width"), &_optionsDialogWidth);
	cfg.Read(wxT("OptionsDialog/Height"), &_optionsDialogHeight);
	cfg.Read(wxT("General/AutoSave"), &_isAutoSave);
	cfg.Read(wxT("General/AutoSaveTimeInterval"), &_autoSaveInterval);
	cfg.Read(wxT("General/ShowShortLocsDescs"), &_isShowShortLocsDescs);
	cfg.Read(wxT("General/LocDescVisible"), &_isLocDescVisible);
	cfg.Read(wxT("General/LocActsVisible"), &_isLocActsVisible);
	cfg.Read(wxT("General/WrapLines"), &_isWrapLines);
	cfg.Read(wxT("General/OpenNewLoc"), &_isOpenNewLoc);
	cfg.Read(wxT("General/OpenNewAct"), &_isOpenNewAct);
	cfg.Read(wxT("General/OpenLastGame"), &_isOpenLastGame);
	cfg.Read(wxT("General/ShowLinesNums"), &_isShowLinesNums);
	cfg.Read(wxT("General/CreateFirstLoc"), &_isCreateFirstLoc);
	cfg.Read(wxT("General/FirstLocName"), &_firstLocName);
	cfg.Read(wxT("General/ShowLocsIcons"), &_isShowLocsIcons);
	cfg.Read(wxT("General/CollapseCode"), &_isCollapseCode);
	cfg.Read(wxT("General/TabSize"), &_tabSize);
	cfg.Read(wxT("Fonts/Statements"), &_font[SYNTAX_STATEMENTS]);
	cfg.Read(wxT("Fonts/Functions"), &_font[SYNTAX_FUNCTIONS]);
	cfg.Read(wxT("Fonts/SysVariables"), &_font[SYNTAX_SYS_VARIABLES]);
	cfg.Read(wxT("Fonts/Strings"), &_font[SYNTAX_STRINGS]);
	cfg.Read(wxT("Fonts/Numbers"), &_font[SYNTAX_NUMBERS]);
	cfg.Read(wxT("Fonts/Operations"), &_font[SYNTAX_OPERATIONS]);
	cfg.Read(wxT("Fonts/Labels"), &_font[SYNTAX_LABELS]);
	cfg.Read(wxT("Fonts/Comments"), &_font[SYNTAX_COMMENTS]);
	cfg.Read(wxT("Fonts/Base"), &_font[SYNTAX_BASE]);
	cfg.Read(wxT("Colours/Statements"), &_colour[SYNTAX_STATEMENTS]);
	cfg.Read(wxT("Colours/Functions"), &_colour[SYNTAX_FUNCTIONS]);
	cfg.Read(wxT("Colours/SysVariables"), &_colour[SYNTAX_SYS_VARIABLES]);
	cfg.Read(wxT("Colours/Strings"), &_colour[SYNTAX_STRINGS]);
	cfg.Read(wxT("Colours/Numbers"), &_colour[SYNTAX_NUMBERS]);
	cfg.Read(wxT("Colours/Operations"), &_colour[SYNTAX_OPERATIONS]);
	cfg.Read(wxT("Colours/Labels"), &_colour[SYNTAX_LABELS]);
	cfg.Read(wxT("Colours/Comments"), &_colour[SYNTAX_COMMENTS]);
	cfg.Read(wxT("Colours/Base"), &_colour[SYNTAX_BASE]);
	cfg.Read(wxT("Colours/TextBackground"), &_textBackColour);
	cfg.Read(wxT("Colours/BaseBackground"), &_baseBackColour);
	_hotKeysStore.LoadHotKeysData(cfg);
	_searchDataStore.LoadSearchData(cfg);
}

void Settings::SaveSettings()
{
	wxFileConfig cfg(wxEmptyString, wxEmptyString, _currentConfigPath);

	cfg.Write(wxT("Paths/Player"), _currentPlayerPath);
	cfg.Write(wxT("Paths/Help"), _currentHelpPath);
	cfg.Write(wxT("Paths/Txt2Gam"), _currentTxt2GamPath);
	cfg.Write(wxT("Paths/LastGame"), _lastGamePath);
	cfg.Write(wxT("Pos/Left"), _leftFramePos);
	cfg.Write(wxT("Pos/Top"), _topFramePos);
	cfg.Write(wxT("Pos/Width"), _frameWidth);
	cfg.Write(wxT("Pos/Height"), _frameHeight);
	cfg.Write(wxT("Pos/Maximize"), _isFrameMaximized);
	cfg.Write(wxT("Pos/ShowStatusBar"), _isShowStatusBar);
	cfg.Write(wxT("Pos/HeightsCoef"), _heightsCoeff);
	cfg.Write(wxT("Pos/WidthsCoef1"), _widthsCoeff1);
	cfg.Write(wxT("Pos/WidthsCoef2"), _widthsCoeff2);
	cfg.Write(wxT("Pos/Panels"), _panelsPos);
	cfg.Write(wxT("OptionsDialog/Width"), _optionsDialogWidth);
	cfg.Write(wxT("OptionsDialog/Height"), _optionsDialogHeight);
	cfg.Write(wxT("General/AutoSave"), _isAutoSave);
	cfg.Write(wxT("General/AutoSaveTimeInterval"), _autoSaveInterval);
	cfg.Write(wxT("General/ShowShortLocsDescs"), _isShowShortLocsDescs);
	cfg.Write(wxT("General/LocDescVisible"), _isLocDescVisible);
	cfg.Write(wxT("General/LocActsVisible"), _isLocActsVisible);
	cfg.Write(wxT("General/WrapLines"), _isWrapLines);
	cfg.Write(wxT("General/OpenNewLoc"), _isOpenNewLoc);
	cfg.Write(wxT("General/OpenNewAct"), _isOpenNewAct);
	cfg.Write(wxT("General/OpenLastGame"), _isOpenLastGame);
	cfg.Write(wxT("General/ShowLinesNums"),_isShowLinesNums);
	cfg.Write(wxT("General/CreateFirstLoc"), _isCreateFirstLoc);
	cfg.Write(wxT("General/FirstLocName"), _firstLocName);
	cfg.Write(wxT("General/ShowLocsIcons"), _isShowLocsIcons);
	cfg.Write(wxT("General/CollapseCode"), _isCollapseCode);
	cfg.Write(wxT("General/TabSize"), _tabSize);
	cfg.Write(wxT("Fonts/Statements"), _font[SYNTAX_STATEMENTS]);
	cfg.Write(wxT("Fonts/Functions"), _font[SYNTAX_FUNCTIONS]);
	cfg.Write(wxT("Fonts/SysVariables"), _font[SYNTAX_SYS_VARIABLES]);
	cfg.Write(wxT("Fonts/Strings"), _font[SYNTAX_STRINGS]);
	cfg.Write(wxT("Fonts/Numbers"), _font[SYNTAX_NUMBERS]);
	cfg.Write(wxT("Fonts/Operations"), _font[SYNTAX_OPERATIONS]);
	cfg.Write(wxT("Fonts/Labels"), _font[SYNTAX_LABELS]);
	cfg.Write(wxT("Fonts/Comments"), _font[SYNTAX_COMMENTS]);
	cfg.Write(wxT("Fonts/Base"), _font[SYNTAX_BASE]);
	cfg.Write(wxT("Colours/Statements"), _colour[SYNTAX_STATEMENTS]);
	cfg.Write(wxT("Colours/Functions"), _colour[SYNTAX_FUNCTIONS]);
	cfg.Write(wxT("Colours/SysVariables"), _colour[SYNTAX_SYS_VARIABLES]);
	cfg.Write(wxT("Colours/Strings"), _colour[SYNTAX_STRINGS]);
	cfg.Write(wxT("Colours/Numbers"), _colour[SYNTAX_NUMBERS]);
	cfg.Write(wxT("Colours/Operations"), _colour[SYNTAX_OPERATIONS]);
	cfg.Write(wxT("Colours/Labels"), _colour[SYNTAX_LABELS]);
	cfg.Write(wxT("Colours/Comments"), _colour[SYNTAX_COMMENTS]);
	cfg.Write(wxT("Colours/Base"), _colour[SYNTAX_BASE]);
	cfg.Write(wxT("Colours/TextBackground"), _textBackColour);
	cfg.Write(wxT("Colours/BaseBackground"), _baseBackColour);
	_hotKeysStore.SaveHotKeysData(cfg);
	_searchDataStore.SaveSearchData(cfg);
}

void Settings::AddObserver( IObserver *obj )
{
	observers.push_back(obj);
}

void Settings::RemoveObserver( IObserver *obj )
{
	observers.remove(obj);
}

void Settings::RemoveAllObservers()
{
	observers.clear();
}

void Settings::NotifyAll()
{
	for (std::list<IObserver *>::const_iterator i = observers.begin(); i != observers.end(); ++i)
		(*i)->Update(true);
}
