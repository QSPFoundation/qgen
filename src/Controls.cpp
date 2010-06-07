// Copyright (C) 2005-2009
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

#include "Controls.h"

Controls::Controls(const wxString &path)
{
	_mainFrame = NULL;
	_locListBox = NULL;
	_locNotebook = NULL;
	_locale = NULL;
	_execHotkeyEnters = 0;
	_currentPath = path;

	_settings = new Settings(_currentPath);
	_container = new DataContainer();

	#ifdef __WXMSW__
		_keysParser = new KeysParser(_settings->GetHotKeys());
	#endif

	_keywordsStore = new KeywordsStore();
	 wxString filename = wxFileName(_currentPath, wxT("keywords.xml")).GetFullPath();
	_keywordsStore->Load(filename);
}

Controls::~Controls()
{
	delete _settings;
	delete _container;

	#ifdef __WXMSW__
		delete _keysParser;
	#endif

	delete _keywordsStore;
	if (_locale) delete _locale;
}

int Controls::GetSelectedLocationIndex() const
{
	LocationPage *page = _locNotebook->GetSelectedPage();
	if (page && !_locListBox->HasFocus()) return page->GetLocationIndex();
	wxString locName(_locListBox->GetStringSelection());
	if (!locName.IsEmpty()) return _container->FindLocationIndex(locName);
	return wxNOT_FOUND;
}

int Controls::AddLocationByName(const wxString &name)
{
	wxString locName(_locListBox->GetStringSelection());
	wxString folder(_locListBox->GetSelectedFolder());
	int locInd = _container->AddLocation(name);
	if (locInd >= 0)
	{
		_locListBox->Insert(name, locName, folder);
		if (_settings->GetOpenNewLoc()) ShowLocation(name);
		return locInd;
	}
	else
		ShowMessage(QGEN_MSG_EXISTS);
	return wxNOT_FOUND;
}

int Controls::AddLocation(const wxString &name)
{
	wxString locName(name);
	while (1)
	{
		wxTextEntryDialog dlgEntry(GetParent(),
			_("Input name for a new location:"),
			_("Add location"), locName);
		if (dlgEntry.ShowModal() == wxID_OK)
		{
			locName = dlgEntry.GetValue().Trim().Trim(false);
			if (locName.IsEmpty())
				ShowMessage( QGEN_MSG_EMPTYDATA );
			else if ((int)locName.Len()>QGEN_MAXLOCATIONNAMELEN)
				ShowMessage( QGEN_MSG_TOOLONGLOCATIONNAME );
			else
			{
				int index = AddLocationByName(locName);
				if (index >= 0) return index;
			}
		}
		else
			return wxNOT_FOUND;
	}
}

bool Controls::RenameSelectedLocation()
{
	int locIndex = GetSelectedLocationIndex();
	if (locIndex < 0) return false;

	wxString name(_container->GetLocationName(locIndex));
	while (1)
	{
		wxTextEntryDialog dlgEntry(GetParent(),
			_("Input new location's name:"),
			_("Rename location"), name);
		if (dlgEntry.ShowModal() == wxID_OK)
		{
			name = dlgEntry.GetValue().Trim().Trim(false);
			if (name.IsEmpty())
				ShowMessage( QGEN_MSG_EMPTYDATA );
			else if ((int)name.Len()>QGEN_MAXLOCATIONNAMELEN)
				ShowMessage( QGEN_MSG_TOOLONGLOCATIONNAME );
			else
			{
				if (RenameLocation(locIndex, name)) return true;
			}
		}
		else
			return false;
	}
}

bool Controls::DeleteSelectedLocation()
{
	int locIndex = GetSelectedLocationIndex();
	if (locIndex < 0) return false;

	wxString locName(_container->GetLocationName(locIndex));
	wxMessageDialog dlgMsg(GetParent(),
		wxString::Format(_("Remove \"%s\" location?"), locName),
		_("Remove location"), wxYES_NO|wxCENTRE|wxICON_QUESTION);
	if (dlgMsg.ShowModal() == wxID_YES)
	{
		int index = _locNotebook->FindPageIndex(locName);
		if ( index >= 0 ) _locNotebook->DeletePage(index);
		_locListBox->Delete(locName);
		_container->DeleteLocation(locIndex);
		UpdateOpenedLocationsIndexes();
		InitSearchData();
		return true;
	}
	return false;
}

bool Controls::AddActionOnSelectedLoc()
{
	LocationPage *page = _locNotebook->GetSelectedPage();
	if (!page) return false;

	wxString name;
	size_t locIndex = page->GetLocationIndex();
	if (_container->GetActionsCount(locIndex) >= QGEN_MAXACTIONS)
	{
		ShowMessage( QGEN_MSG_MAXACTIONSCOUNTREACHED );
		return false;
	}

	while (1)
	{
		wxTextEntryDialog dlgEntry(GetParent(), 
			_("Input name for a new action"),
			_("Add action"), name);

		if (dlgEntry.ShowModal() == wxID_OK)
		{
			name = dlgEntry.GetValue();
			if (name.IsEmpty())
				ShowMessage( QGEN_MSG_EMPTYDATA );
			else if ((int)name.Len()>QGEN_MAXACTIONNAMELEN)
				ShowMessage( QGEN_MSG_TOOLONGACTIONNAME );
			else
			{
				if (_container->AddAction(locIndex, name) >= 0)
				{
					size_t actIndex = page->AddAction(name);
					if (_settings->GetOpenNewAct())
					{
						page->SelectAction(actIndex);
						page->SetFocusOnActionCode();
					}
					_locListBox->UpdateLocationActions(_container->GetLocationName(locIndex));
					return true;
				}
				else
					ShowMessage( QGEN_MSG_EXISTS );
			}
		}
		else
			return false;
	}
}

bool Controls::DeleteSelectedAction()
{
	LocationPage *page = _locNotebook->GetSelectedPage();
	if (!page) return false;

	size_t locIndex = page->GetLocationIndex();
	long actIndex = page->GetSelectedAction();
	if (actIndex < 0) return false;

	_container->DeleteAction(locIndex, actIndex);
	page->DeleteAction(actIndex);
	_locListBox->UpdateLocationActions(_container->GetLocationName(locIndex));
	InitSearchData();
	return true;
}

bool Controls::DeleteAllActions()
{
	LocationPage *page = _locNotebook->GetSelectedPage();
	if (!page) return false;

	size_t locIndex = page->GetLocationIndex();
	wxString locName(_container->GetLocationName(locIndex));

	wxMessageDialog dlgMsg(GetParent(),
		wxString::Format(_("Remove all actions on \"%s\" location?"), locName),
		_("Remove all actions"), wxYES_NO|wxCENTRE|wxICON_QUESTION);
	if (dlgMsg.ShowModal() == wxID_YES)
	{
		_container->DeleteAllActions(locIndex);
		page->DeleteAllActions();
		_locListBox->UpdateLocationActions(locName);
		InitSearchData();
		return true;
	}
	return false;
}

