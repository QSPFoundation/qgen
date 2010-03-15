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

#include "QGen.h"

IMPLEMENT_APP(QGenApp)

BEGIN_EVENT_TABLE(QGenMainFrame, wxFrame)
	EVT_INIT(QGenMainFrame::OnInit)
	EVT_CLOSE(QGenMainFrame::OnQuit)
	EVT_AUI_PANE_CLOSE(QGenMainFrame::OnPaneClose)
	EVT_MENU(ID_QGEN_EXIT, QGenMainFrame::OnExit)
	EVT_MENU(ID_TOGGLE_TOOLBAR, QGenMainFrame::OnToggleToolbar)
	EVT_MENU(ID_TOGGLE_LISTBOX, QGenMainFrame::OnToggleLocations)
	EVT_MENU(ID_TOGGLE_STATUSBAR, QGenMainFrame::OnToggleStatusbar)
	EVT_MENU(ID_TAB_CLOSEALL, QGenMainFrame::OnTabMenu)
	EVT_MENU(ID_TAB_CLOSEEXCEPTSELECTED, QGenMainFrame::OnTabMenu)
	EVT_MENU(ID_TAB_CLOSESELECTED, QGenMainFrame::OnTabMenu)
	EVT_MENU(ID_TAB_FIX, QGenMainFrame::OnFixPage)
	EVT_MENU(ID_HELP, QGenMainFrame::OnChmHelp)
	EVT_MENU(ID_HELP_SEARCH, QGenMainFrame::OnSearchHelp)
	EVT_MENU(ID_FOLDER_CREAT, QGenMainFrame::OnCreateFolder)
	EVT_MENU(ID_FOLDER_RENAME, QGenMainFrame::OnRenameFolder)
	EVT_MENU(ID_FOLDER_DEL, QGenMainFrame::OnDeleteFolder)
	EVT_MENU(ID_LOC_DESCVISIBLE, QGenMainFrame::OnLocDescVisible)
	EVT_MENU(ID_LOC_ACTVISIBLE, QGenMainFrame::OnLocActsVisible)
	EVT_MENU(ID_LOC_CREATE, QGenMainFrame::OnCreateLocation)
	EVT_MENU(ID_LOC_DEL, QGenMainFrame::OnDeleteLocation)
	EVT_MENU(ID_LOC_RENAME, QGenMainFrame::OnRenameLocation)
	EVT_MENU(ID_LOC_CLEAR, QGenMainFrame::OnClearLocation)
	EVT_MENU(ID_LOC_COPY, QGenMainFrame::OnCopyLocation)
	EVT_MENU(ID_LOC_REPLACE, QGenMainFrame::OnPasteLocation)
	EVT_MENU(ID_LOC_PASTE, QGenMainFrame::OnPasteLocation)
	EVT_MENU(ID_LOC_PASTENEW, QGenMainFrame::OnPasteLocation)
	EVT_MENU(ID_LOC_SORTASC, QGenMainFrame::OnSortLocations)
	EVT_MENU(ID_LOC_SORTDESC, QGenMainFrame::OnSortLocations)
	EVT_MENU(ID_LOC_JUMPLOC, QGenMainFrame::OnJumpLocation)
	EVT_MENU(ID_LOC_EXPAND, QGenMainFrame::OnExpandLocation)
	EVT_MENU(ID_LOC_COLLAPSE, QGenMainFrame::OnCollapseLocation)
	EVT_MENU(ID_ACTION_ADD, QGenMainFrame::OnCreateAction)
	EVT_MENU(ID_ACTION_DEL, QGenMainFrame::OnDeleteAction)
	EVT_MENU(ID_ACTION_DELALL, QGenMainFrame::OnDeleteAllActions)
	EVT_MENU(ID_ACTION_REN, QGenMainFrame::OnRenameAction)
	EVT_MENU(ID_QUEST_NEW, QGenMainFrame::OnNewQuest)
	EVT_MENU(ID_QUEST_OPEN, QGenMainFrame::OnLoadFile)
	EVT_MENU(ID_QUEST_SAVE, QGenMainFrame::OnSaveQuest)
	EVT_MENU(ID_QUEST_SAVEAS, QGenMainFrame::OnSaveQuestAs)
	EVT_MENU(ID_QUEST_JOIN, QGenMainFrame::OnJoinQuest)
	EVT_MENU(ID_QUEST_PLAY, QGenMainFrame::OnPlayQuest)
	EVT_MENU(ID_UTIL_FIND, QGenMainFrame::OnFindDialog)
	EVT_MENU(ID_UTIL_INF, QGenMainFrame::OnInformationQuest)
	EVT_MENU(ID_UTIL_OPTIONS, QGenMainFrame::OnOptionsDialog)
	EVT_MENU(ID_TEXT_UNDO, QGenMainFrame::OnUndoText)
	EVT_MENU(ID_TEXT_REDO, QGenMainFrame::OnRedoText)
	EVT_MENU(ID_TEXT_COPY, QGenMainFrame::OnCopyText)
	EVT_MENU(ID_TEXT_PASTE, QGenMainFrame::OnPasteText)
	EVT_MENU(ID_TEXT_CUT, QGenMainFrame::OnCutText)
	EVT_MENU(ID_TEXT_DEL, QGenMainFrame::OnDeleteText)
	EVT_MENU(ID_TEXT_SELALL, QGenMainFrame::OnSelectAllText)
	EVT_MENU(ID_QUEST_EXPORTTXT, QGenMainFrame::OnExportTxtFile)
	EVT_MENU(ID_QUEST_EXPORTTXT2GAM, QGenMainFrame::OnExportTxt2Gam)
	EVT_MENU(ID_QUEST_IMPORT, QGenMainFrame::OnImportTxt2Gam)
	EVT_MENU(ID_ABOUT, QGenMainFrame::OnAbout)
	EVT_MENU_OPEN(QGenMainFrame::OnUpdMenuItems)
	EVT_TIMER(ID_TIMER_AUTO_SAVE, QGenMainFrame::OnTimerAutoSave)
	EVT_TIMER(ID_TIMER_UPD_TOOLBAR, QGenMainFrame::OnTimerUpdToolBar)
	EVT_KEY_DOWN(QGenMainFrame::OnKeyDown)
