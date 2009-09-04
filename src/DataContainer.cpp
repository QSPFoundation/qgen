// Copyright (C) 2005-2009
// BaxZzZz (bauer_v AT mail DOT ru)
// Nex (nex AT otaku DOT ru)
// Shchannikov Dmitry (rrock DOT ru AT gmail DOT com)
// Valeriy Argunov (nporep AT mail DOT ru)
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

#include "DataContainer.h"
#include "wx/arrimpl.cpp"

WX_DEFINE_OBJARRAY(ActionDataArray);
WX_DEFINE_OBJARRAY(LocationDataArray);
WX_DEFINE_OBJARRAY(FolderDataArray);

int CmpLocationsAsc(const wxString &first, const wxString &second)
{
	return first.CmpNoCase(second);
}

int CmpLocationsDesc(const wxString &first, const wxString &second)
{
	return second.CmpNoCase(first);
}

DataContainer::DataContainer()
{
	_isSaved = true;
}

int DataContainer::FindFolderIndex( const wxString &name )
{
	wxString lwrName(name.Lower());
	int i, count = _folders.GetCount();
	for (i = 0; i < count; ++i)
		if (lwrName == _folders[i].name.Lower()) return i;
	return wxNOT_FOUND;
}

int DataContainer::FindLocationIndex(const wxString & nameLocation)
{
	wxString lwrName(nameLocation.Lower());
	int i, count = locationArray.GetCount();
	for (i = 0; i < count; ++i)
		if (lwrName == locationArray[i].name.Lower()) return i;
	return wxNOT_FOUND;
}

int DataContainer::FindActionIndex(size_t indexLoc, const wxString& actName)
{
	wxString lwrName(actName.Lower());
	LocationData &loc = locationArray[indexLoc];
	int i, count = loc.actionArray.GetCount();
	for (i = 0; i < count; ++i)
		if (lwrName == loc.actionArray[i].description.Lower()) return i;
	return wxNOT_FOUND;
}

int DataContainer::AddLocation(const wxString &name)
{
	if (FindLocationIndex(name) >= 0) return wxNOT_FOUND;
	LocationData *loc = new LocationData;
	loc->name = name;
	loc->folderIndex = wxNOT_FOUND;
	locationArray.Add(loc);
	_isSaved = false;
	return locationArray.GetCount() - 1;
}

bool DataContainer::RenameLocation(size_t locIndex, const wxString& newName)
{
	int index = FindLocationIndex(newName);
	if (index >= 0 && index != locIndex) return false;
	locationArray[locIndex].name = newName;
	_isSaved = false;
	return true;
}

void DataContainer::DeleteLocation( size_t locIndex )
{
	locationArray.RemoveAt(locIndex);
	_isSaved = false;
}

void DataContainer::DeleteAction( size_t locIndex, size_t actIndex )
{
	locationArray[locIndex].actionArray.RemoveAt(actIndex);
	_isSaved = false;
}

int DataContainer::AddAction(size_t indexLoc, const wxString& actName)
{
	if (FindActionIndex(indexLoc, actName) >= 0) return wxNOT_FOUND;
	ActionData *act = new ActionData;
	act->description = actName;
	locationArray[indexLoc].actionArray.Add(act);
	_isSaved = false;
	return locationArray[indexLoc].actionArray.GetCount() - 1;
}

bool DataContainer::RenameAction(size_t locIndex, size_t actIndex, const wxString& actNewName)
{
	int index = FindActionIndex(locIndex, actNewName);
	if (index >= 0 && index != actIndex) return false;
	locationArray[locIndex].actionArray[actIndex].description = actNewName;
	_isSaved = false;
	return true;
}

void DataContainer::SetActionCode( size_t indexLoc,size_t indexAct, const wxString& actCode )
{
	locationArray[indexLoc].actionArray[indexAct].onPress = actCode;
	_isSaved = false;
}

wxString DataContainer::GetActionCode(size_t locIndex, size_t actIndex)
{
	return locationArray[locIndex].actionArray[actIndex].onPress;
}

size_t DataContainer::GetActionsCount( size_t locIndex )
{
	return locationArray[locIndex].actionArray.GetCount();
}

wxString DataContainer::GetActionName(size_t locIndex, size_t actIndex)
{
	return locationArray[locIndex].actionArray[actIndex].description;
}

void DataContainer::SetLocationDesc(size_t indexLoc, const wxString& desc)
{
	locationArray[indexLoc].description = desc;
	_isSaved = false;
}

void DataContainer::SetLocationCode(size_t indexLoc, const wxString& code)
{
	locationArray[indexLoc].onVisit = code;
	_isSaved = false;
}

wxString DataContainer::GetLocationDesc(size_t indexLoc)
{
	return locationArray[indexLoc].description;
}

wxString DataContainer::GetLocationCode(size_t indexLoc)
{
	return locationArray[indexLoc].onVisit;
}

void DataContainer::ClearLocation(size_t locIndex)
{
	locationArray[locIndex].description.Clear();
	locationArray[locIndex].onVisit.Clear();
	locationArray[locIndex].actionArray.Clear();
	_isSaved = false;
}

void DataContainer::DeleteAllActions( size_t locIndex )
{
	locationArray[locIndex].actionArray.Clear();
	_isSaved = false;
}

bool DataContainer::IsEmptyLoc(size_t locIndex)
{
	if (locationArray[locIndex].onVisit.IsEmpty() &&
		locationArray[locIndex].description.IsEmpty() &&
		locationArray[locIndex].actionArray.IsEmpty()) return true;
	return false;
}