bool Controls::RenameSelectedAction()
{
	LocationPage *page = _locNotebook->GetSelectedPage();
	if (!page) return false;

	size_t locIndex = page->GetLocationIndex();
	long actIndex = page->GetSelectedAction();
	if (actIndex < 0) return false;

	wxString name(_container->GetActionName(locIndex, actIndex));
	while (1)
	{
		wxTextEntryDialog dlgEntry(GetParent(),
			_("Input new action's name:"),
			_("Rename action"), name);
		if (dlgEntry.ShowModal() == wxID_OK)
		{
			name = dlgEntry.GetValue();
			if (name.IsEmpty())
				ShowMessage( QGEN_MSG_EMPTYDATA );
			else if ((int)name.Len()>QGEN_MAXACTIONNAMELEN)
				ShowMessage( QGEN_MSG_TOOLONGACTIONNAME );
			else
			{
				if (RenameAction(locIndex, actIndex, name)) return true;
			}
		}
		else
			return false;
	}
}

void Controls::ShowMessage( long errorNum )
{
	wxMessageDialog dlgMsg(GetParent(), GetMessageDesc(errorNum),
		_("Info"), wxOK|wxICON_INFORMATION|wxCENTRE);
	dlgMsg.ShowModal();
}

wxString Controls::GetMessageDesc( long errorNum )
{
	wxString str;
	switch (errorNum)
	{
		case QGEN_MSG_EXISTS: str = _("Such name already exists! Input another name."); break;
		case QGEN_MSG_EXISTS_HKEY: str = _("This keys combination is used already! Select another combination."); break;
		case QGEN_MSG_EXISTS_S_HKEY: str = _("This keys combination is used already by the system! Select another combination."); break;
		case QGEN_MSG_EMPTYDATA: str = _("An empty field, input the value!"); break;
		case QGEN_MSG_WRONGPASSWORD: str = _("Wrong password!"); break;
		case QGEN_MSG_CANTSAVEGAME: str = _("Can't write file!"); break;
		case QGEN_MSG_CANTLOADGAME: str = _("Can't load game. Locations with the same name are found!"); break;
		case QGEN_MSG_NOTFOUND: str = _("The specified text was not found"); break;
		case QGEN_MSG_SEARCHENDED: str = _("The specified text was not found anymore."); break;
		case QGEN_MSG_WRONGFORMAT: str = _("Incorrect format!"); break;
		case QGEN_MSG_MAXACTIONSCOUNTREACHED: str = wxString::Format(_("Can't add more than %i actions."), QGEN_MAXACTIONS); break;
		case QGEN_MSG_TOOLONGLOCATIONNAME: str = wxString::Format(_("Location's name can't contain more than %i characters!"), QGEN_MAXLOCATIONNAMELEN); break;
		case QGEN_MSG_TOOLONGACTIONNAME: str = wxString::Format(_("Action's name can't contain more than %i characters!"), QGEN_MAXACTIONNAMELEN); break;
		case QGEN_MSG_TOOLONGFOLDERNAME: str = wxString::Format(_("Folder's name can't contain more than %i characters!"), QGEN_MAXFOLDERNAMELEN); break;
		default: str = _("Unknown error!"); break;
	}
	return str;
}

LocationPage *Controls::ShowLocation(const wxString & locName)
{
	int indexPage = _locNotebook->FindPageIndex(locName);
	if (indexPage >= 0)
	{
		_locNotebook->SetSelection(indexPage);
		return (LocationPage *)_locNotebook->GetPage(indexPage);
	}
	LocationPage *page = _locNotebook->OpenLocationPage(locName, true);
	if (_settings->GetCollapseCode())
		page->ExpandCollapseAll(false);
	return page;
}

void Controls::SortLocations(bool isAscending)
{
	if (_container->GetLocationsCount() > 1)
	{
		SyncWithLocationsList();
		int selFolder = GetSelectedFolderIndex();
		_container->SortLocsInFolder(selFolder, isAscending);
		UpdateOpenedLocationsIndexes();
		if (selFolder >= 0)
			_locListBox->UpdateFolderLocations(_container->GetFolderName(selFolder));
		else
			UpdateLocationsList();
		InitSearchData();
	}
}

bool Controls::IsSelectedLocationEmpty() const
{
	int locIndex = GetSelectedLocationIndex();
	if (locIndex < 0) return true;
	_locNotebook->SaveOpenedPages();
	return _container->IsEmptyLoc(locIndex);
}

bool Controls::IsClipboardEmpty()
{
	wxTextDataObject data;
	bool canGetData = false, res = true;
	if ( !wxTheClipboard->IsOpened() && wxTheClipboard->Open() )
	{
		if (wxTheClipboard->IsSupported(wxDF_TEXT))
		{
			wxTheClipboard->GetData(data);
			canGetData = true;
		}
		wxTheClipboard->Close();
		if (canGetData) res = !IsCorrectDataFormat(data.GetText());
	}
	return res;
}

bool Controls::SerializeLocData( size_t locIndex, wxString &buffer )
{
	size_t actsCount;
	buffer.Append(QGEN_GAMEID);
	buffer.Append(QGEN_STRSDELIM);
	buffer.Append(QGEN_NAME);
	buffer.Append(wxT(" "));
	buffer.Append(QGEN_VER);
	buffer.Append(QGEN_STRSDELIM);
	buffer.Append(_container->GetLocationName(locIndex));
	buffer.Append(QGEN_STRSDELIM);
	buffer.Append(EncodeString(_container->GetLocationDesc(locIndex)));
	buffer.Append(QGEN_STRSDELIM);
	buffer.Append(EncodeString(_container->GetLocationCode(locIndex)));
	buffer.Append(QGEN_STRSDELIM);
	actsCount = _container->GetActionsCount(locIndex);
	buffer.Append(EncodeString(wxString::Format(wxT("%ld"), actsCount)));
	buffer.Append(QGEN_STRSDELIM);
	for (size_t i = 0; i < actsCount; ++i)
	{
		buffer.Append(EncodeString(_container->GetActionPicturePath(locIndex, i)));
		buffer.Append(QGEN_STRSDELIM);
		buffer.Append(EncodeString(_container->GetActionName(locIndex, i)));
		buffer.Append(QGEN_STRSDELIM);
		buffer.Append(EncodeString(_container->GetActionCode(locIndex, i)));
		buffer.Append(QGEN_STRSDELIM);
	}
	return true;
}

bool Controls::GetBufferedLocName(const wxString &buffer, wxString &locName)
{
	size_t first = 0, last = 0;
	if (!IsCorrectDataFormat(buffer))
	{
		ShowMessage(QGEN_MSG_WRONGFORMAT);
		return false;
	}
	//ID формата файла
	last = buffer.find(QGEN_STRSDELIM);
	//Данные о версии редактора
	last = buffer.find(QGEN_STRSDELIM, last + QGEN_LEN(QGEN_STRSDELIM));
	//Имя локации
	first = last + QGEN_LEN(QGEN_STRSDELIM);
	last = buffer.find(QGEN_STRSDELIM, first);
	locName = buffer.Mid(first, last - first);
	return true;
}