END_EVENT_TABLE()

bool QGenApp::OnInit()
{
	InitEvent initEvent;
	wxInitAllImageHandlers();
	wxFileName appPath(argv[0]);
	wxRegisterId(20000);
	appPath.MakeAbsolute();
	_controls = new Controls(appPath.GetPath());
	QGenMainFrame *mainFrame = new QGenMainFrame(_controls);
	_controls->SetMainFrame(mainFrame);
	mainFrame->Show();
	wxCmdLineParser cmdParser(argc, argv);
	if (argc > 1)
	{
		cmdParser.AddParam();
		cmdParser.Parse(false);
		wxFileName path(cmdParser.GetParam());
		path.MakeAbsolute();
		initEvent.SetInitString(path.GetFullPath());
		wxPostEvent(mainFrame, initEvent);
	}
	else
	{
		Settings *settings = _controls->GetSettings();
		if (settings->GetOpenLastGame())
		{
			initEvent.SetInitString(settings->GetLastGamePath());
			wxPostEvent(mainFrame, initEvent);
		}
	}
	return true;
}

int QGenApp::OnExit()
{
	Settings *settings = _controls->GetSettings();
	settings->SetLastGamePath(_controls->GetGamePath());
	settings->SaveSettings();
	delete _controls;
	return wxApp::OnExit();
}

QGenMainFrame::QGenMainFrame(Controls *controls) : 
	_timerAutoSave(this, ID_TIMER_AUTO_SAVE), 
	_timerUpdToolBar(this, ID_TIMER_UPD_TOOLBAR)
{
	_controls = controls;
	if (Create(wxSize(640, 480), wxDEFAULT_FRAME_STYLE)) OnNewQuest(wxCommandEvent());
	_timerAutoSave.Start(1000);
	_timerUpdToolBar.Start(500);
	_findDlg = NULL;
}

QGenMainFrame::~QGenMainFrame()
{
	_manager.UnInit();
}

bool QGenMainFrame::Create(const wxSize& size, long style)
{
	bool res = wxFrame::Create(NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, size, style);
	if (res)
	{
		SetIcon(wxICON(wxwin16x16));
		SetMinSize(wxSize(400, 300));
		CreateControls();
	}
	return res;
}

void QGenMainFrame::CreateControls()
{
	_manager.SetManagedWindow(this);
	CreateStatusBar();
	CreateMenuBar();
	CreateToolBar();
	CreateNotebook();
	CreateLocListBox();

	_manager.AddPane(_locNotebook, wxAuiPaneInfo().CenterPane().Name(wxT("LocationsNotebook")));

	_manager.AddPane(_toolBar, wxAuiPaneInfo().ToolbarPane().
		Name(wxT("Toolbar")).Top().LeftDockable(false).RightDockable(false).Floatable(true));

	_manager.AddPane(_locListBox, wxAuiPaneInfo().Left().
		Layer(1).PinButton().MinimizeButton().MaximizeButton().Name(wxT("LocationsList")).
		Caption(wxT("Список локаций")));

	LoadLayout();
	_manager.Update();
}

void QGenMainFrame::CreateStatusBar()
{
	SetStatusBar(new wxStatusBar(this));
	PositionStatusBar();
}

