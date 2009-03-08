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

#ifndef _QUEST_GENERATOR_DATA_CONTAINER_
	#define _QUEST_GENERATOR_DATA_CONTAINER_

	#include <wx/wx.h>
	#include <wx/dynarray.h>

	struct ActionData
	{
		wxString pathPicture;			//����������� ��� �������� - ������������� ���� � �����
		wxString description;			//�������� ��������
		wxString onPress;				//��� ������� "����� ��������"		
	};

	WX_DECLARE_OBJARRAY(ActionData, ActionDataArray);

	struct LocationData
	{
		wxString name;					//�������� �������
		wxString description;			//�������� �������
		wxString onVisit;				//��� ������� "��������� �������"
		ActionDataArray actionArray;	//������ ��������
	};

	WX_DECLARE_OBJARRAY(LocationData, LocationDataArray);

	class DataContainer
	{
	private:
		LocationDataArray locationArray;
		bool _isSaved;

	public:
		DataContainer();

		void Save() { _isSaved = true; }
		bool IsSaved() { return _isSaved; }

		int InsertLocation(const wxString &name, size_t pos);
		bool RenameLocation(size_t locIndex, const wxString& newName);
		void DeleteLocation(size_t locIndex);
		void ClearLocation(size_t locIndex);
		void DeleteAction(size_t locIndex, size_t actIndex);
		void DeleteAllActions(size_t locIndex);
		int AddAction(size_t indexLoc, const wxString& actName);
		bool RenameAction(size_t locIndex, size_t actIndex, const wxString& actNewName);
		void SetActionCode( size_t indexLoc,size_t indexAct, const wxString& actCode );
		wxString GetActionCode(size_t locIndex, size_t actIndex);
		int FindLocationIndex(const wxString& locName);
		int FindActionIndex(size_t indexLoc, const wxString& actName);
		size_t GetActionsCount(size_t locIndex);
		wxString GetActionName(size_t locIndex, size_t actIndex);
		void SetLocationDesc(size_t indexLoc, const wxString& desc);
		void SetLocationCode(size_t indexLoc, const wxString& code);
		wxString GetLocationDesc(size_t indexLoc);
		wxString GetLocationCode(size_t indexLoc);
		void SortLocations(bool isAscending);
		bool IsEmptyLoc(size_t locIndex);
		wxString GetLocationName(const size_t &locIndex);
		bool SetActionPicturePath( size_t indexLoc, size_t indexAct, const wxString &pathPict );
		wxString GetActionPicturePath( size_t locIndex, size_t actIndex );
		size_t GetLocationsCount();
		void Clear();
		bool IsEmpty();
		bool GetLocActions( size_t indexLoc, wxArrayString & actions );
		void MoveLocationTo(size_t locIndex, size_t moveTo);
		void MoveActionTo(size_t locIndex, size_t actIndex, size_t moveTo);
	};

#endif
