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

#include "mainframe.h"
#include "game.h"

#include "bitmaps/qgen.xpm"
#include "bitmaps/about_logo.xpm"
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

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_INIT(MainFrame::OnInit)
    EVT_CLOSE(MainFrame::OnQuit)
    EVT_AUI_PANE_CLOSE(MainFrame::OnPaneClose)
    EVT_MENU(ID_QGEN_EXIT, MainFrame::OnExit)
    EVT_MENU(ID_TOGGLE_TOOLBAR, MainFrame::OnToggleToolbar)
    EVT_MENU(ID_TOGGLE_LISTBOX, MainFrame::OnToggleLocations)
    EVT_MENU(ID_TOGGLE_STATUSBAR, MainFrame::OnToggleStatusbar)
    EVT_MENU(ID_TAB_CLOSEALL, MainFrame::OnTabMenu)
    EVT_MENU(ID_TAB_CLOSEEXCEPTSELECTED, MainFrame::OnTabMenu)
    EVT_MENU(ID_TAB_CLOSESELECTED, MainFrame::OnTabMenu)
    EVT_MENU(ID_TAB_FIX, MainFrame::OnFixPage)
    EVT_MENU(ID_HELP, MainFrame::OnChmHelp)
    EVT_MENU(ID_HELP_SEARCH, MainFrame::OnSearchHelp)
    EVT_MENU(ID_HELP_CHECKUPDATES, MainFrame::OnCheckUpdates)
    EVT_MENU(ID_FOLDER_CREAT, MainFrame::OnCreateFolder)
    EVT_MENU(ID_FOLDER_RENAME, MainFrame::OnRenameFolder)
    EVT_MENU(ID_FOLDER_DEL, MainFrame::OnDeleteFolder)
    EVT_MENU(ID_LOC_DESCVISIBLE, MainFrame::OnLocDescVisible)
    EVT_MENU(ID_LOC_ACTVISIBLE, MainFrame::OnLocActsVisible)
    EVT_MENU(ID_LOC_CREATE, MainFrame::OnCreateLocation)
    EVT_MENU(ID_LOC_DEL, MainFrame::OnDeleteLocation)
    EVT_MENU(ID_LOC_RENAME, MainFrame::OnRenameLocation)
    EVT_MENU(ID_LOC_CLEAR, MainFrame::OnClearLocation)
    EVT_MENU(ID_LOC_COPY, MainFrame::OnCopyLocation)
    EVT_MENU(ID_LOC_REPLACE, MainFrame::OnPasteLocation)
    EVT_MENU(ID_LOC_PASTE, MainFrame::OnPasteLocation)
    EVT_MENU(ID_LOC_PASTENEW, MainFrame::OnPasteLocation)
    EVT_MENU(ID_LOC_SORTASC, MainFrame::OnSortLocations)
    EVT_MENU(ID_LOC_SORTDESC, MainFrame::OnSortLocations)
    EVT_MENU(ID_LOC_JUMPLOC, MainFrame::OnJumpLocation)
    EVT_MENU(ID_LOC_EXPAND, MainFrame::OnExpandLocation)
    EVT_MENU(ID_LOC_COLLAPSE, MainFrame::OnCollapseLocation)
    EVT_MENU(ID_ACTION_ADD, MainFrame::OnCreateAction)
    EVT_MENU(ID_ACTION_DEL, MainFrame::OnDeleteAction)
    EVT_MENU(ID_ACTION_DELALL, MainFrame::OnDeleteAllActions)
    EVT_MENU(ID_ACTION_REN, MainFrame::OnRenameAction)
    EVT_MENU(ID_GAME_NEW, MainFrame::OnNewGame)
    EVT_MENU(ID_GAME_OPEN, MainFrame::OnLoadFile)
    EVT_MENU(ID_GAME_SAVE, MainFrame::OnSaveQuest)
    EVT_MENU(ID_GAME_SAVEAS, MainFrame::OnSaveQuestAs)
    EVT_MENU(ID_GAME_MERGE, MainFrame::OnMergeQuest)
    EVT_MENU(ID_GAME_PLAY, MainFrame::OnPlayQuest)
    EVT_MENU(ID_UTIL_FIND, MainFrame::OnFindDialog)
    EVT_MENU(ID_UTIL_INF, MainFrame::OnQuestInformation)
    EVT_MENU(ID_UTIL_OPTIONS, MainFrame::OnOptionsDialog)
    EVT_MENU(ID_TEXT_UNDO, MainFrame::OnUndoText)
    EVT_MENU(ID_TEXT_REDO, MainFrame::OnRedoText)
    EVT_MENU(ID_TEXT_COPY, MainFrame::OnCopyText)
    EVT_MENU(ID_TEXT_PASTE, MainFrame::OnPasteText)
    EVT_MENU(ID_TEXT_CUT, MainFrame::OnCutText)
    EVT_MENU(ID_TEXT_DEL, MainFrame::OnDeleteText)
    EVT_MENU(ID_TEXT_SELALL, MainFrame::OnSelectAllText)
    EVT_MENU(ID_GAME_EXPORTTXT, MainFrame::OnExportTxtFile)
    EVT_MENU(ID_GAME_EXPORTTXT2GAM, MainFrame::OnExportTxt2Gam)
    EVT_MENU(ID_GAME_IMPORTTXT2GAM, MainFrame::OnImportTxt2Gam)
    EVT_MENU(ID_ABOUT, MainFrame::OnAbout)
    EVT_MENU_OPEN(MainFrame::OnUpdateMenuItems)
    EVT_TIMER(ID_TIMER_AUTO_SAVE, MainFrame::OnTimerAutoSave)
    EVT_TIMER(ID_TIMER_UPD_TOOLBAR, MainFrame::OnTimerUpdateToolBar)
    EVT_KEY_DOWN(MainFrame::OnKeyDown)
END_EVENT_TABLE()