void QGenMainFrame::CreateMenuBar()
{
	wxMenu *file_menu = new wxMenu;
	wxMenuItem *newGameItem = new wxMenuItem(file_menu, ID_QUEST_NEW, wxT("&Новая\tCtrl+N"), wxT("Создать новую игру"));
	newGameItem->SetBitmap(wxBitmap(menu_game_new_xpm));
	file_menu->Append(newGameItem);

	wxMenuItem *openFileItem = new wxMenuItem(file_menu, ID_QUEST_OPEN, wxT("&Открыть...\tCtrl+O"), wxT("Открыть игру"));
	openFileItem->SetBitmap(wxBitmap(menu_file_open_xpm));
	file_menu->Append(openFileItem);

	file_menu->Append(ID_QUEST_JOIN, wxT("&Присоединить игру...\tCtrl+M"), wxT("Добавить локации другой игры"));

	wxMenuItem *saveFileItem = new wxMenuItem(file_menu, ID_QUEST_SAVE, wxT("&Сохранить\tCtrl+S"), wxT("Сохранить игру"));
	saveFileItem->SetBitmap(wxBitmap(menu_file_save_xpm));
	file_menu->Append(saveFileItem);

	file_menu->Append(ID_QUEST_SAVEAS, wxT("Сохранить &как...\tCtrl+W"), wxT("Сохранить игру под другим именем"));

	file_menu->AppendSeparator();

	wxMenu *file_sub_exp_menu = new wxMenu;
	wxMenu *file_sub_imp_menu = new wxMenu;
	file_sub_exp_menu->Append(ID_QUEST_EXPORTTXT, wxT("Текстовый файл..."));
	file_sub_exp_menu->Append(ID_QUEST_EXPORTTXT2GAM, wxT("Текстовый файл формата TXT2GAM..."));
	file_sub_imp_menu->Append(ID_QUEST_IMPORT, wxT("Текстовый файл формата TXT2GAM..."));

	file_menu->AppendSubMenu(file_sub_exp_menu, wxT("&Экспорт"));
	file_menu->AppendSubMenu(file_sub_imp_menu, wxT("&Импорт"));
	file_menu->AppendSeparator();

	wxMenuItem *exitItem = new wxMenuItem(file_menu, ID_QGEN_EXIT, wxT("&Выход\tAlt+X"), wxT("Выйти из программы"));
	exitItem->SetBitmap(wxBitmap(menu_exit_xpm));
	file_menu->Append(exitItem);

	wxMenu *util_menu = new wxMenu;

	wxMenuItem *playGameItem = new wxMenuItem(util_menu, ID_QUEST_PLAY, wxT("&Запуск игры\tF5"), wxT("Запустить текущую игру"));
	playGameItem->SetBitmap(wxBitmap(menu_game_play_xpm));
	util_menu->Append(playGameItem);

	wxMenuItem *searchTextItem = new wxMenuItem(util_menu, ID_UTIL_FIND, wxT("&Поиск / замена\tCtrl+F"), wxT("Поиск и замена текста"));
	searchTextItem->SetBitmap(wxBitmap(menu_text_search_xpm));
	util_menu->Append(searchTextItem);

	wxMenuItem *gameInfoItem = new wxMenuItem(util_menu, ID_UTIL_INF, wxT("&Информация об игре\tCtrl+I"), wxT("Показать краткую статистику"));
	gameInfoItem->SetBitmap(wxBitmap(menu_game_info_xpm));
	util_menu->Append(gameInfoItem);

	util_menu->AppendSeparator();
	util_menu->Append(ID_UTIL_OPTIONS, wxT("&Настройки...\tCtrl+P"));

	wxMenu *loc_menu = new wxMenu;
	loc_menu->Append(ID_LOC_CREATE, wxT("&Создать...\tF7"), wxT("Создать локацию"));
	loc_menu->Append(ID_LOC_RENAME, wxT("&Переименовать...\tF6"), wxT("Переименовать локацию"));
	loc_menu->Append(ID_LOC_DEL, wxT("&Удалить\tF8"), wxT("Удалить локацию"));
	loc_menu->AppendSeparator();
	loc_menu->Append(ID_FOLDER_CREAT, wxT("Создать папку..."), wxT("Создать папку для локаций"));
	loc_menu->Append(ID_FOLDER_RENAME, wxT("Переименовать папку..."), wxT("Переименовать выбранную папку"));
	loc_menu->Append(ID_FOLDER_DEL, wxT("Удалить папку"), wxT("Удалить папку, но сохранить локации"));
	loc_menu->AppendSeparator();
	loc_menu->Append(ID_LOC_COPY, wxT("&Копировать\tCtrl+Shift+C"), wxT("Копировать локацию в буфер"));
	loc_menu->Append(ID_LOC_PASTE, wxT("&Вставить\tCtrl+Shift+V"), wxT("Вставить локацию из буфера"));
	loc_menu->Append(ID_LOC_REPLACE, wxT("&Заменить\tCtrl+Shift+R"), wxT("Заменить выбранную локацию локацией из буфера"));
	loc_menu->Append(ID_LOC_PASTENEW, wxT("Вс&тавить в...\tCtrl+Shift+N"), wxT("Вставить локацию из буфера в новую локацию"));
	loc_menu->Append(ID_LOC_CLEAR, wxT("&Очистить\tCtrl+Shift+D"), wxT("Очистить локацию"));
	loc_menu->AppendSeparator();

	wxMenu *loc_action_sub_menu = new wxMenu;

	loc_action_sub_menu->Append(ID_ACTION_ADD, wxT("&Создать...\tAlt+F7"), wxT("Создать действие на локации"));
	loc_action_sub_menu->Append(ID_ACTION_REN, wxT("&Переименовать...\tAlt+F6"), wxT("Переименовать действие"));
	loc_action_sub_menu->Append(ID_ACTION_DEL, wxT("&Удалить\tAlt+F8"), wxT("Удалить действие"));
	loc_action_sub_menu->AppendSeparator();
	loc_action_sub_menu->Append(ID_ACTION_DELALL, wxT("У&далить все\tAlt+F10"));
	loc_menu->Append(ID_ACTION_SUBMENU, wxT("&Действия"), loc_action_sub_menu);

	loc_menu->AppendSeparator();

	loc_menu->Append(ID_LOC_SORTASC, wxT("Со&ртировать по алфавиту\tCtrl+Shift+O"));
	loc_menu->Append(ID_LOC_SORTDESC, wxT("Сор&тировать в обратном порядке\tCtrl+Shift+P"));
	loc_menu->AppendSeparator();
	loc_menu->Append(ID_LOC_JUMPLOC, wxT("П&ерейти на выбранную локацию\tCtrl+G"));

	wxMenu *text_menu = new wxMenu;

	wxMenuItem *undoTextItem = new wxMenuItem(text_menu, ID_TEXT_UNDO, wxT("&Отменить\tCtrl+Z"));
	undoTextItem->SetBitmap(wxBitmap(menu_undo_xpm));
	text_menu->Append(undoTextItem);

	wxMenuItem *redoTextItem = new wxMenuItem(text_menu, ID_TEXT_REDO, wxT("&Повторить\tCtrl+Y"));
	redoTextItem->SetBitmap(wxBitmap(menu_redo_xpm));
	text_menu->Append(redoTextItem);

	text_menu->AppendSeparator();

	wxMenuItem *cutTextItem = new wxMenuItem(text_menu, ID_TEXT_CUT, wxT("&Вырезать\tCtrl+X"));
	cutTextItem->SetBitmap(wxBitmap(menu_text_cut_xpm));
	text_menu->Append(cutTextItem);

	wxMenuItem *copyTextItem = new wxMenuItem(text_menu, ID_TEXT_COPY, wxT("&Копировать\tCtrl+C"));
	copyTextItem->SetBitmap(wxBitmap(menu_text_copy_xpm));
	text_menu->Append(copyTextItem);

	wxMenuItem *pasteTextItem = new wxMenuItem(text_menu, ID_TEXT_PASTE, wxT("В&ставить\tCtrl+V"));
	pasteTextItem->SetBitmap(wxBitmap(menu_text_paste_xpm));
	text_menu->Append(pasteTextItem);

	wxMenuItem *deleteTextItem = new wxMenuItem(text_menu, ID_TEXT_DEL, wxT("&Удалить\tCtrl+D"));
	deleteTextItem->SetBitmap(wxBitmap(menu_text_delete_xpm));
	text_menu->Append(deleteTextItem);

	text_menu->AppendSeparator();
	text_menu->Append(ID_TEXT_SELALL, wxT("В&ыделить всё\tCtrl+A"));

	wxMenu *list_controls= new wxMenu;
	list_controls->Append(ID_TOGGLE_TOOLBAR, wxT("&Панель инструментов"), wxEmptyString, wxITEM_CHECK);
	list_controls->Append(ID_TOGGLE_LISTBOX, wxT("&Окно списка локаций"), wxEmptyString, wxITEM_CHECK);
	list_controls->Append(ID_TOGGLE_STATUSBAR, wxT("&Строка состояния"), wxEmptyString, wxITEM_CHECK);

	wxMenu *view_menu= new wxMenu;
	view_menu->Append(wxID_ANY, wxT("&Список окон"), list_controls);
	view_menu->AppendSeparator();
	view_menu->Append(ID_TAB_CLOSEALL, wxT("&Закрыть все вкладки\tCtrl+Alt+F4"));
	view_menu->Append(ID_TAB_CLOSEEXCEPTSELECTED, wxT("Закрыть все &кроме текущей"));
	view_menu->Append(ID_TAB_CLOSESELECTED, wxT("Закрыть &текущую\tCtrl+F4"));
	view_menu->AppendSeparator();
	view_menu->Append(ID_TAB_FIX, wxT("Закрепить/открепить &вкладку"));
	view_menu->AppendSeparator();
	view_menu->Append(ID_LOC_DESCVISIBLE, wxT("Показать/скрыть &описание локации\tCtrl+Alt+D"));
	view_menu->Append(ID_LOC_ACTVISIBLE, wxT("Показать/скрыть &действия локации\tCtrl+Alt+A"));

	wxMenu *help_menu = new wxMenu;

	wxMenuItem *contentsHelpItem = new wxMenuItem(help_menu, ID_HELP, wxT("&Помощь\tF1"));
	contentsHelpItem->SetBitmap(wxBitmap(menu_help_xpm));
	help_menu->Append(contentsHelpItem);

	wxMenuItem *indexSearchHelpItem = new wxMenuItem(help_menu, ID_HELP_SEARCH, wxT("Помощь по ключевому &слову\tCtrl+F1"));
	indexSearchHelpItem->SetBitmap(wxBitmap(menu_help_search_xpm));
	help_menu->Append(indexSearchHelpItem);

	help_menu->AppendSeparator();
	help_menu->Append(ID_ABOUT, wxT("&О программе..."));

	_menu_bar = new wxMenuBar;
	_menu_bar->Append(file_menu, wxT("&Игра"));
	_menu_bar->Append(util_menu, wxT("&Утилиты"));
	_menu_bar->Append(loc_menu, wxT("&Локации"));
	_menu_bar->Append(text_menu, wxT("&Текст"));
	_menu_bar->Append(view_menu, wxT("&Вид"));
	_menu_bar->Append(help_menu, wxT("&Помощь"));
	SetMenuBar(_menu_bar);
}

