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

#include "datacontainer.h"
#include <wx/arrimpl.cpp>

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

int DataContainer::FindFolderIndex(const wxString &name) const
{
    wxString lwrName(name.Lower());
    int i, count = _folders.GetCount();
    for (i = 0; i < count; ++i)
        if (lwrName == _folders[i].Name.Lower()) return i;
    return wxNOT_FOUND;
}

int DataContainer::FindLocationIndex(const wxString& nameLocation) const
{
    wxString lwrName(nameLocation.Lower());
    int i, count = _locationArray.GetCount();
    for (i = 0; i < count; ++i)
        if (lwrName == _locationArray[i].Name.Lower()) return i;
    return wxNOT_FOUND;
}

int DataContainer::FindActionIndex(size_t indexLoc, const wxString& actName) const
{
    wxString lwrName(actName.Lower());
    LocationData &loc = _locationArray[indexLoc];
    int i, count = loc.Actions.GetCount();
    for (i = 0; i < count; ++i)
        if (lwrName == loc.Actions[i].Description.Lower()) return i;
    return wxNOT_FOUND;
}

int DataContainer::AddLocation(const wxString &name)
{
    if (FindLocationIndex(name) >= 0) return wxNOT_FOUND;
    LocationData *loc = new LocationData;
    loc->Name = name;
    loc->FolderIndex = wxNOT_FOUND;
    _locationArray.Add(loc);
    _isSaved = false;
    return _locationArray.GetCount() - 1;
}

bool DataContainer::RenameLocation(size_t locIndex, const wxString& newName)
{
    int index = FindLocationIndex(newName);
    if (index >= 0 && index != locIndex) return false;
    _locationArray[locIndex].Name = newName;
    _isSaved = false;
    return true;
}

void DataContainer::DeleteLocation(size_t locIndex)
{
    _locationArray.RemoveAt(locIndex);
    _isSaved = false;
}

void DataContainer::DeleteAction(size_t locIndex, size_t actIndex)
{
    _locationArray[locIndex].Actions.RemoveAt(actIndex);
    _isSaved = false;
}

int DataContainer::AddAction(size_t indexLoc, const wxString& actName)
{
    if (FindActionIndex(indexLoc, actName) >= 0) return wxNOT_FOUND;
    ActionData *act = new ActionData;
    act->Description = actName;
    _locationArray[indexLoc].Actions.Add(act);
    _isSaved = false;
    return _locationArray[indexLoc].Actions.GetCount() - 1;
}

bool DataContainer::RenameAction(size_t locIndex, size_t actIndex, const wxString& actNewName)
{
    int index = FindActionIndex(locIndex, actNewName);
    if (index >= 0 && index != actIndex) return false;
    _locationArray[locIndex].Actions[actIndex].Description = actNewName;
    _isSaved = false;
    return true;
}

void DataContainer::SetActionCode(size_t indexLoc,size_t indexAct, const wxString& actCode)
{
    _locationArray[indexLoc].Actions[indexAct].Code = actCode;
    _isSaved = false;
}

wxString DataContainer::GetActionCode(size_t locIndex, size_t actIndex) const
{
    return _locationArray[locIndex].Actions[actIndex].Code;
}

size_t DataContainer::GetActionsCount(size_t locIndex) const
{
    return _locationArray[locIndex].Actions.GetCount();
}

wxString DataContainer::GetActionName(size_t locIndex, size_t actIndex) const
{
    return _locationArray[locIndex].Actions[actIndex].Description;
}

void DataContainer::SetLocationDesc(size_t indexLoc, const wxString& desc)
{
    _locationArray[indexLoc].Description = desc;
    _isSaved = false;
}

void DataContainer::SetLocationCode(size_t indexLoc, const wxString& code)
{
    _locationArray[indexLoc].Code = code;
    _isSaved = false;
}

wxString DataContainer::GetLocationDesc(size_t indexLoc) const
{
    return _locationArray[indexLoc].Description;
}

wxString DataContainer::GetLocationCode(size_t indexLoc) const
{
    return _locationArray[indexLoc].Code;
}

void DataContainer::ClearLocation(size_t locIndex)
{
    _locationArray[locIndex].Description.Clear();
    _locationArray[locIndex].Code.Clear();
    _locationArray[locIndex].Actions.Clear();
    _isSaved = false;
}

void DataContainer::DeleteAllActions(size_t locIndex)
{
    _locationArray[locIndex].Actions.Clear();
    _isSaved = false;
}

bool DataContainer::IsEmptyLoc(size_t locIndex) const
{
    if (_locationArray[locIndex].Code.IsEmpty() &&
        _locationArray[locIndex].Description.IsEmpty() &&
        _locationArray[locIndex].Actions.IsEmpty()) return true;
    return false;
}

wxString DataContainer::GetLocationName(const size_t &locIndex) const
{
    return _locationArray[locIndex].Name;
}

