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

#include "LocationsListBox.h"

IMPLEMENT_CLASS(LocationsListBox, wxTreeCtrl)

BEGIN_EVENT_TABLE(LocationsListBox, wxTreeCtrl)
	EVT_RIGHT_DOWN(LocationsListBox::OnRightClick)
	EVT_LEFT_DCLICK(LocationsListBox::OnDoubleClick)
	EVT_TREE_END_LABEL_EDIT(wxID_ANY, LocationsListBox::OnEndLabelEdit)
	EVT_TREE_BEGIN_DRAG(wxID_ANY, LocationsListBox::OnBeginDrag)
	EVT_TREE_END_DRAG(wxID_ANY, LocationsListBox::OnEndDrag)
END_EVENT_TABLE()

LocationsListBox::LocationsListBox(wxWindow *parent, wxWindowID id, IControls *controls, long style) :
	wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize, style)
{
	_controls = controls;

	_statesImageList.Create(15, 15);
	_statesImageList.Add(wxBitmap(location_closed_xpm));
	_statesImageList.Add(wxBitmap(location_xpm));
	_statesImageList.Add(wxBitmap(action_xpm));

	AddRoot(wxT("Locs"));
	Update();
	_controls->GetSettings()->AddObserver(this);
}

LocationsListBox::~LocationsListBox()
{
	Clear();
	_controls->GetSettings()->RemoveObserver(this);
}

void LocationsListBox::Update(bool isFromObservable)
{
	Settings *settings = _controls->GetSettings();
	SetFont(settings->GetFont(SYNTAX_BASE));
	SetForegroundColour(settings->GetColour(SYNTAX_BASE));
	SetBackgroundColour(settings->GetBaseBackColour());
	ApplyStatesImageList();
	if (isFromObservable)
	{
		_controls->UpdateLocationsList();
		_controls->ShowOpenedLocationsIcons();
	}
}

bool LocationsListBox::IsItemOk( wxTreeItemId id, int flags )
{
	return id.IsOk() &&
		(flags == wxTREE_HITTEST_ONITEMINDENT ||
		 flags == wxTREE_HITTEST_ONITEMICON ||
		 flags == wxTREE_HITTEST_ONITEMLABEL ||
		 flags == wxTREE_HITTEST_ONITEMRIGHT);
}

void LocationsListBox::OnRightClick( wxMouseEvent &event )
{
	wxMenu menu;
	int flags;
	wxTreeItemId id = HitTest(event.GetPosition(), flags);
	if (IsItemOk(id, flags))
	{
		SetFocus();
		SelectItem(id);
	}
	menu.Append(LOC_CREAT, wxT("Создать..."));
	menu.Append(LOC_RENAME, wxT("Переименовать..."));
	menu.Append(LOC_DEL, wxT("Удалить"));
	menu.AppendSeparator();
	menu.Append(LOC_COPY, wxT("Копировать"));
	menu.Append(LOC_PASTE, wxT("Вставить"));
	menu.Append(LOC_REPLACE, wxT("Заменить"));
	menu.Append(LOC_PASTE_NEW, wxT("Вставить в..."));
	menu.Append(LOC_CLEAR, wxT("Очистить"));
	menu.AppendSeparator();
	menu.Append(LOC_SORT_ASC, wxT("Сортировать по алфавиту"));
	menu.Append(LOC_SORT_DESC, wxT("Сортировать в обратном порядке"));
	menu.AppendSeparator();
	menu.Append(LOC_EXPAND, wxT("Развернуть все"));
	menu.Append(LOC_COLLAPSE, wxT("Свернуть все"));
	_controls->UpdateMenuItems(&menu);
	PopupMenu(&menu);
}

void LocationsListBox::OnDoubleClick(wxMouseEvent &event )
{
	int flags;
	wxTreeItemId id = HitTest(event.GetPosition(), flags);
	if (IsItemOk(id, flags))
	{
		if (GetItemParent(id) == GetRootItem())
			_controls->ShowLocation(GetItemText(id));
		else
		{
			LocationPage *page = _controls->ShowLocation(GetItemText(GetItemParent(id)));
			page->SelectAction(_controls->GetContainer()->FindActionIndex(page->GetLocationIndex(), GetItemText(id)));
		}
	}
	else
		event.Skip();
}

void LocationsListBox::Insert(const wxString &text, size_t pos)
{
	if (_controls->GetSettings()->GetShowLocsIcons())
		InsertItem(GetRootItem(), pos, text, ICON_NOTACTIVELOCATION);
	else
		InsertItem(GetRootItem(), pos, text);
}

wxTreeItemId LocationsListBox::GetLocByPos(size_t index)
{
	wxTreeItemId parent(GetRootItem());
	wxTreeItemIdValue cookie;
	wxTreeItemId idCur = GetFirstChild(parent, cookie);
	while (index != 0 && idCur.IsOk())
	{
		index--;
		idCur = GetNextChild(parent, cookie);
	}
	return idCur;
}

void LocationsListBox::Select( size_t index )
{
	SelectItem(GetLocByPos(index));
}

wxString LocationsListBox::GetString( size_t index )
{
	return GetItemText(GetLocByPos(index));
}

wxString LocationsListBox::GetStringSelection()
{
	wxTreeItemId id(GetSelection());
	if (id.IsOk())
	{
		if (GetItemParent(id) == GetRootItem())
			return GetItemText(id);
		else
			return GetItemText(GetItemParent(id));
	}
	else
		return wxEmptyString;
}