void QGenMainFrame::CreateToolBar()
{
	_toolBar = new QGenToolBar(this, ID_TOOLBAR, _controls);
	_toolBar->SetToolBitmapSize(wxSize(24, 24));
	_toolBar->AddTool(ID_LOC_CREATE, wxT("New location..."), wxBitmap(toolbar_location_new_xpm), wxT("Создать локацию... (F7)"));
	_toolBar->AddTool(ID_LOC_RENAME, wxT("Rename location..."), wxBitmap(toolbar_location_rename_xpm), wxT("Переименовать выделенную локацию... (F6)"));
	_toolBar->AddTool(ID_LOC_DEL, wxT("Delete location"), wxBitmap(toolbar_location_delete_xpm), wxT("Удалить выделенную локацию (F8)"));
	_toolBar->AddSeparator();
	_toolBar->AddTool(ID_QUEST_OPEN, wxT("Open..."), wxBitmap(toolbar_file_open_xpm), wxT("Открыть игру... (Ctrl+O)"));
	_toolBar->AddTool(ID_QUEST_SAVE, wxT("Save"), wxBitmap(toolbar_file_save_xpm), wxT("Сохранить игру (Ctrl+S)"));
	_toolBar->AddTool(ID_QUEST_SAVEAS, wxT("Save as..."), wxBitmap(toolbar_file_saveas_xpm), wxT("Сохранить игру как... (Ctrl+W)"));
	_toolBar->AddSeparator();
	_toolBar->AddTool(ID_QUEST_PLAY, wxT("Play"), wxBitmap(toolbar_game_play_xpm), wxT("Запуск игры (F5)"));
	_toolBar->AddTool(ID_UTIL_INF, wxT("Information"), wxBitmap(toolbar_game_info_xpm), wxT("Краткая информация об игре (Ctrl+I)"));
	_toolBar->AddSeparator();
	_toolBar->AddTool(ID_TEXT_UNDO, wxT("Undo text"), wxBitmap(toolbar_undo_xpm), wxT("Отменить (Ctrl+Z)"));
	_toolBar->AddTool(ID_TEXT_REDO, wxT("Redo text"), wxBitmap(toolbar_redo_xpm), wxT("Повторить (Ctrl+Y)"));
	_toolBar->AddSeparator();
	_toolBar->AddTool(ID_LOC_COPY, wxT("Copy location"), wxBitmap(toolbar_location_copy_xpm), wxT("Копировать выделенную локацию (Ctrl+Shift+C)"));
	_toolBar->AddTool(ID_LOC_PASTE, wxT("Paste location"), wxBitmap(toolbar_location_paste_xpm), wxT("Вставить локацию (Ctrl+Shift+V)"));
	_toolBar->AddTool(ID_LOC_CLEAR, wxT("Clear location"), wxBitmap(toolbar_location_clear_xpm), wxT("Очистить выделенную локацию (Ctrl+Shift+D)"));
	_toolBar->AddSeparator();
	_toolBar->AddTool(ID_UTIL_FIND, wxT("Search..."), wxBitmap(toolbar_text_search_xpm), wxT("Поиск / замена... (Ctrl+F)"));
	_toolBar->AddTool(ID_UTIL_OPTIONS, wxT("Options..."), wxBitmap(toolbar_options_xpm), wxT("Настройки... (Ctrl+P)"));
	_toolBar->Realize();
}