wxString DataContainer::GetActionPicturePath(size_t locIndex, size_t actIndex) const
{
    return _locationArray[locIndex].Actions[actIndex].ImagePath;
}

bool DataContainer::SetActionPicturePath(size_t indexLoc, size_t indexAct, const wxString &pathPict)
{
    _locationArray[indexLoc].Actions[indexAct].ImagePath = pathPict;
    _isSaved = false;
    return true;
}

size_t DataContainer::GetLocationsCount() const
{
    return _locationArray.GetCount();
}

void DataContainer::Clear()
{
    _locationArray.Clear();
    _folders.Clear();
    _isSaved = false;
}

bool DataContainer::IsEmpty() const
{
    return _locationArray.IsEmpty();
}

bool DataContainer::GetLocActions(size_t indexLoc, wxArrayString& actions) const
{
    actions.Clear();
    for (size_t i = 0; i < _locationArray[indexLoc].Actions.Count(); ++i)
        actions.Add(_locationArray[indexLoc].Actions[i].Description);
    return true;
}

void DataContainer::MoveLocationTo(size_t locIndex, size_t moveTo)
{
    if (locIndex == moveTo) return;
    LocationData *data = _locationArray.Detach(locIndex);
    _locationArray.Insert(data, moveTo);
    _isSaved = false;
}

void DataContainer::MoveActionTo(size_t locIndex, size_t actIndex, size_t moveTo)
{
    ActionData *data = _locationArray[locIndex].Actions.Detach(actIndex);
    _locationArray[locIndex].Actions.Insert(data, moveTo);
    _isSaved = false;
}

void DataContainer::SetLocFolder(size_t locIndex, int folderIndex)
{
    if (_locationArray[locIndex].FolderIndex == folderIndex)
        return;
    _locationArray[locIndex].FolderIndex = folderIndex;
    _isSaved = false;
}

int DataContainer::GetLocFolder(size_t locIndex) const
{
    return _locationArray[locIndex].FolderIndex;
}

int DataContainer::AddFolder(const wxString &name)
{
    if (FindFolderIndex(name) >= 0) return wxNOT_FOUND;
    FolderData *data = new FolderData;
    data->Name = name;
    data->Position = wxNOT_FOUND;
    _folders.Add(data);
    _isSaved = false;
    return _folders.GetCount() - 1;
}

bool DataContainer::RenameFolder(size_t folderIndex, const wxString &newName)
{
    int index = FindFolderIndex(newName);
    if (index >= 0 && index != folderIndex) return false;
    _folders[folderIndex].Name = newName;
    _isSaved = false;
    return true;
}

void DataContainer::DeleteFolder(size_t folderIndex)
{
    long count = _locationArray.GetCount();
    for (long i = count - 1; i >= 0; --i)
    {
        if (_locationArray[i].FolderIndex == folderIndex)
            _locationArray[i].FolderIndex = -1;
        else if (_locationArray[i].FolderIndex > (int)folderIndex)
            _locationArray[i].FolderIndex--;
    }
    count = _folders.GetCount();
    for (long i = count - 1; i >= 0; --i)
    {
        if ((size_t) i > folderIndex)
            _folders[i].Position--;
    }
    _folders.RemoveAt(folderIndex);
    _isSaved = false;
}

void DataContainer::MoveFolder(size_t folderIndex, size_t moveToSecPos)
{
    if (folderIndex == moveToSecPos) return;
    FolderData *data = _folders.Detach(folderIndex);
    _folders.Insert(data, moveToSecPos);
    _isSaved = false;
}

void DataContainer::SetFolderPos(size_t folderIndex, long pos)
{
    if (_folders[folderIndex].Position == pos) return;
    _folders[folderIndex].Position = pos;
    _isSaved = false;
}

size_t DataContainer::GetFoldersCount() const
{
    return _folders.GetCount();
}

wxString DataContainer::GetFolderName(size_t index) const
{
    return _folders[index].Name;
}

int DataContainer::FindFolderForPos(size_t pos) const
{
    size_t count = _folders.GetCount();
    for (size_t i = 0; i < count; ++i)
    {
        if (_folders[i].Position == pos)
            return i;
    }
    return wxNOT_FOUND;
}

void DataContainer::SortLocsInFolder(int folderIndex, bool isAscending)
{
    wxArrayString names;
    wxArrayInt positions;
    LocationData *data = _locationArray.Detach(0);
    size_t count = _locationArray.GetCount();
    long startIndex = -1;
    for (size_t i = 0; i < count; ++i)
    {
        if (_locationArray[i].FolderIndex == folderIndex)
        {
            if (startIndex < 0) startIndex = i;
            positions.Add(startIndex);
            names.Add(_locationArray[i].Name);
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
    _locationArray.Insert(data, 0);
}
