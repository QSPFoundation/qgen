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

#include "locationslistbox.h"
#include "mainframe.h"

IMPLEMENT_CLASS(LocationsListBox, wxTreeCtrl)

BEGIN_EVENT_TABLE(LocationsListBox, wxTreeCtrl)
	EVT_KEY_DOWN(LocationsListBox::OnKeyDown)
	EVT_RIGHT_DOWN(LocationsListBox::OnRightClick)
	EVT_LEFT_DCLICK(LocationsListBox::OnDoubleClick)
	EVT_TREE_END_LABEL_EDIT(wxID_ANY, LocationsListBox::OnEndLabelEdit)
	EVT_TREE_BEGIN_DRAG(wxID_ANY, LocationsListBox::OnBeginDrag)
	EVT_TREE_END_DRAG(wxID_ANY, LocationsListBox::OnEndDrag)
	EVT_MOTION(LocationsListBox::OnMouseMove)
	EVT_LEAVE_WINDOW(LocationsListBox::OnLeaveWindow)
	EVT_TIMER(ID_SHOW_TIMER, LocationsListBox::OnTimer)
END_EVENT_TABLE()

LocationsListBox::LocationsListBox(wxTopLevelWindow *parent, wxWindowID id, IControls *controls, long style) :
	wxTreeCtrl(parent, id, wxDefaultPosition, wxDefaultSize, style), _showTimer(this, ID_SHOW_TIMER)
{
	_controls = controls;
	_mainFrame = parent;
	_needForUpdate = false;

	_statesImageList.Create(16, 16);
	_statesImageList.Add(wxIcon(locslist_folder_closed_xpm));
	_statesImageList.Add(wxIcon(locslist_folder_opened_xpm));
	_statesImageList.Add(wxIcon(locslist_location_ball_closed_xpm));
	_statesImageList.Add(wxIcon(locslist_location_ball_opened_xpm));
	_statesImageList.Add(wxIcon(locslist_action_ball_xpm));

	AddRoot(wxT("Locs"));
	Update();
	_tip = new LocationTip(_mainFrame, _controls);
	_controls->GetSettings()->AddObserver(this);
}

LocationsListBox::~LocationsListBox()
{
	if (_showTimer.IsRunning())
		_showTimer.Stop();
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
		_controls->SyncWithLocationsList();
		_controls->UpdateLocationsList();
	}
}

bool LocationsListBox::IsItemOk( wxTreeItemId id, int flags )
{
	return id.IsOk() &&
		((flags & wxTREE_HITTEST_ONITEMINDENT) |
		 (flags & wxTREE_HITTEST_ONITEMICON) |
		 (flags & wxTREE_HITTEST_ONITEMLABEL) |
		 (flags & wxTREE_HITTEST_ONITEMRIGHT));
}

void LocationsListBox::OnRightClick( wxMouseEvent &event )
{
	wxMenu menu;
	int flags;
	wxTreeItemId id(HitTest(event.GetPosition(), flags));
	if (IsItemOk(id, flags))
	{
		SetFocus();
		SelectItem(id);
	}
	menu.Append(ID_LOC_CREATE, _("Create..."));
	menu.Append(ID_LOC_RENAME, _("Rename..."));
	menu.Append(ID_LOC_DEL, _("Delete"));
	menu.AppendSeparator();
	menu.Append(ID_FOLDER_CREAT, _("Create folder..."));
	menu.Append(ID_FOLDER_RENAME, _("Rename folder..."));
	menu.Append(ID_FOLDER_DEL, _("Delete folder"));
	menu.AppendSeparator();
	menu.Append(ID_LOC_COPY, _("Copy"));
	menu.Append(ID_LOC_PASTE, _("Paste"));
	menu.Append(ID_LOC_REPLACE, _("Replace"));
	menu.Append(ID_LOC_PASTENEW, _("Paste in..."));
	menu.Append(ID_LOC_CLEAR, _("Clear"));
	menu.AppendSeparator();
	menu.Append(ID_LOC_SORTASC, _("Sort ascending"));
	menu.Append(ID_LOC_SORTDESC, _("Sort descending"));
	menu.AppendSeparator();
	menu.Append(ID_LOC_EXPAND, _("Expand all"));
	menu.Append(ID_LOC_COLLAPSE, _("Collapse all"));
	_controls->UpdateMenuItems(&menu);
	PopupMenu(&menu);
}

