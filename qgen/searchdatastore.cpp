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

#include "searchdatastore.h"

SearchDataStore::SearchDataStore()
{
}

void SearchDataStore::SaveSearchData(wxConfigBase &fileConfig)
{
    int count = (int)_searchStrings.GetCount();
    fileConfig.DeleteGroup(wxT("SearchData"));
    for (int i = 0; i < count; i++)
        fileConfig.Write(wxString::Format(wxT("SearchData/Search%d_Text"), i), _searchStrings[i]);
    count = _replaceStrings.GetCount();
    for (int i = 0; i < count; i++)
        fileConfig.Write(wxString::Format(wxT("SearchData/Replace%d_Text"), i), _replaceStrings[i]);
}

void SearchDataStore::LoadSearchData(wxConfigBase &fileConfig)
{
    int i;
    wxString str;
    _searchStrings.Clear();
    _replaceStrings.Clear();
    i = 0;
    while (true)
    {
        if (!fileConfig.Read(wxString::Format(wxT("SearchData/Search%d_Text"), i), &str)) break;
        _searchStrings.Add(str);
        ++i;
    }
    i = 0;
    while (true)
    {
        if (!fileConfig.Read(wxString::Format(wxT("SearchData/Replace%d_Text"), i), &str)) break;
        _replaceStrings.Add(str);
        ++i;
    }
}

void SearchDataStore::ClearStore()
{
    _searchStrings.Clear();
    _replaceStrings.Clear();
}

void SearchDataStore::AddSearchString(const wxString &text)
{
    if (_searchStrings.Index(text) == wxNOT_FOUND)
    {
        size_t count = _searchStrings.GetCount();
        if (count >= 15) _searchStrings.RemoveAt(count - 1);
    }
    else
        _searchStrings.Remove(text);
    _searchStrings.Insert(text, 0);
}

void SearchDataStore::AddReplaceString(const wxString &text)
{
    if (_replaceStrings.Index(text) == wxNOT_FOUND)
    {
        size_t count = _replaceStrings.GetCount();
        if (count >= 15) _replaceStrings.RemoveAt(count - 1);
    }
    else
        _replaceStrings.Remove(text);
    _replaceStrings.Insert(text, 0);
}