MainFrame::MainFrame(IControls *controls) :
    _timerAutoSave(this, ID_TIMER_AUTO_SAVE),
    _timerUpdToolBar(this, ID_TIMER_UPD_TOOLBAR)
{
    Bind(wxEVT_WEBREQUEST_STATE, &MainFrame::OnVersionRequestState, this);

    _controls = controls;
    Create(wxSize(640, 480), wxDEFAULT_FRAME_STYLE);
    _timerAutoSave.Start(1000);
    _timerUpdToolBar.Start(500);
    _findDlg = NULL;
    Update();
    _controls->GetSettings()->AddObserver(this);
}

MainFrame::~MainFrame()
{
    _controls->GetSettings()->RemoveObserver(this);
    _manager.UnInit();
}

bool MainFrame::Create(const wxSize &size, long style)
{
    bool res = wxFrame::Create(NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, size, style);
    if (res)
    {
#ifdef __WXMSW__
        SetIcon(wxICON(qgen));
#endif
        SetMinSize(wxSize(550, 300));
        CreateControls();
    }
    return res;
}

void MainFrame::CreateControls()
{
    _manager.SetManagedWindow(this);
    CreateStatusBar();
    CreateMenuBar();
    CreateToolBar();
    CreateNotebook();
    CreateLocListBox();

    _manager.AddPane(_locNotebook, wxAuiPaneInfo().CenterPane().Name(wxT("LocationNotebook")));

    _manager.AddPane(_toolBar, wxAuiPaneInfo().ToolbarPane().
        Name(wxT("Toolbar")).Top().LeftDockable(false).RightDockable(false).Floatable(true));

    _manager.AddPane(_locListBox, wxAuiPaneInfo().Left().
        Layer(1).PinButton().MinimizeButton().MaximizeButton().Name(wxT("LocationList")));

    LoadLayout();
    _manager.Update();
}

void MainFrame::CreateStatusBar()
{
    SetStatusBar(new wxStatusBar(this));
    PositionStatusBar();
}

