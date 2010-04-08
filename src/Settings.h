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

#ifndef _QGEN_SETTINGS_H_
	#define _QGEN_SETTINGS_H_

	#include <wx/colordlg.h>
	#include <wx/fontdlg.h>
	#include <wx/filename.h>
	#include <wx/fileconf.h>
	#include <wx/list.h>
	#include "IObserver.h"
	#include "HotKeysStore.h"
	#include "SearchDataStore.h"

	enum SyntaxType
	{
		SYNTAX_STATEMENTS,
		SYNTAX_FUNCTIONS,
		SYNTAX_SYS_VARIABLES,
		SYNTAX_STRINGS,
		SYNTAX_NUMBERS,
		SYNTAX_OPERATIONS,
		SYNTAX_LABELS,
		SYNTAX_COMMENTS,
		SYNTAX_BASE,
		SYNTAX_DUMMY
	};

	WX_DECLARE_LIST(IObserver, ObserversList);

	class Settings
	{
	private:
		int				_leftFramePos;
		int				_topFramePos;
		int				_frameWidth;
		int				_frameHeight;
		int				_optionsDialogWidth;
		int				_optionsDialogHeight;
		int				_autoSaveInterval;
		int				_tabSize;
		bool			_isFrameMaximized;
		bool			_isShowStatusBar;
		bool			_isAutoSave;
		bool			_isShowShortLocsDescs;
		bool			_isLocDescVisible;
		bool			_isLocActsVisible;
		bool			_isWrapLines;
		bool			_isOpenNewLoc;
		bool			_isOpenNewAct;
		bool			_isOpenLastGame;
		bool			_isShowLinesNums;
		bool			_isCreateFirstLoc;
		bool			_isShowLocsIcons;
		bool			_isCollapseCode;
		double			_heightsCoeff;
		double			_widthsCoeff1;
		double			_widthsCoeff2;
		wxFont			_font[SYNTAX_DUMMY];
		wxColour		_colour[SYNTAX_DUMMY];
		wxColour		_textBackColour;
		wxColour		_baseBackColour;
		wxString		_path;
		wxString		_currentConfigPath;
		wxString		_currentPlayerPath;
		wxString		_currentHelpPath;
		wxString		_currentTxt2GamPath;
		wxString		_firstLocName;
		wxString		_lastGamePath;
		wxString		_panelsPos;
		HotkeysStore	_hotkeysStore;
		SearchDataStore _searchDataStore;
		int				_idLang;
		ObserversList	_observers;
	public:
		Settings(const wxString &path);

		void LoadSettings();
		void SaveSettings();
		void InitSettings();
		void AddObserver(IObserver *obj);
		void RemoveObserver(IObserver *obj);
		void RemoveAllObservers();
		void NotifyAll();

		void SetFirstLocName(const wxString &name) { _firstLocName = name; }
		wxString GetFirstLocName() { return _firstLocName; }
		void SetAutoSave(bool status) { _isAutoSave = status; }
		bool GetAutoSave() { return _isAutoSave; }
		void SetShowShortLocsDescs(bool status) { _isShowShortLocsDescs = status; }
		bool GetShowShortLocsDescs() { return _isShowShortLocsDescs; }
		void SetLocDescVisible(bool status) { _isLocDescVisible = status; }
		bool GetLocDescVisible() { return _isLocDescVisible; }
		void SetLocActsVisible(bool status) { _isLocActsVisible = status; }
		bool GetLocActsVisible() { return _isLocActsVisible; }
		void SetWrapLines(bool status){ _isWrapLines = status; }
		bool GetWrapLines(){ return _isWrapLines; }
		void SetOpenNewLoc(bool status) { _isOpenNewLoc = status; }
		bool GetOpenNewLoc() { return _isOpenNewLoc; }
		void SetOpenNewAct(bool status) { _isOpenNewAct = status; }
		bool GetOpenNewAct() { return _isOpenNewAct; }
		void SetOpenLastGame(bool status) { _isOpenLastGame = status; }
		bool GetOpenLastGame() { return _isOpenLastGame; }
		void SetShowLinesNums(bool status) { _isShowLinesNums = status; }
		bool GetShowLinesNums() { return _isShowLinesNums; }
		void SetLastGamePath(const wxString &path) { _lastGamePath = path; }
		wxString GetLastGamePath() { return _lastGamePath; }
		void SetCreateFirstLoc(bool status) { _isCreateFirstLoc = status; }
		bool GetCreateFirstLoc() { return _isCreateFirstLoc; }
		void SetShowLocsIcons(bool status) { _isShowLocsIcons = status; }
		bool GetShowLocsIcons() { return _isShowLocsIcons; }
		void SetCollapseCode(bool status) { _isCollapseCode = status; }
		bool GetCollapseCode() { return _isCollapseCode; }
		void SetCurrentPlayerPath(const wxString &path) { _currentPlayerPath = path; }
		wxString GetCurrentPlayerPath() { return _currentPlayerPath; }
		void SetCurrentHelpPath(const wxString &path) { _currentHelpPath = path; }
		wxString GetCurrentHelpPath() { return _currentHelpPath; }
		void SetCurrentTxt2GamPath(const wxString &path) { _currentTxt2GamPath = path; }
		wxString GetCurrentTxt2GamPath() { return _currentTxt2GamPath; }
		void SetAutoSaveInterval(int min) { _autoSaveInterval = min; }
		int GetAutoSaveInterval() { return _autoSaveInterval; }
		void SetHeightsCoeff(double h) { _heightsCoeff = h; }
		double GetHeightsCoeff() { return _heightsCoeff; }
		void SetWidthsCoeff1(double w) { _widthsCoeff1 = w; }
		double GetWidthsCoeff1() { return _widthsCoeff1; }
		void SetWidthsCoeff2(double w) { _widthsCoeff2 = w; }
		double GetWidthsCoeff2() { return _widthsCoeff2; }
		void SetTabSize(int sz) { _tabSize = sz; }
		int GetTabSize() { return _tabSize; }
		wxFont GetFont(SyntaxType type) { return _font[type]; }
		void SetFont(SyntaxType type, const wxFont &font) { _font[type] = font; }
		wxColour GetColour(SyntaxType type) { return _colour[type]; }
		void SetColour(SyntaxType type, const wxColour &col) { _colour[type] = col; }
		wxColour GetTextBackColour() { return _textBackColour; }
		void SetTextBackColour(const wxColour &col) { _textBackColour = col; }
		wxColour GetBaseBackColour() { return _baseBackColour; }
		void SetBaseBackColour(const wxColour &col) { _baseBackColour = col; }
		void SetLeftFramePos(int left) { _leftFramePos = left; }
		int GetLeftFramePos() { return _leftFramePos; }
		void SetTopFramePos(int top) { _topFramePos = top; }
		int GetTopFramePos() { return _topFramePos; }
		void SetFrameWidth(int width) { _frameWidth = width; }
		int GetFrameWidth() { return _frameWidth; }
		void SetFrameHeight(int height) { _frameHeight = height; }
		int GetFrameHeight() { return _frameHeight; }
		void SetOptionsDialogWidth(int width) { _optionsDialogWidth = width; }
		int GetOptionsDialogWidth() { return _optionsDialogWidth; }
		void SetOptionsDialogHeight(int height) { _optionsDialogHeight = height; }
		int GetOptionsDialogHeight() { return _optionsDialogHeight; }
		void SetMaximizeFrame( bool status) { _isFrameMaximized = status; }
		bool GetMaximizeFrame() { return _isFrameMaximized; }
		void SetShowStatusBar( bool status) { _isShowStatusBar = status; }
		bool GetShowStatusBar() { return _isShowStatusBar; }
		void SetPanelsPos(const wxString &panels) { _panelsPos = panels; }
		wxString GetSetPanelsPos() { return _panelsPos; }

		HotkeysStore *GetHotKeys() { return &_hotkeysStore; }
		SearchDataStore *GetSearchDataStore() { return &_searchDataStore; }
		void SetIdLang(int id) { _idLang = id; }
		int GetLangId() { return _idLang; }
		wxString GetPath() const { return _path; }
	};

#endif
