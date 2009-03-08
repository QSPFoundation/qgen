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

int CmpLocationsAsc(LocationData **loc1, LocationData **loc2)
{
	return (*loc1)->name.Cmp((*loc2)->name);
}

int CmpLocationsDesc(LocationData **loc1, LocationData **loc2)
{
	return (*loc2)->name.Cmp((*loc1)->name);
}

DataContainer::DataContainer()
{
	_isSaved = true;
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

int DataContainer::InsertLocation(const wxString &name, size_t pos)
{
	if (FindLocationIndex(name) >= 0) return wxNOT_FOUND;
	LocationData *loc = new LocationData;
	loc->name = name;
	locationArray.Insert(loc, pos);
	_isSaved = false;
	return (int)pos;
}

bool DataContainer::RenameLocation(size_t locIndex, const wxString& newName)
{
	int index = FindLocationIndex(newName);
	if (locIndex < 0 || (index >= 0 && index != locIndex)) return false;
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
	if (actIndex < 0 || (index >= 0 && index != actIndex)) return false;
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

void DataContainer::SortLocations(bool isAscending)
{
	if (locationArray.GetCount() > 2)
	{
		LocationData *data = locationArray.Detach(0);
		if (isAscending)
			locationArray.Sort(CmpLocationsAsc);
		else
			locationArray.Sort(CmpLocationsDesc);
		locationArray.Insert(data, 0);
		_isSaved = false;
	}
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