void QGenMainFrame::OnInit( InitEvent &event )
{
	if (_controls->LoadGame(event.GetInitString())) UpdateTitle();
}

void QGenMainFrame::OnExit( wxCommandEvent &event )
{
	Close();
}

void QGenMainFrame::OnNewQuest( wxCommandEvent &event )
{
	if (QuestChange())
	{
		_controls->NewGame();
		UpdateTitle();
	}
}

void QGenMainFrame::OnQuit( wxCloseEvent &event )
{
	if (QuestChange())
	{
		SaveLayout();
		event.Skip();
	}
}

void QGenMainFrame::OnAbout( wxCommandEvent &event )
{
	wxAboutDialogInfo info;
	info.SetIcon(wxIcon(about_logo_xpm));
	info.SetName(QGEN_TITLE);
	info.SetCopyright(wxT("Quest Soft, 2001-2009"));
	info.SetDescription(wxString::Format( _("Version: %s\nCompiled: %s"),
		QGEN_VER, wxT(__DATE__) wxT(", ") wxT(__TIME__)));
	info.SetWebSite(wxT("http://qsp.su"));
	info.AddDeveloper(wxT("BaxZzZz [baxzzzz@gmail.com]"));
	info.AddDeveloper(wxT("Byte [nporep@mail.ru]"));
	info.AddDeveloper(wxT("Nex [nex@otaku.ru]"));
	info.AddDeveloper(wxT("rrock.ru [rrock.ru@gmail.com]"));
	wxAboutBox(info);
}