bool Controls::DeserializeLocData(size_t locIndex, const wxString &buffer)
{
	size_t first = 0, last = 0;
	wxString str;
	wxString actImage;
	long actsCount;

	if (!IsCorrectDataFormat(buffer))
	{
		ShowMessage(QGEN_MSG_WRONGFORMAT);
		return false;
	}
	_container->ClearLocation(locIndex);
	//ID формата файла
	last = buffer.find(QGEN_STRSDELIM);
	//Данные о версии редактора
	last = buffer.find(QGEN_STRSDELIM, last + QGEN_LEN(QGEN_STRSDELIM));
	//Имя локации
	last = buffer.find(QGEN_STRSDELIM, last + QGEN_LEN(QGEN_STRSDELIM));

	//Описание локации
	first = last + QGEN_LEN(QGEN_STRSDELIM);
	last = buffer.find(QGEN_STRSDELIM, first);
	str = buffer.Mid(first, last - first);
	_container->SetLocationDesc(locIndex, DecodeString(str));

	//Код локации
	first = last + QGEN_LEN(QGEN_STRSDELIM);
	last = buffer.find(QGEN_STRSDELIM, first);
	str = buffer.Mid(first, last - first);
	_container->SetLocationCode(locIndex, DecodeString(str));

	//Количество действий
	first = last + QGEN_LEN(QGEN_STRSDELIM);
	last = buffer.find(QGEN_STRSDELIM, first);
	str = DecodeString(buffer.Mid(first, last - first));
	str.ToLong(&actsCount);

	for (long i = 0; i < actsCount; ++i)
	{
		//Изображение
		first = last + QGEN_LEN(QGEN_STRSDELIM);
		last = buffer.find(QGEN_STRSDELIM, first);
		str = buffer.Mid(first, last - first);
		actImage = DecodeString(str);

		//Название
		first = last + QGEN_LEN(QGEN_STRSDELIM);
		last = buffer.find(QGEN_STRSDELIM, first);
		str = buffer.Mid(first, last - first);
		_container->AddAction(locIndex, DecodeString(str));
		_container->SetActionPicturePath(locIndex, i, actImage);

		//Код
		first = last + QGEN_LEN(QGEN_STRSDELIM);
		last = buffer.find(QGEN_STRSDELIM, first);
		str = buffer.Mid(first, last - first);
		_container->SetActionCode(locIndex, i, DecodeString(str));
	}
	return true;
}

void Controls::CopySelectedLocToClipboard()
{
	int locIndex = GetSelectedLocationIndex();
	if (locIndex < 0) return;
	_locNotebook->SaveOpenedPages();
	if (_container->IsEmptyLoc(locIndex)) return;

	wxString buffer;
	SerializeLocData(locIndex, buffer);
	if ( !wxTheClipboard->IsOpened() && wxTheClipboard->Open() )
	{
		wxTheClipboard->SetData( new wxTextDataObject( buffer ) );
		wxTheClipboard->Close();
	}
}

void Controls::PasteLocFromClipboard( PasteType type )
{
	int locIndex;
	bool canGetData = false;
	wxTextDataObject data;
	wxString locName, baseLocName, buffer;
	if ( IsClipboardEmpty() ) return;

	if ( !wxTheClipboard->IsOpened() && wxTheClipboard->Open() )
	{
		if (wxTheClipboard->IsSupported(wxDF_TEXT))
		{
			wxTheClipboard->GetData( data );
			canGetData = true;
		}
		wxTheClipboard->Close();
	}
	if (!canGetData) return;
	buffer = data.GetText();
	switch (type)
	{
	case PASTE_REPLACE:
		locIndex = GetSelectedLocationIndex();
		if (locIndex >= 0)
		{
			locName = _container->GetLocationName(locIndex);
			_locNotebook->SaveOpenedPages();
			if (!_container->IsEmptyLoc(locIndex))
			{
				wxMessageDialog dlgMsg(GetParent(),
					wxString::Format(_("Replace \"%s\" location?"), locName),
					_("Replace location"), wxYES_NO|wxCENTRE|wxICON_QUESTION);
				if (dlgMsg.ShowModal() == wxID_YES)
					InitSearchData();
				else
					locIndex = wxNOT_FOUND;
			}
		}
		break;
	case PASTE_NEW:
		locIndex = AddLocation();
		if (locIndex >= 0) locName = _container->GetLocationName(locIndex);
		break;
	case PASTE_NEW_AUTO:
		if (GetBufferedLocName(buffer, baseLocName))
		{
			unsigned long ind = 1;
			locName = baseLocName;
			while (_container->FindLocationIndex(locName) >= 0)
			{
				locName = wxString::Format(wxT("%s%ld"), baseLocName, ind);
				++ind;
			}
			locIndex = AddLocationByName(locName);
		}
		else
			locIndex = wxNOT_FOUND;
		break;
	}
	if ( locIndex >= 0 && DeserializeLocData(locIndex, buffer) )
	{
		_locListBox->UpdateLocationActions(locName);
		LocationPage *page = _locNotebook->GetPageByLocName(locName);
		if (page)
		{
			page->LoadPage();
			if (_settings->GetCollapseCode())
				page->ExpandCollapseAll(false);
		}
	}
}

wxString Controls::EncodeString(const wxString &str)
{
	wxString result;
	wxChar ch;
	for (size_t i = 0; i < str.Length(); i++)
	{
		ch = str[i];
		if (ch == QGEN_CODREMOV)
			ch = -QGEN_CODREMOV;
		else
			ch -= QGEN_CODREMOV;
		result.Append(ch);
	}
	return result;
}

wxString Controls::DecodeString(const wxString &str)
{
	wxString result;
	wxChar ch;
	for (size_t i = 0; i < str.Length(); i++)
	{
		ch = str[i];
		if (ch == (wxChar)-QGEN_CODREMOV)
			ch = QGEN_CODREMOV;
		else
			ch += QGEN_CODREMOV;
		result.Append(ch);
	}
	return result;
}

void Controls::ClearSelectedLocation()
{
	int locIndex = GetSelectedLocationIndex();
	if (locIndex < 0) return;
	_locNotebook->SaveOpenedPages();
	if (_container->IsEmptyLoc(locIndex)) return;

	wxString locName(_container->GetLocationName(locIndex));
	wxMessageDialog dlgMsg(GetParent(),
		wxString::Format(_("Clean \"%s\" location?"), locName),
		_("Clean location"), wxYES_NO|wxCENTRE|wxICON_QUESTION);
	if (dlgMsg.ShowModal() == wxID_YES)
	{
		_container->ClearLocation(locIndex);
		LocationPage *page = _locNotebook->GetPageByLocName(locName);
		if (page) page->Clear();
		_locListBox->UpdateLocationActions(locName);
	}
}

wxTextEntryBase *Controls::GetCurrentTextBox()
{
	wxWindow *win = wxWindow::FindFocus();
	wxTextEntryBase *ret = wxDynamicCast(win, SyntaxTextBox);
	if (ret) return ret;
	return wxDynamicCast(win, wxTextCtrl);
}

bool Controls::CanUndoText() const
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) return txt->CanUndo();
	return false;
}

bool Controls::CanRedoText() const
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) return txt->CanRedo();
	return false;
}

bool Controls::CanCopyText()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) return txt->CanCopy();
	return false;
}

bool Controls::CanPasteText()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) return txt->CanPaste();
	return false;
}

bool Controls::CanCutText()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) return txt->CanCut();
	return false;
}

bool Controls::CanSelectText()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) return !txt->IsEmpty();
	return false;
}

void Controls::UndoText()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) txt->Undo();
}

void Controls::RedoText()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) txt->Redo();
}

void Controls::CopySelectedTextToClipboard()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) txt->Copy();
}

void Controls::PasteTextFromClipboard()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) txt->Paste();
}

