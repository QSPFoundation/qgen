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

#ifndef _QGEN_LOCATION_DESCRIPTION_H_
    #define _QGEN_LOCATION_DESCRIPTION_H_

    #include <wx/wx.h>
    #include <wx/aui/aui.h>
    #include "ilocationpage.h"
    #include "icontrols.h"
    #include "datacontainer.h"
    #include "syntaxtextbox.h"

    class LocationDesc : public wxPanel, public IObserver
    {
        DECLARE_CLASS(LocationDesc)
    private:
        ILocationPage * _locPage;
        SyntaxTextBox * _text;
        IControls * _controls;
        wxStaticText * _stTextDesc;
    public:
        LocationDesc(wxWindow *owner, ILocationPage *locPage, IControls *controls);
        ~LocationDesc();

        void Update(bool isFromObservable = false);
        void SaveDesc();
        void LoadDesc();
        void Clear();
        void SelectString(long startPos, long lastPos);
        void ReplaceString(long start, long end, const wxString &str);
    };

#endif
