// Copyright (C) 2005-2025
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

#include "controls.h"

Controls::Controls()
{
    _mainFrame = NULL;
    _locListBox = NULL;
    _locNotebook = NULL;
    _locale = NULL;
    _execHotkeyEnters = 0;

    _settings = new Settings();
    _container = new DataContainer();

#ifdef __WXMSW__
    _keysParser = new KeysParser(_settings->GetHotKeys());
#endif

    _keywordsStore = new KeywordsStore();

    InitData();
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
    return -1;
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
    return -1;
}

int Controls::AddLocation(const wxString &name)
{
    wxString locName(name);
    while (true)
    {
        wxTextEntryDialog dlgEntry(GetCurrentTopLevelWindow(),
            _("Enter name for the new location:"),
            _("Add location"), locName);
        if (dlgEntry.ShowModal() == wxID_OK)
        {
            locName = dlgEntry.GetValue().Trim().Trim(false);
            if (locName.IsEmpty())
                ShowMessage(QGEN_MSG_EMPTYDATA);
            else if ((int)locName.Len()>QGEN_MAXLOCATIONNAMELEN)
                ShowMessage(QGEN_MSG_TOOLONGLOCATIONNAME);
            else
            {
                int index = AddLocationByName(locName);
                if (index >= 0) return index;
            }
        }
        else
            return -1;
    }
}

