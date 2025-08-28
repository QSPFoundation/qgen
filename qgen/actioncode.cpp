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

#include "actioncode.h"

IMPLEMENT_CLASS(ActionCode, wxPanel)

BEGIN_EVENT_TABLE(ActionCode, wxPanel)
    EVT_BUTTON(ID_PICT_OPEN, ActionCode::OnOpenPicture)
END_EVENT_TABLE()

ActionCode::ActionCode(wxWindow *owner, ILocationPage *locPage, IControls *controls) : wxPanel(owner)
{
    _locPage = locPage;
    _controls = controls;

    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

    _button = new wxButton(this, ID_PICT_OPEN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    _picPathTxt = new ImagePathTextBox(this, wxID_ANY, locPage, _controls);

    wxBoxSizer *sizerPicPath = new wxBoxSizer(wxHORIZONTAL);
    sizerPicPath->Add(_picPathTxt, 1, wxGROW);
    sizerPicPath->Add(_button, 0, wxLEFT, 2);

    _actCodeTxt = new SyntaxTextBox(this, _controls, SYNTAX_STYLE_CODE);

    topSizer->Add(sizerPicPath, 0, wxGROW);
    topSizer->Add(_actCodeTxt, 1, wxTOP|wxGROW, 2);

    SetSizerAndFit(topSizer);
    SetAutoLayout(true);
    Update();
    _controls->GetSettings()->AddObserver(this);
}

ActionCode::~ActionCode()
{
    _controls->GetSettings()->RemoveObserver(this);
}

void ActionCode::Update(bool isFromObservable)
{
    _button->SetLabel(_("Image..."));
    GetSizer()->Layout();
}

void ActionCode::LoadAction(size_t actIndex)
{
    DataContainer *container = _controls->GetContainer();
    size_t locIndex = _locPage->GetLocationIndex();
    _picPathTxt->SetValue(container->GetActionPicturePath(locIndex, actIndex));
    _actCodeTxt->SetValue(container->GetActionCode(locIndex, actIndex));
    Enable();
}

void ActionCode::SaveAction(size_t actIndex)
{
    DataContainer *container = _controls->GetContainer();
    size_t locIndex = _locPage->GetLocationIndex();
    if (_picPathTxt->IsModified())
    {
        container->SetActionPicturePath(locIndex, actIndex, _picPathTxt->GetValue());
        _picPathTxt->SetModified(false);
    }
    if (_actCodeTxt->IsModified())
    {
        container->SetActionCode(locIndex, actIndex, _actCodeTxt->GetValue());
        _actCodeTxt->SetModified(false);
    }
}

void ActionCode::ClearAction()
{
    _picPathTxt->Clear();
    _actCodeTxt->Clear();
    Enable(false);
}

void ActionCode::OnOpenPicture(wxCommandEvent &WXUNUSED(event))
{
    wxString str = _controls->SelectPicturePath();
    if (!str.IsEmpty())
    {
        _picPathTxt->SetValue(str);
        _picPathTxt->SetModified(true);
        _locPage->RefreshActions();
    }
}

bool ActionCode::Enable(bool status /*= true*/)
{
    _picPathTxt->SetEditable(status);
    _button->Enable(status);
    _actCodeTxt->Enable(status);
    return true;
}

void ActionCode::SelectPicturePathString(long startPos, long lastPos)
{
    _picPathTxt->SetFocus();
    _picPathTxt->SetSelection(startPos, lastPos);
}

void ActionCode::SelectCodeString(long startPos, long lastPos)
{
    _actCodeTxt->SetFocus();
    _actCodeTxt->SetSelection(startPos, lastPos);
}

void ActionCode::ReplacePicturePathString(long start, long end, const wxString& str)
{
    _picPathTxt->Replace(start, end, str);
}

void ActionCode::ReplaceCodeString(long start, long end, const wxString& str)
{
    _actCodeTxt->Replace(start, end, str);
}

void ActionCode::SetFocusOnActionCode()
{
    _actCodeTxt->SetFocus();
}

void ActionCode::ExpandCollapseAll(bool isExpanded)
{
    _actCodeTxt->ExpandCollapseAll(isExpanded);
}
