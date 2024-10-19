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

#ifndef _QGEN_ACTIONS_PANEL_H_
    #define _QGEN_ACTIONS_PANEL_H_

    #include <wx/wx.h>
    #include "icontrols.h"
    #include "actioncode.h"
    #include "actionslistbox.h"

    #include "bitmaps/toolbar_action_new.xpm"
    #include "bitmaps/toolbar_action_rename.xpm"
    #include "bitmaps/toolbar_action_delete.xpm"

    class ActionsPanel : public wxPanel
    {
        DECLARE_CLASS(ActionsPanel)
        DECLARE_EVENT_TABLE()
    private:
        ActionsListBox * _actList;
        wxButton * _addActButton;
        wxButton * _renActButton;
        wxButton * _delActButton;
        IControls * _controls;

        void OnAddAction(wxCommandEvent &event);
        void OnRenAction(wxCommandEvent &event);
        void OnDelAction(wxCommandEvent &event);
    public:
        ActionsPanel(wxWindow *owner, ILocationPage *locPage, ActionCode *_actCode, IControls *controls);

        void EnableButtons();
        ActionsListBox *GetActionsListBox() { return _actList; };
    };

#endif
