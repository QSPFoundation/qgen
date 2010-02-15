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

#include "LocationsNoteBook.h"

IMPLEMENT_CLASS(LocationsNotebook, wxAuiNotebook)

BEGIN_EVENT_TABLE(LocationsNotebook, wxAuiNotebook)
	EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, LocationsNotebook::OnClosePage)
	EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_ANY, LocationsNotebook::OnPageChanged)
	EVT_AUINOTEBOOK_TAB_RIGHT_UP(wxID_ANY, LocationsNotebook::OnRightUpClick)
	EVT_MENU(ID_MENUCLOSEALLTABS, LocationsNotebook::OnTabMenu)
	EVT_MENU(ID_MENUCLOSEEXCEPTSELECTED, LocationsNotebook::OnTabMenu)
	EVT_MENU(ID_MENUCLOSESELECTED, LocationsNotebook::OnTabMenu)
	EVT_MENU(ID_MENUFIXTAB, LocationsNotebook::OnFixPage)
	EVT_NAVIGATION_KEY(LocationsNotebook::OnNavigationKeyNotebook)
END_EVENT_TABLE()

LocationsNotebook::LocationsNotebook(wxWindow* parent, wxWindowID id, IControls *controls, long style) :
	wxAuiNotebook( parent, id, wxDefaultPosition, wxDefaultSize, style )
{
	_controls = controls;
	Update();
	_controls->GetSettings()->AddObserver(this);
}

LocationsNotebook::~LocationsNotebook()
{
	_controls->GetSettings()->RemoveObserver(this);
}

void LocationsNotebook::NotifyClosePage( int index )
{
	LocationPage *page = (LocationPage *)GetPage(index);
	page->SavePage();
	_controls->UpdateLocationIcon(page->GetLocationIndex(), false);
}

bool LocationsNotebook::DeletePage( size_t page )
{
	NotifyClosePage(page);
	return wxAuiNotebook::DeletePage(page);
}

void LocationsNotebook::OnClosePage(wxAuiNotebookEvent& event)
{
	NotifyClosePage(event.GetSelection());
}

void LocationsNotebook::OnPageChanged( wxAuiNotebookEvent &event )
{
	_controls->SelectLocation(((LocationPage *)GetPage(event.GetSelection()))->GetLocationIndex());
}

bool LocationsNotebook::DeleteAllPages( CloseTypePage closeType, int selIndex )
{
	LocationPage *page;
	int i, count = GetPageCount();
	for (i = count - 1; i >= 0; --i)
	{
		if (selIndex < 0)
			DeletePage(i);
		else
		{
			page = (LocationPage *)GetPage(i);
			if ((closeType == CLOSE_ALL && !page->IsFixed()) ||
				(closeType == CLOSE_ALLEXCEPTSELECTED && i != selIndex && !page->IsFixed()) ||
				(closeType == CLOSE_SELECTED && i == selIndex)) DeletePage(i);
		}
	}
	return true;
}

int LocationsNotebook::FindPageIndex(const wxString& namePage)
{
	size_t i, count = GetPageCount();
	for (i = 0; i < count; ++i)
		if (namePage == GetPageText(i)) return (int)i;
	return wxNOT_FOUND;
}

LocationPage * LocationsNotebook::OpenLocationPage( const wxString& namePage, bool isSelect )
{
	size_t locIndex = _controls->GetContainer()->FindLocationIndex(namePage);
	LocationPage *page = new LocationPage(this, _controls);
	page->SetLocationIndex(locIndex);
	AddPage(page, namePage, isSelect);
	page->LoadPage();
	_controls->UpdateLocationIcon(locIndex, true);
	return page;
}

LocationPage * LocationsNotebook::GetSelectedPage()
{
	int selPage = GetSelection();
	return (GetPageCount() && selPage >= 0 ? ( LocationPage * )GetPage(selPage) : NULL);
}

LocationPage * LocationsNotebook::GetPageByLocName( const wxString &name )
{
	int idx = FindPageIndex( name );
	return (idx >= 0 ? ( LocationPage * )GetPage(idx) : NULL);
}

void LocationsNotebook::LoadOpenedPages()
{
	size_t i, count = GetPageCount();
	for (i = 0; i < count; ++i)
		((LocationPage *)GetPage(i))->LoadPage();
}

void LocationsNotebook::SaveOpenedPages()
{
	size_t i, count = GetPageCount();
	for (i = 0; i < count; ++i)
		((LocationPage *)GetPage(i))->SavePage();
}

void LocationsNotebook::OnRightUpClick( wxAuiNotebookEvent &event )
{
	if (event.GetEventObject() != this)
	{
		event.Skip();
		return;
	}
	selectedPage = event.GetSelection();
	if (selectedPage < 0) return;
	wxMenu menu;
	menu.Append(ID_MENUCLOSESELECTED, wxT("������� ���������"));
	menu.Append(ID_MENUCLOSEEXCEPTSELECTED, wxT("������� ��� ����� ���������"));
	menu.Append(ID_MENUCLOSEALLTABS, wxT("������� ���"));
	menu.AppendSeparator();
	if (((LocationPage *)GetPage(selectedPage))->IsFixed())
		menu.Append(ID_MENUFIXTAB, wxT("���������"));
	else
		menu.Append(ID_MENUFIXTAB, wxT("��������� �������"));
	PopupMenu(&menu);
}

void LocationsNotebook::OnTabMenu( wxCommandEvent &event )
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
	DeleteAllPages(type, selectedPage);
}

void LocationsNotebook::OnFixPage( wxCommandEvent &event )
{
	SwitchPageFixed(selectedPage);
}

void LocationsNotebook::OnNavigationKeyNotebook( wxNavigationKeyEvent &event )
{
	if (event.IsWindowChange())
		AdvanceSelection(event.GetDirection());
	else
		wxAuiNotebook::OnNavigationKeyNotebook(event);
}

void LocationsNotebook::SwitchPageFixed( size_t selPage )
{
	LocationPage *page = (LocationPage *)GetPage(selPage);
	bool isFixed = !page->IsFixed();
	page->SetFixed(isFixed);
	SetPageBitmap(selPage, isFixed ? wxBitmap(locstabs_page_fixed_xpm) : wxNullBitmap);
}

void LocationsNotebook::Update(bool isFromObservable)
{
	Settings *settings = _controls->GetSettings();
	m_mgr.GetArtProvider()->SetColour(wxAUI_DOCKART_BACKGROUND_COLOUR, settings->GetBaseBackColour());
	m_mgr.Update();
}

void LocationsNotebook::AdvanceSelection( bool forward )
{
	int lastIndex = GetPageCount() - 1;
	if (lastIndex <= 0)
		return;
	int currentSelection = GetSelection();
	if (forward)
	{
		if (currentSelection >= 0 && currentSelection < lastIndex)
			++currentSelection;
		else
			currentSelection = 0;
	}
	else
	{
		if (currentSelection > 0)
			--currentSelection;
		else
			currentSelection = lastIndex;
	}
	SetSelection(currentSelection);
}