void QGenMainFrame::OnLoadFile(wxCommandEvent &event)
{
	if (QuestChange())
	{
		wxFileDialog dialog(this, wxT("Открыть файл"), wxEmptyString, wxEmptyString,
			wxT("Файлы игр QSP (*.qsp;*.gam)|*.qsp;*.gam"), wxFD_OPEN);
		dialog.CenterOnParent();
		if (dialog.ShowModal() == wxID_OK)
		{
			if (_controls->LoadGame(dialog.GetPath()))
				UpdateTitle();
		}
	}
}

void QGenMainFrame::OnSaveQuestAs(wxCommandEvent &event)
{
	wxFileDialog dialog(this, wxT("Сохранить файл"), wxEmptyString, wxEmptyString,
		_("Файлы игр QSP (*.qsp)|*.qsp"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	dialog.CenterOnParent();
	if (dialog.ShowModal() == wxID_OK)
	{
		wxString password;
		wxTextEntryDialog dlgEntry(this, wxT("Введите пароль:"),
			wxT("Ввод пароля"), wxEmptyString, wxOK|wxCANCEL|wxTE_PASSWORD );
		dlgEntry.CenterOnParent();
		if (dlgEntry.ShowModal() != wxID_CANCEL) password = dlgEntry.GetValue();
		if (password.IsEmpty()) password = QGEN_PASSWD;
		if (_controls->SaveGame(dialog.GetPath(), password))
			UpdateTitle();
		else
			_controls->ShowMessage(QGEN_MSG_CANTSAVEGAME);
	}
}

void QGenMainFrame::OnSaveQuest( wxCommandEvent &event )
{
	if (!_controls->SaveGameWithCheck()) OnSaveQuestAs(wxCommandEvent());
}

void QGenMainFrame::CreateLocListBox()
{
	_locListBox = new LocationsListBox(this, ID_LOCSLIST, _controls);
	_controls->SetLocListBox(_locListBox);
}

void QGenMainFrame::CreateNotebook()
{
	_locNotebook = new LocationsNotebook(this, wxID_ANY, _controls);
	_controls->SetNotebook(_locNotebook);
}

void QGenMainFrame::LoadLayout()
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
	_manager.LoadPerspective(settings->GetSetPanelsPos());
	_manager.RestoreMaximizedPane();
	_menu_bar->Check(ID_TOGGLE_TOOLBAR, _manager.GetPane(wxT("Toolbar")).IsShown());
	_menu_bar->Check(ID_TOGGLE_LISTBOX, _manager.GetPane(wxT("LocationsList")).IsShown());
	_menu_bar->Check(ID_TOGGLE_STATUSBAR, settings->GetShowStatusBar());
}

void QGenMainFrame::SaveLayout()
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
	settings->SetPanelsPos(_manager.SavePerspective());
	wxRect r = GetRect();
	settings->SetLeftFramePos( r.GetLeft());
	settings->SetTopFramePos(r.GetTop());
	settings->SetFrameWidth(r.GetWidth());
	settings->SetFrameHeight(r.GetHeight());
}

void QGenMainFrame::TogglePaneVisibility(wxString pane_name)
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

void QGenMainFrame::OnPaneClose( wxAuiManagerEvent& event )
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

void QGenMainFrame::OnToggleToolbar(wxCommandEvent &event)
{
	TogglePaneVisibility(wxT("Toolbar"));
	_manager.Update();
}

void QGenMainFrame::OnToggleLocations(wxCommandEvent &event)
{
	TogglePaneVisibility(wxT("LocationsList"));
	_manager.Update();
}

void QGenMainFrame::OnToggleStatusbar(wxCommandEvent &event)
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

void QGenMainFrame::OnFindDialog( wxCommandEvent& event )
{
	if (!_findDlg)
	{
		_findDlg = new SearchDialog(this, wxT("Поиск / замена"), _controls, wxRESIZE_BORDER);
		_findDlg->CenterOnParent();
		_controls->InitSearchData();
	}
	if (_findDlg->IsShown())
		_findDlg->SetFocus();
	else
		_findDlg->Show();
}

void QGenMainFrame::OnCreateLocation( wxCommandEvent &event )
{
	_controls->AddLocation();
}

void QGenMainFrame::OnDeleteLocation( wxCommandEvent &event )
{
	_controls->DeleteSelectedLocation();
}

void QGenMainFrame::OnRenameLocation( wxCommandEvent &event )
{
	_controls->RenameSelectedLocation();
}

void QGenMainFrame::OnClearLocation( wxCommandEvent &event )
{
	_controls->ClearSelectedLocation();
}

void QGenMainFrame::OnCopyLocation( wxCommandEvent &event )
{
	_controls->CopySelectedLocToClipboard();
}

void QGenMainFrame::OnPasteLocation( wxCommandEvent &event )
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

void QGenMainFrame::OnSortLocations( wxCommandEvent &event )
{
	_controls->SortLocations(event.GetId() == ID_LOC_SORTASC);
}

void QGenMainFrame::OnCreateAction( wxCommandEvent &event )
{
	_controls->AddActionOnSelectedLoc();
}

void QGenMainFrame::OnDeleteAction( wxCommandEvent &event )
{
	_controls->DeleteSelectedAction();
}

void QGenMainFrame::OnDeleteAllActions( wxCommandEvent &event )
{
	_controls->DeleteAllActions();
}

void QGenMainFrame::OnRenameAction( wxCommandEvent &event )
{
	_controls->RenameSelectedAction();
}