void Controls::CutSelectedTextToClipboard()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) txt->Cut();
}

void Controls::DeleteSelectedText()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt)
	{
		SyntaxTextBox *syntaxBox = dynamic_cast<SyntaxTextBox *>(txt);
		if (syntaxBox)
			syntaxBox->RemoveSelection();
		else
			txt->RemoveSelection();
	}
}

void Controls::SelectAllText()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) txt->SelectAll();
}

wxString Controls::SelectPicturePath()
{
	wxFileDialog filedlg( GetParent(), _( "Select image file" ),
		wxEmptyString, wxEmptyString, _( "Images (*.png;*.jpg;*.bmp;*.gif)|*.png;*.jpg;*.bmp;*.gif|All files (*.*)|*.*" ), wxFD_OPEN );
	filedlg.CentreOnParent();
	if ( filedlg.ShowModal() == wxID_OK )
	{
		wxFileName path(filedlg.GetPath());
		path.MakeRelativeTo(wxFileName(_currentGamePath).GetPath());
		return path.GetFullPath();
	}
	return wxEmptyString;
}

bool Controls::SaveGameWithCheck()
{
	if (_lastSaveTime == 0) return false;
	if (!IsGameSaved())
		return SaveGame(_currentGamePath, _currentGamePass);
	else
		_lastSaveTime = wxGetLocalTimeMillis();
	return true;
}

void Controls::SyncWithLocationsList()
{
	if (_locListBox->IsNeedForUpdate())
	{
		_locListBox->UpdateDataContainer();
		UpdateOpenedLocationsIndexes();
		InitSearchData();
	}
}

bool Controls::SaveGame(const wxString &filename, const wxString &password)
{
	SyncWithLocationsList();
	_locNotebook->SaveOpenedPages();
	if (qspSaveQuest(filename.wx_str(), password, this))
	{
		wxFileName file(filename);
		SaveConfigFile(_container, file.GetPathWithSep() + file.GetName() + wxT(".qproj"));
		_container->Save();
		_lastSaveTime = wxGetLocalTimeMillis();
		_currentGamePath = filename;
		_currentGamePass = password;
		return true;
	}
	return false;
}

bool Controls::LoadGame(const wxString &filename)
{
	_locNotebook->DeleteAllPages(CLOSE_ALL, wxNOT_FOUND);
	if (qspOpenQuest(filename.wx_str(), GetParent(), this, _currentGamePass, false))
	{
		wxFileName file(filename);
		OpenConfigFile(_container, file.GetPathWithSep() + file.GetName() + wxT(".qproj"));
		InitSearchData();
		_currentGamePath = filename;
		UpdateLocationsList();
		_container->Save();
		_lastSaveTime = wxGetLocalTimeMillis();
		return true;
	}
	return false;
}

bool Controls::JoinGame( const wxString &filename )
{
	wxString dummy;
	if (qspOpenQuest(filename.wx_str(), GetParent(), this, dummy, true))
	{
		InitSearchData();
		UpdateLocationsList();
		_locNotebook->LoadOpenedPages();
		return true;
	}
	return false;
}

void Controls::UpdateLocationsList()
{
	size_t locsCount = _container->GetLocationsCount();
	_locListBox->Freeze();
	_locListBox->Clear();
	wxString folderName;
	wxArrayInt locs;
	long oldPos = -1, pos = 0, folderIndex;
	while (pos != oldPos)
	{
		oldPos = pos;
		folderIndex = _container->FindFolderForPos(pos);
		if (folderIndex >= 0)
		{
			_locListBox->AddFolder(_container->GetFolderName(folderIndex));
			++pos;
		}
		if (locs.GetCount() < locsCount)
		{
			for (size_t i = 0; i < locsCount; ++i)
			{
				if (locs.Index(i) < 0 && _container->GetLocFolder(i) == folderIndex)
				{
					if (folderIndex >= 0)
						folderName = _container->GetFolderName(folderIndex);
					else
					{
						if (_container->FindFolderForPos(pos) >= 0)
							break;
						folderName = wxEmptyString;
					}
					_locListBox->Insert(_container->GetLocationName(i), wxEmptyString, folderName);
					locs.Add(i);
					++pos;
				}
			}
		}
	}
	UpdateActionsOnAllLocs();
	ShowOpenedLocationsIcons();
	_locListBox->Thaw();
}

bool Controls::IsActionsOnSelectedLocEmpty() const
{
	LocationPage *page = _locNotebook->GetSelectedPage();
	return (!page || page->IsActionsEmpty());
}

bool Controls::IsAllLocsClosed() const
{
	return (_locNotebook->GetPageCount() == 0);
}

bool Controls::IsCorrectDataFormat(const wxString &str)
{
	size_t count = 0;
	wxArrayString strArray;
	int last, first = 0;

	last = str.find(QGEN_STRSDELIM);
	if (last == wxNOT_FOUND) return false;	
	do
	{
		++count;
		strArray.Add(str.Mid(first, last - first));
		first = last + QGEN_LEN(QGEN_STRSDELIM);
		last = str.find(QGEN_STRSDELIM, first);
	} while(last != wxNOT_FOUND);

	if (count <= 5) return false;
	//ID формата файла
	if (!strArray[0].IsSameAs(QGEN_GAMEID)) return false;
	//Количество действий
	int actsCount = wxAtoi(strArray[5]);
	if (actsCount < 0)
		actsCount = -actsCount;
	//Общее кличество строк
	if (count < 5 + (size_t)actsCount * 3) return false;
	return true;
} 


wxString Controls::ConvertSearchString(const wxString& s, bool isMatchCase)
{
	return (isMatchCase ? s : s.Lower());
}

int Controls::FindSubString(const wxString& s, const wxString& sub, bool isWholeString, int ind)
{
	if (isWholeString)
	{
		int length = s.length(), subLen = sub.length(), ind2;
		--ind;
		do
		{
			ind = s.find(sub, ind + 1);
			if (ind < 0) break;
			ind2 = ind + subLen;
		} while (!(
			(ind == 0 || QGEN_STRCHR(QGEN_DELIMS, s[ind - 1])) &&
			(ind2 >= length || QGEN_STRCHR(QGEN_DELIMS, s[ind2]))
			));
		return ind;
	}
	else
		return s.find(sub, ind);
}

bool Controls::SearchNextLoc()
{
	int countLocs = _container->GetLocationsCount();
	_dataSearch.findAt = SEARCH_LOCNAME;
	_dataSearch.startPos = wxNOT_FOUND;
	if (++_dataSearch.idxLoc >= countLocs) _dataSearch.idxLoc = 0;
	if (++_dataSearch.countChecking >= countLocs)
	{
		ShowMessage(_dataSearch.isFoundAny ? QGEN_MSG_SEARCHENDED : QGEN_MSG_NOTFOUND);
		return false;
	}
	return true;
}

void Controls::JumpToSelectedLoc()
{
	wxString word = GetSelectedWord();
	if (!word.IsEmpty())
	{
		if (_container->FindLocationIndex(word) >= 0)
			ShowLocation(word);
		else
			AddLocation(word);
	}
}