void MainFrame::CreateMenuBar()
{
    wxMenu *file_menu = new wxMenu;
    wxMenuItem *newGameItem = new wxMenuItem(file_menu, ID_GAME_NEW, wxT("-"));
    newGameItem->SetBitmap(wxBitmap(menu_game_new_xpm));
    file_menu->Append(newGameItem);

    wxMenuItem *openFileItem = new wxMenuItem(file_menu, ID_GAME_OPEN, wxT("-"));
    openFileItem->SetBitmap(wxBitmap(menu_file_open_xpm));
    file_menu->Append(openFileItem);

    file_menu->Append(ID_GAME_MERGE, wxT("-"));

    wxMenuItem *saveFileItem = new wxMenuItem(file_menu, ID_GAME_SAVE, wxT("-"));
    saveFileItem->SetBitmap(wxBitmap(menu_file_save_xpm));
    file_menu->Append(saveFileItem);

    file_menu->Append(ID_GAME_SAVEAS, wxT("-"));

    file_menu->AppendSeparator();

    wxMenu *file_sub_exp_menu = new wxMenu;
    wxMenu *file_sub_imp_menu = new wxMenu;
    file_sub_exp_menu->Append(ID_GAME_EXPORTTXT, wxT("-"));
    file_sub_exp_menu->Append(ID_GAME_EXPORTTXT2GAM, wxT("-"));
    file_sub_imp_menu->Append(ID_GAME_IMPORTTXT2GAM, wxT("-"));

    file_menu->Append(ID_GAME_EXPORT, wxT("-"), file_sub_exp_menu);
    file_menu->Append(ID_GAME_IMPORT, wxT("-"), file_sub_imp_menu);
    file_menu->AppendSeparator();

    wxMenuItem *exitItem = new wxMenuItem(file_menu, ID_QGEN_EXIT, wxT("-"));
    exitItem->SetBitmap(wxBitmap(menu_exit_xpm));
    file_menu->Append(exitItem);

    wxMenu *util_menu = new wxMenu;

    wxMenuItem *playGameItem = new wxMenuItem(util_menu, ID_GAME_PLAY, wxT("-"));
    playGameItem->SetBitmap(wxBitmap(menu_game_play_xpm));
    util_menu->Append(playGameItem);

    wxMenuItem *searchTextItem = new wxMenuItem(util_menu, ID_UTIL_FIND, wxT("-"));
    searchTextItem->SetBitmap(wxBitmap(menu_text_search_xpm));
    util_menu->Append(searchTextItem);

    wxMenuItem *gameInfoItem = new wxMenuItem(util_menu, ID_UTIL_INF, wxT("-"));
    gameInfoItem->SetBitmap(wxBitmap(menu_game_info_xpm));
    util_menu->Append(gameInfoItem);

    util_menu->AppendSeparator();
    util_menu->Append(ID_UTIL_OPTIONS, wxT("-"));

    wxMenu *loc_menu = new wxMenu;
    loc_menu->Append(ID_LOC_CREATE, wxT("-"));
    loc_menu->Append(ID_LOC_RENAME, wxT("-"));
    loc_menu->Append(ID_LOC_DEL, wxT("-"));
    loc_menu->AppendSeparator();
    loc_menu->Append(ID_FOLDER_CREAT, wxT("-"));
    loc_menu->Append(ID_FOLDER_RENAME, wxT("-"));
    loc_menu->Append(ID_FOLDER_DEL, wxT("-"));
    loc_menu->AppendSeparator();
    loc_menu->Append(ID_LOC_COPY, wxT("-"));
    loc_menu->Append(ID_LOC_PASTE, wxT("-"));
    loc_menu->Append(ID_LOC_REPLACE, wxT("-"));
    loc_menu->Append(ID_LOC_PASTENEW, wxT("-"));
    loc_menu->Append(ID_LOC_CLEAR, wxT("-"));
    loc_menu->AppendSeparator();

    wxMenu *loc_action_sub_menu = new wxMenu;

    loc_action_sub_menu->Append(ID_ACTION_ADD, wxT("-"));
    loc_action_sub_menu->Append(ID_ACTION_REN, wxT("-"));
    loc_action_sub_menu->Append(ID_ACTION_DEL, wxT("-"));
    loc_action_sub_menu->AppendSeparator();
    loc_action_sub_menu->Append(ID_ACTION_DELALL, wxT("-"));
    loc_menu->Append(ID_ACTION_SUBMENU, wxT("-"), loc_action_sub_menu);

    loc_menu->AppendSeparator();

    loc_menu->Append(ID_LOC_SORTASC, wxT("-"));
    loc_menu->Append(ID_LOC_SORTDESC, wxT("-"));
    loc_menu->AppendSeparator();
    loc_menu->Append(ID_LOC_JUMPLOC, wxT("-"));

    wxMenu *text_menu = new wxMenu;

    wxMenuItem *undoTextItem = new wxMenuItem(text_menu, ID_TEXT_UNDO, wxT("-"));
    undoTextItem->SetBitmap(wxBitmap(menu_undo_xpm));
    text_menu->Append(undoTextItem);

    wxMenuItem *redoTextItem = new wxMenuItem(text_menu, ID_TEXT_REDO, wxT("-"));
    redoTextItem->SetBitmap(wxBitmap(menu_redo_xpm));
    text_menu->Append(redoTextItem);

    text_menu->AppendSeparator();

    wxMenuItem *cutTextItem = new wxMenuItem(text_menu, ID_TEXT_CUT, wxT("-"));
    cutTextItem->SetBitmap(wxBitmap(menu_text_cut_xpm));
    text_menu->Append(cutTextItem);

    wxMenuItem *copyTextItem = new wxMenuItem(text_menu, ID_TEXT_COPY, wxT("-"));
    copyTextItem->SetBitmap(wxBitmap(menu_text_copy_xpm));
    text_menu->Append(copyTextItem);

    wxMenuItem *pasteTextItem = new wxMenuItem(text_menu, ID_TEXT_PASTE, wxT("-"));
    pasteTextItem->SetBitmap(wxBitmap(menu_text_paste_xpm));
    text_menu->Append(pasteTextItem);

    wxMenuItem *deleteTextItem = new wxMenuItem(text_menu, ID_TEXT_DEL, wxT("-"));
    deleteTextItem->SetBitmap(wxBitmap(menu_text_delete_xpm));
    text_menu->Append(deleteTextItem);

    text_menu->AppendSeparator();
    text_menu->Append(ID_TEXT_SELALL, wxT("-"));

    wxMenu *list_controls = new wxMenu;
    list_controls->Append(ID_TOGGLE_TOOLBAR, wxT("-"), wxEmptyString, wxITEM_CHECK);
    list_controls->Append(ID_TOGGLE_LISTBOX, wxT("-"), wxEmptyString, wxITEM_CHECK);
    list_controls->Append(ID_TOGGLE_STATUSBAR, wxT("-"), wxEmptyString, wxITEM_CHECK);

    wxMenu *view_menu = new wxMenu;
    view_menu->Append(ID_TOGGLE, wxT("-"), list_controls);
    view_menu->AppendSeparator();
    view_menu->Append(ID_TAB_CLOSEALL, wxT("-"));
    view_menu->Append(ID_TAB_CLOSEEXCEPTSELECTED, wxT("-"));
    view_menu->Append(ID_TAB_CLOSESELECTED, wxT("-"));
    view_menu->AppendSeparator();
    view_menu->Append(ID_TAB_FIX, wxT("-"));
    view_menu->AppendSeparator();
    view_menu->Append(ID_LOC_DESCVISIBLE, wxT("-"));
    view_menu->Append(ID_LOC_ACTVISIBLE, wxT("-"));

    wxMenu *help_menu = new wxMenu;

    wxMenuItem *contentsHelpItem = new wxMenuItem(help_menu, ID_HELP, wxT("-"));
    contentsHelpItem->SetBitmap(wxBitmap(menu_help_xpm));
    help_menu->Append(contentsHelpItem);

    wxMenuItem *indexSearchHelpItem = new wxMenuItem(help_menu, ID_HELP_SEARCH, wxT("-"));
    indexSearchHelpItem->SetBitmap(wxBitmap(menu_help_search_xpm));
    help_menu->Append(indexSearchHelpItem);

    help_menu->Append(ID_HELP_CHECKUPDATES, wxT("-"));

    help_menu->AppendSeparator();
    help_menu->Append(ID_ABOUT, wxT("-"));

    _menu_bar = new wxMenuBar;
    _menu_bar->Append(file_menu, wxT("-"));
    _menu_bar->Append(util_menu, wxT("-"));
    _menu_bar->Append(loc_menu, wxT("-"));
    _menu_bar->Append(text_menu, wxT("-"));
    _menu_bar->Append(view_menu, wxT("-"));
    _menu_bar->Append(help_menu, wxT("-"));
    SetMenuBar(_menu_bar);
}

void MainFrame::CreateToolBar()
{
    _toolBar = new ToolBar(this, ID_TOOLBAR, _controls);
}