void QGenMainFrame::OnUndoText( wxCommandEvent &event )
{
	_controls->UndoText();
}

void QGenMainFrame::OnRedoText( wxCommandEvent &event )
{
	_controls->RedoText();
}

void QGenMainFrame::OnCopyText( wxCommandEvent &event )
{
	_controls->CopySelectedTextToClipboard();
}

void QGenMainFrame::OnCutText( wxCommandEvent &event )
{
	_controls->CutSelectedTextToClipboard();
}

void QGenMainFrame::OnPasteText( wxCommandEvent &event )
{
	_controls->PasteTextFromClipboard();
}

void QGenMainFrame::OnSelectAllText( wxCommandEvent &event )
{
	_controls->SelectAllText();
}

void QGenMainFrame::OnDeleteText( wxCommandEvent &event )
{
	_controls->DeleteSelectedText();
}

void QGenMainFrame::OnJoinQuest( wxCommandEvent &event )
{
	wxFileDialog dialog(this, wxT("Открыть файл"), wxEmptyString, wxEmptyString,
		wxT("Файлы игр QSP (*.qsp;*.gam)|*.qsp;*.gam"), wxFD_OPEN);
	dialog.CenterOnParent();
	if (dialog.ShowModal() == wxID_OK)
	_controls->JoinGame(dialog.GetPath());
}

void QGenMainFrame::OnPlayQuest( wxCommandEvent &event )
{
	Settings *settings = _controls->GetSettings();
	if (!wxFile::Exists(settings->GetCurrentPlayerPath()))
	{
		wxFileDialog dialog(this, wxT("Выберите файл плеера"), wxEmptyString, wxEmptyString,
			wxT("Файл плеера (*.exe)|*.exe|Все файлы (*.*)|*.*"), wxFD_OPEN);
		dialog.CenterOnParent();
		if (dialog.ShowModal() == wxID_CANCEL) return;
		settings->SetCurrentPlayerPath(dialog.GetPath());
	}
	OnSaveQuest(wxCommandEvent());
	if (_controls->IsGameSaved())
		wxExecute(wxString::Format("\"%s\" \"%s\"", settings->GetCurrentPlayerPath(), _controls->GetGamePath()));
}

void QGenMainFrame::OnChmHelp( wxCommandEvent &event )
{
	#ifdef __WXMSW__
		DesktopWindow desktop;
		wxCHMHelpController *chmHelp = new wxCHMHelpController(&desktop);
	#else
		wxCHMHelpController *chmHelp = new wxCHMHelpController();
	#endif
	if (_controls->SearchHelpFile())
	{
		chmHelp->LoadFile(_controls->GetSettings()->GetCurrentHelpPath());
		chmHelp->DisplayContents();
	}
	delete chmHelp;
}

void QGenMainFrame::OnSearchHelp( wxCommandEvent &event )
{
	#ifdef __WXMSW__
		DesktopWindow desktop;
		wxCHMHelpController *chmHelp = new wxCHMHelpController(&desktop);
	#else
		wxCHMHelpController *chmHelp = new wxCHMHelpController();
	#endif
	if (_controls->SearchHelpFile())
	{
		chmHelp->LoadFile(_controls->GetSettings()->GetCurrentHelpPath());
		chmHelp->KeywordSearch(_controls->GetSelectedWord());
	}
	delete chmHelp;
}