void LocationsListBox::OnDoubleClick(wxMouseEvent &event )
{
	int flags;
	wxTreeItemId id(HitTest(event.GetPosition(), flags));
	if (IsItemOk(id, flags))
	{
		switch (GetItemType(id))
		{
		case DRAG_LOCATION:
			_controls->ShowLocation(GetItemText(id));
			break;
		case DRAG_ACTION:
			wxTreeItemId parent(GetItemParent(id));
			LocationPage *page = _controls->ShowLocation(GetItemText(parent));
			page->SelectAction(GetItemPos(parent, id));
			break;
		}
	}
	else
		event.Skip();
}

void LocationsListBox::Insert(const wxString &name, const wxString &pos, const wxString &folder)
{
	int image = -1;
	wxTreeItemId parent;
	if (folder.Length() > 0)
		parent = GetFolderByName(folder);
	else
		parent = GetRootItem();
	if (_controls->GetSettings()->GetShowLocsIcons())
		image = ICON_NOTACTIVELOC;
	if (pos.Length() > 0)
		InsertItem(parent, GetLocByName(GetRootItem(), pos), name, image);
	else
		InsertItem(parent, -1, name, image);
	_needForUpdate = true;
}

wxTreeItemId LocationsListBox::GetLocByName( const wxTreeItemId &parent, const wxString &name )
{
	wxTreeItemIdValue cookie;
	wxTreeItemId idCur(GetFirstChild(parent, cookie));
	while (idCur.IsOk())
	{
		if (IsFolderItem(idCur))
		{
			idCur = GetLocByName(idCur, name);
			if (idCur.IsOk()) return idCur;
		}
		else
		{
			if (GetItemText(idCur) == name) return idCur;
		}
		idCur = GetNextChild(parent, cookie);
	}
	return wxTreeItemId();
}

wxTreeItemId LocationsListBox::GetFolderByName( const wxString &name )
{
	wxTreeItemIdValue cookie;
	wxTreeItemId parent(GetRootItem());
	wxTreeItemId idCur(GetFirstChild(parent, cookie));
	while (idCur.IsOk())
	{
		if (IsFolderItem(idCur))
		{
			if (GetItemText(idCur) == name) break;
		}
		idCur = GetNextChild(parent, cookie);
	}
	return idCur;
}

void LocationsListBox::Select( const wxString &name )
{
	SelectItem(GetLocByName(GetRootItem(), name));
}

wxString LocationsListBox::GetStringSelection()
{
	wxTreeItemId id(GetSelection());
	if (id.IsOk())
	{
		if (GetItemParent(id) == GetRootItem())
		{
			if (!IsFolderItem(id))
				return GetItemText(id);
		}
		else
		{
			wxTreeItemId parent(GetItemParent(id));
			if (parent.IsOk())
			{
				if (IsFolderItem(parent))
					return GetItemText(id);
				else
					return GetItemText(parent);
			}
		}
	}
	return wxEmptyString;
}

wxString LocationsListBox::GetSelectedFolder()
{
	wxTreeItemId id(GetSelection());
	if (id.IsOk())
	{
		if (GetItemParent(id) == GetRootItem())
		{
			if (IsFolderItem(id))
				return GetItemText(id);
		}
		else
		{
			wxTreeItemId parent(GetItemParent(id));
			if (parent.IsOk())
			{
				if (IsFolderItem(parent))
					return GetItemText(parent);
				else
					return GetItemText(GetItemParent(parent));
			}
		}
	}
	return wxEmptyString;
}

void LocationsListBox::Clear()
{
	Freeze();
	DeleteChildren(GetRootItem());
	Thaw();
}