void MainFrame::Update(bool isFromObservable /*= false*/)
{
    wxMenuBar *menuBar = GetMenuBar();
    // Menu
    menuBar->SetMenuLabel(0, _("&Game"));
    menuBar->SetMenuLabel(1, _("&Utilities"));
    menuBar->SetMenuLabel(2, _("&Locations"));
    menuBar->SetMenuLabel(3, _("&Text"));
    menuBar->SetMenuLabel(4, _("&View"));
    menuBar->SetMenuLabel(5, _("&Help"));
    // Game
    menuBar->SetLabel(ID_GAME_NEW, _("&New\tCtrl+N"));
    menuBar->SetHelpString(ID_GAME_NEW, _("Create a new game"));
    menuBar->SetLabel(ID_GAME_OPEN, _("&Open...\tCtrl+O"));
    menuBar->SetHelpString(ID_GAME_OPEN, _("Open a game file"));
    menuBar->SetLabel(ID_GAME_MERGE, _("&Merge game...\tCtrl+M"));
    menuBar->SetHelpString(ID_GAME_MERGE, _("Add locations from another game"));
    menuBar->SetLabel(ID_GAME_SAVE, _("&Save\tCtrl+S"));
    menuBar->SetHelpString(ID_GAME_SAVE, _("Save the game"));
    menuBar->SetLabel(ID_GAME_SAVEAS, _("Save &as...\tCtrl+W"));
    menuBar->SetHelpString(ID_GAME_SAVEAS, _("Save the game into another file..."));
    menuBar->SetLabel(ID_GAME_EXPORT, _("&Export"));
    menuBar->SetLabel(ID_GAME_EXPORTTXT, _("Text file..."));
    menuBar->SetLabel(ID_GAME_EXPORTTXT2GAM,  _("Text file in TXT2GAM format..."));
    menuBar->SetLabel(ID_GAME_IMPORT, _("&Import"));
    menuBar->SetLabel(ID_GAME_IMPORTTXT2GAM,  _("Text file in TXT2GAM format..."));
    menuBar->SetLabel(ID_QGEN_EXIT, _("&Exit\tAlt+X"));
    menuBar->SetHelpString(ID_QGEN_EXIT, _("Close program"));
    // Utilities
    menuBar->SetLabel(ID_GAME_PLAY, _("&Run game\tF5"));
    menuBar->SetHelpString(ID_GAME_PLAY, _("Run the current game"));
    menuBar->SetLabel(ID_UTIL_FIND, _("&Find / Replace\tCtrl+F"));
    menuBar->SetHelpString(ID_UTIL_FIND, _("Find / replace some text"));
    menuBar->SetLabel(ID_UTIL_INF, _("&Game info\tCtrl+I"));
    menuBar->SetHelpString(ID_UTIL_INF, _("Show short statistics"));
    menuBar->SetLabel(ID_UTIL_OPTIONS, _("&Settings...\tCtrl+P"));
    // Locations
    menuBar->SetLabel(ID_LOC_CREATE, _("&Create...\tF7"));
    menuBar->SetHelpString(ID_LOC_CREATE, _("Create a location"));
    menuBar->SetLabel(ID_LOC_RENAME, _("&Rename...\tF6"));
    menuBar->SetHelpString(ID_LOC_RENAME,  _("Rename the location"));
    menuBar->SetLabel(ID_LOC_DEL, _("&Delete\tF8"));
    menuBar->SetHelpString(ID_LOC_DEL, _("Delete the location"));
    menuBar->SetLabel(ID_FOLDER_CREAT, _("Create folder..."));
    menuBar->SetHelpString(ID_FOLDER_CREAT, _("Create a folder for locations"));
    menuBar->SetLabel(ID_FOLDER_RENAME, _("Rename folder..."));
    menuBar->SetHelpString(ID_FOLDER_RENAME, _("Rename the selected folder"));
    menuBar->SetLabel(ID_FOLDER_DEL, _("Delete folder"));
    menuBar->SetHelpString(ID_FOLDER_DEL, _("Delete the folder, but keep locations"));
    menuBar->SetLabel(ID_LOC_COPY, _("&Copy\tCtrl+Shift+C"));
    menuBar->SetHelpString(ID_LOC_COPY, _("Copy the selected location to the clipboard"));
    menuBar->SetLabel(ID_LOC_PASTE, _("&Paste\tCtrl+Shift+V"));
    menuBar->SetHelpString(ID_LOC_PASTE, _("Paste location from the clipboard"));
    menuBar->SetLabel(ID_LOC_REPLACE, _("&Replace\tCtrl+Shift+R"));
    menuBar->SetHelpString(ID_LOC_REPLACE, _("Replace the selected location with the clipboard data"));
    menuBar->SetLabel(ID_LOC_PASTENEW, _("P&aste in...\tCtrl+Shift+N"));
    menuBar->SetHelpString(ID_LOC_PASTENEW, _("Paste the clipboard data to a new location"));
    menuBar->SetLabel(ID_LOC_CLEAR, _("C&lear\tCtrl+Shift+D"));
    menuBar->SetHelpString(ID_LOC_CLEAR, _("Clear the location"));
    menuBar->SetLabel(ID_ACTION_SUBMENU, _("&Actions"));
    menuBar->SetLabel(ID_ACTION_ADD, _("&Create...\tAlt+F7"));
    menuBar->SetHelpString(ID_ACTION_ADD, _("Create an action at the selected location"));
    menuBar->SetLabel(ID_ACTION_REN, _("&Rename...\tAlt+F6"));
    menuBar->SetHelpString(ID_ACTION_REN, _("Rename the selected action"));
    menuBar->SetLabel(ID_ACTION_DEL, _("&Delete\tAlt+F8"));
    menuBar->SetHelpString(ID_ACTION_DEL, _("Delete the selected action"));
    menuBar->SetLabel(ID_ACTION_DELALL, _("D&elete all\tAlt+F10"));
    menuBar->SetLabel(ID_LOC_SORTASC, _("So&rt ascending\tCtrl+Shift+O"));
    menuBar->SetLabel(ID_LOC_SORTDESC, _("Sor&t descending\tCtrl+Shift+P"));
    menuBar->SetLabel(ID_LOC_JUMPLOC, _("G&o to selected location\tCtrl+G"));
    // Text
    menuBar->SetLabel(ID_TEXT_UNDO, _("&Undo\tCtrl+Z"));
    menuBar->SetLabel(ID_TEXT_REDO, _("&Redo\tCtrl+Y"));
    menuBar->SetLabel(ID_TEXT_CUT, _("&Cut\tCtrl+X"));
    menuBar->SetLabel(ID_TEXT_COPY, _("C&opy\tCtrl+C"));
    menuBar->SetLabel(ID_TEXT_PASTE, _("&Paste\tCtrl+V"));
    menuBar->SetLabel(ID_TEXT_DEL, _("&Delete\tCtrl+D"));
    menuBar->SetLabel(ID_TEXT_SELALL, _("S&elect all\tCtrl+A"));
    // View
    menuBar->SetLabel(ID_TOGGLE, _("&Window list"));
    menuBar->SetLabel(ID_TOGGLE_TOOLBAR, _("&Toolbar\tAlt+Shift+T"));
    menuBar->SetLabel(ID_TOGGLE_LISTBOX, _("&Location list\tAlt+Shift+L"));
    menuBar->SetLabel(ID_TOGGLE_STATUSBAR, _("&Statusbar\tAlt+Shift+S"));
    menuBar->SetLabel(ID_TAB_CLOSEALL, _("&Close all tabs\tCtrl+Shift+F4"));
    menuBar->SetLabel(ID_TAB_CLOSEEXCEPTSELECTED, _("Close all tabs &except current"));
    menuBar->SetLabel(ID_TAB_CLOSESELECTED, _("Close c&urrent tab\tCtrl+F4"));
    menuBar->SetLabel(ID_TAB_FIX, _("Pin/Unpin &tab"));
    menuBar->SetLabel(ID_LOC_DESCVISIBLE, _("Show/Hide location &description\tAlt+Shift+D"));
    menuBar->SetLabel(ID_LOC_ACTVISIBLE, _("Show/Hide location &actions\tAlt+Shift+A"));
    // Help
    menuBar->SetLabel(ID_HELP, _("&Help\tF1"));
    menuBar->SetLabel(ID_HELP_SEARCH, _("Help by &keyword\tCtrl+F1"));
    menuBar->SetLabel(ID_HELP_CHECKUPDATES, _("Check for latest version"));
    menuBar->SetLabel(ID_ABOUT, _("&About..."));

    _manager.GetPane(_locListBox).Caption(_("Locations"));
    _manager.Update();
}