wxString Controls::GetSelectedWord() const
{
	wxString str, data;
	long beginPos, lastPos, curPos;
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt)
	{
		str = txt->GetStringSelection();
		data = txt->GetValue();
		curPos = txt->GetInsertionPoint();
		if (str.IsEmpty() && !data.IsEmpty())
		{
			if (curPos == data.Length()) --curPos;
			beginPos = curPos;
			lastPos = curPos;
			while (beginPos >= 0)
				if (QGEN_STRCHR(QGEN_DELIMS, data[beginPos]))
					break;
				else
					--beginPos;
			while ((size_t)lastPos < data.Length())
				if (QGEN_STRCHR(QGEN_DELIMS, data[lastPos]))
					break;
				else
					++lastPos;
			if (lastPos > beginPos) str = data.Mid(beginPos + 1, lastPos - beginPos - 1);
		}
	}
	return str;
}

bool Controls::SearchString( const wxString &str, bool findAgain, bool isMatchCase, bool isWholeString )
{
	wxString data;
	wxString locName;
	wxString actName;
	LocationPage *page = NULL;
	int startPos = wxNOT_FOUND;
	int lastPos;
	int locIndex, countLocs = _container->GetLocationsCount();
	if (!countLocs) return false;
	SyncWithLocationsList();

	wxString lwrStr = ConvertSearchString(str, isMatchCase);

	_locNotebook->SaveOpenedPages();

	if (findAgain || _dataSearch.idxLoc >= countLocs || _dataSearch.countChecking >= countLocs)
	{
		InitSearchData();
		locIndex = GetSelectedLocationIndex();
		if (locIndex < 0) locIndex = 0;
		_dataSearch.idxLoc = locIndex;
	}

	_dataSearch.foundAt = SEARCH_NONE;
	_dataSearch.stringLen = str.Length();

	while (_dataSearch.idxLoc < countLocs)
	{
		locName = _container->GetLocationName(_dataSearch.idxLoc);
		if (_dataSearch.findAt == SEARCH_LOCNAME)
		{
			_dataSearch.findAt = SEARCH_LOCDESC;
			_dataSearch.startPos = wxNOT_FOUND;
			if (FindSubString(ConvertSearchString(locName, isMatchCase), lwrStr, isWholeString) != wxNOT_FOUND)
			{
				_locListBox->Select(locName);
				ShowLocation(locName);
				_locListBox->SetFocus();
				_dataSearch.foundAt = SEARCH_LOCNAME;
				_dataSearch.isFoundAny = true;
				return true;
			}
		}
		if (_dataSearch.findAt == SEARCH_LOCDESC)
		{
			data = _container->GetLocationDesc(_dataSearch.idxLoc);
			startPos = FindSubString(ConvertSearchString(data, isMatchCase), lwrStr, isWholeString, _dataSearch.startPos + 1);
			if (startPos != wxNOT_FOUND)
			{
				lastPos = lwrStr.Length();
				lastPos += startPos;
				_locListBox->Select(locName);
				page = ShowLocation(locName);
				page->SelectLocDescString(startPos, lastPos);
				_dataSearch.startPos = startPos;
				_dataSearch.foundAt = SEARCH_LOCDESC;
				_dataSearch.isFoundAny = true;
				return true;
			}
			else
			{
				_dataSearch.findAt = SEARCH_LOCCODE;
				_dataSearch.startPos = wxNOT_FOUND;
			}
		}

		if (_dataSearch.findAt == SEARCH_LOCCODE)
		{
			data = _container->GetLocationCode(_dataSearch.idxLoc);
			startPos = FindSubString(ConvertSearchString(data, isMatchCase), lwrStr, isWholeString, _dataSearch.startPos + 1);
			if (startPos != wxNOT_FOUND)
			{
				lastPos = lwrStr.Length();
				lastPos += startPos;
				_locListBox->Select(locName);
				page = ShowLocation(locName);
				page->SelectLocCodeString(startPos, lastPos);
				_dataSearch.startPos = startPos;
				_dataSearch.foundAt = SEARCH_LOCCODE;
				_dataSearch.isFoundAny = true;
				return true;
			}
			else
			{
				_dataSearch.findAt = SEARCH_ACTNAME;
				_dataSearch.startPos = wxNOT_FOUND;
				_dataSearch.idxAct = 0;
			}
		}

		int countActs = _container->GetActionsCount(_dataSearch.idxLoc);

		for(; _dataSearch.idxAct < countActs; ++_dataSearch.idxAct)
		{
			if (_dataSearch.findAt == SEARCH_ACTNAME)
			{
				_dataSearch.findAt = SEARCH_PATHPICT;
				_dataSearch.startPos = wxNOT_FOUND;
				actName = _container->GetActionName(_dataSearch.idxLoc, _dataSearch.idxAct);
				if (FindSubString(ConvertSearchString(actName, isMatchCase), lwrStr, isWholeString) != wxNOT_FOUND)
				{
					_locListBox->Select(locName);
					page = ShowLocation(locName);
					page->SelectAction(_dataSearch.idxAct);
					_dataSearch.foundAt = SEARCH_ACTNAME;
					_dataSearch.isFoundAny = true;
					return true;
				}
			}
			if (_dataSearch.findAt == SEARCH_PATHPICT)
			{
				data = _container->GetActionPicturePath(_dataSearch.idxLoc, _dataSearch.idxAct);
				startPos = FindSubString(ConvertSearchString(data, isMatchCase), lwrStr, isWholeString, _dataSearch.startPos + 1);
				if (startPos != wxNOT_FOUND)
				{
					lastPos = lwrStr.Length();
					lastPos += startPos;
					_locListBox->Select(locName);
					page = ShowLocation(locName);
					page->SelectAction(_dataSearch.idxAct);
					page->SelectPicturePathString(startPos, lastPos);
					_dataSearch.startPos = startPos;
					_dataSearch.foundAt = SEARCH_PATHPICT;
					_dataSearch.isFoundAny = true;
					return true;
				}
				else
				{
					_dataSearch.findAt = SEARCH_ACTCODE;
					_dataSearch.startPos = wxNOT_FOUND;
				}
			}
			if (_dataSearch.findAt == SEARCH_ACTCODE)
			{
				data = _container->GetActionCode(_dataSearch.idxLoc, _dataSearch.idxAct);
				startPos = FindSubString(ConvertSearchString(data, isMatchCase), lwrStr, isWholeString, _dataSearch.startPos + 1);
				if (startPos != wxNOT_FOUND)
				{
					lastPos = lwrStr.Length();
					lastPos += startPos;
					_locListBox->Select(locName);
					page = ShowLocation(locName);
					page->SelectAction( _dataSearch.idxAct );
					page->SelectActionCodeString(startPos, lastPos);
					_dataSearch.startPos = startPos;
					_dataSearch.foundAt = SEARCH_ACTCODE;
					_dataSearch.isFoundAny = true;
					return true;
				}
				else
				{
					_dataSearch.findAt = SEARCH_ACTNAME;
					_dataSearch.startPos = wxNOT_FOUND;
				}
			}
		}
		if (!SearchNextLoc()) return false;
	}
	ShowMessage(QGEN_MSG_NOTFOUND);
	return false;
}