void LocationsListBox::UpdateLocationActions( const wxString &name )
{
	wxArrayString actions;
	long locIndex = _controls->GetContainer()->FindLocationIndex(name);
	_controls->GetContainer()->GetLocActions(locIndex, actions);
	size_t i, count = actions.GetCount();
	wxTreeItemId id(GetLocByName(GetRootItem(), name));
	DeleteChildren(id);
	if (_controls->GetSettings()->GetShowLocsIcons())
		for (i = 0; i < count; ++i) AppendItem(id, actions[i], ICON_ACTION);
	else
		for (i = 0; i < count; ++i) AppendItem(id, actions[i]);
}

void LocationsListBox::Delete( const wxString &name )
{
	wxTreeItemId id(GetLocByName(GetRootItem(), name));
	if (id.IsOk())
	{
		wxTreeCtrl::Delete(id);
		_needForUpdate = true;
	}
}

void LocationsListBox::ExpandCollapseItems(bool isExpand)
{
	Freeze();
	wxTreeItemIdValue cookie;
	wxTreeItemId parent(GetRootItem());
	wxTreeItemId id(GetFirstChild(parent, cookie));
	wxTreeItemId idFirst(id);
	while (id.IsOk())
	{
		if (isExpand)
			ExpandAllChildren(id);
		else
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
	switch (GetItemType(id))
	{
	case DRAG_FOLDER:
		label = label.Trim().Trim(false);
		if (label.IsEmpty())
			_controls->ShowMessage( QGEN_MSG_EMPTYDATA );
		else
			_controls->RenameFolder(container->FindFolderIndex(oldLabel), label);
		break;
	case DRAG_LOCATION:
		label = label.Trim().Trim(false);
		if (label.IsEmpty())
			_controls->ShowMessage( QGEN_MSG_EMPTYDATA );
		else
			_controls->RenameLocation(container->FindLocationIndex(oldLabel), label);
		break;
	case DRAG_ACTION:
		if (label.IsEmpty())
			_controls->ShowMessage( QGEN_MSG_EMPTYDATA );
		else
		{
			size_t locIndex = container->FindLocationIndex(GetItemText(GetItemParent(id)));
			_controls->RenameAction(locIndex, container->FindActionIndex(locIndex, oldLabel), label);
		}
		break;
	}
	event.Veto();
}

void LocationsListBox::OnBeginDrag( wxTreeEvent &event )
{
	_draggedId = event.GetItem();
	_draggedType = GetItemType(_draggedId);
	SetImageList(&_statesImageList);
	event.Allow();
}

void LocationsListBox::OnEndDrag( wxTreeEvent &event )
{
	ApplyStatesImageList();
	wxTreeItemId id(event.GetItem());
	wxString name(GetItemText(_draggedId));
	long image = GetItemImage(_draggedId);
	if (!id.IsOk())
	{
		if (_draggedType == DRAG_LOCATION)
		{
			wxPoint p = event.GetPoint();
			wxSize size = GetSize();
			if (p.x >= 0 && p.y >= 0 && p.x < size.GetWidth() && p.y < size.GetHeight())
			{
				wxTreeCtrl::Delete(_draggedId);
				SelectItem(AppendItem(GetRootItem(), name, image));
				UpdateLocationActions(name);
				_needForUpdate = true;
			}
		}
		return;
	}
	long pos;
	wxTreeItemId parent(GetItemParent(id));
	long dropOnType = GetItemType(id);
	long openedImage = GetItemImage(_draggedId, wxTreeItemIcon_Expanded);
	switch (_draggedType)
	{
	case DRAG_FOLDER:
		switch (dropOnType)
		{
		case DRAG_LOCATION:
			if (parent != GetRootItem()) break;
		case DRAG_FOLDER:
			_controls->SyncWithLocationsList();
			pos = GetItemPos(parent, id);
			wxTreeCtrl::Delete(_draggedId);
			id = InsertItem(parent, pos, name, image, -1, new FolderItem());
			SelectItem(id);
			SetItemImage(id, openedImage, wxTreeItemIcon_Expanded);
			UpdateFolderLocations(name);
			_needForUpdate = true;
			break;
		}
		break;
	case DRAG_LOCATION:
		switch (dropOnType)
		{
		case DRAG_FOLDER:
			wxTreeCtrl::Delete(_draggedId);
			SelectItem(InsertItem(id, -1, name, image));
			UpdateLocationActions(name);
			_needForUpdate = true;
			break;
		case DRAG_LOCATION:
			pos = GetItemPos(parent, id);
			wxTreeCtrl::Delete(_draggedId);
			SelectItem(InsertItem(parent, pos, name, image));
			UpdateLocationActions(name);
			_needForUpdate = true;
			break;
		}
		break;
	case DRAG_ACTION:
		switch (dropOnType)
		{
		case DRAG_ACTION:
			if (parent == GetItemParent(_draggedId))
			{
				long locIndex = _controls->GetContainer()->FindLocationIndex(GetItemText(parent));
				_controls->MoveActionTo(locIndex, GetItemPos(parent, _draggedId), GetItemPos(parent, id));
			}
			break;
		}
		break;
	}
}

void LocationsListBox::SetLocStatus( const wxString &name, bool isOpened )
{
	if (_controls->GetSettings()->GetShowLocsIcons())
		SetItemImage(GetLocByName(GetRootItem(), name), isOpened ? ICON_ACTIVELOC : ICON_NOTACTIVELOC);
}

void LocationsListBox::ApplyStatesImageList()
{
	if (_controls->GetSettings()->GetShowLocsIcons())
		SetImageList(&_statesImageList);
	else
		SetImageList(NULL);
}

void LocationsListBox::AddFolder( const wxString &name )
{
	if (_controls->GetSettings()->GetShowLocsIcons())
	{
		wxTreeItemId id = AppendItem(GetRootItem(), name, ICON_FOLDER, -1, new FolderItem());
		SetItemImage(id, ICON_FOLDEROPENED, wxTreeItemIcon_Expanded);
	}
	else
		AppendItem(GetRootItem(), name, -1, -1, new FolderItem());
	_needForUpdate = true;
}

long LocationsListBox::GetItemType( const wxTreeItemId &id )
{
	if (GetItemParent(id) == GetRootItem()) // Локация или папка
	{
		if (IsFolderItem(id)) // Папка
			return DRAG_FOLDER;
		else // Локация
			return DRAG_LOCATION;
	}
	else // Локация или действие
	{
		if (IsFolderItem(GetItemParent(id))) // Локация
			return DRAG_LOCATION;
		else // Действие
			return DRAG_ACTION;
	}
}

void LocationsListBox::UpdateFolderLocations( const wxString &name )
{
	wxString text;
	wxTreeItemId parent(GetFolderByName(name));
	DeleteChildren(parent);
	DataContainer *container = _controls->GetContainer();
	long folderIndex = container->FindFolderIndex(name);
	for (size_t i = 0; i < container->GetLocationsCount(); ++i)
	{
		if (container->GetLocFolder(i) == folderIndex)
		{
			text = container->GetLocationName(i);
			if (_controls->GetSettings()->GetShowLocsIcons())
				AppendItem(parent, text, ICON_NOTACTIVELOC);
			else
				AppendItem(parent, text);
			UpdateLocationActions(text);
		}
	}
	_controls->ShowOpenedLocationsIcons();
}

long LocationsListBox::GetItemPos( const wxTreeItemId &parent, const wxTreeItemId &id )
{
	long pos = 0;
	wxTreeItemIdValue cookie;
	wxTreeItemId idCur(GetFirstChild(parent, cookie));
	while (idCur.IsOk())
	{
		if (idCur == id) return pos;
		++pos;
		idCur = GetNextChild(parent, cookie);
	}
	return -1;
}

void LocationsListBox::SetLocName( const wxString &name, const wxString &newName )
{
	wxTreeItemId id(GetLocByName(GetRootItem(), name));
	if (id.IsOk())
		SetItemText(id, newName);
}

void LocationsListBox::SetFolderName( const wxString &name, const wxString &newName )
{
	wxTreeItemId id(GetFolderByName(name));
	if (id.IsOk())
		SetItemText(id, newName);
}

void LocationsListBox::UpdateDataContainer()
{
	long locPos = -1, folderPos = -1, pos = -1;
	UpdateDataContainer(GetRootItem(), -1, &locPos, &folderPos, &pos);
	_needForUpdate = false;
}

void LocationsListBox::UpdateDataContainer( const wxTreeItemId &parent, long folder, long *locPos, long *folderPos, long *pos )
{
	DataContainer *container = _controls->GetContainer();
	wxTreeItemIdValue cookie;
	wxTreeItemId idCur(GetFirstChild(parent, cookie));
	while (idCur.IsOk())
	{
		++(*pos);
		if (IsFolderItem(idCur))
		{
			++(*folderPos);
			long curInd = container->FindFolderIndex(GetItemText(idCur));
			container->SetFolderPos(curInd, *pos);
			container->MoveFolder(curInd, *folderPos);
			UpdateDataContainer(idCur, *folderPos, locPos, folderPos, pos);
		}
		else
		{
			++(*locPos);
			long curInd = container->FindLocationIndex(GetItemText(idCur));
			container->SetLocFolder(curInd, folder);
			container->MoveLocationTo(curInd, *locPos);
		}
		idCur = GetNextChild(parent, cookie);
	}
}

void LocationsListBox::DeleteFolder( const wxString &name )
{
	wxTreeItemId id(GetFolderByName(name));
	if (id.IsOk())
	{
		wxTreeCtrl::Delete(id);
		_needForUpdate = true;
	}
}

bool LocationsListBox::IsFolderItem( const wxTreeItemId &id )
{
	FolderItem *data = dynamic_cast<FolderItem *>(GetItemData(id));
	return (data != NULL);
}

void LocationsListBox::OnMouseMove(wxMouseEvent &event)
{
	if (!_mainFrame->IsActive() || !_controls->GetSettings()->GetShowShortLocsDescs())
	{
		event.Skip();
		return;
	}
	int flags;
	wxPoint mousePos = event.GetPosition();
	if (_prevMousePos != mousePos)
	{
		bool hideTip = true;
		_prevMousePos = mousePos;
		wxTreeItemId id(HitTest(mousePos, flags));
		if (id.IsOk())
		{
			if (GetItemType(id) != DRAG_FOLDER)
			{
				if (IsItemOk(id, flags))
				{
					switch (GetItemType(id))
					{
					case DRAG_ACTION:
						if (GetItemText(id) != _tip->GetActName())
						{
							_tip->HideTip();
							_tip->SetActName(GetItemText(id));
							_tip->SetLocName(GetItemText(GetItemParent(id)));
							_showTimer.Start(500, true);
						}
						break;
					case DRAG_LOCATION:
						if (GetItemText(id) != _tip->GetLocName() || !_tip->GetActName().IsEmpty())
						{
							_tip->HideTip();
							_tip->SetLocName(GetItemText(id));
							_showTimer.Start(500, true);
						}
						break;
					}
					hideTip = false;
				}
			}
		}
		if (hideTip)
		{
			if (_showTimer.IsRunning())
				_showTimer.Stop();
			_tip->HideTip();
		}
	}

	event.Skip();
}

void LocationsListBox::OnLeaveWindow(wxMouseEvent &event)
{
	if (!_mainFrame->IsActive() || !_controls->GetSettings()->GetShowShortLocsDescs())
	{
		event.Skip();
		return;
	}
	if (_showTimer.IsRunning())
		_showTimer.Stop();
	_tip->HideTip();
	event.Skip();
}

void LocationsListBox::OnTimer(wxTimerEvent &event)
{
	_tip->MoveTip(ClientToScreen(_prevMousePos));
}

void LocationsListBox::OnKeyDown(wxKeyEvent &event)
{
	if (!_controls->ExecuteHotkey(event.GetKeyCode(), event.GetModifiers()))
		event.Skip();
}
