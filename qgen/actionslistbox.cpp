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

#include "actionslistbox.h"
#include "mainframe.h"

IMPLEMENT_CLASS(ActionsListBox, wxHtmlListBox)

BEGIN_EVENT_TABLE(ActionsListBox, wxHtmlListBox)
    EVT_KEY_DOWN(ActionsListBox::OnKeyDown)
    EVT_RIGHT_DOWN(ActionsListBox::OnRightClick)
    EVT_MOTION(ActionsListBox::OnMouseMove)
    EVT_MOUSE_CAPTURE_LOST(ActionsListBox::OnMouseCaptureLost)
    EVT_LEFT_UP(ActionsListBox::OnMouseLeftUp)
    EVT_LISTBOX(wxID_ANY, ActionsListBox::OnActionChange)
END_EVENT_TABLE()

ActionsListBox::ActionsListBox(wxWindow *owner, wxWindowID id, ILocationPage *locPage, 
                               ActionCode *actCode, IControls *controls, long style) :
    wxHtmlListBox(owner, id, wxDefaultPosition, wxDefaultSize, style)
{
    _locPage = locPage;
    _controls = controls;
    _actCode = actCode;
    _prevActionIndex = wxNOT_FOUND;
    _isDragging = false;
    Update();
    wxString commonPart(wxString::Format(
        wxT("<META HTTP-EQUIV = \"Content-Type\" CONTENT = \"text/html; charset=%s\">")
        wxT("<FONT COLOR = #%%%%s>%%s<TABLE CELLSPACING = 4 CELLPADDING = 0><TR>%%s</TR></TABLE>%%s</FONT>"),
        wxFontMapper::GetEncodingName(_font.GetEncoding()).wx_str()
       ));
    _outFormat = wxString::Format(commonPart, wxT(""), wxT("<TD WIDTH = 100%%>%s</TD>"), wxT(""));
    _outFormatImage = wxString::Format(commonPart, wxT(""), wxT("<TD><IMG SRC=\"%s\"></TD><TD WIDTH = 100%%>%s</TD>"), wxT(""));
    _outFormatBold = wxString::Format(commonPart, wxT("<B>"), wxT("<TD WIDTH = 100%%>%s</TD>"), wxT("</B>"));
    _outFormatBoldImage = wxString::Format(commonPart, wxT("<B>"), wxT("<TD><IMG SRC=\"%s\"></TD><TD WIDTH = 100%%>%s</TD>"), wxT("</B>"));
    _outFormatItalic = wxString::Format(commonPart, wxT("<I>"), wxT("<TD WIDTH = 100%%>%s</TD>"), wxT("</I>"));
    _outFormatItalicImage = wxString::Format(commonPart, wxT("<I>"), wxT("<TD><IMG SRC=\"%s\"></TD><TD WIDTH = 100%%>%s</TD>"), wxT("</I>"));
    _outFormatBoldItalic = wxString::Format(commonPart, wxT("<B><I>"), wxT("<TD WIDTH = 100%%>%s</TD>"), wxT("</I></B>"));
    _outFormatBoldItalicImage = wxString::Format(commonPart, wxT("<B><I>"), wxT("<TD><IMG SRC=\"%s\"></TD><TD WIDTH = 100%%>%s</TD>"), wxT("</I></B>"));
    SetSelectionBackground(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));

    _controls->GetSettings()->AddObserver(this);
}

ActionsListBox::~ActionsListBox()
{
    _controls->GetSettings()->RemoveObserver(this);
}

void ActionsListBox::SetStandardFonts(int size, const wxString& normal_face, const wxString& fixed_face)
{
    CreateHTMLParser();
    m_htmlParser->SetStandardFonts(size, normal_face, fixed_face);
    RefreshAll();
}

void ActionsListBox::CreateHTMLParser() const
{
    if (!m_htmlParser)
    {
        ActionsListBox *self = wxConstCast(this, ActionsListBox);
        self->m_htmlParser = new wxHtmlWinParser(self);
        m_htmlParser->SetDC(new wxClientDC(self));
        m_htmlParser->SetFS(&self->m_filesystem);
#if !wxUSE_UNICODE
        if (GetFont().Ok()) m_htmlParser->SetInputEncoding(GetFont().GetEncoding());
#endif
        m_htmlParser->SetStandardFonts();
    }
}

void ActionsListBox::Update(bool isFromObservable)
{
    Settings *settings = _controls->GetSettings();
    _font = settings->GetFont(SYNTAX_BASE);
    wxString fontName(_font.GetFaceName());
    SetStandardFonts(_font.GetPointSize(), fontName, fontName);
    SetForegroundColour(settings->GetColour(SYNTAX_BASE));
    SetBackgroundColour(settings->GetTextBackColour());
    RefreshAll();
}