long LocationsListBox::GetStringIndex( const wxString &text )
{
	wxTreeItemId parent(GetRootItem());
	long index = 0;
	wxTreeItemIdValue cookie;
	wxTreeItemId idCur = GetFirstChild(parent, cookie);
	while (idCur.IsOk())
	{
		if (GetItemText(idCur) == text)
			return index;
		index++;
		idCur = GetNextChild(parent, cookie);
	}
	return wxNOT_FOUND;
}

void LocationsListBox::Clear()
{
	Freeze();
	DeleteChildren(GetRootItem());
	Thaw();
}

void LocationsListBox::UpdateLocationActions( size_t locIndex, const wxArrayString & actions )
{
	size_t i, count = actions.GetCount();
	wxTreeItemId id = GetLocByPos(locIndex);
	DeleteChildren(id);
	if (_controls->GetSettings()->GetShowLocsIcons())
		for (i = 0; i < count; ++i) AppendItem(id, actions[i], ICON_ACTION);
	else
		for (i = 0; i < count; ++i) AppendItem(id, actions[i]);
}

size_t LocationsListBox::GetCount()
{
	return GetChildrenCount(GetRootItem(), false);
}

void LocationsListBox::Delete( size_t index )
{
	wxTreeCtrl::Delete(GetLocByPos(index));
}

void LocationsListBox::SetString( size_t index, const wxString & text )
{
	SetItemText(GetLocByPos(index), text);
}

void LocationsListBox::ExpandItems()
{
	if (!GetCount()) return;
	Freeze();
	wxTreeItemId parent(GetRootItem());
	wxTreeItemIdValue cookie;
	wxTreeItemId id = GetFirstChild(parent, cookie);
	wxTreeItemId idFirst = id;
	while (id.IsOk())
	{
		ExpandAllChildren(id);
		id = GetNextChild(parent, cookie);
	}
	ScrollTo(idFirst);
	Thaw();
}

void LocationsListBox::CollapseItems()
{
	if (!GetCount()) return;
	Freeze();
	wxTreeItemId parent(GetRootItem());
	wxTreeItemIdValue cookie;
	wxTreeItemId id = GetFirstChild(parent, cookie);
	wxTreeItemId idFirst = id;
	while (id.IsOk())
	{
		CollapseAllChildren(id);
		id = GetNextChild(parent, cookie);
	}
	ScrollTo(idFirst);
	Thaw();
}

void LocationsListBox::OnEndLabelEdit( wxTreeEvent &event )
{
	if (event.IsEditCancelled()) return;
	wxTreeItemId id(event.GetItem());
	wxString label(event.GetLabel());
	wxString oldLabel(GetItemText(id));
	DataContainer *container = _controls->GetContainer();
	if (GetItemParent(id) == GetRootItem()) // Локация
	{
		label = label.Trim().Trim(false);
		if (label.IsEmpty())
			_controls->ShowMessage( QGEN_MSG_EMPTYDATA );
		else
			_controls->RenameLocation(container->FindLocationIndex(oldLabel), label);
	}
	else // Действие
	{
		if (label.IsEmpty())
			_controls->ShowMessage( QGEN_MSG_EMPTYDATA );
		else
		{
			size_t locIndex = container->FindLocationIndex(GetItemText(GetItemParent(id)));
			_controls->RenameAction(locIndex, container->FindActionIndex(locIndex, oldLabel), label);
		}
	}
	event.Veto();
}

void LocationsListBox::OnBeginDrag( wxTreeEvent &event )
{
	wxTreeItemId id(event.GetItem());
	if (GetItemParent(id) == GetRootItem()) // Локация
	{
		_draggedLocIndex = GetStringIndex(GetItemText(id));
		_draggedActIndex = wxNOT_FOUND;
	}
	else // Действие
	{
		_draggedLocIndex = GetStringIndex(GetItemText(GetItemParent(id)));
		_draggedActIndex = _controls->GetContainer()->FindActionIndex(_draggedLocIndex, GetItemText(id));
	}
	SetImageList(&_statesImageList);
	event.Allow();
}

void LocationsListBox::OnEndDrag( wxTreeEvent &event )
{
	ApplyStatesImageList();
	wxTreeItemId id(event.GetItem());
	if (!id.IsOk()) return;
	if (_draggedActIndex < 0)
	{
		if (GetItemParent(id) == GetRootItem()) // Локация
			_controls->MoveLocationTo(_draggedLocIndex, GetStringIndex(GetItemText(id)));
	}
	else
	{
		if (GetItemParent(id) != GetRootItem()) // Действие
		{
			if (GetStringIndex(GetItemText(GetItemParent(id))) == _draggedLocIndex)
				_controls->MoveActionTo(_draggedLocIndex, _draggedActIndex, _controls->GetContainer()->FindActionIndex(_draggedLocIndex, GetItemText(id)));
		}
	}
}

void LocationsListBox::MoveItemTo( size_t locIndex, size_t moveTo )
{
	wxTreeItemId id(GetLocByPos(locIndex));
	wxString label(GetItemText(id));
	int image = GetItemImage(id);
	wxTreeCtrl::Delete(id);
	InsertItem(GetRootItem(), moveTo, label, image);
	Select(moveTo);
}

void LocationsListBox::SetLocStatus( size_t locIndex, bool isOpened )
{
	if (_controls->GetSettings()->GetShowLocsIcons())
		SetItemImage(GetLocByPos(locIndex), isOpened ? ICON_ACTIVELOCATION : ICON_NOTACTIVELOCATION);
}

void LocationsListBox::ApplyStatesImageList()
{
	if (_controls->GetSettings()->GetShowLocsIcons())
		SetImageList(&_statesImageList);
	else
		SetImageList(NULL);
}
