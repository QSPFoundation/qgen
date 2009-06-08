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

#ifndef _QUEST_GENERATOR_MAIN_FRAME_H
	#define _QUEST_GENERATOR_MAIN_FRAME_H

	#include <wx/filedlg.h>
	#include <wx/wfstream.h>
	#include <wx/filename.h>
	#include <wx/cmdline.h>
	#include <wx/fileconf.h>
	#include <wx/fdrepdlg.h>
	#include <wx/aboutdlg.h>
	#include <wx/help.h>

	#include "Controls.h"
	#include "Idents.h"
	#include "SearchDialog.h"
	#include "OptionsDialog.h"
	#include "InitEvent.h"
	#include "ToolBar.h"
	#ifdef __WXMSW__
		#include "DesktopWindow.h"
	#endif

	#include "bitmaps/play.xpm"
	#include "bitmaps/info.xpm"
	#include "bitmaps/filesaveas.xpm"
	#include "bitmaps/filesave.xpm"
	#include "bitmaps/copy.xpm"
	#include "bitmaps/fileopen.xpm"
	#include "bitmaps/new.xpm"
	#include "bitmaps/rename.xpm"
	#include "bitmaps/delete.xpm"
	#include "bitmaps/clear.xpm"
	#include "bitmaps/paste.xpm"
	#include "bitmaps/search.xpm"
	#include "bitmaps/options.xpm"
	#include "bitmaps/wxwin16x16.xpm"
	#include "bitmaps/logo_big.xpm"
	#include "bitmaps/undo_text.xpm"
	#include "bitmaps/redo_text.xpm"

	#include "bitmaps/open_file_menu.xpm"
	#include "bitmaps/save_file_menu.xpm"
	#include "bitmaps/play_game_menu.xpm"
	#include "bitmaps/new_game_menu.xpm"
	#include "bitmaps/cut_text_menu.xpm"
	#include "bitmaps/copy_text_menu.xpm"
	#include "bitmaps/paste_text_menu.xpm"
	#include "bitmaps/delete_text_menu.xpm"
	#include "bitmaps/search_text_menu.xpm"
	#include "bitmaps/contents_help_menu.xpm"
	#include "bitmaps/index_search_menu.xpm"
	#include "bitmaps/game_info_menu.xpm"
	#include "bitmaps/undo_text_menu.xpm"
	#include "bitmaps/redo_text_menu.xpm"
	#include "bitmaps/exit_menu.xpm"

	#define QGEN_TITLE wxT("Quests Generator")

	class QGenApp : public wxApp
	{
	private:
		Controls *_controls;
	public:
		virtual bool OnInit();
		virtual int OnExit();
	};

	DECLARE_APP(QGenApp)

	enum
	{
		ID_LOCSLIST,
		ID_TOOLBAR,
		ID_TIMER_AUTO_SAVE,
		ID_TIMER_UPD_TOOLBAR
	};

	class QGenMainFrame : public wxFrame
	{
		DECLARE_EVENT_TABLE()
	private:
		wxAuiManager		_manager;
		wxTimer				_timerAutoSave;
		wxTimer				_timerUpdToolBar;
		QGenToolBar			*_toolBar;
		wxStatusBar			*_statusBar;
		wxMenuBar			*_menu_bar;
		LocationsListBox	*_locListBox;
		LocationsNotebook	*_locNotebook;
		Controls			*_controls;
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
		void OnNewQuest(wxCommandEvent &event);
		void OnJoinQuest(wxCommandEvent &event);
		void OnPlayQuest(wxCommandEvent &event);
		void OnExportTxtFile(wxCommandEvent &event);
		void OnExportTxt2Gam(wxCommandEvent &event);
		void OnImportTxt2Gam(wxCommandEvent &event);
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
		void TogglePaneVisibility(wxString pane_name);
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

		void UpdateTitle();
		bool QuestChange();
		bool SearchHelpFile();

	public:
		QGenMainFrame( Controls *controls );
		~QGenMainFrame();

		bool Create(const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);
	};

#endif