wxString ActionsListBox::OnGetItem(size_t n) const
{
    wxString color(GetHexColor(GetForegroundColour()));
    wxString text(HtmlizeWhitespaces(ProceedAsPlain(_items[n])));
    wxString imageOrig(_controls->GetContainer()->GetActionPicturePath(_locPage->GetLocationIndex(), n));
    wxString image(wxFileName(_controls->GetGamePath()).GetPathWithSep() + imageOrig);
    wxString outFormat, outFormatImage;
    if (_font.GetWeight() == wxFONTWEIGHT_BOLD && _font.GetStyle() == wxFONTSTYLE_ITALIC)
    {
        outFormat = _outFormatBoldItalic;
        outFormatImage = _outFormatBoldItalicImage;
    }
    else if (_font.GetWeight() == wxFONTWEIGHT_BOLD)
    {
        outFormat = _outFormatBold;
        outFormatImage = _outFormatBoldImage;
    }
    else if (_font.GetStyle() == wxFONTSTYLE_ITALIC)
    {
        outFormat = _outFormatItalic;
        outFormatImage = _outFormatItalicImage;
    }
    else
    {
        outFormat = _outFormat;
        outFormatImage = _outFormatImage;
    }
    if (wxFileExists(image))
        return wxString::Format(outFormatImage, color.wx_str(), image.wx_str(), text.wx_str());
    else
        return wxString::Format(outFormat, color.wx_str(), text.wx_str());
}

void ActionsListBox::OnRightClick(wxMouseEvent & event)
{
    wxMenu menu;
    long ind = VirtualHitTest(event.GetY());
    if (ind >= 0 && ind != _prevActionIndex)
    {
        SetFocus();
        Select(ind);
    }
    menu.Append(ID_ACTION_ADD, _("Create..."));
    menu.Append(ID_ACTION_REN, _("Rename..."));
    menu.Append(ID_ACTION_DEL, _("Delete"));
    menu.AppendSeparator();
    menu.Append(ID_ACTION_DELALL, _("Delete all"));
    _controls->UpdateMenuItems(&menu);
    PopupMenu(&menu);
}

void ActionsListBox::LoadActionData(size_t actIndex)
{
    Settings *settings = _controls->GetSettings();
    _actCode->LoadAction(actIndex);
    if (settings->GetCollapseCode())
        _actCode->ExpandCollapseAll(false);
}

void ActionsListBox::SaveActionData()
{
    if (_prevActionIndex >= 0)
        _actCode->SaveAction(_prevActionIndex);
}

void ActionsListBox::LoadAllActions()
{
    DataContainer *container = _controls->GetContainer();
    size_t actionsCount, locIndex = _locPage->GetLocationIndex();
    DeleteAllActions();
    actionsCount = container->GetActionsCount(locIndex);
    for (size_t i = 0; i < actionsCount; ++i)
        Append(container->GetActionName(locIndex, i));
    if (actionsCount)
    {
        Select(0);
        ScrollToRow(0);
    }
}

void ActionsListBox::OnActionChange(wxCommandEvent &event)
{
    int index = event.GetSelection();
    if (index != _prevActionIndex) Select(index);
}

size_t ActionsListBox::AddAction( const wxString& name)
{
    size_t idx = Append(name);
    if (GetCount() == 1)
    {
        Select(0);
        ScrollToRow(0);
    }
    return idx;
}

void ActionsListBox::DeleteAction( size_t actIndex)
{
    Delete(actIndex);
    _actCode->ClearAction();
    _prevActionIndex = wxNOT_FOUND;
    size_t count = GetCount();
    if (count > 0)
    {
        if (actIndex == count) actIndex = count - 1;
        Select(actIndex);
    }
}

void ActionsListBox::DeleteAllActions()
{
    Clear();
    _actCode->ClearAction();
    _prevActionIndex = wxNOT_FOUND;
}

void ActionsListBox::Select( int index)
{
    SetSelection(index);
    SaveActionData();
    LoadActionData(index);
    _prevActionIndex = index;
}

void ActionsListBox::MoveItemTo( size_t actIndex, size_t moveTo)
{
    wxString label(GetString(actIndex));
    Delete(actIndex);
    Insert(label, moveTo);
    Select(moveTo);
}

int ActionsListBox::GetSelection() const
{
    return _prevActionIndex;
}

wxString ActionsListBox::GetHexColor(const wxColour& color)
{
    return wxString::Format(wxT("%.2X%.2X%.2X"), (int)color.Red(), (int)color.Green(), (int)color.Blue());
}

