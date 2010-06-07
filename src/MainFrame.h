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

#ifndef _QGEN_MAIN_FRAME_H_
	#define _QGEN_MAIN_FRAME_H_

	#include <wx/filedlg.h>
	#include <wx/wfstream.h>
	#include <wx/fdrepdlg.h>
	#include <wx/aboutdlg.h>
	#include <wx/help.h>
	#include <wx/aui/aui.h>
	#include "InitEvent.h"
	#include "IControls.h"
	#include "LocationsListBox.h"
	#include "LocationsNotebook.h"
	#include "SearchDialog.h"
	#include "OptionsDialog.h"
	#include "ToolBar.h"
	#ifdef __WXMSW__
		#include "DesktopWindow.h"
	#endif
	#include "bitmaps/toolbar_game_play.xpm"
	#include "bitmaps/toolbar_game_info.xpm"
	#include "bitmaps/toolbar_file_saveas.xpm"
	#include "bitmaps/toolbar_file_save.xpm"
	#include "bitmaps/toolbar_location_copy.xpm"
	#include "bitmaps/toolbar_file_open.xpm"
	#include "bitmaps/toolbar_location_new.xpm"
	#include "bitmaps/toolbar_location_rename.xpm"
	#include "bitmaps/toolbar_location_delete.xpm"
	#include "bitmaps/toolbar_location_clear.xpm"
	#include "bitmaps/toolbar_location_paste.xpm"
	#include "bitmaps/toolbar_text_search.xpm"
	#include "bitmaps/toolbar_options.xpm"
	#include "bitmaps/qgen.xpm"
	#include "bitmaps/about_logo.xpm"
	#include "bitmaps/toolbar_redo.xpm"
	#include "bitmaps/toolbar_undo.xpm"
	#include "bitmaps/menu_file_open.xpm"
	#include "bitmaps/menu_file_save.xpm"
	#include "bitmaps/menu_game_play.xpm"
	#include "bitmaps/menu_game_new.xpm"
	#include "bitmaps/menu_text_cut.xpm"
	#include "bitmaps/menu_text_copy.xpm"
	#include "bitmaps/menu_text_paste.xpm"
	#include "bitmaps/menu_text_delete.xpm"
	#include "bitmaps/menu_text_search.xpm"
	#include "bitmaps/menu_help.xpm"
	#include "bitmaps/menu_help_search.xpm"
	#include "bitmaps/menu_game_info.xpm"
	#include "bitmaps/menu_undo.xpm"
	#include "bitmaps/menu_redo.xpm"
	#include "bitmaps/menu_exit.xpm"

	#define QGEN_TITLE wxT("Quests Generator")
	#define QGEN_APPNAME wxT("qgen")

	enum
	{
		ID_LOCSLIST = 10000,
		ID_TOOLBAR,
		ID_TIMER_AUTO_SAVE,
		ID_TIMER_UPD_TOOLBAR,
		ID_TOGGLE,
		ID_TOGGLE_TOOLBAR,
		ID_TOGGLE_STATUSBAR,
		ID_TOGGLE_LISTBOX,
		ID_GAME_NEW,
		ID_GAME_OPEN,
		ID_GAME_JOIN,
		ID_GAME_SAVE,
		ID_GAME_SAVEAS,
		ID_GAME_EXPORT,
		ID_GAME_EXPORTTXT,
		ID_GAME_EXPORTTXT2GAM,
		ID_GAME_IMPORT,
		ID_GAME_IMPORTTXT2GAM,
		ID_GAME_PLAY,
		ID_QGEN_EXIT,
		ID_UTIL_FIND,
		ID_UTIL_INF,
		ID_UTIL_OPTIONS,
		ID_FOLDER_CREAT,
		ID_FOLDER_RENAME,
		ID_FOLDER_DEL,
		ID_LOC_CREATE,
		ID_LOC_RENAME,
		ID_LOC_DEL,
		ID_LOC_COPY,
		ID_LOC_REPLACE,
		ID_LOC_PASTE,
		ID_LOC_PASTENEW,
		ID_LOC_CLEAR,
		ID_LOC_SORTASC,
		ID_LOC_SORTDESC,
		ID_LOC_JUMPLOC,
		ID_LOC_EXPAND,
		ID_LOC_COLLAPSE,
		ID_LOC_SELECT,
		ID_LOC_DESCVISIBLE,
		ID_LOC_ACTVISIBLE,
		ID_ACTION_ADD,
		ID_ACTION_REN,
		ID_ACTION_DEL,
		ID_ACTION_DELALL,
		ID_ACTION_SUBMENU,
		ID_HELP,
		ID_HELP_SEARCH,
		ID_ABOUT,
		ID_TEXT_UNDO,
		ID_TEXT_REDO,
		ID_TEXT_CUT,
		ID_TEXT_COPY,
		ID_TEXT_PASTE,
		ID_TEXT_DEL,
		ID_TEXT_SELALL,
		ID_TAB_CLOSEALL,
		ID_TAB_CLOSEEXCEPTSELECTED,
		ID_TAB_CLOSESELECTED,
		ID_TAB_FIX
	};

	class MainFrame : public wxFrame, public IObserver
	{
		DECLARE_EVENT_TABLE()
	private:
		wxAuiManager		_manager;
		wxTimer				_timerAutoSave;
		wxTimer				_timerUpdToolBar;
		ToolBar				*_toolBar;
		wxMenuBar			*_menu_bar;
		LocationsListBox	*_locListBox;
		LocationsNotebook	*_locNotebook;
		IControls			*_controls;
		SearchDialog		*_findDlg;
		
		void OnInit(InitEvent &event);
		void OnExit(wxCommandEvent &event);
		void OnQuit(wxCloseEvent &event);
		void OnChmHelp(wxCommandEvent &event);
		void OnSearchHelp(wxCommandEvent &event);
		void OnAbout(wxCommandEvent &event);
		void OnLoadFile(wxCommandEvent &event);
		void OnSaveQuestAs(wxCommandEvent &event);
		void OnSaveQuest(wxCommandEvent &event);
		void OnNewGame(wxCommandEvent &event);
		void OnJoinQuest(wxCommandEvent &event);
		void OnPlayQuest(wxCommandEvent &event);
		void OnExportTxtFile(wxCommandEvent &event);
		void OnExportTxt2Gam(wxCommandEvent &event);
		void OnImportTxt2Gam(wxCommandEvent &event);
		void OnCreateFolder(wxCommandEvent &event);
		void OnRenameFolder(wxCommandEvent &event);
		void OnDeleteFolder(wxCommandEvent &event);
		void OnCreateLocation(wxCommandEvent &event);
		void OnDeleteLocation(wxCommandEvent &event);
		void OnRenameLocation(wxCommandEvent &event);
		void OnClearLocation(wxCommandEvent &event);
		void OnCopyLocation(wxCommandEvent &event);
		void OnPasteLocation(wxCommandEvent &event);
		void OnPasteNewLocation(wxCommandEvent &event);
		void OnSortLocations(wxCommandEvent &event);
		void OnJumpLocation(wxCommandEvent &event);
		void OnExpandLocation(wxCommandEvent &event);
		void OnCollapseLocation(wxCommandEvent &event);
		void OnToggleToolbar(wxCommandEvent &event);
		void OnToggleLocations(wxCommandEvent &event);
		void OnToggleStatusbar(wxCommandEvent &event);
		void OnFindDialog(wxCommandEvent&event);
		void OnInformationQuest(wxCommandEvent &event);
		void OnOptionsDialog(wxCommandEvent &event);
		void OnOpenMenu(wxMenuEvent& event);
		void OnCreateAction(wxCommandEvent &event);
		void OnDeleteAction(wxCommandEvent &event);
		void OnDeleteAllActions(wxCommandEvent &event);
		void OnRenameAction(wxCommandEvent &event);
		void OnUndoText(wxCommandEvent &event);
		void OnRedoText(wxCommandEvent &event);
		void OnCopyText(wxCommandEvent &event);
		void OnCutText(wxCommandEvent &event);
		void OnPasteText(wxCommandEvent &event);
		void OnDeleteText(wxCommandEvent &event);
		void OnSelectAllText(wxCommandEvent &event);
		void OnTabMenu(wxCommandEvent &event);
		void OnFixPage(wxCommandEvent &event);
		void OnLocDescVisible(wxCommandEvent &event);
		void OnLocActsVisible(wxCommandEvent &event);
		void OnPaneClose(wxAuiManagerEvent& event);
		void OnUpdMenuItems(wxMenuEvent& event);
		void OnTimerAutoSave(wxTimerEvent &event);
		void OnTimerUpdToolBar(wxTimerEvent &event);
		void OnKeyDown(wxKeyEvent& event);

		void CreateControls();
		void CreateStatusBar();
		void CreateMenuBar();
		void CreateToolBar();
		void CreateLocListBox();
		void CreateNotebook();
		void LoadLayout();
		void SaveLayout();
		bool QuestChange();
		void TogglePaneVisibility(const wxString &pane_name);
	public:
		MainFrame(IControls *controls);
		~MainFrame();

		bool Create(const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);
		void Update(bool isFromObservable = false);
		LocationsListBox *GetLocListBox() const { return _locListBox; }
		LocationsNotebook *GetNotebook() const { return _locNotebook; }
		void UpdateTitle();
	};

#endif