void MainFrame::OnInit(InitEvent &event)
{
    if (_controls->LoadGame(event.GetInitString()))
        UpdateTitle();
}

void MainFrame::OnExit(wxCommandEvent &WXUNUSED(event))
{
    Close();
}

void MainFrame::OnVersionRequestState(wxWebRequestEvent &event)
{
    switch (event.GetState())
    {
        case wxWebRequest::State_Completed:
            _controls->ProcessVersionResult(event.GetResponse().AsString(), event.GetId());
            break;
        case wxWebRequest::State_Failed:
        case wxWebRequest::State_Unauthorized:
            _controls->ProcessVersionResult(wxEmptyString, event.GetId());
            break;
    }
}

void MainFrame::OnNewGame(wxCommandEvent &WXUNUSED(event))
{
    if (CloseCurrentGame())
    {
        _controls->NewGame();
        UpdateTitle();
    }
}

void MainFrame::OnQuit(wxCloseEvent &event)
{
    if (CloseCurrentGame())
    {
        SaveLayout();
        event.Skip();
    }
}

void MainFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxAboutDialogInfo info;
    info.SetIcon(wxIcon(about_logo_xpm));
    info.SetName(QGEN_TITLE);
    info.SetCopyright(wxT("QSP Foundation, 2001-2025"));
    info.SetDescription(wxString::Format(_("Version: %s\nCompiled: %s\n\nUsed: %s"),
        QGEN_VER, wxT(__DATE__) wxT(", ") wxT(__TIME__), wxGetLibraryVersionInfo().ToString()));
    info.SetWebSite(wxT("https://qsp.org"));
    info.AddDeveloper(wxT("BaxZzZz [baxzzzz@gmail.com]"));
    info.AddDeveloper(wxT("Byte [byte@qsp.org]"));
    info.AddDeveloper(wxT("Nex [nex@otaku.ru]"));
    info.AddDeveloper(wxT("rrock.ru [rrock.ru@gmail.com]"));
    wxAboutBox(info);
}

void MainFrame::OnLoadFile(wxCommandEvent &WXUNUSED(event))
{
    if (CloseCurrentGame())
    {
        wxFileDialog dialog(this,
            _("Open game file"), wxEmptyString, wxEmptyString,
            _("QSP games (*.qsp;*.gam)|*.qsp;*.gam"), wxFD_OPEN);
        dialog.CenterOnParent();
        if (dialog.ShowModal() == wxID_OK)
        {
            if (_controls->LoadGame(dialog.GetPath()))
                UpdateTitle();
            else
                _controls->ShowMessage(QGEN_MSG_CANTLOADGAME);
        }
    }
}

