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

#include "locationpage.h"

IMPLEMENT_CLASS(LocationPage, wxPanel);

LocationPage::LocationPage(wxAuiNotebook *owner, IControls *controls) : wxPanel(owner)
{
    _controls = controls;
    _settings = _controls->GetSettings();
    _descWidth = _actsHeight = -1;
    _isFixed = false;

    _splitterh = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH);

    _splitterv_up = new wxSplitterWindow(_splitterh, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH);
    _locDesc = new LocationDesc(_splitterv_up, this, _controls);
    _locCode = new LocationCode(_splitterv_up, this, _controls);
    _splitterv_up->SetMinimumPaneSize(1);
    _splitterv_up->SplitVertically(_locDesc, _locCode);

    _locActs = new LocationActions(_splitterh, this, _controls);

    _splitterh->SetMinimumPaneSize(1);
    _splitterh->SplitHorizontally(_splitterv_up, _locActs);

    wxSizer *sizerMain = new wxBoxSizer(wxVERTICAL);
    sizerMain->Add(_splitterh, 1, wxGROW);

    SetSizerAndFit(sizerMain);
    SetAutoLayout(true);
    FitInside();

    Update();
    _descWidth = _actsHeight = -1;

    _settings->AddObserver(this);
}

LocationPage::~LocationPage()
{
    _settings->RemoveObserver(this);
}

void LocationPage::Update(bool isFromObservable)
{
    LocDescVisible(_settings->GetLocDescVisible());
    LocActsVisible(_settings->GetLocActsVisible());
}

void LocationPage::LocDescVisible(bool isVisible)
{
    int pageWidth = GetPageWidth();
    if (isVisible)
    {
        _splitterv_up->SetSashGravity(_settings->GetWidthsCoeff1());
        if (_descWidth < 0 || _descWidth > pageWidth)
            _descWidth = pageWidth * _settings->GetWidthsCoeff1() - _splitterv_up->GetSashSize();
        Freeze();
        _splitterv_up->SplitVertically(_locDesc, _locCode);
        _splitterv_up->SetSashPosition(_descWidth);
        Thaw();
    }
    else if (_splitterv_up->IsSplit())
    {
        _descWidth = _splitterv_up->GetSashPosition();
        _splitterv_up->Unsplit(_locDesc);
    }
}

void LocationPage::LocActsVisible(bool isVisible)
{
    int pageHeight = GetPageHeight();
    if (isVisible)
    {
        _splitterh->SetSashGravity(_settings->GetHeightsCoeff());
        if (_actsHeight < 0 || _actsHeight > pageHeight)
            _actsHeight = pageHeight * _settings->GetHeightsCoeff() - _splitterh->GetSashSize();
        Freeze();
        _splitterh->SplitHorizontally(_splitterv_up, _locActs);
        _splitterh->SetSashPosition(_actsHeight);
        Thaw();
    }
    else if (_splitterh->IsSplit())
    {
        _actsHeight = _splitterh->GetSashPosition();
        _splitterh->Unsplit(_locActs);
    }
}

void LocationPage::SetFixed(bool isFixed)
{
    _isFixed = isFixed;
}

void LocationPage::SavePage()
{
    _locDesc->SaveDesc();
    _locCode->SaveCode();
    _locActs->SaveAction();
}

void LocationPage::LoadPage()
{
    _locDesc->LoadDesc();
    _locCode->LoadCode();
    _locActs->LoadAllActions();
}

size_t LocationPage::GetLocationIndex()
{
    return _locIndex;
}

void LocationPage::SetLocationIndex(size_t locIndex)
{
    _locIndex = locIndex;
}

long LocationPage::GetSelectedAction()
{
    return _locActs->GetSelectedAction();
}

void LocationPage::DeleteAction(size_t actIndex)
{
    _locActs->DeleteActionFromList(actIndex);
}

size_t LocationPage::AddAction(const wxString& name)
{
    return _locActs->AddActionToList(name);
}

void LocationPage::RenameAction(size_t actIndex, const wxString& name)
{
    _locActs->RenameActionInList(actIndex, name);
}

void LocationPage::Clear()
{
    _locDesc->Clear();
    _locCode->Clear();
    _locActs->Clear();
}

bool LocationPage::IsActionsEmpty()
{
    return _locActs->IsActionsListEmpty();
}

void LocationPage::SelectLocDescString(long startPos, long lastPos)
{
    _locDesc->SetFocus();
    _locDesc->SelectString(startPos, lastPos);
}

void LocationPage::SelectLocCodeString(long startPos, long lastPos)
{
    _locCode->SetFocus();
    _locCode->SelectString(startPos, lastPos);
}

void LocationPage::SelectAction(size_t actIndex)
{
    _locActs->SelectActionInList(actIndex);
}

void LocationPage::SelectPicturePathString(long startPos, long lastPos)
{
    _locActs->SelectPicturePathString(startPos, lastPos);
}

void LocationPage::SelectActionCodeString(long startPos, long lastPos)
{
    _locActs->SelectActionCodeString(startPos, lastPos);
}

void LocationPage::ReplaceLocDescString(long start, long end, const wxString& str)
{
    _locDesc->ReplaceString(start, end, str);
}

void LocationPage::ReplaceLocCodeString(long start, long end, const wxString& str)
{
    _locCode->ReplaceString(start, end, str);
}

void LocationPage::ReplacePicturePathString(long start, long end, const wxString& str)
{
    _locActs->ReplacePicturePathString(start, end, str);
}

void LocationPage::ReplaceActionCodeString(long start, long end, const wxString& str)
{
    _locActs->ReplaceActionCodeString(start, end, str);
}

void LocationPage::MoveActionTo(size_t actIndex, size_t moveTo)
{
    _locActs->MoveActionTo(actIndex, moveTo);
}

void LocationPage::DeleteAllActions()
{
    _locActs->Clear();
}

void LocationPage::RefreshActions()
{
    _locActs->RefreshActions();
}

void LocationPage::SetFocusOnActionCode()
{
    _locActs->SetFocusOnActionCode();
}

void LocationPage::ExpandCollapseAll(bool isExpanded)
{
    _locCode->ExpandCollapseAll(isExpanded);
}