bool Controls::RenameSelectedLocation()
{
    int locIndex = GetSelectedLocationIndex();
    if (locIndex < 0) return false;

    wxString name(_container->GetLocationName(locIndex));
    while (true)
    {
        wxTextEntryDialog dlgEntry(GetCurrentTopLevelWindow(),
            _("Enter new name of the location:"),
            _("Rename location"), name);
        if (dlgEntry.ShowModal() == wxID_OK)
        {
            name = dlgEntry.GetValue().Trim().Trim(false);
            if (name.IsEmpty())
                ShowMessage(QGEN_MSG_EMPTYDATA);
            else if ((int)name.Len()>QGEN_MAXLOCATIONNAMELEN)
                ShowMessage(QGEN_MSG_TOOLONGLOCATIONNAME);
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
    wxMessageDialog dlgMsg(GetCurrentTopLevelWindow(),
        wxString::Format(_("Remove the location \"%s\"?"), locName),
        _("Remove location"), wxYES_NO|wxCENTRE|wxICON_QUESTION);
    if (dlgMsg.ShowModal() == wxID_YES)
    {
        int pageIndex = _locNotebook->FindPageIndex(locName);
        if (pageIndex >= 0) _locNotebook->ClosePage(pageIndex);
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
        ShowMessage(QGEN_MSG_MAXACTIONSCOUNTREACHED);
        return false;
    }

    while (true)
    {
        wxTextEntryDialog dlgEntry(GetCurrentTopLevelWindow(),
            _("Enter name for the new action:"),
            _("Add action"), name);

        if (dlgEntry.ShowModal() == wxID_OK)
        {
            name = dlgEntry.GetValue();
            if (name.IsEmpty())
                ShowMessage(QGEN_MSG_EMPTYDATA);
            else if ((int)name.Len()>QGEN_MAXACTIONNAMELEN)
                ShowMessage(QGEN_MSG_TOOLONGACTIONNAME);
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
                    ShowMessage(QGEN_MSG_EXISTS);
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

    wxMessageDialog dlgMsg(GetCurrentTopLevelWindow(),
        wxString::Format(_("Remove all actions at the location \"%s\"?"), locName),
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
    while (true)
    {
        wxTextEntryDialog dlgEntry(GetCurrentTopLevelWindow(),
            _("Enter new name of the action:"),
            _("Rename action"), name);
        if (dlgEntry.ShowModal() == wxID_OK)
        {
            name = dlgEntry.GetValue();
            if (name.IsEmpty())
                ShowMessage(QGEN_MSG_EMPTYDATA);
            else if ((int)name.Len() > QGEN_MAXACTIONNAMELEN)
                ShowMessage(QGEN_MSG_TOOLONGACTIONNAME);
            else
            {
                if (RenameAction(locIndex, actIndex, name)) return true;
            }
        }
        else
            return false;
    }
}

void Controls::ShowMessage(long errorNum)
{
    wxMessageDialog dlgMsg(GetCurrentTopLevelWindow(), GetMessageDesc(errorNum),
        _("Info"), wxOK|wxICON_INFORMATION|wxCENTRE);
    dlgMsg.ShowModal();
}

wxString Controls::GetMessageDesc(long errorNum)
{
    wxString str;
    switch (errorNum)
    {
        case QGEN_MSG_EXISTS: str = _("Such name already exists! Enter another name."); break;
        case QGEN_MSG_EXISTS_HKEY: str = _("This key combination is used already! Select another combination."); break;
        case QGEN_MSG_EXISTS_S_HKEY: str = _("This key combination is used already by the system! Select another combination."); break;
        case QGEN_MSG_EMPTYDATA: str = _("Empty values aren't allowed! Enter the value."); break;
        case QGEN_MSG_WRONGPASSWORD: str = _("Wrong password!"); break;
        case QGEN_MSG_CANTSAVEGAME: str = _("Can't save the game!"); break;
        case QGEN_MSG_CANTLOADGAME: str = _("Can't load the game!"); break;
        case QGEN_MSG_NOTFOUND: str = _("The specified text was not found"); break;
        case QGEN_MSG_SEARCHENDED: str = _("The specified text was not found anymore."); break;
        case QGEN_MSG_INVALIDREGEXP: str = _("Invalid regular expression!"); break;
        case QGEN_MSG_WRONGFORMAT: str = _("Incorrect data format!"); break;
        case QGEN_MSG_MAXACTIONSCOUNTREACHED: str = wxString::Format(_("Can't add more than %i actions."), QGEN_MAXACTIONS); break;
        case QGEN_MSG_TOOLONGLOCATIONNAME: str = wxString::Format(_("Location name can't contain more than %i characters!"), QGEN_MAXLOCATIONNAMELEN); break;
        case QGEN_MSG_TOOLONGACTIONNAME: str = wxString::Format(_("Action name can't contain more than %i characters!"), QGEN_MAXACTIONNAMELEN); break;
        case QGEN_MSG_TOOLONGFOLDERNAME: str = wxString::Format(_("Folder name can't contain more than %i characters!"), QGEN_MAXFOLDERNAMELEN); break;
        default: str = _("Unknown error!"); break;
    }
    return str;
}

LocationPage *Controls::ShowLocation(const wxString& locName)
{
    int pageIndex = _locNotebook->FindPageIndex(locName);
    if (pageIndex >= 0)
    {
        _locNotebook->SetSelection(pageIndex);
        return (LocationPage *)_locNotebook->GetPage(pageIndex);
    }
    LocationPage *page = _locNotebook->OpenLocationPage(locName, true);
    if (page && _settings->GetCollapseCode())
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

    _locNotebook->SaveOpenPages();
    return _container->IsLocEmpty(locIndex);
}

bool Controls::IsClipboardEmpty()
{
    wxString buffer;
    wxTextDataObject data;
    if (!wxTheClipboard->IsOpened() && wxTheClipboard->Open())
    {
        if (wxTheClipboard->IsSupported(wxDF_TEXT))
        {
            if (wxTheClipboard->GetData(data))
                buffer = data.GetText();
        }
        wxTheClipboard->Close();
        return !IsCorrectDataFormat(buffer);
    }
    return true;
}

bool Controls::IsCorrectDataFormat(const wxString &str)
{
    int actsCount;
    size_t count = 0;
    wxArrayString lineArray;
    wxStringInputStream inputStream(str);
    wxTextInputStream textStream(inputStream);
    while (!inputStream.Eof())
    {
        wxString line = textStream.ReadLine();
        if (!inputStream.IsOk()) break;
        lineArray.Add(line);
        ++count;
    }

    if (count < 6) return false;
    if (!lineArray[0].IsSameAs(QSP_GAMEID)) return false;
    wxString actsCountString = DecodeString(lineArray[5]);
    if (!actsCountString.ToInt(&actsCount)) return false;
    if (actsCount < 0) return false;
    if (count < 5 + actsCount * 3) return false;
    return true;
}

wxString Controls::GetBufferedLocName(const wxString &buffer)
{
    if (!IsCorrectDataFormat(buffer))
    {
        ShowMessage(QGEN_MSG_WRONGFORMAT);
        return wxEmptyString;
    }
    wxStringInputStream inputStream(buffer);
    wxTextInputStream textStream(inputStream);

    // skip game id
    textStream.ReadLine();
    // skip app name
    textStream.ReadLine();
    // get location name
    return textStream.ReadLine();
}

wxString Controls::SerializeLocData(size_t locIndex)
{
    size_t i, actsCount;
    wxStringOutputStream outputStream;
    wxTextOutputStream textStream(outputStream);

    textStream << QSP_GAMEID << wxT('\n');
    textStream << wxString::Format(wxT("%s %s"), QGEN_APPNAME, QGEN_VER) << wxT('\n');
    textStream << _container->GetLocationName(locIndex) << wxT('\n');
    textStream << EncodeString(_container->GetLocationDesc(locIndex)) << wxT('\n');
    textStream << EncodeString(_container->GetLocationCode(locIndex)) << wxT('\n');
    actsCount = _container->GetActionsCount(locIndex);
    textStream << EncodeString(wxString::Format(wxT("%ld"), actsCount)) << wxT('\n');
    for (i = 0; i < actsCount; ++i)
    {
        textStream << EncodeString(_container->GetActionPicturePath(locIndex, i)) << wxT('\n');
        textStream << EncodeString(_container->GetActionName(locIndex, i)) << wxT('\n');
        textStream << EncodeString(_container->GetActionCode(locIndex, i)) << wxT('\n');
    }
    textStream.Flush();
    return outputStream.GetString();
}

bool Controls::DeserializeLocData(size_t locIndex, const wxString &buffer)
{
    wxString str;
    wxString actImage;
    int i, actsCount;
    if (!IsCorrectDataFormat(buffer))
    {
        ShowMessage(QGEN_MSG_WRONGFORMAT);
        return false;
    }
    _container->ClearLocation(locIndex);

    wxStringInputStream inputStream(buffer);
    wxTextInputStream textStream(inputStream);

    // skip game id
    textStream.ReadLine();
    // skip app name
    textStream.ReadLine();
    // skip location name
    textStream.ReadLine();

    // get location desc
    str = DecodeString(textStream.ReadLine());
    _container->SetLocationDesc(locIndex, str);

    // get location code
    str = DecodeString(textStream.ReadLine());
    _container->SetLocationCode(locIndex, str);

    // get actions count
    str = DecodeString(textStream.ReadLine());
    str.ToInt(&actsCount);

    for (i = 0; i < actsCount; ++i)
    {
        // get action image
        actImage = DecodeString(textStream.ReadLine());

        // get action name
        str = DecodeString(textStream.ReadLine());
        _container->AddAction(locIndex, str);
        _container->SetActionPicturePath(locIndex, i, actImage);

        // get action code
        str = DecodeString(textStream.ReadLine());
        _container->SetActionCode(locIndex, i, str);
    }
    return true;
}

void Controls::CopySelectedLocToClipboard()
{
    int locIndex = GetSelectedLocationIndex();
    if (locIndex < 0) return;
    _locNotebook->SaveOpenPages();
    if (_container->IsLocEmpty(locIndex)) return;

    wxString buffer = SerializeLocData(locIndex);
    if (!wxTheClipboard->IsOpened() && wxTheClipboard->Open())
    {
        wxTheClipboard->SetData(new wxTextDataObject(buffer));
        wxTheClipboard->Close();
    }
}

void Controls::PasteLocFromClipboard(PasteType type)
{
    wxTextDataObject data;
    wxString sourceLocName, locName, buffer;
    int locIndex = -1;

    if (!wxTheClipboard->IsOpened() && wxTheClipboard->Open())
    {
        if (wxTheClipboard->IsSupported(wxDF_TEXT))
        {
            if (wxTheClipboard->GetData(data))
                buffer = data.GetText();
        }
        wxTheClipboard->Close();
    }
    if (!IsCorrectDataFormat(buffer)) return;

    switch (type)
    {
    case PASTE_REPLACE:
        locIndex = GetSelectedLocationIndex();
        if (locIndex >= 0)
        {
            locName = _container->GetLocationName(locIndex);
            _locNotebook->SaveOpenPages();
            if (!_container->IsLocEmpty(locIndex))
            {
                wxMessageDialog dlgMsg(GetCurrentTopLevelWindow(),
                    wxString::Format(_("Replace the location \"%s\"?"), locName),
                    _("Replace location"), wxYES_NO|wxCENTRE|wxICON_QUESTION);
                if (dlgMsg.ShowModal() == wxID_YES)
                    InitSearchData();
                else
                    locIndex = -1;
            }
        }
        break;
    case PASTE_NEW:
        locIndex = AddLocation();
        if (locIndex >= 0) locName = _container->GetLocationName(locIndex);
        break;
    case PASTE_NEW_AUTO:
        sourceLocName = GetBufferedLocName(buffer);
        if (!sourceLocName.IsEmpty())
        {
            unsigned long ind = 1;
            locName = sourceLocName;
            while (_container->FindLocationIndex(locName) >= 0)
            {
                locName = wxString::Format(wxT("%s%ld"), sourceLocName, ind);
                ++ind;
            }
            locIndex = AddLocationByName(locName);
        }
        break;
    }
    if (locIndex >= 0 && DeserializeLocData(locIndex, buffer))
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
        if (ch == QSP_CODREMOV)
            ch = -QSP_CODREMOV;
        else
            ch -= QSP_CODREMOV;
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
        if (ch == (wxChar)-QSP_CODREMOV)
            ch = QSP_CODREMOV;
        else
            ch += QSP_CODREMOV;
        result.Append(ch);
    }
    return result;
}

void Controls::ClearSelectedLocation()
{
    int locIndex = GetSelectedLocationIndex();
    if (locIndex < 0) return;
    _locNotebook->SaveOpenPages();
    if (_container->IsLocEmpty(locIndex)) return;

    wxString locName(_container->GetLocationName(locIndex));
    wxMessageDialog dlgMsg(GetCurrentTopLevelWindow(),
        wxString::Format(_("Clear the location \"%s\"?"), locName),
        _("Clear location"), wxYES_NO|wxCENTRE|wxICON_QUESTION);
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
    wxFileDialog fileDlg(GetCurrentTopLevelWindow(), _("Select image file"),
        wxEmptyString, wxEmptyString, _("Images (*.png;*.jpg;*.bmp;*.gif)|*.png;*.jpg;*.bmp;*.gif|All files (*.*)|*"), wxFD_OPEN);
    fileDlg.CentreOnParent();
    if (fileDlg.ShowModal() == wxID_OK)
    {
        wxFileName path(fileDlg.GetPath());
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
    _locNotebook->SaveOpenPages();

    // We save game to a temporary file & replace the target file if everything goes well
    wxString tempFile(wxFileName::CreateTempFileName(QGEN_FILEPREFIX));

    bool wasSaved = false;
    char *buf;
    long fileSize = qspSaveQuest(password, this, &buf);
    if (fileSize > 0)
    {
        wxFile file;
        if (file.Open(tempFile, wxFile::write))
            wasSaved = (file.Write(buf, fileSize) == fileSize);
        free(buf);
    }
    if (wasSaved)
    {
        if (wxRenameFile(tempFile, filename, true))
        {
            wxFileName configFile(filename);
            configFile.SetExt("qproj");
            SaveConfigFile(_container, configFile.GetFullPath());
            _container->Save();
            _lastSaveTime = wxGetLocalTimeMillis();
            _currentGamePath = filename;
            _currentGamePass = password;
            return true;
        }
    }
    return false;
}

bool Controls::LoadGame(const wxString &filename)
{
    _locNotebook->ClosePages(CLOSE_ALL);

    bool wasOpened = false;
    wxFile file;
    if (file.Open(filename, wxFile::read))
    {
        long fileSize = file.Length();
        long bufSize = fileSize + QSP_LEN(QSP_STRSDELIM);
        char *buf = (char *)malloc(bufSize);
        if (file.Read(buf, fileSize) == fileSize)
        {
            if (qspOpenQuest(buf, bufSize, GetCurrentTopLevelWindow(), this, _currentGamePass, false))
            {
                wxFileName configFile(filename);
                configFile.SetExt("qproj");
                OpenConfigFile(_container, configFile.GetFullPath());
                InitSearchData();
                _currentGamePath = filename;
                UpdateLocationsList();
                _container->Save();
                _lastSaveTime = wxGetLocalTimeMillis();
                wasOpened = true;
            }
        }
        free(buf);
    }
    return wasOpened;
}

bool Controls::JoinGame(const wxString &filename)
{
    bool wasOpened = false;
    wxFile file;
    if (file.Open(filename, wxFile::read))
    {
        size_t fileSize = file.Length();
        char *buf = (char *)malloc(fileSize);
        if (file.Read(buf, fileSize) == fileSize)
        {
            wxString dummy;
            if (qspOpenQuest(buf, fileSize, GetCurrentTopLevelWindow(), this, dummy, true))
            {
                InitSearchData();
                UpdateLocationsList();
                _locNotebook->LoadOpenPages();
                wasOpened = true;
            }
        }
        free(buf);
    }
    return wasOpened;
}

void Controls::UpdateLocationsList()
{
    size_t locsCount = _container->GetLocationsCount();
    _locListBox->Freeze();
    _locListBox->Clear();
    wxString folderName;
    wxArrayInt locs;
    int oldPos = -1, pos = 0, folderIndex;
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

SearchResult Controls::FindSubString(const wxString& s, const wxString& sub, bool isCaseSensitive, bool isWholeString, bool isRegExp, size_t startInd)
{
    size_t subLen, textEndInd, length = s.length();
    if (isRegExp)
    {
        wxRegEx regExp(sub, isCaseSensitive ? wxRE_DEFAULT : wxRE_DEFAULT | wxRE_ICASE);
        if (!regExp.IsValid())
        {
            ShowMessage(QGEN_MSG_INVALIDREGEXP);
            return SearchResult(true);
        }

        size_t subStart;
        wxString textToSearch(s.Mid(startInd));
        while (regExp.Matches(textToSearch))
        {
            if (!regExp.GetMatch(&subStart, &subLen))
                break;
            startInd += subStart;

            if (!isWholeString)
                return SearchResult((int)startInd, textToSearch.Mid(subStart, subLen));

            textEndInd = startInd + subLen;
            if ((startInd == 0 || QSP_STRCHR(QSP_DELIMS, s[startInd - 1])) &&
                (textEndInd >= length || QSP_STRCHR(QSP_DELIMS, s[textEndInd])))
                return SearchResult((int)startInd, textToSearch.Mid(subStart, subLen));

            ++startInd;
            textToSearch = s.Mid(startInd);
        }
    }
    else
    {
        wxString sourceText(isCaseSensitive ? s : s.Upper());
        wxString textToFind(isCaseSensitive ? sub : sub.Upper());
        subLen = sub.length();
        while (true)
        {
            /* We search depending on casing setting, but the found text always matches the source */
            startInd = sourceText.find(textToFind, startInd);
            if (startInd == wxString::npos)
                break;

            if (!isWholeString)
                return SearchResult((int)startInd, s.Mid(startInd, subLen));

            textEndInd = startInd + subLen;
            if ((startInd == 0 || QSP_STRCHR(QSP_DELIMS, s[startInd - 1])) &&
                (textEndInd >= length || QSP_STRCHR(QSP_DELIMS, s[textEndInd])))
                return SearchResult((int)startInd, s.Mid(startInd, subLen));

            ++startInd;
        }
    }
    return SearchResult();
}

bool Controls::SearchNextLoc()
{
    size_t locsCount = _container->GetLocationsCount();
    _dataSearch.FindAt = SEARCH_LOCNAME;
    _dataSearch.StartPos = -1;
    _dataSearch.FoundString.Clear();
    if (++_dataSearch.LocIndex >= locsCount) _dataSearch.LocIndex = 0;
    if (++_dataSearch.LocsChecked >= locsCount)
    {
        ShowMessage(_dataSearch.FoundAny ? QGEN_MSG_SEARCHENDED : QGEN_MSG_NOTFOUND);
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
                if (QSP_STRCHR(QSP_DELIMS, data[beginPos]))
                    break;
                else
                    --beginPos;
            while ((size_t)lastPos < data.Length())
                if (QSP_STRCHR(QSP_DELIMS, data[lastPos]))
                    break;
                else
                    ++lastPos;
            if (lastPos > beginPos) str = data.Mid(beginPos + 1, lastPos - beginPos - 1);
        }
    }
    return str;
}

bool Controls::SearchString(const wxString &str, bool toFindAgain, bool isCaseSensitive, bool isWholeString, bool isRegExp)
{
    wxString data;
    wxString locName;
    wxString actName;
    LocationPage *page = NULL;
    size_t locsCount = _container->GetLocationsCount();
    if (!locsCount) return false;
    SyncWithLocationsList();

    _locNotebook->SaveOpenPages();

    if (_dataSearch.LocIndex >= locsCount)
        _dataSearch.LocIndex = 0;

    if (toFindAgain
        || str != _dataSearch.SearchString
        || _dataSearch.LocsChecked >= locsCount)
    {
        InitSearchData();
        int locIndex = GetSelectedLocationIndex();
        if (locIndex < 0) locIndex = 0;
        _dataSearch.LocIndex = locIndex;
    }

    _dataSearch.FoundAt = SEARCH_NONE;
    _dataSearch.SearchString = str;

    SearchResult searchRes;
    while (true) // we stop when _dataSearch.LocsChecked >= locsCount
    {
        locName = _container->GetLocationName(_dataSearch.LocIndex);
        if (_dataSearch.FindAt == SEARCH_LOCNAME)
        {
            _dataSearch.FindAt = SEARCH_LOCDESC;
            _dataSearch.StartPos = -1;
            _dataSearch.FoundString.Clear();
            searchRes = FindSubString(locName, str, isCaseSensitive, isWholeString, isRegExp);
            if (searchRes.IsError) return false;
            if (searchRes.Position >= 0)
            {
                _locListBox->Select(locName);
                ShowLocation(locName);
                _locListBox->SetFocus();
                _dataSearch.FoundAt = SEARCH_LOCNAME;
                _dataSearch.FoundAny = true;
                return true;
            }
        }
        if (_dataSearch.FindAt == SEARCH_LOCDESC)
        {
            data = _container->GetLocationDesc(_dataSearch.LocIndex);
            searchRes = FindSubString(data, str, isCaseSensitive, isWholeString, isRegExp, _dataSearch.StartPos + 1);
            if (searchRes.IsError) return false;
            if (searchRes.Position >= 0)
            {
                _locListBox->Select(locName);
                page = ShowLocation(locName);
                if (page) page->SelectLocDescString(searchRes.Position, searchRes.Position + searchRes.FoundString.length());
                _dataSearch.StartPos = searchRes.Position;
                _dataSearch.FoundString = searchRes.FoundString;
                _dataSearch.FoundAt = SEARCH_LOCDESC;
                _dataSearch.FoundAny = true;
                return true;
            }
            else
            {
                _dataSearch.FindAt = SEARCH_LOCCODE;
                _dataSearch.StartPos = -1;
                _dataSearch.FoundString.Clear();
            }
        }

        if (_dataSearch.FindAt == SEARCH_LOCCODE)
        {
            data = _container->GetLocationCode(_dataSearch.LocIndex);
            searchRes = FindSubString(data, str, isCaseSensitive, isWholeString, isRegExp, _dataSearch.StartPos + 1);
            if (searchRes.IsError) return false;
            if (searchRes.Position >= 0)
            {
                _locListBox->Select(locName);
                page = ShowLocation(locName);
                if (page) page->SelectLocCodeString(searchRes.Position, searchRes.Position + searchRes.FoundString.length());
                _dataSearch.StartPos = searchRes.Position;
                _dataSearch.FoundString = searchRes.FoundString;
                _dataSearch.FoundAt = SEARCH_LOCCODE;
                _dataSearch.FoundAny = true;
                return true;
            }
            else
            {
                _dataSearch.FindAt = SEARCH_ACTNAME;
                _dataSearch.StartPos = -1;
                _dataSearch.FoundString.Clear();
                _dataSearch.ActIndex = 0;
            }
        }

        size_t actsCount = _container->GetActionsCount(_dataSearch.LocIndex);

        for (; _dataSearch.ActIndex < actsCount; ++_dataSearch.ActIndex)
        {
            if (_dataSearch.FindAt == SEARCH_ACTNAME)
            {
                actName = _container->GetActionName(_dataSearch.LocIndex, _dataSearch.ActIndex);
                searchRes = FindSubString(actName, str, isCaseSensitive, isWholeString, isRegExp, _dataSearch.StartPos + 1);
                if (searchRes.IsError) return false;
                if (searchRes.Position >= 0)
                {
                    _locListBox->Select(locName);
                    page = ShowLocation(locName);
                    if (page) page->SelectAction(_dataSearch.ActIndex);
                    _dataSearch.StartPos = searchRes.Position;
                    _dataSearch.FoundString = searchRes.FoundString;
                    _dataSearch.FoundAt = SEARCH_ACTNAME;
                    _dataSearch.FoundAny = true;
                    return true;
                }
                else
                {
                    _dataSearch.FindAt = SEARCH_PATHPICT;
                    _dataSearch.StartPos = -1;
                    _dataSearch.FoundString.Clear();
                }
            }
            if (_dataSearch.FindAt == SEARCH_PATHPICT)
            {
                data = _container->GetActionPicturePath(_dataSearch.LocIndex, _dataSearch.ActIndex);
                searchRes = FindSubString(data, str, isCaseSensitive, isWholeString, isRegExp, _dataSearch.StartPos + 1);
                if (searchRes.IsError) return false;
                if (searchRes.Position >= 0)
                {
                    _locListBox->Select(locName);
                    page = ShowLocation(locName);
                    if (page)
                    {
                        page->SelectAction(_dataSearch.ActIndex);
                        page->SelectPicturePathString(searchRes.Position, searchRes.Position + searchRes.FoundString.length());
                    }
                    _dataSearch.StartPos = searchRes.Position;
                    _dataSearch.FoundString = searchRes.FoundString;
                    _dataSearch.FoundAt = SEARCH_PATHPICT;
                    _dataSearch.FoundAny = true;
                    return true;
                }
                else
                {
                    _dataSearch.FindAt = SEARCH_ACTCODE;
                    _dataSearch.StartPos = -1;
                    _dataSearch.FoundString.Clear();
                }
            }
            if (_dataSearch.FindAt == SEARCH_ACTCODE)
            {
                data = _container->GetActionCode(_dataSearch.LocIndex, _dataSearch.ActIndex);
                searchRes = FindSubString(data, str, isCaseSensitive, isWholeString, isRegExp, _dataSearch.StartPos + 1);
                if (searchRes.IsError) return false;
                if (searchRes.Position >= 0)
                {
                    _locListBox->Select(locName);
                    page = ShowLocation(locName);
                    if (page)
                    {
                        page->SelectAction( _dataSearch.ActIndex);
                        page->SelectActionCodeString(searchRes.Position, searchRes.Position + searchRes.FoundString.length());
                    }
                    _dataSearch.StartPos = searchRes.Position;
                    _dataSearch.FoundString = searchRes.FoundString;
                    _dataSearch.FoundAt = SEARCH_ACTCODE;
                    _dataSearch.FoundAny = true;
                    return true;
                }
                else
                {
                    _dataSearch.FindAt = SEARCH_ACTNAME;
                    _dataSearch.StartPos = -1;
                    _dataSearch.FoundString.Clear();
                }
            }
        }
        if (!SearchNextLoc()) return false;
    }
}

bool Controls::ReplaceSearchString(const wxString& replaceString, bool isCaseSensitive, bool isRegExp)
{
    if (_dataSearch.FoundAt == SEARCH_NONE) return true;
    LocationPage *page = _locNotebook->GetPageByLocName(_container->GetLocationName(_dataSearch.LocIndex));
    wxString temp, newSubString;

    if (isRegExp)
    {
        wxRegEx regExp(_dataSearch.SearchString, isCaseSensitive ? wxRE_DEFAULT : wxRE_DEFAULT | wxRE_ICASE);
        if (!regExp.IsValid())
        {
            ShowMessage(QGEN_MSG_INVALIDREGEXP);
            return false;
        }

        wxString newString = _dataSearch.FoundString;
        regExp.ReplaceFirst(&newString, replaceString);
        newSubString = newString;
    }
    else
        newSubString = replaceString;

    switch (_dataSearch.FoundAt)
    {
    case SEARCH_LOCDESC:
        temp = _container->GetLocationDesc(_dataSearch.LocIndex);
        temp.replace(_dataSearch.StartPos, _dataSearch.FoundString.length(), newSubString);
        _container->SetLocationDesc(_dataSearch.LocIndex, temp);
        if (page) page->ReplaceLocDescString(_dataSearch.StartPos, _dataSearch.StartPos + _dataSearch.FoundString.length(), newSubString);
        break;
    case SEARCH_LOCCODE:
        temp = _container->GetLocationCode(_dataSearch.LocIndex);
        temp.replace(_dataSearch.StartPos, _dataSearch.FoundString.length(), newSubString);
        _container->SetLocationCode(_dataSearch.LocIndex, temp);
        if (page) page->ReplaceLocCodeString(_dataSearch.StartPos, _dataSearch.StartPos + _dataSearch.FoundString.length(), newSubString);
        break;
    case SEARCH_ACTNAME:
        temp = _container->GetActionName(_dataSearch.LocIndex, _dataSearch.ActIndex);
        temp.replace(_dataSearch.StartPos, _dataSearch.FoundString.length(), newSubString);
        if (_container->RenameAction(_dataSearch.LocIndex, _dataSearch.ActIndex, temp))
        {
            if (page) page->RenameAction(_dataSearch.ActIndex, temp);
            _locListBox->UpdateLocationActions(_container->GetLocationName(_dataSearch.LocIndex));
        }
        else
        {
            ShowMessage(QGEN_MSG_EXISTS);
            return false;
        }
        break;
    case SEARCH_PATHPICT:
        temp = _container->GetActionPicturePath(_dataSearch.LocIndex, _dataSearch.ActIndex);
        temp.replace(_dataSearch.StartPos, _dataSearch.FoundString.length(), newSubString);
        _container->SetActionPicturePath(_dataSearch.LocIndex, _dataSearch.ActIndex, temp);
        if (page) page->ReplacePicturePathString(_dataSearch.StartPos, _dataSearch.StartPos + _dataSearch.FoundString.length(), newSubString);
        break;
    case SEARCH_ACTCODE:
        temp = _container->GetActionCode(_dataSearch.LocIndex, _dataSearch.ActIndex);
        temp.replace(_dataSearch.StartPos, _dataSearch.FoundString.length(), newSubString);
        _container->SetActionCode(_dataSearch.LocIndex, _dataSearch.ActIndex, temp);
        if (page) page->ReplaceActionCodeString(_dataSearch.StartPos, _dataSearch.StartPos + _dataSearch.FoundString.length(), newSubString);
        break;
    }
    _dataSearch.StartPos += (int)newSubString.length() - 1; /* point to the last char of replacement */
    _dataSearch.FoundString.Clear();
    _dataSearch.FoundAt = SEARCH_NONE;
    return true;
}

void Controls::InitSearchData()
{
    _dataSearch.SearchString.Clear();

    _dataSearch.FoundAt = SEARCH_NONE;
    _dataSearch.FindAt = SEARCH_LOCNAME;

    _dataSearch.LocIndex = 0;
    _dataSearch.ActIndex = 0;
    _dataSearch.StartPos = -1;
    _dataSearch.FoundString.Clear();

    _dataSearch.LocsChecked = 0;
    _dataSearch.FoundAny = false;
}

void Controls::InitData()
{
    _currentGamePath = Utils::GetDocumentPath(wxEmptyString, wxT("NoName.qsp"));
    _currentGamePass = QGEN_PASSWD;
    InitSearchData();
    _lastSaveTime = 0;
}

void Controls::NewGame()
{
    if (!_container->IsEmpty())
    {
        _locNotebook->ClosePages(CLOSE_ALL);
        _locListBox->Clear();
        _container->Clear();
        _container->Save();
    }
    InitData();
    wxString locName = _settings->GetFirstLocName().Trim(true).Trim(false);
    if (_settings->GetCreateFirstLoc() && !locName.IsEmpty())
    {
        _container->AddLocation(locName);
        _locListBox->Insert(locName, wxEmptyString, wxEmptyString);
        _container->Save();
    }
}

bool Controls::IsGameSaved()
{
    // We have to call SyncWithLocationsList because some locations could be moved in the locs list
    SyncWithLocationsList();
    _locNotebook->SaveOpenPages();
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
        page = (LocationPage *)_locNotebook->GetPage(index);
        page->SetLocationIndex(_container->FindLocationIndex(_locNotebook->GetPageText(index)));
    }
}

void Controls::UpdateLocationIcon(size_t locIndex, bool isOpened)
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
    if (createActFound)    menu->Enable(ID_ACTION_ADD, !IsAllLocsClosed());
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

bool Controls::RenameFolder(size_t folderIndex, const wxString &name)
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

bool Controls::RenameLocation(size_t locIndex, const wxString &name)
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

bool Controls::RenameAction(size_t locIndex, size_t actIndex, const wxString &name)
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

void Controls::MoveActionTo(size_t locIndex, size_t actIndex, size_t moveTo)
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
        avgLocSize = 0;
    float avgActionsPerLoc = 0.0f;

    totalLocsCount = _container->GetLocationsCount();
    if (totalLocsCount > 0)
    {
        for (int i = 0; i < totalLocsCount; ++i)
        {
            int locSize = 0;
            wxString locName = _container->GetLocationName(i);
            locSize = locName.Length();
            wxString locDesc = _container->GetLocationDesc(i);
            locSize += locDesc.Length();
            if (locDesc.Trim(true).Trim(false).IsEmpty())
                ++totalEmptyDesc;
            wxString locCode = _container->GetLocationCode(i);
            locSize += locCode.Length();
            if (locCode.Trim(true).Trim(false).IsEmpty())
                ++totalEmptyCode;
            int actsCount = _container->GetActionsCount(i);
            if (actsCount)
            {
                totalActs += actsCount;
                for (int j = 0; j < actsCount; ++j)
                {
                    wxString actName = _container->GetActionName(i, j);
                    locSize += actName.Length();
                    locSize += _container->GetActionPicturePath(i, j).Length();
                    wxString actCode = _container->GetActionCode(i, j);
                    locSize += actCode.Length();
                    if (actCode.Trim(true).Trim(false).IsEmpty())
                        ++totalEmptyActsCode;
                }
            }
            if (locSize > maxLocSize) maxLocSize = locSize;
            totalLocsSize += locSize;
        }
        avgActionsPerLoc = (float)((double)totalActs / totalLocsCount);
        avgLocSize = (int)((double)totalLocsSize / totalLocsCount + 0.5);
    }
    wxString message = wxString::Format(_("This game contains %i location(s)\n"), totalLocsCount);
    message += wxString::Format(_("Locations without base description: %i\n"), totalEmptyDesc);
    message += wxString::Format(_("Locations without \"on visit\" code: %i\n"), totalEmptyCode);
    message += wxString::Format(_("Average number of actions per location: %.2f\n"), avgActionsPerLoc);
    message += wxString::Format(_("Actions without code: %i\n"), totalEmptyActsCode);
    message += wxString::Format(_("Max location size: %i characters\n"), maxLocSize);
    message += wxString::Format(_("Average location size: %i characters\n"), avgLocSize);
    message += wxString::Format(_("Total game size: %i characters"), totalLocsSize);
    return message;
}

bool Controls::ExportTxt(const wxString &filename)
{
    SyncWithLocationsList();
    _locNotebook->SaveOpenPages();

    bool wasSaved = false;
    char *buf;
    long fileSize = qspExportTxt(this, &buf);
    if (fileSize > 0)
    {
        wxFile file;
        if (file.Open(filename, wxFile::write))
        {
            file.Write(QGEN_BOM, sizeof(QGEN_BOM) - 1);
            wasSaved = (file.Write(buf, fileSize) == fileSize);
        }
        free(buf);
    }
    return wasSaved;
}

bool Controls::ExportTxt2Gam(const wxString &filename)
{
    if (!IsGameSaved()) return false;
    return qspExportTxt2Gam(filename.wx_str(), this);
}

bool Controls::ImportTxt2Gam(const wxString &filename)
{
    if (qspImportTxt2Gam(filename.wx_str(), this))
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

bool Controls::ExecuteHotkey(int keyCode, int modifiers)
{
    bool res = false;
#ifdef __WXMSW__
    ++_execHotkeyEnters;
    res = _keysParser->ExecuteHotkeyAction(keyCode, modifiers);
    --_execHotkeyEnters;
#endif
    return res;
}

void Controls::SetStatusText(const wxString &text)
{
    wxStatusBar *statusBar = _mainFrame->GetStatusBar();
    if (statusBar) statusBar->SetStatusText(text);
}

void Controls::SelectLocation(size_t locIndex)
{
    _locListBox->Select(_container->GetLocationName(locIndex));
}

bool Controls::AddFolder()
{
    wxString name;
    while (true)
    {
        wxTextEntryDialog dlgEntry(GetCurrentTopLevelWindow(),
            _("Enter name for the new folder:"),
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
    wxMessageDialog dlgMsg(GetCurrentTopLevelWindow(),
        wxString::Format(_("Remove the \"%s\" folder?"), folderName),
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
    while (true)
    {
        wxTextEntryDialog dlgEntry(GetCurrentTopLevelWindow(),
            _("Enter new name of the folder:"),
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
        wxFileDialog dialog(GetCurrentTopLevelWindow(),
            _("Select help file"), wxEmptyString, wxEmptyString,
            _("Help file (*.chm)|*.chm|All files (*.*)|*"), wxFD_OPEN);
        dialog.CenterOnParent();
        if (dialog.ShowModal() == wxID_CANCEL) return false;
        _settings->SetCurrentHelpPath(dialog.GetPath());
    }
    return true;
}

wxWindow *Controls::GetCurrentTopLevelWindow()
{
    return wxGetTopLevelParent(wxGetActiveWindow());
}

void Controls::UpdateLocale(int lang)
{
    if (_locale) delete _locale;
    _locale = new wxLocale;
    _locale->Init(lang);

    wxString langCode(_locale->GetCanonicalName().BeforeFirst(wxT('_')));

    // Load general UI translations
    _locale->AddCatalogLookupPathPrefix(Utils::GetResourcePath(QGEN_TRANSLATIONS));

    wxString specificLangFile;
    specificLangFile.Printf(QGEN_LOCALEFILE, langCode.wx_str());
    if (!_locale->AddCatalog(QGEN_DEFLOCALEFILE))
        _locale->AddCatalog(specificLangFile);

    // Load keywords
    wxString specificKeywordsFile;
    specificKeywordsFile.Printf(QGEN_KEYWORDSFILE, langCode.wx_str());
    if (!_keywordsStore->Load(Utils::GetResourcePath(wxEmptyString, QGEN_DEFKEYWORDSFILE)))
    {
        if (!_keywordsStore->Load(Utils::GetResourcePath(QGEN_TRANSLATIONS, specificKeywordsFile)))
            _keywordsStore->Load(Utils::GetResourcePath(QGEN_TRANSLATIONS, QGEN_DEFKEYWORDSFILE));
    }
}