void MainFrame::OnSaveQuestAs(wxCommandEvent &WXUNUSED(event))
{
    wxFileDialog dialog(this,
        _("Save game file"), wxEmptyString, wxT("game.qsp"),
        _("QSP games (*.qsp)|*.qsp"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    dialog.CenterOnParent();
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString password;
        wxPasswordEntryDialog dlgEntry(this,
            _("Enter the password:"),
            _("Game password"), wxEmptyString);
        if (dlgEntry.ShowModal() != wxID_CANCEL) password = dlgEntry.GetValue();
        if (password.IsEmpty()) password = QGEN_PASSWD;
        if (_controls->SaveGame(dialog.GetPath(), password))
            UpdateTitle();
        else
            _controls->ShowMessage(QGEN_MSG_CANTSAVEGAME);
    }
}

void MainFrame::OnSaveQuest(wxCommandEvent &WXUNUSED(event))
{
    wxCommandEvent dummy;
    if (!_controls->SaveGameWithCheck()) OnSaveQuestAs(dummy);
}

void MainFrame::CreateLocListBox()
{
    _locListBox = new LocationsListBox(this, ID_LOCSLIST, _controls);
}

void MainFrame::CreateNotebook()
{
    _locNotebook = new LocationsNotebook(this, wxID_ANY, _controls);
}

void MainFrame::LoadLayout()
{
    Settings *settings = _controls->GetSettings();
    SetSize(settings->GetLeftFramePos(), settings->GetTopFramePos(),
        settings->GetFrameWidth(), settings->GetFrameHeight());
    Maximize(settings->GetMaximizeFrame());
    wxStatusBar *statusBar = GetStatusBar();
    if (settings->GetShowStatusBar())
    {
        if (!statusBar) CreateStatusBar();
    }
    else
    {
        if (statusBar)
        {
            SetStatusBar(NULL);
            statusBar->Destroy();
        }
    }

    _manager.LoadPerspective(settings->GetPanelsConfig());
    wxAuiPaneInfo &locsListPane = _manager.GetPane(_locListBox);
    locsListPane.Caption(_("Locations"));
    _manager.RestoreMaximizedPane();
    _menu_bar->Check(ID_TOGGLE_TOOLBAR, _manager.GetPane(wxT("Toolbar")).IsShown());
    _menu_bar->Check(ID_TOGGLE_LISTBOX, locsListPane.IsShown());
    _menu_bar->Check(ID_TOGGLE_STATUSBAR, settings->GetShowStatusBar());
}

void MainFrame::SaveLayout()
{
    Settings *settings = _controls->GetSettings();
    if (IsMaximized())
    {
        settings->SetMaximizeFrame(true);
        Maximize(false);
    }
    else
        settings->SetMaximizeFrame(false);
    settings->SetShowStatusBar(GetStatusBar() != NULL);
    if (IsFullScreen()) ShowFullScreen(false);
    if (IsIconized()) Iconize(false);
    settings->SetPanelsConfig(_manager.SavePerspective());
    wxRect r = GetRect();
    settings->SetLeftFramePos(r.GetLeft());
    settings->SetTopFramePos(r.GetTop());
    settings->SetFrameWidth(r.GetWidth());
    settings->SetFrameHeight(r.GetHeight());
}

void MainFrame::TogglePaneVisibility(const wxString &pane_name)
{
    wxAuiPaneInfoArray& all_panes = _manager.GetAllPanes();
    size_t i, count;
    for (i = 0, count = all_panes.GetCount(); i < count; ++i)
    {
        if (all_panes.Item(i).name == pane_name)
        {
            all_panes.Item(i).Show(!all_panes.Item(i).IsShown());
            _manager.Update();
            break;
        }
    }
}

void MainFrame::OnPaneClose(wxAuiManagerEvent &event)
{
    switch (event.GetPane()->window->GetId())
    {
    case ID_LOCSLIST:
        _menu_bar->Check(ID_TOGGLE_LISTBOX, false);
        break;
    case ID_TOOLBAR:
        _menu_bar->Check(ID_TOGGLE_TOOLBAR, false);
        break;
    }
}

void MainFrame::OnToggleToolbar(wxCommandEvent &WXUNUSED(event))
{
    TogglePaneVisibility(wxT("Toolbar"));
    _manager.Update();
}

void MainFrame::OnToggleLocations(wxCommandEvent &WXUNUSED(event))
{
    TogglePaneVisibility(wxT("LocationList"));
    _manager.Update();
}

void MainFrame::OnToggleStatusbar(wxCommandEvent &WXUNUSED(event))
{
    wxStatusBar *statusBar = GetStatusBar();
    if (statusBar)
    {
        SetStatusBar(NULL);
        statusBar->Destroy();
    }
    else
        CreateStatusBar();
}

void MainFrame::OnFindDialog(wxCommandEvent &WXUNUSED(event))
{
    wxString word = _controls->GetSelectedWord();
    if (!_findDlg)
    {
        _findDlg = new SearchDialog(this, _("Find / Replace"), _controls, wxRESIZE_BORDER);
        _findDlg->CenterOnParent();
        _controls->InitSearchData();
    }
    if (!word.IsEmpty())
        _findDlg->SetTextToSearch(word);

    if (_findDlg->IsShown())
        _findDlg->SetFocus();
    else
        _findDlg->Show();
}

void MainFrame::OnCreateLocation(wxCommandEvent &WXUNUSED(event))
{
    _controls->AddLocation();
}

void MainFrame::OnDeleteLocation(wxCommandEvent &WXUNUSED(event))
{
    _controls->DeleteSelectedLocation();
}

void MainFrame::OnRenameLocation(wxCommandEvent &WXUNUSED(event))
{
    _controls->RenameSelectedLocation();
}

void MainFrame::OnClearLocation(wxCommandEvent &WXUNUSED(event))
{
    _controls->ClearSelectedLocation();
}

void MainFrame::OnCopyLocation(wxCommandEvent &WXUNUSED(event))
{
    _controls->CopySelectedLocToClipboard();
}

void MainFrame::OnPasteLocation(wxCommandEvent &event)
{
    switch (event.GetId())
    {
    case ID_LOC_REPLACE:
        _controls->PasteLocFromClipboard(PASTE_REPLACE);
        break;
    case ID_LOC_PASTE:
        _controls->PasteLocFromClipboard(PASTE_NEW_AUTO);
        break;
    case ID_LOC_PASTENEW:
        _controls->PasteLocFromClipboard(PASTE_NEW);
        break;
    }
}

void MainFrame::OnSortLocations(wxCommandEvent &event)
{
    _controls->SortLocations(event.GetId() == ID_LOC_SORTASC);
}

void MainFrame::OnCreateAction(wxCommandEvent &WXUNUSED(event))
{
    _controls->AddActionOnSelectedLoc();
}

void MainFrame::OnDeleteAction(wxCommandEvent &WXUNUSED(event))
{
    _controls->DeleteSelectedAction();
}

void MainFrame::OnDeleteAllActions(wxCommandEvent &WXUNUSED(event))
{
    _controls->DeleteAllActions();
}

void MainFrame::OnRenameAction(wxCommandEvent &WXUNUSED(event))
{
    _controls->RenameSelectedAction();
}

void MainFrame::OnUndoText(wxCommandEvent &WXUNUSED(event))
{
    _controls->UndoText();
}

void MainFrame::OnRedoText(wxCommandEvent &WXUNUSED(event))
{
    _controls->RedoText();
}

void MainFrame::OnCopyText(wxCommandEvent &WXUNUSED(event))
{
    _controls->CopySelectedTextToClipboard();
}

void MainFrame::OnCutText(wxCommandEvent &WXUNUSED(event))
{
    _controls->CutSelectedTextToClipboard();
}

void MainFrame::OnPasteText(wxCommandEvent &WXUNUSED(event))
{
    _controls->PasteTextFromClipboard();
}

void MainFrame::OnSelectAllText(wxCommandEvent &WXUNUSED(event))
{
    _controls->SelectAllText();
}

void MainFrame::OnDeleteText(wxCommandEvent &WXUNUSED(event))
{
    _controls->DeleteSelectedText();
}

void MainFrame::OnChmHelp(wxCommandEvent &WXUNUSED(event))
{
#ifdef __WXMSW__
    DesktopWindow desktop;
    wxCHMHelpController *chmHelp = new wxCHMHelpController(&desktop);
#else
    wxExtHelpController *chmHelp = new wxExtHelpController();
#endif
    if (_controls->SearchHelpFile())
    {
        chmHelp->LoadFile(_controls->GetSettings()->GetCurrentHelpPath());
        chmHelp->DisplayContents();
    }
    delete chmHelp;
}

void MainFrame::OnSearchHelp(wxCommandEvent &WXUNUSED(event))
{
#ifdef __WXMSW__
    DesktopWindow desktop;
    wxCHMHelpController *chmHelp = new wxCHMHelpController(&desktop);
#else
    wxExtHelpController *chmHelp = new wxExtHelpController();
#endif
    if (_controls->SearchHelpFile())
    {
        chmHelp->LoadFile(_controls->GetSettings()->GetCurrentHelpPath());
        chmHelp->KeywordSearch(_controls->GetSelectedWord());
    }
    delete chmHelp;
}

void MainFrame::OnCheckUpdates(wxCommandEvent &WXUNUSED(event))
{
    _controls->CheckLatestVersion(this, UPDATE_SHOW_ALL_RESULTS);
}

void MainFrame::OnMergeQuest(wxCommandEvent &WXUNUSED(event))
{
    wxFileDialog dialog(this,
        _("Merge game file"), wxEmptyString, wxEmptyString,
        _("QSP games (*.qsp;*.gam)|*.qsp;*.gam"), wxFD_OPEN);
    dialog.CenterOnParent();
    if (dialog.ShowModal() == wxID_OK)
    {
        if (!_controls->JoinGame(dialog.GetPath()))
            _controls->ShowMessage(QGEN_MSG_CANTLOADGAME);
    }
}

void MainFrame::OnPlayQuest(wxCommandEvent &WXUNUSED(event))
{
    wxCommandEvent dummy;
    Settings *settings = _controls->GetSettings();
    if (!wxFile::Exists(settings->GetCurrentPlayerPath()))
    {
        wxFileDialog dialog(this,
            _("Select player file"), wxEmptyString, wxEmptyString,
            _("Executables (*.exe)|*.exe|All files (*.*)|*"), wxFD_OPEN);
        dialog.CenterOnParent();
        if (dialog.ShowModal() == wxID_CANCEL) return;
        settings->SetCurrentPlayerPath(dialog.GetPath());
    }
    OnSaveQuest(dummy);
    if (_controls->IsGameSaved())
        wxExecute(wxString::Format("\"%s\" \"%s\"", settings->GetCurrentPlayerPath(), _controls->GetGamePath()));
}

void MainFrame::OnExportTxtFile(wxCommandEvent &WXUNUSED(event))
{
    wxFileDialog dialog(this,
        _("Save text file"), wxEmptyString, wxT("game.txt"),
        _("Text files (*.txt)|*.txt"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    dialog.CenterOnParent();
    if (dialog.ShowModal() == wxID_OK)
    {
        if (!_controls->ExportTxt(dialog.GetPath()))
            _controls->ShowMessage(QGEN_MSG_CANTSAVEGAME);
    }
}

void MainFrame::OnExportTxt2Gam(wxCommandEvent &WXUNUSED(event))
{
    wxFileDialog dialog(this,
        _("Save text file"), wxEmptyString, wxT("game.txt"),
        _("Text files (*.txt)|*.txt"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    dialog.CenterOnParent();
    if (dialog.ShowModal() == wxID_OK)
    {
        wxCommandEvent dummy;
        if (!SelectTxt2Gam()) return;
        OnSaveQuest(dummy);
        if (_controls->IsGameSaved())
        {
            if (!_controls->ExportTxt2Gam(dialog.GetPath()))
                _controls->ShowMessage(QGEN_MSG_CANTSAVEGAME);
        }
    }
}

void MainFrame::OnImportTxt2Gam(wxCommandEvent &WXUNUSED(event))
{
    if (!_controls->GetContainer()->IsEmpty())
    {
        wxMessageDialog dlgMsg(this,
            _("The new file will replace the current game file. Continue?"),
            _("Import text file"),
            wxYES_NO | wxCENTRE | wxICON_QUESTION);
        if (dlgMsg.ShowModal() == wxID_NO) return;
    }
    wxFileDialog dialog(this,
        _("Open text file"), wxEmptyString, wxEmptyString,
        _("Text files (*.txt)|*.txt"), wxFD_OPEN);
    dialog.CenterOnParent();
    if (dialog.ShowModal() == wxID_OK)
    {
        if (!SelectTxt2Gam()) return;
        if (!_controls->ImportTxt2Gam(dialog.GetPath()))
            _controls->ShowMessage(QGEN_MSG_CANTLOADGAME);
    }
}

void MainFrame::OnQuestInformation(wxCommandEvent &WXUNUSED(event))
{
    wxMessageDialog dialog(this, _controls->GetGameInfo(),
        _("Game statistics"), wxOK | wxCENTRE | wxICON_INFORMATION);
    dialog.ShowModal();
}

void MainFrame::OnOptionsDialog(wxCommandEvent &WXUNUSED(event))
{
    OptionsDialog dialog(this, _("Settings"), _controls);
    dialog.CenterOnParent();
    dialog.ShowModal();
}

void MainFrame::OnUpdateMenuItems(wxMenuEvent &event)
{
    _controls->UpdateMenuItems(event.GetMenu());
}

void MainFrame::OnTimerAutoSave(wxTimerEvent &WXUNUSED(event))
{
    if (!_controls->GetSettings()->GetAutoSave()) return;
    if (wxGetLocalTimeMillis() - _controls->GetLastSaveTime() >= (wxLongLong)_controls->GetSettings()->GetAutoSaveInterval() * 60000)
        _controls->SaveGameWithCheck();
}

void MainFrame::OnTimerUpdateToolBar(wxTimerEvent &WXUNUSED(event))
{
    bool isLocSelected = _controls->GetSelectedLocationIndex() >= 0;
    bool isSelLocNotEmpty = !_controls->IsSelectedLocationEmpty();
    bool isCanPlay = !_controls->GetContainer()->IsEmpty();
    bool isCanUndoText = _controls->CanUndoText();
    bool isCanRedoText = _controls->CanRedoText();
    bool isActsNotEmpty = !_controls->IsActionsOnSelectedLocEmpty();
    bool isLocsNotAllClosed = !_controls->IsAllLocsClosed();
    // ---------------------------------------------------
    _toolBar->EnableTool(ID_LOC_DEL, isLocSelected);
    _toolBar->EnableTool(ID_LOC_RENAME, isLocSelected);
    _toolBar->EnableTool(ID_LOC_COPY, isSelLocNotEmpty);
    _toolBar->EnableTool(ID_LOC_CLEAR, isSelLocNotEmpty);
    _toolBar->EnableTool(ID_ACTION_ADD, isLocsNotAllClosed);
    _toolBar->EnableTool(ID_ACTION_REN, isActsNotEmpty);
    _toolBar->EnableTool(ID_ACTION_DEL, isActsNotEmpty);
    _toolBar->EnableTool(ID_GAME_PLAY, isCanPlay);
    _toolBar->EnableTool(ID_GAME_SAVE, isCanPlay);
    _toolBar->EnableTool(ID_GAME_SAVEAS, isCanPlay);
    _toolBar->EnableTool(ID_TEXT_UNDO, isCanUndoText);
    _toolBar->EnableTool(ID_TEXT_REDO, isCanRedoText);
    _toolBar->Refresh();
    UpdateTitle();
}

bool MainFrame::SelectTxt2Gam()
{
    Settings *settings = _controls->GetSettings();
    if (!wxFile::Exists(settings->GetCurrentTxt2GamPath()))
    {
        wxFileDialog dialog(this,
            _("Select converter file"), wxEmptyString, wxEmptyString,
            _("Executables (*.exe)|*.exe|All files (*.*)|*"), wxFD_OPEN);
        dialog.CenterOnParent();
        if (dialog.ShowModal() == wxID_CANCEL) return false;
        settings->SetCurrentTxt2GamPath(dialog.GetPath());
    }
    return true;
}

bool MainFrame::CloseCurrentGame()
{
    wxCommandEvent dummy;
    if (!_controls->IsGameSaved())
    {
        wxMessageDialog dlgMsg(this,
            _("Do you want to save the game file?"),
            _("File was changed"),
            wxYES_NO | wxCANCEL | wxCENTRE | wxICON_QUESTION);
        switch (dlgMsg.ShowModal())
        {
        case wxID_YES:
            OnSaveQuest(dummy);
            return true;
        case wxID_NO:
            return true;
        }
        return false;
    }
    return true;
}

void MainFrame::UpdateTitle()
{
    wxString title;
    if (_controls->IsGameSaved())
        title.Printf(wxT("%s - %s %s"), _controls->GetGamePath(), QGEN_TITLE, QGEN_VER);
    else
        title.Printf(wxT("* %s - %s %s"), _controls->GetGamePath(), QGEN_TITLE, QGEN_VER);
    SetTitle(title);
}

void MainFrame::OnTabMenu(wxCommandEvent &event)
{
    CloseTypePage type = CLOSE_ALL;
    switch (event.GetId())
    {
    case ID_TAB_CLOSEALL:
        type = CLOSE_ALL_NONFIXED;
        break;
    case ID_TAB_CLOSEEXCEPTSELECTED:
        type = CLOSE_ALL_NONFIXED_EXCEPT_SELECTED;
        break;
    case ID_TAB_CLOSESELECTED:
        type = CLOSE_SELECTED;
        break;
    }
    _locNotebook->ClosePages(type);
}

void MainFrame::OnFixPage(wxCommandEvent &WXUNUSED(event))
{
    int selPage = _locNotebook->GetSelection();
    if (selPage >= 0) _locNotebook->SwitchPageFixed(selPage);
}

void MainFrame::OnLocDescVisible(wxCommandEvent &WXUNUSED(event))
{
    _controls->SwitchLocDesc();
}

void MainFrame::OnLocActsVisible(wxCommandEvent &WXUNUSED(event))
{
    _controls->SwitchLocActs();
}

void MainFrame::OnExpandLocation(wxCommandEvent &WXUNUSED(event))
{
    _locListBox->ExpandCollapseItems(true);
}

void MainFrame::OnCollapseLocation(wxCommandEvent &WXUNUSED(event))
{
    _locListBox->ExpandCollapseItems(false);
}

void MainFrame::OnJumpLocation(wxCommandEvent &WXUNUSED(event))
{
    _controls->JumpToSelectedLoc();
}

void MainFrame::OnKeyDown(wxKeyEvent &event)
{
    if (!_controls->ExecuteHotkey(event.GetKeyCode(), event.GetModifiers()))
        event.Skip();
}

void MainFrame::OnCreateFolder(wxCommandEvent &WXUNUSED(event))
{
    _controls->AddFolder();
}

void MainFrame::OnRenameFolder(wxCommandEvent &WXUNUSED(event))
{
    _controls->RenameSelectedFolder();
}

void MainFrame::OnDeleteFolder(wxCommandEvent &WXUNUSED(event))
{
    _controls->DeleteSelectedFolder();
}