void QGenMainFrame::OnExportTxtFile( wxCommandEvent &event )
{
	wxFileDialog dialog(this, wxT("Сохранить файл"), wxEmptyString, wxEmptyString,
		_("Текстовые файлы (*.txt)|*.txt"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	dialog.CenterOnParent();

	if (dialog.ShowModal() == wxID_OK)
	{
		if (!_controls->ExportTxt(dialog.GetPath()))
			_controls->ShowMessage(QGEN_MSG_CANTSAVEGAME);
	}
}

void QGenMainFrame::OnExportTxt2Gam( wxCommandEvent &event )
{
	wxFileDialog dialog(this, wxT("Сохранить файл"), wxEmptyString, wxEmptyString,
		_("Текстовые файлы (*.txt)|*.txt"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	dialog.CenterOnParent();

	if (dialog.ShowModal() == wxID_OK)
	{
		if (!_controls->ExportTxt2Gam(dialog.GetPath()))
			_controls->ShowMessage(QGEN_MSG_CANTSAVEGAME);
	}
}

void QGenMainFrame::OnImportTxt2Gam( wxCommandEvent &event )
{
	if (!_controls->GetContainer()->IsEmpty())
	{
		wxMessageDialog dlgMsg(this, wxT("Импортируемый файл заменит текущий файл игры. Продолжить?"), wxT("Импорт"),
			wxYES_NO|wxICON_QUESTION, wxDefaultPosition);
		dlgMsg.CenterOnParent();
		if (dlgMsg.ShowModal() == wxID_NO)
			return;
	}
	wxFileDialog dialog(this, wxT("Открыть файл"), wxEmptyString, wxEmptyString,
		wxT("Текстовые файлы (*.txt)|*.txt"), wxFD_OPEN);
	dialog.CenterOnParent();
	if (dialog.ShowModal() == wxID_OK)
	{
		Settings *settings = _controls->GetSettings();
		if (!wxFile::Exists(settings->GetCurrentTxt2GamPath()))
		{
			wxFileDialog dialog2(this, wxT("Выберите файл"), wxEmptyString, wxEmptyString,
				wxT("Файл конвертора (*.exe)|*.exe|Все файлы (*.*)|*.*"), wxFD_OPEN);
			dialog2.CenterOnParent();
			if (dialog2.ShowModal() == wxID_CANCEL) return;
			settings->SetCurrentTxt2GamPath(dialog2.GetPath());
		}
		_controls->ImportTxt2Gam(dialog.GetPath());
	}
}

void QGenMainFrame::OnInformationQuest( wxCommandEvent &event )
{
	wxMessageDialog dialog(this, _controls->GetGameInfo(), wxT("Информация об игре"), wxOK|wxICON_INFORMATION);
	dialog.CenterOnParent();
	dialog.ShowModal();
}

void QGenMainFrame::OnOptionsDialog( wxCommandEvent &event )
{
	OptionsDialog dialog(this, wxT("Настройки"), _controls);
	dialog.CenterOnParent();
	dialog.ShowModal();
}

void QGenMainFrame::OnUpdMenuItems(wxMenuEvent& event)
{
	_controls->UpdateMenuItems(event.GetMenu());
}

void QGenMainFrame::OnTimerAutoSave(wxTimerEvent &event)
{
	if (!_controls->GetSettings()->GetAutoSave()) return;
	if (wxGetLocalTimeMillis() - _controls->GetLastSaveTime() >= (wxLongLong)_controls->GetSettings()->GetAutoSaveInterval() * 60000)
		_controls->SaveGameWithCheck();
}

void QGenMainFrame::OnTimerUpdToolBar(wxTimerEvent &event)
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
	_toolBar->EnableTool(ID_QUEST_PLAY, isCanPlay);
	_toolBar->EnableTool(ID_QUEST_SAVE, isCanPlay);
	_toolBar->EnableTool(ID_QUEST_SAVEAS, isCanPlay);
	_toolBar->EnableTool(ID_TEXT_UNDO, isCanUndoText);
	_toolBar->EnableTool(ID_TEXT_REDO, isCanRedoText);
	_toolBar->Realize();
}

bool QGenMainFrame::QuestChange()
{
	if (!_controls->IsGameSaved())
	{
		wxMessageDialog dlgMsg(this, wxT("Желаете сохранить файл?"), wxT("Файл был изменён"),
			wxYES_NO|wxCANCEL|wxICON_QUESTION, wxDefaultPosition);
		dlgMsg.CenterOnParent();
		switch (dlgMsg.ShowModal())
		{
		case wxID_YES:
			OnSaveQuest(wxCommandEvent());
			return true;
		case wxID_NO:
			return true;
		}
		return false;
	}
	return true;
}

void QGenMainFrame::UpdateTitle()
{
	wxString title;
	title.Printf(wxT("%s - %s %s"), _controls->GetGamePath(), QGEN_TITLE, QGEN_VER);
	SetTitle(title);
}

void QGenMainFrame::OnTabMenu( wxCommandEvent &event )
{
	CloseTypePage type;
	int selPage = _locNotebook->GetSelection();
	if (selPage < 0) return;
	switch (event.GetId())
	{
	case ID_TAB_CLOSEALL:
		type = CLOSE_ALL;
		break;
	case ID_TAB_CLOSEEXCEPTSELECTED:
		type = CLOSE_ALLEXCEPTSELECTED;
		break;
	case ID_TAB_CLOSESELECTED:
		type = CLOSE_SELECTED;
		break;
	}
	_locNotebook->DeleteAllPages(type, selPage);
}

void QGenMainFrame::OnFixPage( wxCommandEvent &event )
{
	int selPage = _locNotebook->GetSelection();
	if (selPage < 0) return;
	_locNotebook->SwitchPageFixed(selPage);
}

void QGenMainFrame::OnLocDescVisible(wxCommandEvent &event)
{
	_controls->SwitchLocDesc();
}

void QGenMainFrame::OnLocActsVisible(wxCommandEvent &event)
{
	_controls->SwitchLocActs();
}

void QGenMainFrame::OnExpandLocation( wxCommandEvent &event )
{
	_locListBox->ExpandCollapseItems(true);
}

void QGenMainFrame::OnCollapseLocation( wxCommandEvent &event )
{
	_locListBox->ExpandCollapseItems(false);
}

void QGenMainFrame::OnJumpLocation( wxCommandEvent &event )
{
	_controls->JumpToSelectedLoc();
}

void QGenMainFrame::OnKeyDown( wxKeyEvent& event )
{
	if (!_controls->ExecuteHotkey(event.GetKeyCode(), event.GetModifiers()))
		event.Skip();
}

void QGenMainFrame::OnCreateFolder( wxCommandEvent &event )
{
	_controls->AddFolder();
}

void QGenMainFrame::OnRenameFolder( wxCommandEvent &event )
{
	_controls->RenameSelectedFolder();
}

void QGenMainFrame::OnDeleteFolder( wxCommandEvent &event )
{
	_controls->DeleteSelectedFolder();
}