wxString DataContainer::GetLocationName(const size_t &locIndex)
{
	return locationArray[locIndex].name;
}

wxString DataContainer::GetActionPicturePath( size_t locIndex, size_t actIndex )
{
	return locationArray[locIndex].actionArray[actIndex].pathPicture;
}

bool DataContainer::SetActionPicturePath( size_t indexLoc, size_t indexAct, const wxString &pathPict )
{
	locationArray[indexLoc].actionArray[indexAct].pathPicture = pathPict;
	_isSaved = false;
	return true;
}

size_t DataContainer::GetLocationsCount()
{
	return locationArray.GetCount();
}

void DataContainer::Clear()
{
	locationArray.Clear();
	_folders.Clear();
	_isSaved = false;
}

bool DataContainer::IsEmpty()
{
	return locationArray.IsEmpty();
}

bool DataContainer::GetLocActions( size_t indexLoc, wxArrayString & actions )
{
	if (indexLoc < 0) return false;
	actions.Clear();
	for (size_t i = 0; i < locationArray[indexLoc].actionArray.Count(); ++i)
		actions.Add(locationArray[indexLoc].actionArray[i].description);
	return true;
}

void DataContainer::MoveLocationTo( size_t locIndex, size_t moveTo )
{
	if (locIndex == moveTo) return;
	LocationData *data = locationArray.Detach(locIndex);
	locationArray.Insert(data, moveTo);
	_isSaved = false;
}

void DataContainer::MoveActionTo( size_t locIndex, size_t actIndex, size_t moveTo )
{
	ActionData *data = locationArray[locIndex].actionArray.Detach(actIndex);
	locationArray[locIndex].actionArray.Insert(data, moveTo);
	_isSaved = false;
}

void DataContainer::SetLocFolder( size_t locIndex, int folderIndex )
{
	if (locationArray[locIndex].folderIndex == folderIndex)
		return;
	locationArray[locIndex].folderIndex = folderIndex;
	_isSaved = false;
}

int DataContainer::GetLocFolder( size_t locIndex )
{
	return locationArray[locIndex].folderIndex;
}

int DataContainer::AddFolder( const wxString &name )
{
	if (FindFolderIndex(name) >= 0) return wxNOT_FOUND;
	FolderData *data = new FolderData;
	data->name = name;
	data->pos = wxNOT_FOUND;
	_folders.Add(data);
	_isSaved = false;
	return _folders.GetCount() - 1;
}

bool DataContainer::RenameFolder( size_t folderIndex, const wxString &newName )
{
	int index = FindFolderIndex(newName);
	if (index >= 0 && index != folderIndex) return false;
	_folders[folderIndex].name = newName;
	_isSaved = false;
	return true;
}

void DataContainer::DeleteFolder( size_t folderIndex )
{
	long count = locationArray.GetCount();
	for (long i = count - 1; i >= 0; --i)
	{
		if (locationArray[i].folderIndex == folderIndex)
			locationArray[i].folderIndex = -1;
		else if (locationArray[i].folderIndex > (int)folderIndex)
			locationArray[i].folderIndex--;
	}
	count = _folders.GetCount();
	for (long i = count - 1; i >= 0; --i)
	{
		if (i > folderIndex)
			_folders[i].pos--;
	}
	_folders.RemoveAt(folderIndex);
	_isSaved = false;
}

void DataContainer::MoveFolder( size_t folderIndex, size_t moveToSecPos )
{
	if (folderIndex == moveToSecPos) return;
	FolderData *data = _folders.Detach(folderIndex);
	_folders.Insert(data, moveToSecPos);
	_isSaved = false;
}

void DataContainer::SetFolderPos( size_t folderIndex, long pos )
{
	if (_folders[folderIndex].pos == pos) return;
	_folders[folderIndex].pos = pos;
	_isSaved = false;
}

size_t DataContainer::GetFoldersCount()
{
	return _folders.GetCount();
}

wxString DataContainer::GetFolderName( size_t index )
{
	return _folders[index].name;
}

int DataContainer::FindFolderForPos( size_t pos )
{
	size_t count = _folders.GetCount();
	for (size_t i = 0; i < count; ++i)
	{
		if (_folders[i].pos == pos)
			return i;
	}
	return wxNOT_FOUND;
}

void DataContainer::SortLocsInFolder( int folderIndex, bool isAscending )
{
	wxArrayString names;
	wxArrayInt positions;
	LocationData *data = locationArray.Detach(0);
	size_t count = locationArray.GetCount();
	long startIndex = -1;
	for (size_t i = 0; i < count; ++i)
	{
		if (locationArray[i].folderIndex == folderIndex)
		{
			if (startIndex < 0) startIndex = i;
			positions.Add(startIndex);
			names.Add(locationArray[i].name);
		}
		else
			startIndex = -1;
	}
	if (isAscending)
		names.Sort(CmpLocationsAsc);
	else
		names.Sort(CmpLocationsDesc);
	size_t namesCount = names.GetCount();
	for (size_t i = 0; i < namesCount; ++i)
	{
		long index = FindLocationIndex(names[i]);
		long moveTo = i + positions[i];
		if (moveTo >= count)
			moveTo = count - 1;
		MoveLocationTo(index, moveTo);
	}
	locationArray.Insert(data, 0);
}
