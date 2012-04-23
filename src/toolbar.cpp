// Copyright (C) 2005-2012
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

#include "toolbar.h"
#include "mainframe.h"

IMPLEMENT_CLASS(ToolBar, wxAuiToolBar)

BEGIN_EVENT_TABLE(ToolBar, wxAuiToolBar)
	EVT_MOTION(ToolBar::OnMotion)
	EVT_LEAVE_WINDOW(ToolBar::OnLeaveWindow)
END_EVENT_TABLE()

ToolBar::ToolBar( wxWindow *parent, wxWindowID id, IControls *controls ) : wxAuiToolBar( parent, id )
{
	_controls = controls;
	SetToolBitmapSize(wxSize(24, 24));
	AddTool(ID_LOC_CREATE, wxEmptyString, wxBitmap(toolbar_location_new_xpm));
	AddTool(ID_LOC_RENAME, wxEmptyString, wxBitmap(toolbar_location_rename_xpm));
	AddTool(ID_LOC_DEL, wxEmptyString, wxBitmap(toolbar_location_delete_xpm));
	AddSeparator();
	AddTool(ID_GAME_OPEN, wxEmptyString, wxBitmap(toolbar_file_open_xpm));
	AddTool(ID_GAME_SAVE, wxEmptyString, wxBitmap(toolbar_file_save_xpm));
	AddTool(ID_GAME_SAVEAS, wxEmptyString, wxBitmap(toolbar_file_saveas_xpm));
	AddSeparator();
	AddTool(ID_GAME_PLAY, wxEmptyString, wxBitmap(toolbar_game_play_xpm));
	AddTool(ID_UTIL_INF, wxEmptyString, wxBitmap(toolbar_game_info_xpm));
	AddSeparator();
	AddTool(ID_TEXT_UNDO, wxEmptyString, wxBitmap(toolbar_undo_xpm));
	AddTool(ID_TEXT_REDO, wxEmptyString, wxBitmap(toolbar_redo_xpm));
	AddSeparator();
	AddTool(ID_LOC_COPY, wxEmptyString, wxBitmap(toolbar_location_copy_xpm));
	AddTool(ID_LOC_PASTE, wxEmptyString, wxBitmap(toolbar_location_paste_xpm));
	AddTool(ID_LOC_CLEAR, wxEmptyString, wxBitmap(toolbar_location_clear_xpm));
	AddSeparator();
	AddTool(ID_UTIL_FIND, wxEmptyString, wxBitmap(toolbar_text_search_xpm));
	AddTool(ID_UTIL_OPTIONS, wxEmptyString, wxBitmap(toolbar_options_xpm));
	Realize();
	Update();
	_controls->GetSettings()->AddObserver(this);
}

ToolBar::~ToolBar()
{
	_controls->GetSettings()->RemoveObserver(this);
}

void ToolBar::Update( bool isFromObservable /*= false*/ )
{
	SetToolShortHelp(ID_LOC_CREATE, _("Create location... (F7)"));
	SetToolShortHelp(ID_LOC_RENAME, _("Rename selected location... (F6)"));
	SetToolShortHelp(ID_LOC_DEL, _("Delete selected location (F8)"));
	SetToolShortHelp(ID_GAME_OPEN, _("Open game... (Ctrl+O)"));
	SetToolShortHelp(ID_GAME_SAVE, _("Save game (Ctrl+S)"));
	SetToolShortHelp(ID_GAME_SAVEAS, _("Save game into another file... (Ctrl+W)"));
	SetToolShortHelp(ID_GAME_PLAY, _("Run game (F5)"));
	SetToolShortHelp(ID_UTIL_INF, _("Show game statistics (Ctrl+I)"));
	SetToolShortHelp(ID_TEXT_UNDO, _("Undo (Ctrl+Z)"));
	SetToolShortHelp(ID_TEXT_REDO, _("Redo (Ctrl+Y)"));
	SetToolShortHelp(ID_LOC_COPY, _("Copy selected location (Ctrl+Shift+C)"));
	SetToolShortHelp(ID_LOC_PASTE, _("Paste location (Ctrl+Shift+V)"));
	SetToolShortHelp(ID_LOC_CLEAR, _("Clear selected location (Ctrl+Shift+D)"));
	SetToolShortHelp(ID_UTIL_FIND, _("Find / Replace... (Ctrl+F)"));
	SetToolShortHelp(ID_UTIL_OPTIONS, _("Settings... (Ctrl+P)"));
}

void ToolBar::OnMotion( wxMouseEvent &evt )
{
	wxAuiToolBar::OnMotion(evt);
	if (m_tipItem)
		_controls->SetStatusText(m_tipItem->GetShortHelp());
	else
		_controls->SetStatusText(wxEmptyString);
}

void ToolBar::OnLeaveWindow( wxMouseEvent &evt )
{
	wxAuiToolBar::OnLeaveWindow(evt);
	_controls->SetStatusText(wxEmptyString);
}
