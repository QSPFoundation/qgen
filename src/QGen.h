// Copyright (C) 2005-2009
// BaxZzZz (bauer_v AT mail DOT ru)
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
	#include <wx/mstream.h>

	#include "Controls.h"
	#include "Idents.h"
	#include "SearchDialog.h"
	#include "OptionsDialog.h"
	#include "InitEvent.h"
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

	#define wxGetBitmapFromMemory(name) _wxGetBitmapFromMemory(name ## _png, sizeof(name ## _png))
		inline wxBitmap _wxGetBitmapFromMemory(const unsigned char *data, int length)
		{
			wxMemoryInputStream is(data, length);
			return wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1);
		}

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

		void UpdateTitle();
		bool QuestChange();

	public:
		QGenMainFrame( Controls *controls );
		~QGenMainFrame();

		bool Create(const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);
	};

#endif
