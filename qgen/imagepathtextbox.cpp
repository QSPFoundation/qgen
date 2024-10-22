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

#include "imagepathtextbox.h"

IMPLEMENT_CLASS(ImagePathTextBox, wxTextCtrl)

BEGIN_EVENT_TABLE(ImagePathTextBox, wxTextCtrl)
    EVT_KILL_FOCUS(ImagePathTextBox::OnLostFocus)
    EVT_KEY_DOWN(ImagePathTextBox::OnKeyDown)
END_EVENT_TABLE()

ImagePathTextBox::ImagePathTextBox(wxWindow *owner, wxWindowID id, ILocationPage *locPage, IControls *controls) :
    wxTextCtrl(owner, id)
{
    _locPage = locPage;
    _controls = controls;

    Update();
    _controls->GetSettings()->AddObserver(this);
}

ImagePathTextBox::~ImagePathTextBox()
{
    _controls->GetSettings()->RemoveObserver(this);
}

void ImagePathTextBox::Update(bool isFromObservable)
{
    Settings *settings = _controls->GetSettings();
    SetFont(settings->GetFont(SYNTAX_BASE));
    SetForegroundColour(settings->GetColour(SYNTAX_BASE));
    SetBackgroundColour(settings->GetTextBackColour());
    Refresh();
}

void ImagePathTextBox::OnLostFocus(wxFocusEvent &event)
{
    _locPage->RefreshActions();
    event.Skip();
}

void ImagePathTextBox::OnKeyDown(wxKeyEvent& event)
{
    if (!_controls->ExecuteHotkey(event.GetKeyCode(), event.GetModifiers()))
        event.Skip();
    if (event.GetKeyCode() == WXK_RETURN) event.Skip(false);
}
