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

#ifndef _QGEN_GAME_H_
	#define _QGEN_GAME_H_

	#include <wx/wx.h>
	#include <wx/xml/xml.h>
	#include "Idents.h"
	#include "DataContainer.h"

	class Controls;

	bool qspOpenQuest(const QGEN_CHAR *fileName, wxWindow *_mainFrame, Controls *controls, wxString &password, bool merge);
	bool qspSaveQuest(const QGEN_CHAR *fileName, const wxString &passwd, Controls *controls);
	bool qspExportTxt(const QGEN_CHAR *fileName, Controls *controls);
	bool qspExportTxt2Game(const QGEN_CHAR *fileName, Controls *controls);
	bool qspImportTxt2Game(const QGEN_CHAR *fileName, Controls *controls);

	bool OpenConfigFile(DataContainer *container, const wxString &file);
	bool SaveConfigFile(DataContainer *container, const wxString &file);

#endif
