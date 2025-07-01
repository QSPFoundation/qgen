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

#include "utils.h"
#include "game.h"

wxString Utils::GetDocumentPath(const wxString &path, const wxString &file)
{
    wxFileName docFullPath(wxFileName::DirName(wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Documents)));
    wxFileName docPath(docFullPath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) + path, file);
    return docPath.GetFullPath();
}

wxString Utils::GetAppPath(const wxString &path, const wxString &file)
{
    wxFileName appFullPath(wxStandardPaths::Get().GetExecutablePath());
    wxFileName appPath(appFullPath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) + path, file);
    return appPath.GetFullPath();
}

wxString Utils::GetResourcePath(const wxString &path, const wxString &file)
{
    wxPathList resourcePathList;
    resourcePathList.AddEnvList(wxT("XDG_DATA_DIRS"));
    resourcePathList.Add(wxStandardPaths::Get().GetResourcesDir());

    wxArrayString prefixes;
    prefixes.Add(QGEN_FILEPREFIX);
    prefixes.Add(wxEmptyString);

    for (wxPathList::iterator it = resourcePathList.begin(); it != resourcePathList.end(); ++it)
    {
        for (wxArrayString::iterator prefixIt = prefixes.begin(); prefixIt != prefixes.end(); ++prefixIt)
        {
            wxFileName resourcePath(*it, file); /* directory & file names are separated */
            if (!prefixIt->IsEmpty())
                resourcePath.Assign(resourcePath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) + *prefixIt, file);

            if (!path.IsEmpty())
                resourcePath.Assign(resourcePath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) + path, file);

            if (resourcePath.Exists())
                return resourcePath.GetFullPath();
        }
    }

    return GetAppPath(path, file);
}

wxString Utils::GetConfigPath(const wxString &path, const wxString &file)
{
    wxFileName configPath(wxStandardPaths::Get().GetUserDir(wxStandardPathsBase::Dir_Config), file);

    if (!path.IsEmpty())
        configPath.Assign(configPath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) + path, file);

    return configPath.GetFullPath();
}

wxString Utils::GetWordFromPos(const wxString &text, long pos)
{
    long beginPos, lastPos;
    if (pos >= text.Length())
        pos = text.Length() - 1;
    if (pos > 0 && QSP_STRCHR(QSP_DELIMS, text[pos]))
        --pos;
    beginPos = pos;
    lastPos = pos;
    while (beginPos >= 0)
    {
        if (QSP_STRCHR(QSP_DELIMS, text[beginPos]))
            break;
        --beginPos;
    }
    while (lastPos < text.Length())
    {
        if (QSP_STRCHR(QSP_DELIMS, text[lastPos]))
            break;
        ++lastPos;
    }
    if (lastPos > beginPos)
        return text.Mid(beginPos + 1, lastPos - beginPos - 1);
    return wxEmptyString;
}