wxString ActionsListBox::HtmlizeWhitespaces(const wxString& str)
{
    wxString::const_iterator i;
    wxChar ch, quote;
    wxString out;
    size_t j, linepos = 0;
    bool isLastSpace = true;
    for (i = str.begin(); i != str.end(); ++i)
    {
        switch (ch = *i)
        {
        case wxT('<'):
            quote = 0;
            while (i != str.end())
            {
                ch = *i;
                if (quote)
                {
                    if (ch == wxT('&'))
                        out << wxT("&amp;");
                    else
                    {
                        if (ch == wxT('\\'))
                        {
                            out << ch;
                            if (++i == str.end()) break;
                            ch = *i;
                        }
                        else if (ch == quote)
                            quote = 0;
                        out << ch;
                    }
                }
                else
                {
                    out << ch;
                    if (ch == wxT('>'))
                        break;
                    else if (ch == wxT('"') || ch == wxT('\''))
                        quote = ch;
                }
                ++i;
            }
            if (i == str.end()) return out;
            isLastSpace = true;
            break;
        case wxT(' '):
            if (isLastSpace)
                out << wxT("&nbsp;");
            else
                out << wxT(' ');
            isLastSpace = !isLastSpace;
            ++linepos;
            break;
        case wxT('\r'):
            break;
        case wxT('\n'):
            out << wxT("<br />");
            isLastSpace = true;
            linepos = 0;
            break;
        case wxT('\t'):
            for (j = 4 - linepos % 4; j > 0; --j)
            {
                if (isLastSpace)
                    out << wxT("&nbsp;");
                else
                    out << wxT(' ');
                isLastSpace = !isLastSpace;
            }
            linepos += 4 - linepos % 4;
            break;
        default:
            out << ch;
            isLastSpace = false;
            ++linepos;
            break;
        }
    }
    return out;
}

wxString ActionsListBox::ProceedAsPlain(const wxString& str)
{
    wxString::const_iterator i;
    wxChar ch;
    wxString out;
    for (i = str.begin(); i != str.end(); ++i)
    {
        switch (ch = *i)
        {
        case wxT('<'):
            out << wxT("&lt;");
            break;
        case wxT('>'):
            out << wxT("&gt;");
            break;
        case wxT('&'):
            out << wxT("&amp;");
            break;
        default:
            out << ch;
            break;
        }
    }
    return out;
}

size_t ActionsListBox::Append( const wxString &name)
{
    size_t newIndex = _items.Add(name);
    SetItemCount(newIndex + 1);
    RefreshRow(newIndex);
    return newIndex;
}

size_t ActionsListBox::GetCount() const
{
    return _items.GetCount();
}

void ActionsListBox::Delete( size_t index)
{
    _items.RemoveAt(index);
    SetItemCount(_items.GetCount());
    RefreshAll();
}

wxString ActionsListBox::GetString( size_t index) const
{
    return _items[index];
}

void ActionsListBox::SetString( size_t index, const wxString & name )
{
    _items[index] = name;
    RefreshAll();
}

void ActionsListBox::Insert( const wxString & name, size_t index )
{
    _items.Insert(name, index);
    SetItemCount(_items.GetCount());
    RefreshAll();
}

void ActionsListBox::Clear()
{
    _items.Clear();
    wxHtmlListBox::Clear();
    RefreshAll();
}

void ActionsListBox::RefreshActions()
{
    SaveActionData();
    RefreshAll();
}

void ActionsListBox::OnMouseMove( wxMouseEvent &event )
{
    int item = VirtualHitTest(event.GetY());
    if (!_isDragging && event.ButtonIsDown(wxMOUSE_BTN_LEFT) && item >= 0)
    {
        _draggedAction = item;
        _isDragging = true;
        SetCursor(wxCursor(wxCURSOR_HAND));
        CaptureMouse();
    }
}

void ActionsListBox::OnMouseCaptureLost( wxMouseCaptureLostEvent &event )
{
    SetCursor(wxNullCursor);
    _isDragging = false;
}

void ActionsListBox::OnMouseLeftUp( wxMouseEvent &event )
{
    if (_isDragging)
    {
        int item = VirtualHitTest(event.GetY());
        if (item >= 0 && item != _draggedAction)
            _controls->MoveActionTo(_locPage->GetLocationIndex(), _draggedAction, item);
        ReleaseMouse();
        SetCursor(wxNullCursor);
        _isDragging = false;
    }
}

void ActionsListBox::OnKeyDown( wxKeyEvent &event )
{
    if (!_controls->ExecuteHotkey(event.GetKeyCode(), event.GetModifiers()))
        event.Skip();
}
