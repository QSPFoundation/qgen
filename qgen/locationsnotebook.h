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

#ifndef _QGEN_LOCATIONS_NOTEBOOK_H_
    #define _QGEN_LOCATIONS_NOTEBOOK_H_

    #include <wx/aui/aui.h>
    #include "locationpage.h"

    enum CloseTypePage
    {
        CLOSE_ALL,
        CLOSE_ALL_NONFIXED,
        CLOSE_ALL_NONFIXED_EXCEPT_SELECTED,
        CLOSE_SELECTED
    };

    class LocationsNotebook : public wxAuiNotebook, public IObserver
    {
        DECLARE_CLASS(LocationsNotebook)
        DECLARE_EVENT_TABLE()
    private:
        IControls * _controls;
        DataContainer * _container;
        int                selectedPage;

        void OnClosePage(wxAuiNotebookEvent &event);
        void OnPageChanged(wxAuiNotebookEvent &event);
        void OnRightUpClick(wxAuiNotebookEvent &event);
        void OnNavigationKeyNotebook(wxNavigationKeyEvent &event);

        void NotifyClosePage(int index);
    public:
        LocationsNotebook(wxWindow *parent, wxWindowID id, IControls *controls,
                          long style = wxAUI_NB_DEFAULT_STYLE|wxAUI_NB_WINDOWLIST_BUTTON);

        ~LocationsNotebook();

        LocationPage *GetSelectedPage();
        LocationPage *GetPageByLocName(const wxString &name);
        int FindPageIndex(const wxString& pageName);
        LocationPage *OpenLocationPage(const wxString& pageName, bool toSelect);
        bool ClosePage(size_t page);
        bool ClosePages(CloseTypePage closeType);
        void LoadOpenPages();
        void SaveOpenPages();
        void Update(bool isFromObservable = false);
        void AdvanceSelection(bool forward = true);
        void SwitchPageFixed(size_t pageInd);
    };

#endif
