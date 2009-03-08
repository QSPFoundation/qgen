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

class LocationPage;

#ifndef _QUEST_GENERATOR_CONCRETE_COMMAND_
	#define _QUEST_GENERATOR_CONCRETE_COMMAND_

	#include "Settings.h"
	#include "DataContainer.h"

	class IControls
	{
	public:
		virtual Settings *GetSettings() const = 0;
		virtual DataContainer *GetContainer() const = 0;
		virtual wxString GetGamePath() const = 0;

		virtual void UpdateLocationsList() = 0;
		virtual void UpdateMenuItems(wxMenu *menu) = 0;
		virtual void ShowOpenedLocationsIcons() = 0;
		virtual void UpdateLocationIcon(size_t locIndex, bool isOpened) = 0;
		virtual LocationPage *ShowLocation(const wxString &locName) = 0;
		virtual void SelectLocation(size_t locIndex) = 0;
		virtual bool RenameLocation(size_t locIndex, const wxString &name) = 0;
		virtual void MoveLocationTo(size_t locIndex, size_t moveTo) = 0;

		virtual wxString SelectPicturePath() = 0;

		virtual bool AddActionOnSelectedLoc() = 0;
		virtual bool RenameAction(size_t locIndex, size_t actIndex, const wxString &name) = 0;

		virtual void ShowMessage(long errorNum) = 0;
		virtual void MoveActionTo(size_t locIndex, size_t actIndex, size_t moveTo) = 0;
	};

#endif
