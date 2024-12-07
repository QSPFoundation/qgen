// Copyright (C) 2005-2012
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

class LocationPage;

#ifndef _QGEN_ICONTROLS_H_
    #define _QGEN_ICONTROLS_H_

    #include "settings.h"
    #include "datacontainer.h"
    #include "keywordsstore.h"

    enum PasteType
    {
        PASTE_REPLACE,
        PASTE_NEW,
        PASTE_NEW_AUTO
    };

    enum SearchPlace
    {
        SEARCH_NONE,
        SEARCH_LOCNAME,
        SEARCH_LOCDESC,
        SEARCH_LOCCODE,
        SEARCH_ACTNAME,
        SEARCH_PATHPICT,
        SEARCH_ACTCODE
    };

    enum MessageType
    {
        QGEN_MSG_EXISTS,
        QGEN_MSG_EXISTS_HKEY,
        QGEN_MSG_EXISTS_S_HKEY,
        QGEN_MSG_EMPTYDATA,
        QGEN_MSG_WRONGPASSWORD,
        QGEN_MSG_CANTSAVEGAME,
        QGEN_MSG_CANTLOADGAME,
        QGEN_MSG_NOTFOUND,
        QGEN_MSG_SEARCHENDED,
        QGEN_MSG_INVALIDREGEXP,
        QGEN_MSG_WRONGFORMAT,
        QGEN_MSG_MAXACTIONSCOUNTREACHED,
        QGEN_MSG_TOOLONGFOLDERNAME,
        QGEN_MSG_TOOLONGLOCATIONNAME,
        QGEN_MSG_TOOLONGACTIONNAME
    };

    class IControls
    {
    public:
        virtual ~IControls() {}

        virtual Settings *GetSettings() const = 0;
        virtual DataContainer *GetContainer() const = 0;
        virtual KeywordsStore *GetKeywordsStore() const = 0;
        virtual wxString GetGamePath() const = 0;

        virtual bool RenameFolder(size_t folderIndex, const wxString &name) = 0;
        virtual void SyncWithLocationsList() = 0;

        virtual void UpdateLocationsList() = 0;
        virtual void UpdateMenuItems(wxMenu *menu) = 0;
        virtual void ShowOpenedLocationsIcons() = 0;
        virtual void UpdateLocationIcon(size_t locIndex, bool isOpened) = 0;
        virtual LocationPage *ShowLocation(const wxString &locName) = 0;
        virtual void SelectLocation(size_t locIndex) = 0;
        virtual bool RenameLocation(size_t locIndex, const wxString &name) = 0;

        virtual wxString SelectPicturePath() = 0;

        virtual bool AddActionOnSelectedLoc() = 0;
        virtual bool DeleteSelectedAction() = 0;
        virtual bool RenameSelectedAction() = 0;
        virtual bool RenameAction(size_t locIndex, size_t actIndex, const wxString &name) = 0;

        virtual void ShowMessage(long errorNum) = 0;
        virtual void MoveActionTo(size_t locIndex, size_t actIndex, size_t moveTo) = 0;
        virtual bool ExecuteHotkey(int keyCode, int modifiers) = 0;
        virtual bool IsInHotkeyExecution() const = 0;
        virtual void SetStatusText(const wxString &text) = 0;
        virtual bool LoadGame(const wxString &filename) = 0;
        virtual void NewGame() = 0;
        virtual bool SaveGame(const wxString &path, const wxString &password) = 0;
        virtual bool SaveGameWithCheck() = 0;
        virtual bool SearchString(const wxString &str, bool toFindAgain,
            bool isCaseSensitive = false, bool isWholeString = false, bool isRegExp = false) = 0;
        virtual bool ReplaceSearchString(const wxString& replaceString,
            bool isCaseSensitive = false, bool isRegExp = false) = 0;
        virtual bool SearchNextLoc() = 0;
        virtual void InitSearchData() = 0;
        virtual int AddLocation(const wxString &name = wxEmptyString) = 0;
        virtual bool DeleteSelectedLocation() = 0;
        virtual bool RenameSelectedLocation() = 0;
        virtual void ClearSelectedLocation() = 0;
        virtual void CopySelectedLocToClipboard() = 0;
        virtual void PasteLocFromClipboard(PasteType type) = 0;
        virtual void SortLocations(bool isAscending) = 0;
        virtual bool DeleteAllActions() = 0;
        virtual void UndoText() = 0;
        virtual void RedoText() = 0;
        virtual void CopySelectedTextToClipboard() = 0;
        virtual void CutSelectedTextToClipboard() = 0;
        virtual void PasteTextFromClipboard() = 0;
        virtual void DeleteSelectedText() = 0;
        virtual void SelectAllText() = 0;
        virtual bool JoinGame(const wxString &filename) = 0;
        virtual bool IsGameSaved() = 0;
        virtual bool SearchHelpFile() = 0;
        virtual wxString GetSelectedWord() const = 0;
        virtual bool ExportTxt(const wxString &filename) = 0;
        virtual bool ExportTxt2Gam(const wxString &filename) = 0;
        virtual bool ImportTxt2Gam(const wxString &filename) = 0;
        virtual wxString GetGameInfo() const = 0;
        virtual wxLongLong GetLastSaveTime() const = 0;
        virtual void UpdateLocale(int lang) = 0;
        virtual bool CanUndoText() const = 0;
        virtual bool CanRedoText() const = 0;
        virtual int GetSelectedLocationIndex() const = 0;
        virtual bool IsSelectedLocationEmpty() const = 0;
        virtual bool IsActionsOnSelectedLocEmpty() const = 0;
        virtual bool IsAllLocsClosed() const = 0;
        virtual void SwitchLocDesc() = 0;
        virtual void SwitchLocActs() = 0;
        virtual void JumpToSelectedLoc() = 0;
        virtual bool AddFolder() = 0;
        virtual bool DeleteSelectedFolder() = 0;
        virtual bool RenameSelectedFolder() = 0;
    };

#endif
