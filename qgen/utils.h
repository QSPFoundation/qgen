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

#ifndef _QGEN_UTILS_H
    #define _QGEN_UTILS_H

    #include <wx/wx.h>
    #include <wx/filename.h>
    #include <wx/stdpaths.h>
    #include <wx/filefn.h>
    #include <wx/uri.h>

    #include "qgen_config.h"

    #define QGEN_VER wxT(QGEN_VER_STR)
    #define QGEN_APPNAME wxT("QGen")
    #define QGEN_FILEPREFIX wxT("qgen")
    #define QGEN_CONFIG wxT("qgen.cfg")
    #define QGEN_TRANSLATIONS wxT("langs")
    #define QGEN_DEFLOCALEFILE wxT("qgen")
    #define QGEN_LOCALEFILE wxT("qgen_%s")
    #define QGEN_DEFKEYWORDSFILE wxT("keywords.xml")
    #define QGEN_KEYWORDSFILE wxT("keywords_%s.xml")

    class Utils
    {
    public:
        static wxString GetDocumentPath(const wxString &path = wxEmptyString, const wxString &file = wxEmptyString);
        static wxString GetAppPath(const wxString &path = wxEmptyString, const wxString &file = wxEmptyString);
        static wxString GetResourcePath(const wxString &path = wxEmptyString, const wxString &file = wxEmptyString);
        static wxString GetConfigPath(const wxString &path = wxEmptyString, const wxString &file = wxEmptyString);
    };

#endif