void Controls::ReplaceSearchString(const wxString & replaceString)
{
	wxString temp;
	if (_dataSearch.foundAt == SEARCH_NONE) return;
	LocationPage *page = _locNotebook->GetPageByLocName(_container->GetLocationName(_dataSearch.idxLoc));
	switch (_dataSearch.foundAt)
	{
	case SEARCH_LOCDESC:
		temp = _container->GetLocationDesc(_dataSearch.idxLoc);
		temp.replace(_dataSearch.startPos, _dataSearch.stringLen, replaceString);
		_container->SetLocationDesc(_dataSearch.idxLoc, temp);
		if (page)
			page->ReplaceLocDescString(_dataSearch.startPos, _dataSearch.startPos + _dataSearch.stringLen, replaceString);
		break;
	case SEARCH_LOCCODE:
		temp = _container->GetLocationCode(_dataSearch.idxLoc);
		temp.replace(_dataSearch.startPos, _dataSearch.stringLen, replaceString);
		_container->SetLocationCode(_dataSearch.idxLoc, temp);
		if (page)
			page->ReplaceLocCodeString(_dataSearch.startPos, _dataSearch.startPos + _dataSearch.stringLen, replaceString);
		break;
	case SEARCH_PATHPICT:
		temp = _container->GetActionPicturePath(_dataSearch.idxLoc, _dataSearch.idxAct);
		temp.replace(_dataSearch.startPos, _dataSearch.stringLen, replaceString);
		_container->SetActionPicturePath(_dataSearch.idxLoc, _dataSearch.idxAct, temp);
		if (page)
			page->ReplacePicturePathString(_dataSearch.startPos, _dataSearch.startPos + _dataSearch.stringLen, replaceString);
		break;
	case SEARCH_ACTCODE:
		temp = _container->GetActionCode(_dataSearch.idxLoc, _dataSearch.idxAct);
		temp.replace(_dataSearch.startPos, _dataSearch.stringLen, replaceString);
		_container->SetActionCode(_dataSearch.idxLoc, _dataSearch.idxAct, temp);
		if (page)
			page->ReplaceActionCodeString(_dataSearch.startPos, _dataSearch.startPos + _dataSearch.stringLen, replaceString);
		break;
	}
	_dataSearch.startPos += replaceString.Length() - 1;
	_dataSearch.foundAt = SEARCH_NONE;
}

void Controls::InitSearchData()
{
	_dataSearch.foundAt = SEARCH_NONE;
	_dataSearch.findAt = SEARCH_LOCNAME;

	_dataSearch.idxLoc = 0;
	_dataSearch.idxAct = 0;
	_dataSearch.startPos = wxNOT_FOUND;
	_dataSearch.stringLen = 0;

	_dataSearch.countChecking = 0;
	_dataSearch.isFoundAny = false;
}

void Controls::NewGame()
{
	if ( !_container->IsEmpty() )
	{
		_locNotebook->DeleteAllPages(CLOSE_ALL, wxNOT_FOUND);
		_locListBox->Clear();
		_container->Clear();
		_container->Save();
	}
	_currentGamePath = wxFileName(_currentPath, wxT("NoName.qsp")).GetFullPath();
	_currentGamePass = QGEN_PASSWD;
	InitSearchData();
	wxString locName = _settings->GetFirstLocName().Trim().Trim(false);
	if (_settings->GetCreateFirstLoc() && !locName.IsEmpty())
	{
		_container->AddLocation(locName);
		_locListBox->Insert(locName, wxEmptyString, wxEmptyString);
		_container->Save();
	}
	_lastSaveTime = 0;
}

bool Controls::IsGameSaved()
{
//	SyncWithLocationsList();
	_locNotebook->SaveOpenedPages();
	return _container->IsSaved();
}

void Controls::UpdateActionsOnAllLocs()
{
	size_t count = _container->GetLocationsCount();
	for (size_t i = 0; i < count; ++i)
		_locListBox->UpdateLocationActions(_container->GetLocationName(i));
}

void Controls::UpdateOpenedLocationsIndexes()
{
	LocationPage *page;
	size_t count = _locNotebook->GetPageCount();
	for (size_t index = 0; index < count; ++index)
	{
		page = ( LocationPage * )_locNotebook->GetPage(index);
		page->SetLocationIndex(_container->FindLocationIndex(_locNotebook->GetPageText(index)));
	}
}

void Controls::UpdateLocationIcon( size_t locIndex, bool isOpened )
{
	_locListBox->SetLocStatus(_container->GetLocationName(locIndex), isOpened);
}

void Controls::ShowOpenedLocationsIcons()
{
	size_t index, count = _locNotebook->GetPageCount();
	for (index = 0; index < count; ++index)
		_locListBox->SetLocStatus(_locNotebook->GetPageText(index), true);
}

