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

#include "SearchDataStore.h"

SearchDataStore::SearchDataStore()
{
}

void SearchDataStore::SaveSearchData( wxConfigBase &fileConfig )
{
	size_t count = _searchStrings.GetCount();
	fileConfig.DeleteGroup(wxT("DataSearch"));
	for (size_t i = 0; i < count; i++)
		fileConfig.Write(wxString::Format(wxT("DataSearch/Text%d_search"), i), _searchStrings[i]);
	count = _replaceStrings.GetCount();
	for (size_t i = 0; i < count; i++)
		fileConfig.Write(wxString::Format(wxT("DataSearch/Text%d_replace"), i), _replaceStrings[i]);
}

void SearchDataStore::LoadSearchData( wxConfigBase &fileConfig )
{
	wxString str;
	size_t i = 0;
	_searchStrings.Clear();
	_replaceStrings.Clear();
	while (1)
	{
		if (!fileConfig.Read(wxString::Format(wxT("DataSearch/Text%d_search"), i), &str)) break;
		_searchStrings.Add(str);
		++i;
	}
	i = 0;
	while (1)
	{
		if (!fileConfig.Read(wxString::Format(wxT("DataSearch/Text%d_replace"), i), &str)) break;
		_replaceStrings.Add(str);
		++i;
	}
}

void SearchDataStore::ClearStore()
{
	_searchStrings.Clear();
	_replaceStrings.Clear();
}

bool SearchDataStore::AddSearchString( const wxString &text )
{
	if (_searchStrings.Index(text) == wxNOT_FOUND)
	{
		size_t count = _searchStrings.GetCount();
		if (count >= 10)
			_searchStrings.RemoveAt(count - 1);
		_searchStrings.Insert(text, 0);
		return true;
	}
	return false;
}

bool SearchDataStore::AddReplaceString( const wxString &text )
{
	if (_replaceStrings.Index(text) == wxNOT_FOUND)
	{
		size_t count = _replaceStrings.GetCount();
		if (count >= 10)
			_replaceStrings.RemoveAt(count - 1);
		_replaceStrings.Insert(text, 0);
		return true;
	}
	return false;
}