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

#ifndef _QGEN_SETTINGS_H_
    #define _QGEN_SETTINGS_H_

    #include <wx/colordlg.h>
    #include <wx/fontdlg.h>
    #include <wx/filename.h>
    #include <wx/fileconf.h>
    #include <wx/stdpaths.h>
    #include <wx/list.h>
    #include "iobserver.h"
    #include "hotkeysstore.h"
    #include "searchdatastore.h"
    #include "utils.h"

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
        SYNTAX_LINE_NUMBERS,
        SYNTAX_BASE,
        SYNTAX_DUMMY
    };

    WX_DECLARE_LIST(IObserver, ObserversList);

    class Settings
    {
    private:
        int _leftFramePos;
        int _topFramePos;
        int _frameWidth;
        int _frameHeight;
        int _optionsDialogWidth;
        int _optionsDialogHeight;
        int _autoSaveInterval;
        int _tabSize;
        bool _isFrameMaximized;
        bool _toShowStatusBar;
        bool _toAutoSave;
        bool _toShowShortLocsDescs;
        bool _isLocDescVisible;
        bool _isLocActsVisible;
        bool _toWrapLines;
        bool _toOpenNewLoc;
        bool _toOpenNewAct;
        bool _toOpenLastGame;
        bool _toShowLinesNums;
        bool _toShowHiddenChars;
        bool _toShowCallTips;
        bool _toCreateFirstLoc;
        bool _toShowLocsIcons;
        bool _toCollapseCode;
        double _heightsCoeff;
        double _widthsCoeff1;
        double _widthsCoeff2;
        wxFont _font[SYNTAX_DUMMY];
        wxColour _colour[SYNTAX_DUMMY];
        wxColour _textBackColour;
        wxColour _altTextBackColour;
        wxColour _baseBackColour;
        wxString _currentConfigPath;
        wxString _currentPlayerPath;
        wxString _currentHelpPath;
        wxString _currentTxt2GamPath;
        wxString _firstLocName;
        wxString _lastGamePath;
        wxString _panelsConfig;
        HotkeysStore _hotkeysStore;
        SearchDataStore _searchDataStore;
        int _idLang;
        ObserversList _observers;
    public:
        Settings();

        void LoadSettings();
        void SaveSettings();
        void InitSettings();
        void PostInitLocaleSettings();
        void AddObserver(IObserver *obj);
        void RemoveObserver(IObserver *obj);
        void RemoveAllObservers();
        void NotifyAll();

        void SetFirstLocName(const wxString &name) { _firstLocName = name; }
        wxString GetFirstLocName() const { return _firstLocName; }
        void SetAutoSave(bool status) { _toAutoSave = status; }
        bool GetAutoSave() const { return _toAutoSave; }
        void SetShowShortLocsDescs(bool status) { _toShowShortLocsDescs = status; }
        bool GetShowShortLocsDescs() const { return _toShowShortLocsDescs; }
        void SetLocDescVisible(bool status) { _isLocDescVisible = status; }
        bool GetLocDescVisible() const { return _isLocDescVisible; }
        void SetLocActsVisible(bool status) { _isLocActsVisible = status; }
        bool GetLocActsVisible() const { return _isLocActsVisible; }
        void SetWrapLines(bool status){ _toWrapLines = status; }
        bool GetWrapLines() const { return _toWrapLines; }
        void SetOpenNewLoc(bool status) { _toOpenNewLoc = status; }
        bool GetOpenNewLoc() const { return _toOpenNewLoc; }
        void SetOpenNewAct(bool status) { _toOpenNewAct = status; }
        bool GetOpenNewAct() const { return _toOpenNewAct; }
        void SetOpenLastGame(bool status) { _toOpenLastGame = status; }
        bool GetOpenLastGame() const { return _toOpenLastGame; }
        void SetShowLinesNums(bool status) { _toShowLinesNums = status; }
        bool GetShowLinesNums() const { return _toShowLinesNums; }
        void SetShowHiddenChars(bool status) { _toShowHiddenChars = status; }
        bool GetShowHiddenChars() const { return _toShowHiddenChars; }
        void SetShowCallTips(bool status) { _toShowCallTips = status; }
        bool GetShowCallTips() const { return _toShowCallTips; }
        void SetLastGamePath(const wxString &path) { _lastGamePath = path; }
        wxString GetLastGamePath() const { return _lastGamePath; }
        void SetCreateFirstLoc(bool status) { _toCreateFirstLoc = status; }
        bool GetCreateFirstLoc() const { return _toCreateFirstLoc; }
        void SetShowLocsIcons(bool status) { _toShowLocsIcons = status; }
        bool GetShowLocsIcons() const { return _toShowLocsIcons; }
        void SetCollapseCode(bool status) { _toCollapseCode = status; }
        bool GetCollapseCode() const { return _toCollapseCode; }
        void SetCurrentPlayerPath(const wxString &path) { _currentPlayerPath = path; }
        wxString GetCurrentPlayerPath() const { return _currentPlayerPath; }
        void SetCurrentHelpPath(const wxString &path) { _currentHelpPath = path; }
        wxString GetCurrentHelpPath() const { return _currentHelpPath; }
        void SetCurrentTxt2GamPath(const wxString &path) { _currentTxt2GamPath = path; }
        wxString GetCurrentTxt2GamPath() const { return _currentTxt2GamPath; }
        void SetAutoSaveInterval(int min) { _autoSaveInterval = min; }
        int GetAutoSaveInterval() const { return _autoSaveInterval; }
        void SetHeightsCoeff(double h) { _heightsCoeff = h; }
        double GetHeightsCoeff() const { return _heightsCoeff; }
        void SetWidthsCoeff1(double w) { _widthsCoeff1 = w; }
        double GetWidthsCoeff1() const { return _widthsCoeff1; }
        void SetWidthsCoeff2(double w) { _widthsCoeff2 = w; }
        double GetWidthsCoeff2() const { return _widthsCoeff2; }
        void SetTabSize(int sz) { _tabSize = sz; }
        int GetTabSize() const { return _tabSize; }
        wxFont GetFont(SyntaxType type) const { return _font[type]; }
        void SetFont(SyntaxType type, const wxFont &font) { _font[type] = font; }
        wxColour GetColour(SyntaxType type) const { return _colour[type]; }
        void SetColour(SyntaxType type, const wxColour &col) { _colour[type] = col; }
        wxColour GetTextBackColour() const { return _textBackColour; }
        void SetTextBackColour(const wxColour &col) { _textBackColour = col; }
        wxColour GetAltTextBackColour() const { return _altTextBackColour; }
        void SetAltTextBackColour(const wxColour &col) { _altTextBackColour = col; }
        wxColour GetBaseBackColour() const { return _baseBackColour; }
        void SetBaseBackColour(const wxColour &col) { _baseBackColour = col; }
        void SetLeftFramePos(int left) { _leftFramePos = left; }
        int GetLeftFramePos() const { return _leftFramePos; }
        void SetTopFramePos(int top) { _topFramePos = top; }
        int GetTopFramePos() const { return _topFramePos; }
        void SetFrameWidth(int width) { _frameWidth = width; }
        int GetFrameWidth() const { return _frameWidth; }
        void SetFrameHeight(int height) { _frameHeight = height; }
        int GetFrameHeight() const { return _frameHeight; }
        void SetOptionsDialogWidth(int width) { _optionsDialogWidth = width; }
        int GetOptionsDialogWidth() const { return _optionsDialogWidth; }
        void SetOptionsDialogHeight(int height) { _optionsDialogHeight = height; }
        int GetOptionsDialogHeight() const { return _optionsDialogHeight; }
        void SetMaximizeFrame(bool status) { _isFrameMaximized = status; }
        bool GetMaximizeFrame() const { return _isFrameMaximized; }
        void SetShowStatusBar(bool status) { _toShowStatusBar = status; }
        bool GetShowStatusBar() const { return _toShowStatusBar; }
        void SetPanelsConfig(const wxString &panels) { _panelsConfig = panels; }
        wxString GetPanelsConfig() const { return _panelsConfig; }

        HotkeysStore *GetHotKeys() { return &_hotkeysStore; }
        SearchDataStore *GetSearchDataStore() { return &_searchDataStore; }
        void SetIdLang(int id) { _idLang = id; }
        int GetLangId() const { return _idLang; }
    };

#endif
