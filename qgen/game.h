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

#ifndef _QGEN_GAME_H_
    #define _QGEN_GAME_H_

    #include <wx/wx.h>
    #include <wx/xml/xml.h>
    #include <wx/mstream.h>
    #include "datacontainer.h"
    #include "utils.h"

    #ifdef _UNICODE
        typedef wchar_t QSP_CHAR;
        #define QSP_FMT2(x) L##x
        #define QSP_FMT(x) QSP_FMT2(x)
    #else
        typedef char QSP_CHAR;
        #define QSP_FMT(x) x
    #endif

    #ifdef _UNICODE
        #define QSP_STRCPY wcscpy
        #define QSP_STRNCPY wcsncpy
        #define QSP_STRLEN wcslen
        #define QSP_STRSTR wcsstr
        #define QSP_STRCHR wcschr
        #define QSP_STRTOL wcstol
        #define QSP_STRCMP wcscmp
        #define QSP_STRPBRK wcspbrk
        #define QSP_ISDIGIT iswdigit
        #define QSP_CHRLWR towlower
        #define QSP_CHRUPR towupper
        #define QSP_STRCOLL wcscmp
        #define QSP_MBTOSB(a) ((a) % 256)
        #define QSP_FROM_OS_CHAR(a) qspReverseConvertUC(a, qspCP1251ToUnicodeTable)
        #define QSP_TO_OS_CHAR(a) qspDirectConvertUC(a, qspCP1251ToUnicodeTable)
        #define QSP_WCTOB
        #define QSP_BTOWC
    #else
        #define QSP_STRCPY strcpy
        #define QSP_STRNCPY strncpy
        #define QSP_STRLEN strlen
        #define QSP_STRSTR strstr
        #define QSP_STRCHR strchr
        #define QSP_STRTOL strtol
        #define QSP_STRCMP strcmp
        #define QSP_STRPBRK strpbrk
        #define QSP_ISDIGIT isdigit
        #define QSP_MBTOSB
        #ifdef _WINDOWS
            #define QSP_FROM_OS_CHAR
            #define QSP_TO_OS_CHAR
            #define QSP_WCTOB(a) qspReverseConvertUC(a, qspCP1251ToUnicodeTable)
            #define QSP_BTOWC(a) qspDirectConvertUC(a, qspCP1251ToUnicodeTable)
            #define QSP_CHRLWR(a) qspCP1251ToLowerTable[(unsigned char)(a)]
            #define QSP_CHRUPR(a) qspCP1251ToUpperTable[(unsigned char)(a)]
            #define QSP_STRCOLL(a, b) qspStrCmpSB(a, b, qspCP1251OrderTable)
        #else
            #define QSP_FROM_OS_CHAR(a) qspReverseConvertSB(a, qspCP1251ToKOI8RTable)
            #define QSP_TO_OS_CHAR(a) qspDirectConvertSB(a, qspCP1251ToKOI8RTable)
            #define QSP_WCTOB(a) qspReverseConvertUC(a, qspKOI8RToUnicodeTable)
            #define QSP_BTOWC(a) qspDirectConvertUC(a, qspKOI8RToUnicodeTable)
            #define QSP_CHRLWR(a) qspKOI8RToLowerTable[(unsigned char)(a)]
            #define QSP_CHRUPR(a) qspKOI8RToUpperTable[(unsigned char)(a)]
            #define QSP_STRCOLL(a, b) qspStrCmpSB(a, b, qspKOI8ROrderTable)
        #endif
    #endif

    #define QSP_LEN(x) (sizeof(x) / sizeof(QSP_CHAR) - 1)

    #define QSP_CODREMOV 5
    #define QSP_SPACES QSP_FMT(" \t")
    #define QSP_STRSDELIM QSP_FMT("\r\n")
    #define QSP_DELIMS QSP_FMT("():;=+-*/<>!&,[] \t'\"{}^.?~\\|\r\n")

    #define QSP_GAMEID QSP_FMT("QSPGAME")
    #define QGEN_PASSWD QSP_FMT("No")
    #define QGEN_MAXACTIONS 50
    #define QGEN_MAXFOLDERNAMELEN 100
    #define QGEN_MAXLOCATIONNAMELEN 100
    #define QGEN_MAXACTIONNAMELEN 500
    #define QGEN_BOM "\xFF\xFE"

    #define wxGetBitmapFromMemory(name) _wxGetBitmapFromMemory(name ## _png, sizeof(name ## _png))
        inline wxBitmap _wxGetBitmapFromMemory(const unsigned char *data, int length)
        {
            wxMemoryInputStream is(data, length);
            return wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1);
        }

    class Controls;

    bool qspOpenQuest(char *buf, size_t bufSize, wxWindow *parent, Controls *controls, wxString &password, bool merge);
    long qspSaveQuest(const wxString &passwd, Controls *controls, char **buf);
    long qspExportTxt(Controls *controls, char **buf);
    long qspExportTxt2Gam(const wxString &fileName, Controls *controls);
    bool qspImportTxt2Gam(const wxString &fileName, Controls *controls);

    bool OpenConfigFile(DataContainer *container, const wxString &file);
    bool SaveConfigFile(DataContainer *container, const wxString &file);

#endif
