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

#ifndef _QGEN_LOCATIONS_LIST_BOX_H_
    #define _QGEN_LOCATIONS_LIST_BOX_H_

    #include <wx/wx.h>
    #include <wx/treectrl.h>
    #include <wx/imaglist.h>
    #include "icontrols.h"
    #include "locationpage.h"
    #include "locationtip.h"

    enum
    {
        ICON_FOLDER,
        ICON_FOLDEROPENED,
        ICON_NOTACTIVELOC,
        ICON_ACTIVELOC,
        ICON_ACTION
    };

    enum
    {
        DRAG_ACTION,
        DRAG_LOCATION,
        DRAG_FOLDER,
    };

    enum
    {
        ID_SHOW_TIMER = 10200
    };

    class FolderItem : public wxTreeItemData
    {
    };

    class LocationsListBox : public wxTreeCtrl, public IObserver
    {
        DECLARE_CLASS(LocationsListBox)
        DECLARE_EVENT_TABLE()
    private:
        IControls * _controls;
        wxImageList _statesImageList;
        wxTreeItemId _draggedId;
        LocationTip * _tip;
        long _draggedType;
        bool _needForUpdate;
        wxTopLevelWindow * _mainFrame;
        wxPoint _prevMousePos;
        wxTimer _showTimer;

        void OnRightClick(wxMouseEvent &event);
        void OnKeyDown(wxKeyEvent &event);
        void OnDoubleClick(wxMouseEvent &event);
        void OnEndLabelEdit(wxTreeEvent &event);
        void OnBeginDrag(wxTreeEvent &event);
        void OnEndDrag(wxTreeEvent &event);
        void OnMouseMove(wxMouseEvent &event);
        void OnLeaveWindow(wxMouseEvent &event);
        void OnTimer(wxTimerEvent &event);
        bool IsItemOk(wxTreeItemId id, int flags);
        wxTreeItemId GetLocByName(const wxTreeItemId &parent, const wxString &name);
        wxTreeItemId GetFolderByName(const wxString &name);
        long GetItemType(const wxTreeItemId &id);
        long GetItemPos(const wxTreeItemId &parent, const wxTreeItemId &id);
        void UpdateDataContainer(const wxTreeItemId &parent, long folder, long *locPos, long *folderPos, long *pos);
        bool IsFolderItem(const wxTreeItemId &id);
    public:
        LocationsListBox(wxTopLevelWindow *parent, wxWindowID id, IControls *controls,
                         long style = wxTR_HAS_BUTTONS|wxTR_LINES_AT_ROOT|
                                      wxTR_HIDE_ROOT|wxTR_NO_LINES|
                                      wxTR_ROW_LINES|wxTR_EDIT_LABELS|
                                      wxTR_FULL_ROW_HIGHLIGHT);
        ~LocationsListBox();

        bool IsNeedForUpdate() const { return _needForUpdate; }
        void Update(bool isFromObservable = false);
        void Insert(const wxString &name, const wxString &pos, const wxString &folder);
        void Delete(const wxString &name);
        void Select(const wxString &name);
        void SetLocName(const wxString &name, const wxString &newName);
        void SetFolderName(const wxString &name, const wxString &newName);
        wxString GetStringSelection();
        wxString GetSelectedFolder();
        void Clear();
        void ExpandCollapseItems(bool isExpand);
        void ApplyStatesImageList();

        void UpdateLocationActions(const wxString &name);
        void UpdateFolderLocations(const wxString &name);
        void SetLocStatus(const wxString &name, bool isOpened);

        void AddFolder(const wxString &name);
        void DeleteFolder(const wxString &name);

        void UpdateDataContainer();
    };

#endif
