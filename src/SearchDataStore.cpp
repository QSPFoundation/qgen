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
	wxString str;
	size_t count = _searchData.GetCount();
	fileConfig.DeleteGroup(wxT("DataSearch"));
	for (size_t i = 0; i < count; i++)
	{
		str = wxString::Format(wxT("DataSearch/Text%d_search"), i);
		fileConfig.Write(str, _searchData[i]);
	}
	count = _replaceData.GetCount();
	for (size_t i = 0; i < count; i++)
	{
		str = wxString::Format(wxT("DataSearch/Text%d_replace"), i);
		fileConfig.Write(str, _replaceData[i]);
	}
}

void SearchDataStore::LoadSearchData( wxConfigBase &fileConfig )
{
	wxString str, data;
	size_t i = 0;
	_searchData.Clear();
	_replaceData.Clear();
	while (1)
	{
		str = wxString::Format(wxT("DataSearch/Text%d_search"), i);
		if (!fileConfig.Read(str, &data)) break;
		_searchData.Add(data);
		++i;
	}
	i = 0;
	while (1)
	{
		str = wxString::Format(wxT("DataSearch/Text%d_replace"), i);
		if (!fileConfig.Read(str, &data)) break;
		_replaceData.Add(data);
		++i;
	}
}

void SearchDataStore::ClearStore()
{
	_searchData.Clear();
	_replaceData.Clear();
}