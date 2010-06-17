// Copyright (C) 2005-2010
// Vladimir Bauer (baxzzzz AT gmail DOT com)
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

#ifndef _QGEN_DATA_CONTAINER_H_
	#define _QGEN_DATA_CONTAINER_H_

	#include <wx/wx.h>
	#include <wx/dynarray.h>

	struct ActionData
	{
		wxString pathPicture;			//Изображение для действия - относительный путь к файлу
		wxString description;			//Описание действия
		wxString onPress;				//Код события "выбор действия"
	};

	WX_DECLARE_OBJARRAY(ActionData, ActionDataArray);

	struct LocationData
	{
		wxString		name;			//Название локации
		wxString		description;	//Описание локации
		wxString		onVisit;		//Код события "посещение локации"
		ActionDataArray	actionArray;	//Массив действий
		int				folderIndex;	//Индекс секции
	};

	WX_DECLARE_OBJARRAY(LocationData, LocationDataArray);

	struct FolderData
	{
		wxString	name;				//Имя секции
		int			pos;				//Позиция секции
	};

	WX_DECLARE_OBJARRAY(FolderData, FolderDataArray);

	class DataContainer
	{
	private:
		LocationDataArray	locationArray;
		FolderDataArray		_folders;
		bool				_isSaved;
	public:
		DataContainer();

		void Save() { _isSaved = true; }
		bool IsSaved() const { return _isSaved; }

		int AddLocation(const wxString &name);
		bool RenameLocation(size_t locIndex, const wxString& newName);
		void DeleteLocation(size_t locIndex);
		void ClearLocation(size_t locIndex);
		void DeleteAction(size_t locIndex, size_t actIndex);
		void DeleteAllActions(size_t locIndex);
		int AddAction(size_t indexLoc, const wxString& actName);
		bool RenameAction(size_t locIndex, size_t actIndex, const wxString& actNewName);
		void SetActionCode( size_t indexLoc,size_t indexAct, const wxString& actCode );
		wxString GetActionCode(size_t locIndex, size_t actIndex) const;
		int FindLocationIndex(const wxString& locName) const;
		int FindActionIndex(size_t indexLoc, const wxString& actName) const;
		size_t GetActionsCount(size_t locIndex) const;
		wxString GetActionName(size_t locIndex, size_t actIndex) const;
		void SetLocationDesc(size_t indexLoc, const wxString& desc);
		void SetLocationCode(size_t indexLoc, const wxString& code);
		wxString GetLocationDesc(size_t indexLoc) const;
		wxString GetLocationCode(size_t indexLoc) const;
		void SortLocsInFolder(int folderIndex, bool isAscending);
		bool IsEmptyLoc(size_t locIndex) const;
		wxString GetLocationName(const size_t &locIndex) const;
		bool SetActionPicturePath( size_t indexLoc, size_t indexAct, const wxString &pathPict );
		wxString GetActionPicturePath( size_t locIndex, size_t actIndex ) const;
		size_t GetLocationsCount() const;
		void Clear();
		bool IsEmpty() const;
		bool GetLocActions( size_t indexLoc, wxArrayString & actions ) const;
		void MoveLocationTo(size_t locIndex, size_t moveTo);
		void MoveActionTo(size_t locIndex, size_t actIndex, size_t moveTo);

		size_t GetFoldersCount() const;
		wxString GetFolderName(size_t index) const;
		void SetLocFolder(size_t locIndex, int folderIndex);
		int GetLocFolder(size_t locIndex) const;
		int AddFolder(const wxString &name);
		bool RenameFolder(size_t folderIndex, const wxString &newName);
		void DeleteFolder(size_t folderIndex);
		int FindFolderIndex(const wxString &name) const;
		void MoveFolder(size_t folderIndex, size_t moveToSecPos);
		void SetFolderPos(size_t folderIndex, long pos);
		int FindFolderForPos(size_t pos) const;
	};

#endif
