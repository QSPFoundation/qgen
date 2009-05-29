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

#include "Controls.h"

Controls::Controls(const wxString &path)
{
	_currentPath = path;

	_settings = new Settings(_currentPath);
	_container = new DataContainer();
}

Controls::~Controls()
{
	delete _settings;
	delete _container;
}

void Controls::SetMainFrame(wxWindow *mainFrame)
{
	_mainFrame = mainFrame;
}

void Controls::SetLocListBox(LocationsListBox *locListBox)
{
	_locListBox = locListBox;
}

void Controls::SetNotebook(LocationsNotebook *locNotebook)
{
	_locNotebook = locNotebook;
}

int Controls::GetSelectedLocationIndex()
{
	LocationPage *page = _locNotebook->GetSelectedPage();
	if (page && !_locListBox->HasFocus()) return page->GetLocationIndex();
	wxString locName(_locListBox->GetStringSelection());
	if (!locName.IsEmpty()) return _container->FindLocationIndex(locName);
	return wxNOT_FOUND;
}

void Controls::SelectLocation(size_t locIndex)
{
	_locListBox->Select(locIndex);
}

int Controls::AddLocationByName(const wxString &name)
{
	int locInd = _locListBox->GetStringIndex(_locListBox->GetStringSelection());
	locInd = (locInd >= 0 ? locInd + 1 : _container->GetLocationsCount());
	if (_container->InsertLocation(name, locInd) >= 0)
	{
		UpdateOpenedLocationsIndexes();
		_locListBox->Insert(name, locInd);
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
		wxTextEntryDialog dlgEntry(_mainFrame, wxT("Введите название новой локации:"),
			wxT("Добавить новую локацию"), locName, wxOK | wxCANCEL);
		dlgEntry.CenterOnParent();
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
		wxTextEntryDialog dlgEntry(_mainFrame, wxT("Введите новое название локации:"),
			wxT("Переименовать локацию"), name, wxOK | wxCANCEL);
		dlgEntry.CenterOnParent();
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
	wxMessageDialog dlgMsg(_mainFrame,
		wxString::Format(wxT("Желаете удалить локацию %s?"), locName),
		wxT("Удалить локацию"), wxYES_NO|wxICON_QUESTION);
	dlgMsg.CenterOnParent();
	if (dlgMsg.ShowModal() == wxID_YES)
	{
		int index = _locNotebook->FindPageIndex(locName);
		if ( index >= 0 ) _locNotebook->DeletePage(index);
		_locListBox->Delete(locIndex);
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
		wxTextEntryDialog dlgEntry(_mainFrame, wxT("Введите название действия:"),
			wxT("Создать действие"), name, wxOK | wxCANCEL);
		dlgEntry.CenterOnParent();
		if (dlgEntry.ShowModal() == wxID_OK)
		{
			name = dlgEntry.GetValue();
			if (name.IsEmpty())
				ShowMessage( QGEN_MSG_EMPTYDATA );
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
					UpdateLocActions(locIndex);
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

	_container->DeleteAction(locIndex, actIndex);
	page->DeleteAction(actIndex);
	UpdateLocActions(locIndex);
	InitSearchData();
	return true;
}

bool Controls::DeleteAllActions()
{
	LocationPage *page = _locNotebook->GetSelectedPage();
	if (!page) return false;

	size_t locIndex = page->GetLocationIndex();

	wxMessageDialog dlgMsg(_mainFrame,
		wxString::Format(wxT("Желаете удалить все действия на локации %s?"), _container->GetLocationName(locIndex)),
		wxT("Удалить все действия"), wxYES_NO|wxICON_QUESTION);
	dlgMsg.CenterOnParent();
	if (dlgMsg.ShowModal() == wxID_YES)
	{
		_container->DeleteAllActions(locIndex);
		page->DeleteAllActions();
		UpdateLocActions(locIndex);
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
	wxString name(_container->GetActionName(locIndex, actIndex));

	while (1)
	{
		wxTextEntryDialog dlgEntry(_mainFrame, wxT("Введите новое название действия:"),
			wxT("Переименовать действие"), name, wxOK | wxCANCEL);
		dlgEntry.CenterOnParent();
		if (dlgEntry.ShowModal() == wxID_OK)
		{
			name = dlgEntry.GetValue();
			if (name.IsEmpty())
				ShowMessage( QGEN_MSG_EMPTYDATA );
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
	wxMessageDialog dlgMsg (_mainFrame, GetMessageDesc(errorNum), wxT("Инфо"), wxOK | wxICON_INFORMATION);
	dlgMsg.CenterOnParent();
	dlgMsg.ShowModal();
}

wxString Controls::GetMessageDesc( long errorNum )
{
	wxString str;
	switch (errorNum)
	{
		case QGEN_MSG_EXISTS: str = wxT("Такое название уже существует, введите другое название!"); break;
		case QGEN_MSG_EXISTS_HKEY: str = wxT("Такая комбинация клавиш уже используется, введите другую комбинацию!"); break;
		case QGEN_MSG_EMPTYDATA: str = wxT("Пустое поле, введите значение!"); break;
		case QGEN_MSG_WRONGPASSWORD: str = wxT("Неверный пароль!"); break;
		case QGEN_MSG_CANTSAVEGAME: str = wxT("Ошибка записи файла!"); break;
		case QGEN_MSG_NOTFOUND: str = wxT("Ничего не найдено!"); break;
		case QGEN_MSG_SEARCHENDED: str = wxT("Указанный текст больше не найден."); break;
		case QGEN_MSG_WRONGFORMAT: str = wxT("Неверный формат!"); break;
		case QGEN_MSG_MAXACTIONSCOUNTREACHED: str = wxString::Format(wxT("Вы не можете добавить на локацию более чем %i действий."), QGEN_MAXACTIONS); break;
		case QGEN_MSG_TOOLONGLOCATIONNAME: str = wxString::Format(wxT("Имя локации не может содержать более %i символов."), QGEN_MAXLOCATIONNAMELEN); break;
		default: str = wxT("Неизвестная ошибка!"); break;
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
	return _locNotebook->OpenLocationPage(locName, true);
}

void Controls::SortLocations(bool isAscending)
{
	if (_container->GetLocationsCount() > 2)
	{
		_container->SortLocations(isAscending);
		UpdateOpenedLocationsIndexes();
		UpdateLocationsList();
		ShowOpenedLocationsIcons();
		InitSearchData();
	}
}

bool Controls::IsSelectedLocationEmpty()
{
	int locIndex = GetSelectedLocationIndex();
	if (locIndex < 0) return true;
	_locNotebook->SaveOpenedPages();
	return _container->IsEmptyLoc(locIndex);
}

bool Controls::IsClipboardEmpty()
{
	wxTextDataObject data;
	bool res = true;
	if ( !wxTheClipboard->IsOpened() && wxTheClipboard->Open() )
	{
		if (wxTheClipboard->IsSupported(wxDF_TEXT))
		{
			wxTheClipboard->GetData( data );
			res = !IsCorrectDataFormat( data.GetText().Mid(0, 7) );
		}
		wxTheClipboard->Close();
	}
	return res;
}

bool Controls::SerializeLocData( size_t locIndex, wxString &buffer )
{
	wxString cntActs;
	size_t countActs;
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
	countActs = _container->GetActionsCount(locIndex);
	cntActs.Printf(wxT("%ld"), countActs);
	buffer.Append(EncodeString(cntActs));
	buffer.Append(QGEN_STRSDELIM);
	for (size_t i = 0; i < countActs; ++i)
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
	wxString str;

	//ID формата файла
	last = buffer.find(QGEN_STRSDELIM);
	str = buffer.Mid(first, last);
	if (!IsCorrectDataFormat(str))
	{
		ShowMessage(QGEN_MSG_WRONGFORMAT);
		return false;
	}
	//Данные о версии редактора
	first = last + 2;
	last = buffer.find(QGEN_STRSDELIM, first);
	str = buffer.Mid(first, last - first);

	//Имя локации
	first = last + 2;
	last = buffer.find(QGEN_STRSDELIM, first);
	locName = buffer.Mid(first, last - first);
	return true;
}

bool Controls::DeserializeLocData(size_t locIndex, const wxString &buffer)
{
	size_t first = 0, last = 0;
	wxString str;
	wxString actName;
	wxString pathPictAct;
	wxVariant convCount;
	size_t countActs;

	//ID формата файла
	last = buffer.find(QGEN_STRSDELIM);
	str = buffer.Mid(first, last);
	if (!IsCorrectDataFormat(str))
	{
		ShowMessage(QGEN_MSG_WRONGFORMAT);
		return false;
	}
	_container->ClearLocation(locIndex);

	//Данные о версии редактора
	first = last + 2;
	last = buffer.find(QGEN_STRSDELIM, first);
	str = buffer.Mid(first, last - first);

	//Имя локации
	first = last + 2;
	last = buffer.find(QGEN_STRSDELIM, first);
	str = buffer.Mid(first, last - first);

	//Описание локации
	first = last + 2;
	last = buffer.find(QGEN_STRSDELIM, first);
	str = buffer.Mid(first, last - first);
	_container->SetLocationDesc(locIndex, DecodeString(str));

	//Код локации
	first = last + 2;
	last = buffer.find(QGEN_STRSDELIM, first);
	str = buffer.Mid(first, last - first);
	_container->SetLocationCode(locIndex, DecodeString(str));

	//Количество действий
	first = last + 2;
	last = buffer.find(QGEN_STRSDELIM, first);
	str = buffer.Mid(first, last - first);
	convCount = DecodeString(str);
	countActs = convCount.GetLong();

	for (size_t i = 0; i < countActs; ++i)
	{
		//Изображение
		first = last + 2;
		last = buffer.find(QGEN_STRSDELIM, first);
		str = buffer.Mid(first, last - first);
		pathPictAct = DecodeString(str);

		//Название
		first = last + 2;
		last = buffer.find(QGEN_STRSDELIM, first);
		str = buffer.Mid(first, last - first);
		actName = DecodeString(str);
		_container->AddAction(locIndex, actName);
		_container->SetActionPicturePath(locIndex, i, pathPictAct);

		//Код
		first = last + 2;
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
				wxMessageDialog dlgMsg(_mainFrame, wxString::Format(wxT("Желаете заменить локацию %s?"), locName),
					wxT("Заменить локацию"), wxYES_NO|wxICON_QUESTION);
				dlgMsg.CenterOnParent();
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
		UpdateLocActions(locIndex);
		LocationPage *page = _locNotebook->GetPageByLocName(locName);
		if (page) page->LoadPage();
	}
}

wxString Controls::EncodeString( const wxString &str )
{
	wxString result;
	long lstr = str.Len();
	for (long i = 0; i < lstr; i++)
		result.Append((wxChar)((wxChar)str[i] - QGEN_CODREMOV));
	return result;
}

wxString Controls::DecodeString(const wxString &str)
{
	wxString result;
	long lstr = str.Len();
	for (long i = 0; i < lstr; i++)
		result.Append((wxChar)((wxChar)str[i] + QGEN_CODREMOV));
	return result;
}

void Controls::ClearSelectedLocation()
{
	int locIndex = GetSelectedLocationIndex();
	if (locIndex < 0) return;
	_locNotebook->SaveOpenedPages();
	if (_container->IsEmptyLoc(locIndex)) return;

	wxString locName(_container->GetLocationName(locIndex));
	wxMessageDialog dlgMsg(_mainFrame,
		wxString::Format(wxT("Желаете очистить локацию %s?"), locName),
		wxT("Очистить локацию"), wxYES_NO|wxICON_QUESTION);
	dlgMsg.CenterOnParent();
	if (dlgMsg.ShowModal() == wxID_YES)
	{
		_container->ClearLocation(locIndex);
		LocationPage *page = _locNotebook->GetPageByLocName(locName);
		if (page) page->Clear();
		UpdateLocActions(locIndex);
	}
}

wxTextEntryBase *Controls::GetCurrentTextBox()
{
	wxWindow *win = wxWindow::FindFocus();
	wxTextEntryBase *ret = wxDynamicCast(win, SyntaxTextBox);
	if (ret) return ret;
	return wxDynamicCast(win, wxTextCtrl);
}

bool Controls::CanUndoText()
{
	wxTextEntryBase *txt = GetCurrentTextBox();
	if (txt) return txt->CanUndo();
	return false;
}

bool Controls::CanRedoText()
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
	wxFileDialog filedlg( _mainFrame, wxT( "Выберите изображение" ),
		wxEmptyString, wxEmptyString, wxT( "Файлы изображений (*.png;*.jpg;*.bmp;*.gif)|*.png;*.jpg;*.bmp;*.gif|Все файлы (*.*)|*.*" ), wxFD_OPEN );
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

bool Controls::SaveGame(const wxString &filename, const wxString &password)
{
	_locNotebook->SaveOpenedPages();
	if (qspSaveQuest(filename.wx_str(), password, this))
	{
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
	if (qspOpenQuest(filename.wx_str(), _mainFrame, this, _currentGamePass, false))
	{
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
	if (qspOpenQuest(filename.wx_str(), _mainFrame, this, wxString(), true))
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
	size_t count = _container->GetLocationsCount();
	_locListBox->Freeze();
	_locListBox->Clear();
	for (size_t i = 0; i < count; ++i)
		_locListBox->Insert(_container->GetLocationName(i), i);
	UpdateActionsOnAllLocs();
	_locListBox->Thaw();
}

bool Controls::IsActionsOnSelectedLocEmpty()
{
	LocationPage *page = _locNotebook->GetSelectedPage();
	return (!page || page->IsActionsEmpty());
}

bool Controls::IsAllLocsClosed()
{
	return (_locNotebook->GetPageCount() == 0);
}

bool Controls::IsCorrectDataFormat(const wxString &str)
{
	return (str == QGEN_GAMEID);
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

wxString Controls::GetSelectedWord()
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
				_locListBox->Select(_dataSearch.idxLoc);
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
				_locListBox->Select(_dataSearch.idxLoc);
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
				_locListBox->Select(_dataSearch.idxLoc);
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
					_locListBox->Select(_dataSearch.idxLoc);
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
					_locListBox->Select(_dataSearch.idxLoc);
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
					_locListBox->Select(_dataSearch.idxLoc);
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
		_container->InsertLocation(locName, 0);
		_locListBox->Insert(locName, 0);
		_container->Save();
	}
	_lastSaveTime = 0;
}

bool Controls::IsGameSaved()
{
	_locNotebook->SaveOpenedPages();
	return _container->IsSaved();
}

void Controls::UpdateLocActions(size_t locIndex)
{
	wxArrayString actions;
	_container->GetLocActions(locIndex, actions);
	_locListBox->UpdateLocationActions(locIndex, actions);
}

void Controls::UpdateActionsOnAllLocs()
{
	wxArrayString actions;
	size_t count = _container->GetLocationsCount();
	for (size_t i = 0; i < count; ++i)
	{
		_container->GetLocActions(i, actions);
		_locListBox->UpdateLocationActions(i, actions);
	}
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
	_locListBox->SetLocStatus(locIndex, isOpened);
}

void Controls::ShowOpenedLocationsIcons()
{
	LocationPage *page;
	size_t index, count = _locNotebook->GetPageCount();
	for (index = 0; index < count; ++index)
	{
		page = ( LocationPage * )_locNotebook->GetPage(index);
		_locListBox->SetLocStatus(page->GetLocationIndex(), true);
	}
}

void Controls::UpdateMenuItems(wxMenu *menu)
{
	if (!menu) return;

	bool saveFound = menu->FindItem(SAVE_QUEST) != NULL;
	bool saveAsFound = menu->FindItem(SAVE_QUESTAS) != NULL;
	bool playQuestFound = menu->FindItem(PLAY_QUEST) != NULL;
	bool exportTxtFound = menu->FindItem(EXPORT_QUEST_TXT) != NULL;
	bool exportTxt2GamFound = menu->FindItem(EXPORT_QUEST_TXT2GAM) != NULL;
	bool locDelFound = menu->FindItem(LOC_DEL) != NULL;
	bool locRenameFound = menu->FindItem(LOC_RENAME) != NULL;
	bool locClearFound = menu->FindItem(LOC_CLEAR) != NULL;
	bool locCopyFound = menu->FindItem(LOC_COPY) != NULL;
	bool locReplaceFound = menu->FindItem(LOC_REPLACE) != NULL;
	bool locPasteFound = menu->FindItem(LOC_PASTE) != NULL;
	bool locPasteNewFound = menu->FindItem(LOC_PASTE_NEW) != NULL;
	bool locSortAscFound = menu->FindItem(LOC_SORT_ASC) != NULL;
	bool locSortDescFound = menu->FindItem(LOC_SORT_DESC) != NULL;
	bool locJumpLocFound = menu->FindItem(LOC_JUMP_LOC) != NULL;
	bool locExpandFound = menu->FindItem(LOC_EXPAND) != NULL;
	bool locCollapseFound = menu->FindItem(LOC_COLLAPSE) != NULL;
	bool createActFound = menu->FindItem(CREATE_ACTION) != NULL;
	bool delActFound = menu->FindItem(DEL_ACTION) != NULL;
	bool delAllActFound = menu->FindItem(DEL_ALL_ACTIONS) != NULL;
	bool renameActFound = menu->FindItem(RENAME_ACTION) != NULL;
	bool undoTextFound = menu->FindItem(UNDO_TEXT) != NULL;
	bool redoTextFound = menu->FindItem(REDO_TEXT) != NULL;
	bool copyTextFound = menu->FindItem(COPY_TEXT) != NULL;
	bool pasteTextFound = menu->FindItem(PASTE_TEXT) != NULL;
	bool cutTextFound = menu->FindItem(CUT_TEXT) != NULL;
	bool delTextFound = menu->FindItem(DEL_TEXT) != NULL;
	bool selAllTextFound = menu->FindItem(SELALL_TEXT) != NULL;
	bool closeAllTabsFound = menu->FindItem(ID_MENUCLOSEALLTABS) != NULL;
	bool closeExecptSelFound = menu->FindItem(ID_MENUCLOSEEXCEPTSELECTED) != NULL;
	bool closeSelFound = menu->FindItem(ID_MENUCLOSESELECTED) != NULL;
	bool showHideLocDesc = menu->FindItem(ID_LOCDESCVISIBLE) != NULL;
	bool showHideLocActs = menu->FindItem(ID_LOCACTVISIBLE) != NULL;
	bool res = false;

	if (saveFound || saveAsFound || playQuestFound || exportTxtFound || exportTxt2GamFound ||
		locExpandFound || locCollapseFound)
		res = !_container->IsEmpty();
	if (saveFound) menu->Enable(SAVE_QUEST, res);
	if (saveAsFound) menu->Enable(SAVE_QUESTAS, res);
	if (playQuestFound) menu->Enable(PLAY_QUEST, res);
	if (exportTxtFound) menu->Enable(EXPORT_QUEST_TXT, res);
	if (exportTxt2GamFound) menu->Enable(EXPORT_QUEST_TXT2GAM, res);
	if (locExpandFound) menu->Enable(LOC_EXPAND, res);
	if (locCollapseFound) menu->Enable(LOC_COLLAPSE, res);
	res = false;

	if (locDelFound || locRenameFound)
		res = GetSelectedLocationIndex() >= 0;
	if (locDelFound) menu->Enable(LOC_DEL, res);
	if (locRenameFound) menu->Enable(LOC_RENAME, res);
	res = false;

	if (delActFound || delAllActFound || renameActFound)
		res = !IsActionsOnSelectedLocEmpty();
	if (delActFound) menu->Enable(DEL_ACTION, res);
	if (delAllActFound) menu->Enable(DEL_ALL_ACTIONS, res);
	if (renameActFound) menu->Enable(RENAME_ACTION, res);
	res = false;

	if (locClearFound || locCopyFound)
		res = !IsSelectedLocationEmpty();
	if (locClearFound) menu->Enable(LOC_CLEAR, res);
	if (locCopyFound) menu->Enable(LOC_COPY, res);
	res = false;

	if (locReplaceFound || locPasteFound || locPasteNewFound)
		res = !IsClipboardEmpty();
	if (locReplaceFound) menu->Enable(LOC_REPLACE, res);
	if (locPasteFound) menu->Enable(LOC_PASTE, res);
	if (locPasteNewFound) menu->Enable(LOC_PASTE_NEW, res);
	res = false;

	if (locSortAscFound || locSortDescFound)
		res = _locListBox->GetCount() > 2;
	if (locSortAscFound) menu->Enable(LOC_SORT_ASC, res);
	if (locSortDescFound) menu->Enable(LOC_SORT_DESC, res);
	res = false;

	if (locJumpLocFound) menu->Enable(LOC_JUMP_LOC, GetCurrentTextBox() != NULL);
	if (createActFound)	menu->Enable(CREATE_ACTION, !IsAllLocsClosed());
	if (undoTextFound) menu->Enable(UNDO_TEXT, CanUndoText());
	if (redoTextFound) menu->Enable(REDO_TEXT, CanRedoText());
	if (pasteTextFound) menu->Enable(PASTE_TEXT, CanPasteText());
	if (selAllTextFound) menu->Enable(SELALL_TEXT, CanSelectText());

	if (copyTextFound || cutTextFound || delTextFound)
		res = CanCopyText();
	if (copyTextFound) menu->Enable(COPY_TEXT, res);
	if (cutTextFound) menu->Enable(CUT_TEXT, res);
	if (delTextFound) menu->Enable(DEL_TEXT, res);
	res = false;

	if (closeAllTabsFound || closeExecptSelFound || closeSelFound || showHideLocDesc || showHideLocActs)
		res = !IsAllLocsClosed();
	if (closeAllTabsFound) menu->Enable(ID_MENUCLOSEALLTABS, res);
	if (closeExecptSelFound) menu->Enable(ID_MENUCLOSEEXCEPTSELECTED, res);
	if (closeSelFound) menu->Enable(ID_MENUCLOSESELECTED, res);
	if (showHideLocDesc) menu->Enable(ID_LOCDESCVISIBLE, res);
	if (showHideLocActs) menu->Enable(ID_LOCACTVISIBLE, res);
}

bool Controls::RenameLocation( size_t locIndex, const wxString &name )
{
	wxString oldName(_container->GetLocationName(locIndex));
	if (_container->RenameLocation(locIndex, name))
	{
		_locListBox->SetString(locIndex, name);
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
	LocationPage *page;
	if (_container->RenameAction(locIndex, actIndex, name))
	{
		page = _locNotebook->GetPageByLocName(_container->GetLocationName(locIndex));
		if (page) page->RenameAction(actIndex, name);
		UpdateLocActions(locIndex);
		return true;
	}
	else
		ShowMessage( QGEN_MSG_EXISTS );
	return false;
}

void Controls::MoveLocationTo( size_t locIndex, size_t moveTo )
{
	_container->MoveLocationTo(locIndex, moveTo);
	UpdateOpenedLocationsIndexes();
	_locListBox->MoveItemTo(locIndex, moveTo);
	UpdateLocActions(moveTo);
	InitSearchData();
}

void Controls::MoveActionTo( size_t locIndex, size_t actIndex, size_t moveTo )
{
	_container->MoveActionTo(locIndex, actIndex, moveTo);
	LocationPage *page = _locNotebook->GetPageByLocName(_locListBox->GetString(locIndex));
	if (page) page->MoveActionTo(actIndex, moveTo);
	UpdateLocActions(locIndex);
	InitSearchData();
}

wxString Controls::GetGameInfo()
{
	int cntGenLocs,
		cntEmptyDesc = 0,
		cntEmptyCode = 0,
		cntEmptyActsCode = 0,
		sizeLocs = 0,
		cntGenActs = 0,
		maxSizeLoc = 0,
		currentSizeLoc = 0,
		currentCntActs;
	wxString locName, actName, data;

	cntGenLocs = _container->GetLocationsCount();

	if (cntGenLocs > 0)
	{
		for (int i = 0; i < cntGenLocs; ++i)
		{
			currentSizeLoc = 0;
			currentCntActs = 0;
			locName = _container->GetLocationName(i);
			currentSizeLoc = locName.Length();
			data = _container->GetLocationDesc(i);
			currentSizeLoc += data.Length();
			if (data.Trim().Trim(false).IsEmpty()) ++cntEmptyDesc;
			data = _container->GetLocationCode(i);
			currentSizeLoc += data.Length();
			if (data.Trim().Trim(false).IsEmpty()) ++cntEmptyCode;
			currentCntActs += _container->GetActionsCount(i);			
			if (currentCntActs)
			{
				cntGenActs += currentCntActs;
				for (int j = 0; j < currentCntActs; ++j)
				{
					actName = _container->GetActionName(i, j);
					currentSizeLoc += actName.Length();
					currentSizeLoc += _container->GetActionPicturePath(i, j).Length();
					data = _container->GetActionCode(i, j);
					currentSizeLoc += data.Length();
					if(data.Trim().Trim(false).IsEmpty()) ++cntEmptyActsCode;
				}
			}
			if (currentSizeLoc > maxSizeLoc) maxSizeLoc =  currentSizeLoc;
			sizeLocs += currentSizeLoc;
		}
	}
	int cntMiddle = 0;
	int sizeMiddle = 0;
	if (cntGenLocs > 0)
	{
		cntMiddle = (int) ((float) cntGenActs / cntGenLocs + 0.5);
		sizeMiddle = (int) ((float) sizeLocs / cntGenLocs + 0.5);
	}
	wxString message = wxString::Format(wxT("Игра содержит %i локаци(ю/й/и)\n"), cntGenLocs);
	message += wxString::Format(wxT("Из них локаций, не содержащих текст описания: %i\n"), cntEmptyDesc);
	message += wxString::Format(wxT("Локаций, не имеющих кода обработки события \"Посещение\": %i\n"), cntEmptyCode);
	message += wxString::Format(wxT("Среднее число базовых действий на 1 локацию: %i\n"), cntMiddle);
	message += wxString::Format(wxT("Базовых действий не имеющих кода обработки события \"Выбор\": %i\n"), cntEmptyActsCode);
	message += wxString::Format(wxT("Максимальный размер локации: %i символ(а/ов)\n"), maxSizeLoc);
	message += wxString::Format(wxT("Средний размер локации: %i символ(а/ов)\n"), sizeMiddle);
	message += wxString::Format(wxT("Всего локации игры содержат: %i символ(а/ов)"), sizeLocs);
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
