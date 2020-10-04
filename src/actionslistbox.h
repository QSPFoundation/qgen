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

#ifndef _QGEN_ACTIONS_LIST_BOX_H_
    #define _QGEN_ACTIONS_LIST_BOX_H_

    #include <wx/wx.h>
    #include <wx/fontmap.h>
    #include <wx/htmllbox.h>
    #include "datacontainer.h"
    #include "actioncode.h"

    class ActionsListBox : public wxHtmlListBox, public IObserver
    {
        DECLARE_CLASS(ActionsListBox)
        DECLARE_EVENT_TABLE()
    private:
        ILocationPage * _locPage;
        IControls * _controls;
        ActionCode * _actCode;
        DataContainer * _container;
        wxString _outFormat;
        wxString _outFormatImage;
        wxString _outFormatBold;
        wxString _outFormatBoldImage;
        wxString _outFormatItalic;
        wxString _outFormatItalicImage;
        wxString _outFormatBoldItalic;
        wxString _outFormatBoldItalicImage;
        wxArrayString _items;
        bool _isDragging;
        int _draggedAction;
        int _prevActionIndex;
        wxFont _font;

        void OnRightClick(wxMouseEvent &event);
        void OnKeyDown(wxKeyEvent &event);
        void OnMouseMove(wxMouseEvent &event);
        void OnMouseLeftUp(wxMouseEvent &event);
        void OnMouseCaptureLost(wxMouseCaptureLostEvent &event);
        void OnActionChange(wxCommandEvent &event);

        static wxString GetHexColor(const wxColour& color);
        static wxString HtmlizeWhitespaces(const wxString& str);
        static wxString ProceedAsPlain(const wxString& str);

        void CreateHTMLParser() const;
        virtual wxString OnGetItem(size_t n) const;
    public:
        ActionsListBox(wxWindow *owner, wxWindowID id, ILocationPage *locPage,
            ActionCode *actCode, IControls *controls, long style = 0);
        ~ActionsListBox();

        void SetStandardFonts(int size = -1,
            const wxString& normal_face = wxEmptyString,
            const wxString& fixed_face = wxEmptyString);
        void Update(bool isFromObservable = false);
        void LoadActionData(size_t actIndex);
        void SaveActionData();
        void LoadAllActions();
        void RefreshActions();

        size_t AddAction(const wxString& name);
        void DeleteAction(size_t actIndex);
        void DeleteAllActions();

        void MoveItemTo(size_t actIndex, size_t moveTo);
        int GetSelection() const;
        void Select(int index);
        size_t Append(const wxString &name);
        size_t GetCount() const;
        void Delete(size_t index);
        wxString GetString(size_t index) const;
        void SetString(size_t index, const wxString & name);
        void Insert(const wxString & name, size_t index);
        void Clear();
    };

#endif
