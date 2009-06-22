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

#include "DataContainer.h"
#include "wx/arrimpl.cpp"

WX_DEFINE_OBJARRAY(ActionDataArray);
WX_DEFINE_OBJARRAY(LocationDataArray);
WX_DEFINE_OBJARRAY(SectionDataArray);

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

int DataContainer::FindSectionIndex( const wxString &name )
{
	wxString lwrName(name.Lower());
	int i, count = _sections.GetCount();
	for (i = 0; i < count; ++i)
		if (lwrName == _sections[i].name.Lower()) return i;
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
	loc->sectionIndex = wxNOT_FOUND;
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
	_sections.Clear();
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

void DataContainer::SetLocSection( size_t locIndex, int sectionIndex )
{
	if (locationArray[locIndex].sectionIndex == sectionIndex)
		return;
	locationArray[locIndex].sectionIndex = sectionIndex;
	_isSaved = false;
}

int DataContainer::GetLocSection( size_t locIndex )
{
	return locationArray[locIndex].sectionIndex;
}

int DataContainer::AddSection( const wxString &name )
{
	if (FindSectionIndex(name) >= 0) return wxNOT_FOUND;
	SectionData *data = new SectionData;
	data->name = name;
	data->pos = wxNOT_FOUND;
	_sections.Add(data);
	_isSaved = false;
	return _sections.GetCount() - 1;
}

bool DataContainer::RenameSection( size_t sectionIndex, const wxString &newName )
{
	int index = FindSectionIndex(newName);
	if (index >= 0 && index != sectionIndex) return false;
	_sections[sectionIndex].name = newName;
	_isSaved = false;
	return true;
}

void DataContainer::DeleteSection( size_t sectionIndex )
{
	long count = locationArray.GetCount();
	for (long i = count - 1; i >= 0; --i)
	{
		if (locationArray[i].sectionIndex == sectionIndex)
			locationArray[i].sectionIndex = -1;
		else if (locationArray[i].sectionIndex > (int)sectionIndex)
			locationArray[i].sectionIndex--;
	}
	count = _sections.GetCount();
	for (long i = count - 1; i >= 0; --i)
	{
		if (i > sectionIndex)
			_sections[i].pos--;
	}
	_sections.RemoveAt(sectionIndex);
	_isSaved = false;
}

void DataContainer::MoveSection( size_t sectionIndex, size_t moveToSecPos )
{
	if (sectionIndex == moveToSecPos) return;
	SectionData *data = _sections.Detach(sectionIndex);
	_sections.Insert(data, moveToSecPos);
	_isSaved = false;
}

void DataContainer::SetFolderPos( size_t sectionIndex, long pos )
{
	if (_sections[sectionIndex].pos == pos) return;
	_sections[sectionIndex].pos = pos;
	_isSaved = false;
}

size_t DataContainer::GetSectionsCount()
{
	return _sections.GetCount();
}

wxString DataContainer::GetSectionName( size_t index )
{
	return _sections[index].name;
}

int DataContainer::FindSectionForPos( size_t pos )
{
	size_t count = _sections.GetCount();
	for (size_t i = 0; i < count; ++i)
	{
		if (_sections[i].pos == pos)
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
		if (locationArray[i].sectionIndex == folderIndex)
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
