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

#ifndef _QGEN_CONTROLS_H_
	#define _QGEN_CONTROLS_H_

	#include <wx/wx.h>
	#include <wx/clipbrd.h>
	#include <wx/filename.h>
	#include "DataContainer.h"
	#include "LocationsListBox.h"
	#include "LocationsNotebook.h"
	#include "DataContainer.h"
	#include "IControls.h"
	#include "MergeDialog.h"
	#include "Game.h"
	#ifdef __WXMSW__
		#include "KeysParser.h"
	#endif

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
		QGEN_MSG_WRONGFORMAT,
		QGEN_MSG_MAXACTIONSCOUNTREACHED,
		QGEN_MSG_TOOLONGFOLDERNAME,
		QGEN_MSG_TOOLONGLOCATIONNAME,
		QGEN_MSG_TOOLONGACTIONNAME
	};

	struct DataSearch
	{
		int			idxLoc;
		int			idxAct;
		long		startPos;
		long		stringLen;
		int			countChecking;
		SearchPlace foundAt;
		SearchPlace findAt;
		bool		isFoundAny;
	};

	class Controls : public IControls
	{
	private:
		wxFrame				*_mainFrame;
		LocationsListBox	*_locListBox;
		LocationsNotebook	*_locNotebook;
		DataContainer		*_container;
		Settings			*_settings;

		#ifdef __WXMSW__
			KeysParser		*_keysParser;
		#endif

		KeywordsStore		*_keywordsStore;
		DataSearch			_dataSearch;
		wxString			_currentPath;
		wxString			_currentGamePath;
		wxString			_currentGamePass;
		wxLongLong			_lastSaveTime;
		long				_execHotkeyEnters;

		static wxString ConvertSearchString(const wxString& s, bool isMatchCase);
		static int FindSubString(const wxString& s, const wxString& sub, bool isWholeString, int ind = 0);
		static wxWindow *GetParent();
	public:
		Controls(const wxString &path);
		~Controls();

		Settings *GetSettings() const { return _settings; }
		DataContainer *GetContainer() const { return _container; }
		KeywordsStore *GetKeywordsStore() const { return _keywordsStore; }

		void SetMainFrame(wxFrame *mainFrame) { _mainFrame = mainFrame; }
		void SetLocListBox(LocationsListBox *locListBox) { _locListBox = locListBox; }
		void SetNotebook(LocationsNotebook *locNotebook) { _locNotebook = locNotebook; }

		wxString GetGamePath() const { return _currentGamePath; }
		wxString GetGamePass() const { return _currentGamePass; }

		int GetSelectedLocationIndex();
		int GetSelectedFolderIndex();
		void SelectLocation(size_t locIndex);
		bool RenameFolder(size_t folderIndex, const wxString &name);
		bool RenameLocation(size_t locIndex, const wxString &name);
		bool RenameAction(size_t locIndex, size_t actIndex, const wxString &name);
		int AddLocationByName(const wxString &name);
		int AddLocation(const wxString &name = wxEmptyString);
		void UpdateActionsOnAllLocs();

		bool DeleteSelectedLocation();
		bool RenameSelectedLocation();
		void SortLocations(bool isAscending);
		void ClearSelectedLocation();
		void UpdateLocationsList();
		void UpdateOpenedLocationsIndexes();
		void UpdateLocationIcon(size_t locIndex, bool isOpened);
		void UpdateMenuItems(wxMenu *menu);
		void ShowOpenedLocationsIcons();
		void SyncWithLocationsList();

		bool AddActionOnSelectedLoc();
		bool DeleteSelectedAction();
		bool RenameSelectedAction();
		bool DeleteAllActions();

		void ShowMessage(long errorNum);
		static wxString GetMessageDesc(long errorNum);

		bool GetBufferedLocName(const wxString &buffer, wxString &locName);
		bool IsClipboardEmpty();
		void CopySelectedLocToClipboard();
		void PasteLocFromClipboard(PasteType type);

		wxString EncodeString(const wxString &str);
		wxString DecodeString(const wxString &str);

		bool IsSelectedLocationEmpty();
		bool IsAllLocsClosed();
		bool IsActionsOnSelectedLocEmpty();

		static wxTextEntryBase *GetCurrentTextBox();
		bool CanUndoText();
		bool CanRedoText();
		bool CanCopyText();
		bool CanPasteText();
		bool CanCutText();
		bool CanSelectText();
		void UndoText();
		void RedoText();
		void CopySelectedTextToClipboard();
		void PasteTextFromClipboard();
		void CutSelectedTextToClipboard();
		void DeleteSelectedText();
		void SelectAllText();

		bool SerializeLocData(size_t locIndex, wxString &buffer);
		bool DeserializeLocData(size_t locIndex, const wxString &buffer);

		wxString SelectPicturePath();

		bool SaveGameWithCheck();
		bool SaveGame(const wxString &path, const wxString &password);
		bool LoadGame(const wxString &filename);
		void NewGame();
		bool JoinGame(const wxString &filename);
		bool ExportTxt(const wxString &filename);
		bool ExportTxt2Gam(const wxString &filename);
		bool ImportTxt2Gam(const wxString &filename);

		wxString GetSelectedWord();
		void JumpToSelectedLoc();
		bool SearchString(const wxString &str, bool findAgain, bool isMatchCase = false, bool isWholeString = false);
		void ReplaceSearchString(const wxString & replaceString);
		bool SearchNextLoc();
		void InitSearchData();
		bool IsGameSaved();
		bool IsCorrectDataFormat(const wxString &str);
		wxString GetGameInfo();

		void SwitchLocDesc();
		void SwitchLocActs();

		LocationPage *ShowLocation(const wxString & locName);
		void MoveActionTo(size_t locIndex, size_t actIndex, size_t moveTo);

		void SetLastSaveTime(wxLongLong lastSaveTime) { _lastSaveTime = lastSaveTime; }
		wxLongLong GetLastSaveTime() const { return _lastSaveTime; }
		bool ExecuteHotkey(int keyCode, int modifiers);
		bool IsInHotkeyExecution() const { return _execHotkeyEnters != 0; }
		void SetStatusText(const wxString &text);

		bool AddFolder();
		bool DeleteSelectedFolder();
		bool RenameSelectedFolder();

		bool SearchHelpFile();
	};

#endif
