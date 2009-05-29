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

#include "QGen.h"

IMPLEMENT_APP(QGenApp)

BEGIN_EVENT_TABLE(QGenMainFrame, wxFrame)
	EVT_INIT(QGenMainFrame::OnInit)
	EVT_CLOSE(QGenMainFrame::OnQuit)
	EVT_AUI_PANE_CLOSE(QGenMainFrame::OnPaneClose)
	EVT_MENU(EXIT_QGEN, QGenMainFrame::OnExit)
	EVT_MENU(ID_TOGGLE_TOOLBAR, QGenMainFrame::OnToggleToolbar)
	EVT_MENU(ID_TOGGLE_LISTBOX, QGenMainFrame::OnToggleLocations)
	EVT_MENU(ID_TOGGLE_STATUSBAR, QGenMainFrame::OnToggleStatusbar)
	EVT_MENU(ID_MENUCLOSEALLTABS, QGenMainFrame::OnTabMenu)
	EVT_MENU(ID_MENUCLOSEEXCEPTSELECTED, QGenMainFrame::OnTabMenu)
	EVT_MENU(ID_MENUCLOSESELECTED, QGenMainFrame::OnTabMenu)
	EVT_MENU(ID_LOCDESCVISIBLE, QGenMainFrame::OnLocDescVisible)
	EVT_MENU(ID_LOCACTVISIBLE, QGenMainFrame::OnLocActsVisible)
	EVT_MENU(HELP_ABOUT, QGenMainFrame::OnAbout)
	EVT_MENU(HELP_HELP, QGenMainFrame::OnChmHelp)
	EVT_MENU(HELP_SEARCH_INDEX, QGenMainFrame::OnSearchHelp)
	EVT_MENU(LOC_CREAT, QGenMainFrame::OnCreateLocation)
	EVT_MENU(LOC_DEL, QGenMainFrame::OnDeleteLocation)
	EVT_MENU(LOC_RENAME, QGenMainFrame::OnRenameLocation)
	EVT_MENU(LOC_CLEAR, QGenMainFrame::OnClearLocation)
	EVT_MENU(LOC_COPY, QGenMainFrame::OnCopyLocation)
	EVT_MENU(LOC_REPLACE, QGenMainFrame::OnPasteLocation)
	EVT_MENU(LOC_PASTE, QGenMainFrame::OnPasteLocation)
	EVT_MENU(LOC_PASTE_NEW, QGenMainFrame::OnPasteLocation)
	EVT_MENU(LOC_SORT_ASC, QGenMainFrame::OnSortLocations)
	EVT_MENU(LOC_SORT_DESC, QGenMainFrame::OnSortLocations)
	EVT_MENU(LOC_JUMP_LOC, QGenMainFrame::OnJumpLocation)
	EVT_MENU(LOC_EXPAND, QGenMainFrame::OnExpandLocation)
	EVT_MENU(LOC_COLLAPSE, QGenMainFrame::OnCollapseLocation)
	EVT_MENU(CREATE_ACTION, QGenMainFrame::OnCreateAction)
	EVT_MENU(DEL_ACTION, QGenMainFrame::OnDeleteAction)
	EVT_MENU(DEL_ALL_ACTIONS, QGenMainFrame::OnDeleteAllActions)
	EVT_MENU(RENAME_ACTION, QGenMainFrame::OnRenameAction)
	EVT_MENU(NEW_QUEST, QGenMainFrame::OnNewQuest)
	EVT_MENU(OPEN_QUEST, QGenMainFrame::OnLoadFile)
	EVT_MENU(SAVE_QUEST, QGenMainFrame::OnSaveQuest)
	EVT_MENU(SAVE_QUESTAS, QGenMainFrame::OnSaveQuestAs)
	EVT_MENU(JOIN_QUEST, QGenMainFrame::OnJoinQuest)
	EVT_MENU(PLAY_QUEST, QGenMainFrame::OnPlayQuest)
	EVT_MENU(UTIL_FIND, QGenMainFrame::OnFindDialog)
	EVT_MENU(UTIL_INF, QGenMainFrame::OnInformationQuest)
	EVT_MENU(UTIL_OPTIONS, QGenMainFrame::OnOptionsDialog)
	EVT_MENU(UNDO_TEXT, QGenMainFrame::OnUndoText)
	EVT_MENU(REDO_TEXT, QGenMainFrame::OnRedoText)
	EVT_MENU(COPY_TEXT, QGenMainFrame::OnCopyText)
	EVT_MENU(PASTE_TEXT, QGenMainFrame::OnPasteText)
	EVT_MENU(CUT_TEXT, QGenMainFrame::OnCutText)
	EVT_MENU(DEL_TEXT, QGenMainFrame::OnDeleteText)
	EVT_MENU(SELALL_TEXT, QGenMainFrame::OnSelectAllText)

	EVT_MENU(EXPORT_QUEST_TXT, QGenMainFrame::OnExportTxtFile)
	EVT_MENU(EXPORT_QUEST_TXT2GAM, QGenMainFrame::OnExportTxt2Gam)
	EVT_MENU(IMPORT_QUEST, QGenMainFrame::OnImportTxt2Gam)

	EVT_MENU_OPEN(QGenMainFrame::OnUpdMenuItems)

	EVT_TIMER(ID_TIMER_AUTO_SAVE, QGenMainFrame::OnTimerAutoSave)
	EVT_TIMER(ID_TIMER_UPD_TOOLBAR, QGenMainFrame::OnTimerUpdToolBar)