void Controls::UpdateMenuItems(wxMenu *menu)
{
	if (!menu) return;

	bool saveFound = menu->FindItem(ID_GAME_SAVE) != NULL;
	bool saveAsFound = menu->FindItem(ID_GAME_SAVEAS) != NULL;
	bool playQuestFound = menu->FindItem(ID_GAME_PLAY) != NULL;
	bool exportTxtFound = menu->FindItem(ID_GAME_EXPORTTXT) != NULL;
	bool exportTxt2GamFound = menu->FindItem(ID_GAME_EXPORTTXT2GAM) != NULL;
	bool folderDelFound = menu->FindItem(ID_FOLDER_DEL) != NULL;
	bool folderRenameFound = menu->FindItem(ID_FOLDER_RENAME) != NULL;
	bool locDelFound = menu->FindItem(ID_LOC_DEL) != NULL;
	bool locRenameFound = menu->FindItem(ID_LOC_RENAME) != NULL;
	bool locClearFound = menu->FindItem(ID_LOC_CLEAR) != NULL;
	bool locCopyFound = menu->FindItem(ID_LOC_COPY) != NULL;
	bool locReplaceFound = menu->FindItem(ID_LOC_REPLACE) != NULL;
	bool locPasteFound = menu->FindItem(ID_LOC_PASTE) != NULL;
	bool locPasteNewFound = menu->FindItem(ID_LOC_PASTENEW) != NULL;
	bool locSortAscFound = menu->FindItem(ID_LOC_SORTASC) != NULL;
	bool locSortDescFound = menu->FindItem(ID_LOC_SORTDESC) != NULL;
	bool locJumpLocFound = menu->FindItem(ID_LOC_JUMPLOC) != NULL;
	bool locExpandFound = menu->FindItem(ID_LOC_EXPAND) != NULL;
	bool locCollapseFound = menu->FindItem(ID_LOC_COLLAPSE) != NULL;
	bool createActFound = menu->FindItem(ID_ACTION_ADD) != NULL;
	bool delActFound = menu->FindItem(ID_ACTION_DEL) != NULL;
	bool delAllActFound = menu->FindItem(ID_ACTION_DELALL) != NULL;
	bool renameActFound = menu->FindItem(ID_ACTION_REN) != NULL;
	bool undoTextFound = menu->FindItem(ID_TEXT_UNDO) != NULL;
	bool redoTextFound = menu->FindItem(ID_TEXT_REDO) != NULL;
	bool copyTextFound = menu->FindItem(ID_TEXT_COPY) != NULL;
	bool pasteTextFound = menu->FindItem(ID_TEXT_PASTE) != NULL;
	bool cutTextFound = menu->FindItem(ID_TEXT_CUT) != NULL;
	bool delTextFound = menu->FindItem(ID_TEXT_DEL) != NULL;
	bool selAllTextFound = menu->FindItem(ID_TEXT_SELALL) != NULL;
	bool closeAllTabsFound = menu->FindItem(ID_TAB_CLOSEALL) != NULL;
	bool closeExecptSelFound = menu->FindItem(ID_TAB_CLOSEEXCEPTSELECTED) != NULL;
	bool closeSelFound = menu->FindItem(ID_TAB_CLOSESELECTED) != NULL;
	bool fixTabFound = menu->FindItem(ID_TAB_FIX) != NULL;
	bool showHideLocDesc = menu->FindItem(ID_LOC_DESCVISIBLE) != NULL;
	bool showHideLocActs = menu->FindItem(ID_LOC_ACTVISIBLE) != NULL;
	bool res = false;

	if (saveFound || saveAsFound || playQuestFound || exportTxtFound || exportTxt2GamFound ||
		locExpandFound || locCollapseFound)
		res = !_container->IsEmpty();
	if (saveFound) menu->Enable(ID_GAME_SAVE, res);
	if (saveAsFound) menu->Enable(ID_GAME_SAVEAS, res);
	if (playQuestFound) menu->Enable(ID_GAME_PLAY, res);
	if (exportTxtFound) menu->Enable(ID_GAME_EXPORTTXT, res);
	if (exportTxt2GamFound) menu->Enable(ID_GAME_EXPORTTXT2GAM, res);
	if (locExpandFound) menu->Enable(ID_LOC_EXPAND, res);
	if (locCollapseFound) menu->Enable(ID_LOC_COLLAPSE, res);
	res = false;

	if (locDelFound || locRenameFound)
		res = GetSelectedLocationIndex() >= 0;
	if (locDelFound) menu->Enable(ID_LOC_DEL, res);
	if (locRenameFound) menu->Enable(ID_LOC_RENAME, res);
	res = false;

	if (folderDelFound || folderRenameFound)
		res = GetSelectedFolderIndex() >= 0;
	if (folderDelFound) menu->Enable(ID_FOLDER_DEL, res);
	if (folderRenameFound) menu->Enable(ID_FOLDER_RENAME, res);
	res = false;

	if (delActFound || delAllActFound || renameActFound)
		res = !IsActionsOnSelectedLocEmpty();
	if (delActFound) menu->Enable(ID_ACTION_DEL, res);
	if (delAllActFound) menu->Enable(ID_ACTION_DELALL, res);
	if (renameActFound) menu->Enable(ID_ACTION_REN, res);
	res = false;

	if (locClearFound || locCopyFound)
		res = !IsSelectedLocationEmpty();
	if (locClearFound) menu->Enable(ID_LOC_CLEAR, res);
	if (locCopyFound) menu->Enable(ID_LOC_COPY, res);
	res = false;

	if (locReplaceFound || locPasteFound || locPasteNewFound)
		res = !IsClipboardEmpty();
	if (locReplaceFound) menu->Enable(ID_LOC_REPLACE, res && GetSelectedLocationIndex() >= 0);
	if (locPasteFound) menu->Enable(ID_LOC_PASTE, res);
	if (locPasteNewFound) menu->Enable(ID_LOC_PASTENEW, res);
	res = false;

	if (locSortAscFound || locSortDescFound)
		res = _locListBox->GetCount() > 1;
	if (locSortAscFound) menu->Enable(ID_LOC_SORTASC, res);
	if (locSortDescFound) menu->Enable(ID_LOC_SORTDESC, res);
	res = false;

	if (locJumpLocFound) menu->Enable(ID_LOC_JUMPLOC, GetCurrentTextBox() != NULL);
	if (createActFound)	menu->Enable(ID_ACTION_ADD, !IsAllLocsClosed());
	if (undoTextFound) menu->Enable(ID_TEXT_UNDO, CanUndoText());
	if (redoTextFound) menu->Enable(ID_TEXT_REDO, CanRedoText());
	if (pasteTextFound) menu->Enable(ID_TEXT_PASTE, CanPasteText());
	if (selAllTextFound) menu->Enable(ID_TEXT_SELALL, CanSelectText());

	if (copyTextFound || cutTextFound || delTextFound)
		res = CanCopyText();
	if (copyTextFound) menu->Enable(ID_TEXT_COPY, res);
	if (cutTextFound) menu->Enable(ID_TEXT_CUT, res);
	if (delTextFound) menu->Enable(ID_TEXT_DEL, res);
	res = false;

	if (closeAllTabsFound || closeExecptSelFound || closeSelFound || showHideLocDesc || showHideLocActs)
		res = !IsAllLocsClosed();
	if (closeAllTabsFound) menu->Enable(ID_TAB_CLOSEALL, res);
	if (closeExecptSelFound) menu->Enable(ID_TAB_CLOSEEXCEPTSELECTED, res);
	if (closeSelFound) menu->Enable(ID_TAB_CLOSESELECTED, res);
	if (fixTabFound) menu->Enable(ID_TAB_FIX, res);
	if (showHideLocDesc) menu->Enable(ID_LOC_DESCVISIBLE, res);
	if (showHideLocActs) menu->Enable(ID_LOC_ACTVISIBLE, res);
}

bool Controls::RenameFolder( size_t folderIndex, const wxString &name )
{
	wxString oldName(_container->GetFolderName(folderIndex));
	if (_container->RenameFolder(folderIndex, name))
	{
		_locListBox->SetFolderName(oldName, name);
		return true;
	}
	else
		ShowMessage( QGEN_MSG_EXISTS );
	return false;
}

bool Controls::RenameLocation( size_t locIndex, const wxString &name )
{
	wxString oldName(_container->GetLocationName(locIndex));
	if (_container->RenameLocation(locIndex, name))
	{
		_locListBox->SetLocName(oldName, name);
		int pageIndex = _locNotebook->FindPageIndex(oldName);
		if (pageIndex >= 0) _locNotebook->SetPageText(pageIndex, name);
		return true;
	}
	else
		ShowMessage( QGEN_MSG_EXISTS );
	return false;
}

bool Controls::RenameAction( size_t locIndex, size_t actIndex, const wxString &name )
{
	if (_container->RenameAction(locIndex, actIndex, name))
	{
		LocationPage *page = _locNotebook->GetPageByLocName(_container->GetLocationName(locIndex));
		if (page) page->RenameAction(actIndex, name);
		_locListBox->UpdateLocationActions(_container->GetLocationName(locIndex));
		return true;
	}
	else
		ShowMessage( QGEN_MSG_EXISTS );
	return false;
}

void Controls::MoveActionTo( size_t locIndex, size_t actIndex, size_t moveTo )
{
	_container->MoveActionTo(locIndex, actIndex, moveTo);
	wxString locName(_container->GetLocationName(locIndex));
	LocationPage *page = _locNotebook->GetPageByLocName(locName);
	if (page) page->MoveActionTo(actIndex, moveTo);
	_locListBox->UpdateLocationActions(locName);
	InitSearchData();
}

