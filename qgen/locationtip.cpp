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

#include "locationtip.h"

IMPLEMENT_CLASS(LocationTip, wxPopupWindow);

LocationTip::LocationTip(wxWindow *parent, IControls *controls) :
    wxPopupWindow(parent)
{
    _mainFrame = parent;
    _controls = controls;
    wxColour textColor(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOTEXT));
    wxColour backColor(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK));

    SetBackgroundColour(backColor);

    _title = new wxStaticText(this, wxID_ANY, wxEmptyString);
    _title->SetFont(_title->GetFont().MakeBold().MakeLarger());
    _title->SetForegroundColour(textColor);

    _desc = new wxStaticText(this, wxID_ANY, _("Base description:"));
    _desc->SetFont(_desc->GetFont().MakeBold());
    _desc->SetForegroundColour(textColor);
    _locDesc = new SyntaxTextBox(this, _controls, SYNTAX_STYLE_NOSCROLLBARS | SYNTAX_STYLE_SIMPLE |
                                                        SYNTAX_STYLE_NOMARGINS | SYNTAX_STYLE_NOHOTKEYS |
                                                        SYNTAX_STYLE_SIMPLEMENU | SYNTAX_STYLE_NOHELPTIPS |
                                                        SYNTAX_STYLE_READONLY);

    _code = new wxStaticText(this, wxID_ANY, wxEmptyString);
    _code->SetFont(_code->GetFont().MakeBold());
    _code->SetForegroundColour(textColor);
    _locCode = new SyntaxTextBox(this, _controls, SYNTAX_STYLE_NOSCROLLBARS | SYNTAX_STYLE_CODE |
                                                        SYNTAX_STYLE_NOHOTKEYS | SYNTAX_STYLE_SIMPLEMENU |
                                                        SYNTAX_STYLE_NOMARGINS | SYNTAX_STYLE_NOHELPTIPS |
                                                        SYNTAX_STYLE_READONLY);

    _emptyLabel = new wxButton(this, wxID_ANY, _("(empty)"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
    _emptyLabel->SetFont(_emptyLabel->GetFont().MakeLarger().MakeLarger());
    _emptyLabel->SetBackgroundColour(backColor);
    _emptyLabel->SetForegroundColour(textColor);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    sizer->Add(_title, 0, wxALL|wxALIGN_CENTER, 4);
    sizer->Add(_desc, 0, wxLEFT|wxRIGHT|wxGROW, 4);
    sizer->Add(_locDesc, 1, wxALL|wxGROW, 4);
    sizer->Add(_code, 0, wxLEFT|wxRIGHT|wxGROW, 4);
    sizer->Add(_locCode, 1, wxALL|wxGROW, 4);
    sizer->Add(_emptyLabel, 1, wxGROW);

    SetSizer(sizer);
    SetAutoLayout(true);
    SetSize(TIP_SIZE_X, TIP_SIZE_Y);
}

void LocationTip::MoveTip(const wxPoint &pos)
{
    wxPoint position(pos);
    LoadTip();
    wxRect displaySize(wxGetClientDisplayRect());
    int curWidth, curHeight;
    GetSize(&curWidth, &curHeight);
    if (displaySize.GetHeight() < (position.y + curHeight + TIP_Y_OFFSET))
        position.y -= curHeight;
    else
        position.y += TIP_Y_OFFSET;
    if (displaySize.GetWidth() < (position.x + curWidth + TIP_X_OFFSET))
        position.x -= curWidth;
    else
        position.x += TIP_X_OFFSET;
    Move(position.x, position.y);
    if (!IsShown()) Show();
}

void LocationTip::HideTip()
{
    if (IsShown()) Hide();
    _locName.Clear();
    _actName.Clear();
}

void LocationTip::LoadTip()
{
    bool showEmpty = true;
    DataContainer *container = _controls->GetContainer();
    int locIndex = container->FindLocationIndex(_locName);

    wxSizer *sizer = GetSizer();
    sizer->Hide(_emptyLabel);

    if (_actName.IsEmpty())
    {
        _title->SetLabel(_locName);
        if (locIndex >= 0)
        {
            wxString descText(container->GetLocationDesc(locIndex));
            bool hasDesc = !descText.IsEmpty();
            sizer->Show(_desc, hasDesc);
            sizer->Show(_locDesc, hasDesc);
            _locDesc->SetValue(descText);
            _code->SetLabel(_("Location code:"));
            wxString codeText(container->GetLocationCode(locIndex));
            bool hasCode = !codeText.IsEmpty();
            sizer->Show(_code, hasCode);
            sizer->Show(_locCode, hasCode);
            _locCode->SetValue(codeText);
            if (hasCode || hasDesc) showEmpty = false;
        }
    }
    else
    {
        _title->SetLabel(_actName);
        if (locIndex >= 0)
        {
            int actIndex = container->FindActionIndex(locIndex, _actName);
            if (actIndex >= 0)
            {
                wxString actCode(container->GetActionCode(locIndex, actIndex));
                bool hasCode = !actCode.IsEmpty();
                sizer->Hide(_desc);
                sizer->Hide(_locDesc);
                sizer->Show(_code, hasCode);
                sizer->Show(_locCode, hasCode);
                _code->SetLabel(_("Action code:"));
                _locCode->SetValue(actCode);
                if (hasCode) showEmpty = false;
            }
        }
    }

    if (showEmpty)
    {
        sizer->Show(_emptyLabel);
        SetSize(TIP_EMPTY_SIZE_X, TIP_EMPTY_SIZE_Y);
    }
    else
        SetSize(TIP_SIZE_X, TIP_SIZE_Y);

    Layout();
}

void LocationTip::SetLocName(const wxString &name)
{
    _locName = name;
    _actName.Clear();
}

void LocationTip::SetActName(const wxString &name)
{
    _actName = name;
}
