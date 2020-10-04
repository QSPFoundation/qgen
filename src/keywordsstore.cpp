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

#include "keywordsstore.h"
#include <wx/arrimpl.cpp>

WX_DEFINE_OBJARRAY(KeywordsArray);

int CmpWordsAsc( Keyword **first, Keyword **second )
{
    return (*first)->word.CmpNoCase((*second)->word);
}

bool KeywordsStore::Load( const wxString &filename )
{
    wxXmlDocument doc;
    wxXmlNode *node;
    if (!(wxFileExists(filename) && doc.Load(filename))) return false;
    node = doc.GetRoot();
    if (node == NULL || node->GetName() != wxT("QGen-keywords")) return false;
    node = node->GetChildren();
    if (node == NULL) return false;
    while (node)
    {
        if (node->GetName() == wxT("Statements"))
        {
            ParseKeywords(node, STATEMENT);
        }
        else if (node->GetName() == wxT("Expressions"))
        {
            ParseKeywords(node, EXPRESSION);
        }
        else if (node->GetName() == wxT("Variables"))
        {
            ParseKeywords(node, VARIABLE);
        }
        node = node->GetNext();
    }
    _keywords.Sort(CmpWordsAsc);
    return true;
}

void KeywordsStore::ParseKeywords( wxXmlNode *node, KeywordType type )
{
    Keyword *keyword;

    node = node->GetChildren();
    while (node)
    {
        if (node->GetName() == wxT("Keyword"))
        {
            keyword = new Keyword();
            keyword->word = node->GetAttribute(wxT("name")).Lower();
            keyword->desc = node->GetAttribute(wxT("desc"));
            keyword->type = type;
            _keywords.Add(keyword);
        }
        node = node->GetNext();
    }
}

wxString KeywordsStore::GetWords( KeywordType type ) const
{
    wxString result;

    size_t count = _keywords.GetCount();
    for (size_t idx = 0; idx < count; ++idx)
    {
        if (_keywords[idx].type == type)
        {
            if (result.Length() == 0)
                result = _keywords[idx].word;
            else
                result += wxT(' ') + _keywords[idx].word;
        }
    }
    return result;
}

size_t KeywordsStore::GetCount()
{
    return _keywords.GetCount();
}

wxString KeywordsStore::GetWord( size_t index ) const
{
    if (index >= _keywords.GetCount()) return wxEmptyString;
    return _keywords[index].word;
}

wxString KeywordsStore::FindTip( const wxString &word ) const
{
    wxString result;
    int idx, first = 0;
    size_t count = _keywords.GetCount();
    if (count == 0) return result;
    int last = count - 1;
    wxString w = word.Lower();
    wxString str;
    do
    {
        idx = first + (last - first) / 2;
        str = _keywords[idx].word;
        if(w.Cmp(str) > 0)
            first = idx + 1;
        else if(w.Cmp(str) < 0)
            last = idx - 1;
        else
        {
            result = _keywords[idx].desc;
            break;
        }
    } while (first <= last);
    return result;
}