wxString Controls::GetGameInfo() const
{
	int totalLocsCount,
		totalEmptyDesc = 0,
		totalEmptyCode = 0,
		totalEmptyActsCode = 0,
		totalLocsSize = 0,
		totalActs = 0,
		maxLocSize = 0,
		avgActions = 0,
		avgSize = 0,
		locSize,
		actsCount;
	wxString locName, actName, data;

	totalLocsCount = _container->GetLocationsCount();
	if (totalLocsCount > 0)
	{
		for (int i = 0; i < totalLocsCount; ++i)
		{
			locName = _container->GetLocationName(i);
			locSize = locName.Length();
			data = _container->GetLocationDesc(i);
			locSize += data.Length();
			if (data.Trim().Trim(false).IsEmpty()) ++totalEmptyDesc;
			data = _container->GetLocationCode(i);
			locSize += data.Length();
			if (data.Trim().Trim(false).IsEmpty()) ++totalEmptyCode;
			actsCount = _container->GetActionsCount(i);
			if (actsCount)
			{
				totalActs += actsCount;
				for (int j = 0; j < actsCount; ++j)
				{
					actName = _container->GetActionName(i, j);
					locSize += actName.Length();
					locSize += _container->GetActionPicturePath(i, j).Length();
					data = _container->GetActionCode(i, j);
					locSize += data.Length();
					if (data.Trim().Trim(false).IsEmpty()) ++totalEmptyActsCode;
				}
			}
			if (locSize > maxLocSize) maxLocSize = locSize;
			totalLocsSize += locSize;
		}
		avgActions = (int)((float)totalActs / totalLocsCount + 0.5);
		avgSize = (int)((float)totalLocsSize / totalLocsCount + 0.5);
	}
	wxString message = wxString::Format(_("This game contains %i location(s)\n"), totalLocsCount);
	message += wxString::Format(_("Locations without base description: %i\n"), totalEmptyDesc);
	message += wxString::Format(_("Locations without \"on visit\" code: %i\n"), totalEmptyCode);
	message += wxString::Format(_("Average count of actions per location: %i\n"), avgActions);
	message += wxString::Format(_("Actions without code: %i\n"), totalEmptyActsCode);
	message += wxString::Format(_("Max location size: %i characters\n"), maxLocSize);
	message += wxString::Format(_("Average location size: %i characters\n"), avgSize);
	message += wxString::Format(_("Total game size: %i characters"), totalLocsSize);
	return message;
}

bool Controls::ExportTxt(const wxString &filename)
{
	_locNotebook->SaveOpenedPages();
	return qspExportTxt(filename.wx_str(), this);
}

bool Controls::ExportTxt2Gam(const wxString &filename)
{
	_locNotebook->SaveOpenedPages();
	return qspExportTxt2Game(filename.wx_str(), this);
}

bool Controls::ImportTxt2Gam(const wxString &filename)
{
	if (qspImportTxt2Game(filename.wx_str(), this))
		return LoadGame(_currentGamePath);
	return false;
}

void Controls::SwitchLocDesc()
{
	LocationPage *page = _locNotebook->GetSelectedPage();
	if (page) page->LocDescVisible(!page->IsDescShown());
}

void Controls::SwitchLocActs()
{
	LocationPage *page = _locNotebook->GetSelectedPage();
	if (page) page->LocActsVisible(!page->IsActsShown());
}

bool Controls::ExecuteHotkey( int keyCode, int modifiers )
{
	bool res = false;
	#ifdef __WXMSW__
	++_execHotkeyEnters;
	res = _keysParser->ExecuteHotkeyAction(keyCode, modifiers);
	--_execHotkeyEnters;
	#endif
	return res;
}

void Controls::SetStatusText( const wxString &text )
{
	wxStatusBar *statusBar = _mainFrame->GetStatusBar();
	if (statusBar) statusBar->SetStatusText(text);
}

void Controls::SelectLocation( size_t locIndex )
{
	_locListBox->Select(_container->GetLocationName(locIndex));
}

bool Controls::AddFolder()
{
	wxString name;
	while (1)
	{
		wxTextEntryDialog dlgEntry(GetParent(),
			_("Input name for a new folder:"),
			_("Add folder"), name);
		if (dlgEntry.ShowModal() == wxID_OK)
		{
			name = dlgEntry.GetValue().Trim().Trim(false);
			if (name.IsEmpty())
				ShowMessage( QGEN_MSG_EMPTYDATA );
			else if ((int)name.Len() > QGEN_MAXFOLDERNAMELEN)
				ShowMessage( QGEN_MSG_TOOLONGFOLDERNAME );
			else
			{
				if (_container->AddFolder(name) >= 0)
				{
					_locListBox->AddFolder(name);
					break;
				}
				else
					ShowMessage(QGEN_MSG_EXISTS);
			}
		}
		else
			return false;
	}
	return true;
}

bool Controls::DeleteSelectedFolder()
{
	int folder = GetSelectedFolderIndex();
	if (folder < 0) return false;

	wxString folderName(_container->GetFolderName(folder));
	wxMessageDialog dlgMsg(GetParent(),
		wxString::Format(_("Remove \"%s\" folder?"), folderName),
		_("Remove folder"), wxYES_NO|wxCENTRE|wxICON_QUESTION);
	if (dlgMsg.ShowModal() == wxID_YES)
	{
		SyncWithLocationsList();
		_container->DeleteFolder(_container->FindFolderIndex(folderName));
		UpdateLocationsList();
		return true;
	}
	return false;
}

bool Controls::RenameSelectedFolder()
{
	int folder = GetSelectedFolderIndex();
	if (folder < 0) return false;

	wxString name(_container->GetFolderName(folder));
	while (1)
	{
		wxTextEntryDialog dlgEntry(GetParent(),
			_("Input new folder's name:"),
			_("Rename folder"), name);
		if (dlgEntry.ShowModal() == wxID_OK)
		{
			name = dlgEntry.GetValue().Trim().Trim(false);
			if (name.IsEmpty())
				ShowMessage( QGEN_MSG_EMPTYDATA );
			else if ((int)name.Len() > QGEN_MAXFOLDERNAMELEN)
				ShowMessage( QGEN_MSG_TOOLONGFOLDERNAME );
			else
			{
				if (RenameFolder(folder, name)) break;
			}
		}
		else
			return false;
	}
	return true;
}

int Controls::GetSelectedFolderIndex() const
{
	int locIndex = GetSelectedLocationIndex();
	if (locIndex >= 0)
		return _container->GetLocFolder(locIndex);
	else
		return _container->FindFolderIndex(_locListBox->GetSelectedFolder());
}

bool Controls::SearchHelpFile()
{
	if (!wxFile::Exists(_settings->GetCurrentHelpPath()))
	{
		wxFileDialog dialog(GetParent(),
			_("Select help file"), wxEmptyString, wxEmptyString,
			_("Help file (*.chm)|*.chm|All files (*.*)|*.*"), wxFD_OPEN);
		dialog.CenterOnParent();
		if (dialog.ShowModal() == wxID_CANCEL) return false;
		_settings->SetCurrentHelpPath(dialog.GetPath());
	}
	return true;
}

wxWindow *Controls::GetParent()
{
	return wxGetTopLevelParent(wxGetActiveWindow());
}

void Controls::UpdateLocale(int lang)
{
	if (_locale) delete _locale;
	_locale = new wxLocale;
	_locale->Init(lang);
	_locale->AddCatalogLookupPathPrefix(_currentPath + wxT("langs"));
	
	if (!_locale->AddCatalog(QGEN_APPNAME))
		_locale->AddCatalog(wxString(QGEN_APPNAME) + wxT('_') + _locale->GetCanonicalName().BeforeFirst(wxT('_')));
}
