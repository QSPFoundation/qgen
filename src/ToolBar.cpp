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

#include "ToolBar.h"

IMPLEMENT_CLASS(QGenToolBar, wxAuiToolBar)

BEGIN_EVENT_TABLE(QGenToolBar, wxAuiToolBar)
	EVT_MOTION(QGenToolBar::OnMotion)
	EVT_LEAVE_WINDOW(QGenToolBar::OnLeaveWindow)
END_EVENT_TABLE()

QGenToolBar::QGenToolBar( wxWindow *parent, wxWindowID id, IControls *controls ) : wxAuiToolBar( parent, id )
{
	_controls = controls;
}

void QGenToolBar::OnMotion( wxMouseEvent &evt )
{
	wxAuiToolBar::OnMotion(evt);
	if (m_tip_item)
		_controls->SetStatusText(m_tip_item->GetShortHelp());
	else
		_controls->SetStatusText(wxEmptyString);
}

void QGenToolBar::OnLeaveWindow( wxMouseEvent &evt )
{
	wxAuiToolBar::OnLeaveWindow(evt);
	_controls->SetStatusText(wxEmptyString);
}