END_EVENT_TABLE()

bool QGenApp::OnInit()
{
	InitEvent initEvent;
	wxInitAllImageHandlers();
	wxFileName appPath(argv[0]);
	appPath.MakeAbsolute();
	_controls = new Controls(appPath.GetPath());
	QGenMainFrame *main_frame = new QGenMainFrame(_controls);
	_controls->SetMainFrame(main_frame);
	main_frame->Show();
	wxCmdLineParser cmdParser(argc, argv);
	if (argc > 1)
	{
		cmdParser.AddParam();
		cmdParser.Parse(false);
		wxFileName path(cmdParser.GetParam());
		path.MakeAbsolute();
		initEvent.SetInitString(path.GetFullPath());
		wxPostEvent(main_frame, initEvent);
	}
	else
	{
		Settings *settings = _controls->GetSettings();
		if (settings->GetOpenLastGame())
		{
			initEvent.SetInitString(settings->GetLastGamePath());
			wxPostEvent(main_frame, initEvent);
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

QGenMainFrame::QGenMainFrame(Controls *controls) : _timerAutoSave(this, ID_TIMER_AUTO_SAVE), _timerUpdToolBar(this, ID_TIMER_UPD_TOOLBAR)
{
	_controls = controls;
	if (Create(wxSize(640, 480), wxDEFAULT_FRAME_STYLE)) OnNewQuest(wxCommandEvent());
	_timerAutoSave.Start(1000);
	_timerUpdToolBar.Start(500);
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
		SetIcon(wxIcon(wxwin16x16_xpm));
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

	_manager.AddPane(_locNotebook, wxAuiPaneInfo().CenterPane().
		Name(wxT("Properties Location")));

	_manager.AddPane(_toolBar, wxAuiPaneInfo().ToolbarPane().
		Name(wxT("Standard Toolbar")).Caption(wxT("Toolbar")).
		Top().LeftDockable(false).RightDockable(false).Floatable(true));

	_manager.AddPane(_locListBox, wxAuiPaneInfo().Left().
						Layer(1).PinButton().MinimizeButton().
						MaximizeButton().Name(wxT("Locations")).
						Caption(wxT("Список локаций:")));

	LoadLayout();
	_manager.Update();
}

void QGenMainFrame::CreateMenuBar()
{
	wxMenu *file_menu = new wxMenu;
	wxMenuItem *newGameItem = new wxMenuItem(file_menu, NEW_QUEST, wxT("&Новая\tCtrl+N"), wxT("Создание новой игры"));
	newGameItem->SetBitmap(wxBitmap(new_game_menu_xpm));
	file_menu->Append(newGameItem);

	wxMenuItem *openFileItem = new wxMenuItem(file_menu, OPEN_QUEST, wxT("&Открыть...\tCtrl+O"), wxT("Открыть игру"));
	openFileItem->SetBitmap(wxBitmap(open_file_menu_xpm));
	file_menu->Append(openFileItem);

	file_menu->Append(JOIN_QUEST, wxT("&Присоединить игру...\tCtrl+M"));

	wxMenuItem *saveFileItem = new wxMenuItem(file_menu, SAVE_QUEST, wxT("&Сохранить\tCtrl+S"), wxT("Сохранить игру"));
	saveFileItem->SetBitmap(wxBitmap(save_file_menu_xpm));
	file_menu->Append(saveFileItem);

	file_menu->Append(SAVE_QUESTAS, wxT("Сохранить &как...\tCtrl+W"), wxT("Сохранить игру под другим именем"));

	file_menu->AppendSeparator();

	wxMenu *file_sub_exp_menu = new wxMenu;
	wxMenu *file_sub_imp_menu = new wxMenu;
	file_sub_exp_menu->Append(EXPORT_QUEST_TXT, wxT("Текстовый файл..."));
	file_sub_exp_menu->Append(EXPORT_QUEST_TXT2GAM, wxT("Текстовый файл формата TXT2GAM..."));
	file_sub_imp_menu->Append(IMPORT_QUEST, wxT("Текстовый файл формата TXT2GAM..."));

	file_menu->AppendSubMenu(file_sub_exp_menu, wxT("&Экспорт"));
	file_menu->AppendSubMenu(file_sub_imp_menu, wxT("&Импорт"));
	file_menu->AppendSeparator();

	wxMenuItem *exitItem = new wxMenuItem(file_menu, EXIT_QGEN, wxT("&Выход\tAlt+X"), wxT("Выход из программы"));
	exitItem->SetBitmap(wxBitmap(exit_menu_xpm));
	file_menu->Append(exitItem);

	wxMenu *util_menu = new wxMenu;

	wxMenuItem *playGameItem = new wxMenuItem(util_menu, PLAY_QUEST, wxT("&Запуск игры\tF5"), wxT("Запуск текущей игры"));
	playGameItem->SetBitmap(wxBitmap(play_game_menu_xpm));
	util_menu->Append(playGameItem);

	wxMenuItem *searchTextItem = new wxMenuItem(util_menu, UTIL_FIND, wxT("&Поиск / замена\tCtrl+F"), wxT("Поиск и замена текста"));
	searchTextItem->SetBitmap(wxBitmap(search_text_menu_xpm));
	util_menu->Append(searchTextItem);

	wxMenuItem *gameInfoItem = new wxMenuItem(util_menu, UTIL_INF, wxT("&Информация об игре\tCtrl+I"));
	gameInfoItem->SetBitmap(wxBitmap(game_info_menu_xpm));
	util_menu->Append(gameInfoItem);

	util_menu->AppendSeparator();
	util_menu->Append(UTIL_OPTIONS, wxT("&Настройки...\tCtrl+P"));

	wxMenu *loc_menu = new wxMenu;
	loc_menu->Append(LOC_CREAT, wxT("&Создать...\tF7"), wxT("Создание локации"));
	loc_menu->Append(LOC_RENAME, wxT("&Переименовать...\tF6"), wxT("Переименование локации"));
	loc_menu->Append(LOC_DEL, wxT("&Удалить\tF8"), wxT("Удаление локации"));
	loc_menu->AppendSeparator();
	loc_menu->Append(LOC_COPY, wxT("&Копировать\tCtrl+Shift+C"), wxT("Копировать локацию в буфер"));
	loc_menu->Append(LOC_PASTE, wxT("&Вставить\tCtrl+Shift+V"), wxT("Вставить локацию из буфера"));
	loc_menu->Append(LOC_REPLACE, wxT("&Заменить\tCtrl+Shift+R"), wxT("Заменить выбранную локацию локацией из буфера"));
	loc_menu->Append(LOC_PASTE_NEW, wxT("Вс&тавить в...\tCtrl+Shift+N"), wxT("Вставить локацию из буфера в новую локацию"));
	loc_menu->Append(LOC_CLEAR, wxT("&Очистить\tCtrl+Shift+D"), wxT("Очистить локацию"));
	loc_menu->AppendSeparator();

	wxMenu *loc_action_sub_menu = new wxMenu;

	loc_action_sub_menu->Append(CREATE_ACTION, wxT("&Создать...\tAlt+F7"));
	loc_action_sub_menu->Append(RENAME_ACTION, wxT("&Переименовать...\tAlt+F6"));
	loc_action_sub_menu->Append(DEL_ACTION, wxT("&Удалить\tAlt+F8"));
	loc_action_sub_menu->AppendSeparator();
	loc_action_sub_menu->Append(DEL_ALL_ACTIONS, wxT("У&далить все\tAlt+F10"));
	loc_menu->Append(SUB_MENU_ACT, wxT("&Действия"), loc_action_sub_menu);

	loc_menu->AppendSeparator();

	loc_menu->Append(LOC_SORT_ASC, wxT("Со&ртировать по алфавиту\tCtrl+Shift+O"));
	loc_menu->Append(LOC_SORT_DESC, wxT("Сор&тировать в обратном порядке\tCtrl+Shift+P"));
	loc_menu->AppendSeparator();
	loc_menu->Append(LOC_JUMP_LOC, wxT("П&ерейти на выбранную локацию\tCtrl+G"));

	wxMenu *text_menu = new wxMenu;

	wxMenuItem *undoTextItem = new wxMenuItem(text_menu, UNDO_TEXT, wxT("&Отменить\tCtrl+Z"));
	undoTextItem->SetBitmap(wxBitmap(undo_text_menu_xpm));
	text_menu->Append(undoTextItem);

	wxMenuItem *redoTextItem = new wxMenuItem(text_menu, REDO_TEXT, wxT("&Повторить\tCtrl+Y"));
	redoTextItem->SetBitmap(wxBitmap(redo_text_menu_xpm));
	text_menu->Append(redoTextItem);

	text_menu->AppendSeparator();

	wxMenuItem *cutTextItem = new wxMenuItem(text_menu, CUT_TEXT, wxT("&Вырезать\tCtrl+X"));
	cutTextItem->SetBitmap(wxBitmap(cut_text_menu_xpm));
	text_menu->Append(cutTextItem);

	wxMenuItem *copyTextItem = new wxMenuItem(text_menu, COPY_TEXT, wxT("&Копировать\tCtrl+C"));
	copyTextItem->SetBitmap(wxBitmap(copy_text_menu_xpm));
	text_menu->Append(copyTextItem);

	wxMenuItem *pasteTextItem = new wxMenuItem(text_menu, PASTE_TEXT, wxT("В&ставить\tCtrl+V"));
	pasteTextItem->SetBitmap(wxBitmap(paste_text_menu_xpm));
	text_menu->Append(pasteTextItem);

	wxMenuItem *deleteTextItem = new wxMenuItem(text_menu, DEL_TEXT, wxT("&Удалить\tCtrl+D"));
	deleteTextItem->SetBitmap(wxBitmap(delete_text_menu_xpm));
	text_menu->Append(deleteTextItem);

	text_menu->AppendSeparator();
	text_menu->Append(SELALL_TEXT, wxT("В&ыделить всё\tCtrl+A"));

	wxMenu *list_controls= new wxMenu;
	list_controls->Append(ID_TOGGLE_TOOLBAR, wxT("&Панель инструментов"), wxEmptyString, wxITEM_CHECK);
	list_controls->Append(ID_TOGGLE_LISTBOX, wxT("&Окно списка локаций"), wxEmptyString, wxITEM_CHECK);
	list_controls->Append(ID_TOGGLE_STATUSBAR, wxT("&Строка состояния"), wxEmptyString, wxITEM_CHECK);

	wxMenu *view_menu= new wxMenu;
	view_menu->Append(LIST_WINDOWS, wxT("&Список окон"), list_controls);
	view_menu->AppendSeparator();
	view_menu->Append(ID_MENUCLOSEALLTABS, wxT("&Закрыть все страницы"));
	view_menu->Append(ID_MENUCLOSEEXCEPTSELECTED, wxT("Закрыть все &кроме текущей"));
	view_menu->Append(ID_MENUCLOSESELECTED, wxT("Закрыть &текущую"));
	view_menu->AppendSeparator();
	view_menu->Append(ID_LOCDESCVISIBLE, wxT("Показать/скрыть &описание локации\tCtrl+Alt+D"));
	view_menu->Append(ID_LOCACTVISIBLE, wxT("Показать/скрыть &действия локации\tCtrl+Alt+A"));

	wxMenu *help_menu = new wxMenu;

	wxMenuItem *contentsHelpItem = new wxMenuItem(help_menu, HELP_HELP, wxT("&Помощь\tF1"));
	contentsHelpItem->SetBitmap(wxBitmap(contents_help_menu_xpm));
	help_menu->Append(contentsHelpItem);

	wxMenuItem *indexSearchHelpItem = new wxMenuItem(help_menu, HELP_SEARCH_INDEX, wxT("Помощь по ключевому &слову\tCtrl+F1"));
	indexSearchHelpItem->SetBitmap(wxBitmap(index_search_menu_xpm));
	help_menu->Append(indexSearchHelpItem);

	help_menu->AppendSeparator();
	help_menu->Append(HELP_ABOUT, wxT("&О программе..."));

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
	wxBitmap bitmaps[18];
	bitmaps[0]	= wxBitmap(play_xpm);
	bitmaps[1]	= wxBitmap(info_xpm);
	bitmaps[2]	= wxBitmap(filesaveas_xpm);
	bitmaps[3]	= wxBitmap(filesave_xpm);
	bitmaps[4]	= wxBitmap(fileopen_xpm);
	bitmaps[5]	= wxBitmap(new_xpm);
	bitmaps[6]	= wxBitmap(rename_xpm);
	bitmaps[7]	= wxBitmap(delete_xpm);
	bitmaps[8]	= wxBitmap(copy_xpm);
	bitmaps[9]	= wxBitmap(paste_xpm);
	bitmaps[10] = wxBitmap(clear_xpm);
	bitmaps[11] = wxBitmap(search_xpm);
	bitmaps[12] = wxBitmap(options_xpm);
	bitmaps[13] = wxBitmap(undo_text_xpm);
	bitmaps[14] = wxBitmap(redo_text_xpm);

	_toolBar = new wxAuiToolBar(this, ID_TOOLBAR);

	_toolBar->SetToolBitmapSize(wxSize(24, 24));
	_toolBar->AddTool(LOC_CREAT, wxT("New location..."), bitmaps[5], wxT("Создать локацию... (F7)"));
	_toolBar->AddTool(LOC_RENAME, wxT("Rename location..."), bitmaps[6], wxT("Переименовать выделенную локацию... (F6)"));
	_toolBar->AddTool(LOC_DEL, wxT("Delete location"), bitmaps[7], wxT("Удалить выделенную локацию (F8)"));
	_toolBar->AddSeparator();
	_toolBar->AddTool(OPEN_QUEST, wxT("Open..."), bitmaps[4], wxT("Открыть игру... (Ctrl+O)"));
	_toolBar->AddTool(SAVE_QUEST, wxT("Save"), bitmaps[3], wxT("Сохранить игру (Ctrl+S)"));
	_toolBar->AddTool(SAVE_QUESTAS, wxT("Save as..."), bitmaps[2], wxT("Сохранить игру как... (Ctrl+W)"));
	_toolBar->AddSeparator();
	_toolBar->AddTool(PLAY_QUEST, wxT("Play"), bitmaps[0], wxT("Запуск игры (F5)"));
	_toolBar->AddTool(UTIL_INF, wxT("Information"), bitmaps[1], wxT("Краткая информация об игре (Ctrl+I)"));
	_toolBar->AddSeparator();
	_toolBar->AddTool(UNDO_TEXT, wxT("Undo text"), bitmaps[13], wxT("Отменить (Ctrl+Z)"));
	_toolBar->AddTool(REDO_TEXT, wxT("Redo text"), bitmaps[14], wxT("Повторить (Ctrl+Y)"));
	_toolBar->AddSeparator();
	_toolBar->AddTool(LOC_COPY, wxT("Copy location"), bitmaps[8], wxT("Копировать выделенную локацию (Ctrl+Shift+C)"));
	_toolBar->AddTool(LOC_PASTE, wxT("Paste location"), bitmaps[9], wxT("Вставить локацию (Ctrl+Shift+V)"));
	_toolBar->AddTool(LOC_CLEAR, wxT("Clear location"), bitmaps[10], wxT("Очистить выделенную локацию (Ctrl+Shift+D)"));
	_toolBar->AddSeparator();
	_toolBar->AddTool(UTIL_FIND, wxT("Search..."), bitmaps[11], wxT("Поиск / замена... (Ctrl+F)"));
	_toolBar->AddTool(UTIL_OPTIONS, wxT("Options..."), bitmaps[12], wxT("Настройки... (Ctrl+P)"));
	_toolBar->Realize();
}

void QGenMainFrame::OnInit( InitEvent &event )
{
	if ( _controls->LoadGame(event.GetInitString()) ) UpdateTitle();
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
	info.SetIcon(wxIcon(logo_big_xpm));
	info.SetName(QGEN_TITLE);
	info.SetCopyright(wxT("Quest Soft, 2001-2009"));
	info.SetDescription(wxString::Format( _("Version: %s\nCompiled: %s"),
		QGEN_VER, wxT(__DATE__) wxT(", ") wxT(__TIME__)));
	info.SetWebSite(wxT("http://qsp.su"));
	info.AddDeveloper(wxT("BaxZzZz [bauer_v@mail.ru]"));
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
		_("Файлы игр QSP (*.qsp)|*.qsp|Файлы игр QSP (*.gam)|*.gam"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	dialog.CenterOnParent();
	if (dialog.ShowModal() == wxID_OK)
	{
		wxString password;
		wxTextEntryDialog dlgEntry(this, wxT("Введите пароль:"),
			wxT("Ввод пароля"), wxEmptyString, wxOK|wxCANCEL|wxTE_PASSWORD );
		dlgEntry.CenterOnParent();
		if (dlgEntry.ShowModal() != wxID_CANCEL) password = dlgEntry.GetValue();
		if (password.IsEmpty()) password = QGEN_PASSWD;
		if ( _controls->SaveGame(dialog.GetPath(), password ) )
			UpdateTitle();
		else
			_controls->ShowMessage( QGEN_MSG_CANTSAVEGAME );
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
	SetSize(settings->GetLeftFramePos(), settings->GetTopFramePos(), settings->GetFrameWidth(), settings->GetFrameHeight());
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
	_menu_bar->Check(ID_TOGGLE_TOOLBAR, _manager.GetPane(wxT("Standard Toolbar")).IsShown());
	_menu_bar->Check(ID_TOGGLE_LISTBOX, _manager.GetPane(wxT("Locations")).IsShown());
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
		if(all_panes.Item(i).name == pane_name)
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
	TogglePaneVisibility(wxT("Standard Toolbar"));
	_manager.Update();
}

void QGenMainFrame::OnToggleLocations(wxCommandEvent &event)
{
	TogglePaneVisibility(wxT("Locations"));
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
	SearchDialog *fnddlg = new SearchDialog(this, wxT("Поиск / замена"), _controls, wxRESIZE_BORDER);
	fnddlg->CenterOnParent();
	_controls->InitSearchData();
	fnddlg->Show();
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
	case LOC_REPLACE:
		_controls->PasteLocFromClipboard(PASTE_REPLACE);
		break;
	case LOC_PASTE:
		_controls->PasteLocFromClipboard(PASTE_NEW_AUTO);
		break;
	case LOC_PASTE_NEW:
		_controls->PasteLocFromClipboard(PASTE_NEW);
		break;
	}
}

void QGenMainFrame::OnSortLocations( wxCommandEvent &event )
{
	_controls->SortLocations(event.GetId() == LOC_SORT_ASC);
}

void QGenMainFrame::OnCreateAction( wxCommandEvent &event )
{
	if ( _controls->AddActionOnSelectedLoc() )
		_controls->GetSettings()->NotifyAll();
}

void QGenMainFrame::OnDeleteAction( wxCommandEvent &event )
{
	if ( _controls->DeleteSelectedAction() )
		_controls->GetSettings()->NotifyAll();
}

void QGenMainFrame::OnDeleteAllActions( wxCommandEvent &event )
{
	if ( _controls->DeleteAllActions() )
		_controls->GetSettings()->NotifyAll();
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
			wxT("Файл плеера (*.exe)|*.exe"), wxFD_OPEN);
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
	DesktopWindow desktop;
	wxCHMHelpController *chmHelp = new wxCHMHelpController(&desktop);
	if (SearchHelpFile())
	{
		chmHelp->LoadFile(_controls->GetSettings()->GetCurrentHelpPath());
		chmHelp->DisplayContents();
	}
	delete chmHelp;
}

void QGenMainFrame::OnSearchHelp( wxCommandEvent &event )
{
	DesktopWindow desktop;
	wxCHMHelpController *chmHelp = new wxCHMHelpController(&desktop);
	if (SearchHelpFile())
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
				wxT("Файл конвертора (*.exe)|*.exe"), wxFD_OPEN);
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
	_toolBar->EnableTool(LOC_DEL, isLocSelected);
	_toolBar->EnableTool(LOC_RENAME, isLocSelected);
	_toolBar->EnableTool(LOC_COPY, isSelLocNotEmpty);
	_toolBar->EnableTool(LOC_CLEAR, isSelLocNotEmpty);
	_toolBar->EnableTool(CREATE_ACTION, isLocsNotAllClosed);
	_toolBar->EnableTool(RENAME_ACTION, isActsNotEmpty);
	_toolBar->EnableTool(DEL_ACTION, isActsNotEmpty);
	_toolBar->EnableTool(PLAY_QUEST, isCanPlay);
	_toolBar->EnableTool(SAVE_QUEST, isCanPlay);
	_toolBar->EnableTool(SAVE_QUESTAS, isCanPlay);
	_toolBar->EnableTool(UNDO_TEXT, isCanUndoText);
	_toolBar->EnableTool(REDO_TEXT, isCanRedoText);
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
	switch (event.GetId())
	{
	case ID_MENUCLOSEALLTABS:
		type = CLOSE_ALL;
		break;
	case ID_MENUCLOSEEXCEPTSELECTED:
		type = CLOSE_ALLEXCEPTSELECTED;
		break;
	case ID_MENUCLOSESELECTED:
		type = CLOSE_SELECTED;
		break;
	}
	_locNotebook->DeleteAllPages(type, _locNotebook->GetSelection());
}

void QGenMainFrame::OnLocDescVisible(wxCommandEvent &event)
{
	_controls->SwitchLocDesc();
}

void QGenMainFrame::OnLocActsVisible(wxCommandEvent &event)
{
	_controls->SwitchLocActs();
}

bool QGenMainFrame::SearchHelpFile()
{
	Settings *settings = _controls->GetSettings();
	if (!wxFile::Exists(settings->GetCurrentHelpPath()))
	{
		wxFileDialog dialog(this, wxT("Выберите файл справки"), wxEmptyString, wxEmptyString,
			wxT("Файл справки (*.chm)|*.chm"), wxFD_OPEN);
		dialog.CenterOnParent();
		if (dialog.ShowModal() == wxID_CANCEL) return false;
		settings->SetCurrentHelpPath(dialog.GetPath());
	}
	return true;
}

void QGenMainFrame::OnExpandLocation( wxCommandEvent &event )
{
	_locListBox->ExpandItems();
}

void QGenMainFrame::OnCollapseLocation( wxCommandEvent &event )
{
	_locListBox->CollapseItems();
}

void QGenMainFrame::OnJumpLocation( wxCommandEvent &event )
{
	_controls->JumpToSelectedLoc();
}
