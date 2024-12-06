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

#include "game.h"
#include "controls.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

wchar_t qspCP1251ToUnicodeTable[] =
{
    0x0402, 0x0403, 0x201A, 0x0453, 0x201E, 0x2026, 0x2020, 0x2021,
    0x20AC, 0x2030, 0x0409, 0x2039, 0x040A, 0x040C, 0x040B, 0x040F,
    0x0452, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
    0x0020, 0x2122, 0x0459, 0x203A, 0x045A, 0x045C, 0x045B, 0x045F,
    0x00A0, 0x040E, 0x045E, 0x0408, 0x00A4, 0x0490, 0x00A6, 0x00A7,
    0x0401, 0x00A9, 0x0404, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x0407,
    0x00B0, 0x00B1, 0x0406, 0x0456, 0x0491, 0x00B5, 0x00B6, 0x00B7,
    0x0451, 0x2116, 0x0454, 0x00BB, 0x0458, 0x0405, 0x0455, 0x0457,
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,
    0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,
    0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,
    0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,
    0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F
};

wchar_t qspDirectConvertUC(char ch, wchar_t *table)
{
    unsigned char ch2 = (unsigned char)ch;
    return (ch2 >= 0x80 ? table[ch2 - 0x80] : ch);
}

char qspReverseConvertUC(wchar_t ch, wchar_t *table)
{
    long i;
    if (ch < 0x80) return (char)ch;
    for (i = 127; i >= 0; --i)
        if (table[i] == ch) return (char)(i + 0x80);
    return 0x20;
}

void qspFreeStrs(char **strs, long count, bool isVerify)
{
    if (strs)
    {
        if (isVerify)
        {
            while (--count >= 0)
                if (strs[count]) free(strs[count]);
        }
        else
            while (--count >= 0) free(strs[count]);
        free(strs);
    }
}

bool qspIsInList(QSP_CHAR *list, QSP_CHAR ch)
{
    while (*list)
        if (*list++ == ch) return true;
    return false;
}

QSP_CHAR *qspSkipSpaces(QSP_CHAR *s)
{
    while (qspIsInList(QSP_SPACES, *s)) ++s;
    return s;
}

long qspStrToNum(QSP_CHAR *s, QSP_CHAR **endChar)
{
    long num;
    s = qspSkipSpaces(s);
    num = QSP_STRTOL(s, endChar, 10);
    if (endChar)
    {
        *endChar = qspSkipSpaces(*endChar);
        if (**endChar) return 0;
    }
    return num;
}

long qspUCS2StrLen(char *str)
{
    unsigned short *ptr = (unsigned short *)str;
    while (*ptr) ++ptr;
    return (long)(ptr - (unsigned short *)str);
}

char *qspUCS2StrStr(char *str, char *subStr)
{
    unsigned short *s1, *s2, *cp = (unsigned short *)str;
    while (*cp)
    {
        s1 = cp;
        s2 = (unsigned short *)subStr;
        while (*s1 && *s2 && !(*s1 - *s2))
            ++s1, ++s2;
        if (!(*s2)) return (char *)cp;
        ++cp;
    }
    return 0;
}

char *qspQSPToGameString(const QSP_CHAR *s, bool isUCS2, bool isCode)
{
    unsigned short uCh, *ptr;
    long len = (long)QSP_STRLEN(s);
    char ch, *ret = (char *)malloc((len + 1) * (isUCS2 ? 2 : 1));
    if (isUCS2)
    {
        ptr = (unsigned short *)ret;
        ptr[len] = 0;
        if (isCode)
        {
            while (--len >= 0)
            {
                uCh = QSP_BTOWC(s[len]);
                if (uCh == QSP_CODREMOV)
                    uCh = (unsigned short)-QSP_CODREMOV;
                else
                    uCh -= QSP_CODREMOV;
                ptr[len] = uCh;
            }
        }
        else
        {
            while (--len >= 0)
                ptr[len] = QSP_BTOWC(s[len]);
        }
    }
    else
    {
        ret[len] = 0;
        if (isCode)
        {
            while (--len >= 0)
            {
                ch = QSP_FROM_OS_CHAR(s[len]);
                if (ch == QSP_CODREMOV)
                    ch = -QSP_CODREMOV;
                else
                    ch -= QSP_CODREMOV;
                ret[len] = ch;
            }
        }
        else
        {
            while (--len >= 0)
                ret[len] = QSP_FROM_OS_CHAR(s[len]);
        }
    }
    return ret;
}

QSP_CHAR *qspGameToQSPString(char *s, bool isUCS2, bool isCoded)
{
    char ch;
    unsigned short uCh, *ptr;
    long len = (isUCS2 ? qspUCS2StrLen(s) : (long)strlen(s));
    QSP_CHAR *ret = (QSP_CHAR *)malloc((len + 1) * sizeof(QSP_CHAR));
    ret[len] = 0;
    if (isUCS2)
    {
        ptr = (unsigned short *)s;
        if (isCoded)
        {
            while (--len >= 0)
            {
                uCh = ptr[len];
                if (uCh == (unsigned short)-QSP_CODREMOV)
                    uCh = QSP_CODREMOV;
                else
                    uCh += QSP_CODREMOV;
                ret[len] = QSP_WCTOB(uCh);
            }
        }
        else
        {
            while (--len >= 0)
                ret[len] = QSP_WCTOB(ptr[len]);
        }
    }
    else
    {
        if (isCoded)
        {
            while (--len >= 0)
            {
                ch = s[len];
                if (ch == -QSP_CODREMOV)
                    ch = QSP_CODREMOV;
                else
                    ch += QSP_CODREMOV;
                ret[len] = QSP_TO_OS_CHAR(ch);
            }
        }
        else
        {
            while (--len >= 0)
                ret[len] = QSP_TO_OS_CHAR(s[len]);
        }
    }
    return ret;
}

long qspSplitGameStr(char *str, bool isUCS2, QSP_CHAR *delim, char ***res)
{
    char *delimStr, *newStr, **ret, *found, *curPos = str;
    long charSize, delimSize, allocChars, count = 0, bufSize = 8;
    charSize = (isUCS2 ? 2 : 1);
    delimSize = (long)QSP_STRLEN(delim) * charSize;
    delimStr = qspQSPToGameString(delim, isUCS2, false);
    found = (isUCS2 ? qspUCS2StrStr(str, delimStr) : strstr(str, delimStr));
    ret = (char **)malloc(bufSize * sizeof(char *));
    while (found)
    {
        allocChars = (long)(found - curPos);
        newStr = (char *)malloc(allocChars + charSize);
        memcpy(newStr, curPos, allocChars);
        if (isUCS2)
            *((unsigned short *)(newStr + allocChars)) = 0;
        else
            newStr[allocChars] = 0;
        if (++count > bufSize)
        {
            bufSize <<= 1;
            ret = (char **)realloc(ret, bufSize * sizeof(char *));
        }
        ret[count - 1] = newStr;
        curPos = found + delimSize;
        found = (isUCS2 ? qspUCS2StrStr(curPos, delimStr) : strstr(curPos, delimStr));
    }
    free(delimStr);
    allocChars = (isUCS2 ? (qspUCS2StrLen(curPos) + 1) * charSize : (long)strlen(curPos) + 1);
    newStr = (char *)malloc(allocChars);
    memcpy(newStr, curPos, allocChars);
    if (++count > bufSize)
        ret = (char **)realloc(ret, count * sizeof(char *));
    ret[count - 1] = newStr;
    *res = ret;
    return count;
}

bool qspCheckQuest(char **strs, long count, bool isUCS2)
{
    long i, ind, locsCount, actsCount;
    bool isOldFormat;
    QSP_CHAR *data = qspGameToQSPString(strs[0], isUCS2, false);
    isOldFormat = QSP_STRCMP(data, QSP_GAMEID) != 0;
    free(data);
    ind = (isOldFormat ? 30 : 4);
    if (ind > count) return false;
    data = (isOldFormat ?
        qspGameToQSPString(strs[0], isUCS2, false) : qspGameToQSPString(strs[3], isUCS2, true));
    locsCount = qspStrToNum(data, 0);
    free(data);
    if (locsCount <= 0) return false;
    for (i = 0; i < locsCount; ++i)
    {
        if ((ind += 3) > count) return false;
        if (isOldFormat)
            actsCount = 20;
        else
        {
            if (ind + 1 > count) return false;
            data = qspGameToQSPString(strs[ind++], isUCS2, true);
            actsCount = qspStrToNum(data, 0);
            free(data);
            if (actsCount < 0 || actsCount > QGEN_MAXACTIONS) return false;
        }
        if ((ind += (actsCount * (isOldFormat ? 2 : 3))) > count) return false;
    }
    return true;
}

bool qspOpenQuest(char *buf, long bufSize, wxWindow *parent, Controls *controls, wxString &password, bool merge)
{
    bool isOldFormat, isUCS2;
    long i, j, ind, count, locsCount, actsCount;
    QSP_CHAR *data;
    wxString temp;
    char **strs;
    if (bufSize < QSP_LEN(QSP_STRSDELIM)) return false;
    buf[bufSize - 1] = buf[bufSize - 2] = 0;
    count = qspSplitGameStr(buf, isUCS2 = !buf[1], QSP_STRSDELIM, &strs);
    if (!qspCheckQuest(strs, count, isUCS2))
    {
        qspFreeStrs(strs, count, false);
        return false;
    }
    data = qspGameToQSPString(strs[0], isUCS2, false);
    isOldFormat = QSP_STRCMP(data, QSP_GAMEID) != 0;
    free(data);

    data = qspGameToQSPString(isOldFormat ? strs[1] : strs[2], isUCS2, true);
    if (QSP_STRCMP(data, QGEN_PASSWD))
    {
        wxPasswordEntryDialog dlgEntry(parent,
            _("Input password:"),
            _("Game password"), wxEmptyString);
        if (dlgEntry.ShowModal() == wxID_OK)
        {
            if (dlgEntry.GetValue() != data)
            {
                free(data);
                qspFreeStrs(strs, count, false);
                controls->ShowMessage(QGEN_MSG_WRONGPASSWORD);
                return false;
            }
        }
        else
        {
            free(data);
            qspFreeStrs(strs, count, false);
            return false;
        }
    }
    DataContainer *container = controls->GetContainer();
    if (!merge)
    {
        container->Clear();
        password = data;
    }
    free(data);
    data = (isOldFormat ? qspGameToQSPString(strs[0], isUCS2, false) : qspGameToQSPString(strs[3], isUCS2, true));
    locsCount = qspStrToNum(data, 0);
    free(data);
    ind = (isOldFormat ? 30 : 4);

    int locIndex, actIndex;
    int mergeType = 0;
    bool canAddLoc = true;
    for (i = 0; i < locsCount; ++i)
    {
        locIndex = -1;
        data = qspGameToQSPString(strs[ind++], isUCS2, true);
        if (merge)
        {
            locIndex = container->FindLocationIndex(data);
            if (locIndex >= 0)
            {
                if (!(mergeType & MERGE_ALL))
                {
                    MergeDialog dialog(parent, _("Replace location"),
                        wxString::Format(_("Location with the same name already exists!\nLocation: \"%s\"\nReplace existing location?"), data));
                    mergeType = dialog.ShowModal();
                    if (mergeType & MERGE_CANCEL)
                    {
                        free(data);
                        qspFreeStrs(strs, count, false);
                        return true;
                    }
                }
                if (mergeType & MERGE_REPLACE) container->ClearLocation(locIndex);
                canAddLoc = !(mergeType & MERGE_SKIP);
            }
        }
        if (locIndex < 0)
        {
            locIndex = container->AddLocation(data);
            canAddLoc = true;
        }
        free(data);
        if (locIndex < 0)
        {
            container->Clear();
            qspFreeStrs(strs, count, false);
            return false;
        }
        data = qspGameToQSPString(strs[ind++], isUCS2, true);
        (temp = data).Replace(QSP_STRSDELIM, wxT("\n"));
        free(data);

        if (canAddLoc) container->SetLocationDesc(locIndex, temp);

        data = qspGameToQSPString(strs[ind++], isUCS2, true);
        (temp = data).Replace(QSP_STRSDELIM, wxT("\n"));
        free(data);

        if (canAddLoc) container->SetLocationCode(locIndex, temp);

        if (isOldFormat)
            actsCount = 20;
        else
        {
            data = qspGameToQSPString(strs[ind++], isUCS2, true);
            actsCount = qspStrToNum(data, 0);
            free(data);
        }

        wxString nameAct, actImage;
        for (j = 0; j < actsCount; ++j)
        {
            if (!isOldFormat)
            {
                data = qspGameToQSPString(strs[ind++], isUCS2, true);
                (actImage = data).Replace(QSP_STRSDELIM, wxT("\n"));
                free(data);
            }
            data = qspGameToQSPString(strs[ind++], isUCS2, true);
            (nameAct = data).Replace(QSP_STRSDELIM, wxT("\n"));
            free(data);
            if (!nameAct.empty() && canAddLoc)
            {
                actIndex = container->AddAction(locIndex, nameAct);
                if (actIndex < 0)
                {
                    container->Clear();
                    qspFreeStrs(strs, count, false);
                    return true;
                }
                container->SetActionPicturePath(locIndex, actIndex, actImage);
                data = qspGameToQSPString(strs[ind], isUCS2, true);
                (temp = data).Replace(QSP_STRSDELIM, wxT("\n"));
                free(data);
                container->SetActionCode(locIndex, actIndex, temp);
            }
            ++ind;
        }
    }
    qspFreeStrs(strs, count, false);
    return true;
}

long qspAddGameText(char **dest, char *val, bool isUCS2, long destLen, long valLen, bool isCreate)
{
    char *destPtr;
    unsigned short *destUCS2, *valUCS2;
    long ret, charSize = (isUCS2 ? 2 : 1);
    if (valLen < 0) valLen = (isUCS2 ? qspUCS2StrLen(val) : (long)strlen(val));
    if (!isCreate && *dest)
    {
        if (destLen < 0) destLen = (isUCS2 ? qspUCS2StrLen(*dest) : (long)strlen(*dest));
        ret = destLen + valLen;
        destPtr = (char *)realloc(*dest, (ret + 1) * charSize);
        *dest = destPtr;
        destPtr += destLen * charSize;
    }
    else
    {
        ret = valLen;
        destPtr = (char *)malloc((ret + 1) * charSize);
        *dest = destPtr;
    }
    if (isUCS2)
    {
        valUCS2 = (unsigned short *)val;
        destUCS2 = (unsigned short *)destPtr;
        while (valLen && (*destUCS2++ = *valUCS2++)) --valLen;
        *destUCS2 = 0;
    }
    else
    {
        strncpy(destPtr, val, valLen);
        destPtr[valLen] = 0;
    }
    return ret;
}

QSP_CHAR *qspNumToStr(QSP_CHAR *buf, long val)
{
    QSP_CHAR temp, *str = buf, *first = str;
    if (val < 0)
    {
        *str++ = QSP_FMT('-');
        val = -val;
        ++first;
    }
    do
    {
        *str++ = (QSP_CHAR)(val % 10 + QSP_FMT('0'));
        val /= 10;
    } while (val > 0);
    *str-- = 0;
    while (first < str)
    {
        temp = *str;
        *str = *first;
        *first = temp;
        --str;
        ++first;
    }
    return buf;
}

long qspGameCodeWriteIntVal(char **s, long len, long val, bool isUCS2, bool isCode)
{
    char *temp;
    QSP_CHAR buf[12];
    qspNumToStr(buf, val);
    temp = qspQSPToGameString(buf, isUCS2, isCode);
    len = qspAddGameText(s, temp, isUCS2, len, -1, false);
    free(temp);
    temp = qspQSPToGameString(QSP_STRSDELIM, isUCS2, false);
    len = qspAddGameText(s, temp, isUCS2, len, QSP_LEN(QSP_STRSDELIM), false);
    free(temp);
    return len;
}

long qspGameCodeWriteVal(char **s, long len, wxString &val, bool isUCS2, bool isCode)
{
    char *temp;
    if (!val.IsEmpty())
    {
        val.Replace(wxT("\n"), QSP_STRSDELIM);
        temp = qspQSPToGameString(val.wx_str(), isUCS2, isCode);
        len = qspAddGameText(s, temp, isUCS2, len, -1, false);
        free(temp);
    }
    temp = qspQSPToGameString(QSP_STRSDELIM, isUCS2, false);
    len = qspAddGameText(s, temp, isUCS2, len, QSP_LEN(QSP_STRSDELIM), false);
    free(temp);
    return len;
}

long qspSaveQuest(const wxString &passwd, Controls *controls, char **buf)
{
    long i, j, len, locsCount, actsCount;
    wxString str, currentDate = wxDateTime::Now().FormatISODate();
    DataContainer *container = controls->GetContainer();
    locsCount = container->GetLocationsCount();
    *buf = 0;
    str = wxString(QSP_GAMEID);
    len = qspGameCodeWriteVal(buf, 0, str, true, false);
    str = wxString::Format(wxT("%s (%s %s)"), currentDate.wx_str(), QGEN_APPNAME, QGEN_VER);
    len = qspGameCodeWriteVal(buf, len, str, true, false);
    str = wxString(passwd);
    len = qspGameCodeWriteVal(buf, len, str, true, true);
    len = qspGameCodeWriteIntVal(buf, len, locsCount, true, true);
    for (i = 0; i < locsCount; ++i)
    {
        str = container->GetLocationName(i);
        len = qspGameCodeWriteVal(buf, len, str, true, true);
        str = container->GetLocationDesc(i);
        len = qspGameCodeWriteVal(buf, len, str, true, true);
        str = container->GetLocationCode(i);
        len = qspGameCodeWriteVal(buf, len, str, true, true);
        actsCount = container->GetActionsCount(i);
        len = qspGameCodeWriteIntVal(buf, len, actsCount, true, true);
        for (j = 0; j < actsCount; ++j)
        {
            str = container->GetActionPicturePath(i, j);
            len = qspGameCodeWriteVal(buf, len, str, true, true);
            str = container->GetActionName(i, j);
            len = qspGameCodeWriteVal(buf, len, str, true, true);
            str = container->GetActionCode(i, j);
            len = qspGameCodeWriteVal(buf, len, str, true, true);
        }
    }
    return len * 2;
}

long qspExportTxt(Controls *controls, char **buf)
{
    DataContainer *container = controls->GetContainer();
    wxString str, actPictPath;
    wxArrayString lines;
    long len = 0;
    *buf = 0;
    for (size_t idxLoc = 0; idxLoc < container->GetLocationsCount(); ++idxLoc)
    {
        str = wxString::Format(_("Location: \"%s\""), container->GetLocationName(idxLoc).wx_str());
        len = qspGameCodeWriteVal(buf, len, str, true, false);
        str = wxString(wxT("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
        len = qspGameCodeWriteVal(buf, len, str, true, false);
        str = container->GetLocationDesc(idxLoc);
        if (str.Length())
        {
            lines = wxSplit(str, '\n');
            str = wxString(_("Location description:\n"));
            len = qspGameCodeWriteVal(buf, len, str, true, false);
            for (size_t i = 0; i < lines.GetCount(); ++i)
            {
                str = wxString::Format(wxT("\t%s"), lines[i].wx_str());
                len = qspGameCodeWriteVal(buf, len, str, true, false);
            }
            str = wxEmptyString;
            len = qspGameCodeWriteVal(buf, len, str, true, false);
        }
        if (container->GetActionsCount(idxLoc))
        {
            str = wxString(_("Location actions:\n"));
            len = qspGameCodeWriteVal(buf, len, str, true, false);
            for (size_t idxAct = 0; idxAct < container->GetActionsCount(idxLoc); ++idxAct)
            {
                str = wxString::Format(wxT("\t%s:"), container->GetActionName(idxLoc, idxAct).wx_str());
                len = qspGameCodeWriteVal(buf, len, str, true, false);
                actPictPath = container->GetActionPicturePath(idxLoc, idxAct);
                if (actPictPath.Length())
                {
                    str = wxString::Format(_("\tAction image: %s"), actPictPath);
                    len = qspGameCodeWriteVal(buf, len, str, true, false);
                }
                lines = wxSplit(container->GetActionCode(idxLoc, idxAct), '\n');
                for (size_t i = 0; i < lines.GetCount(); ++i)
                {
                    str = wxString::Format(wxT("\t\t%s"), lines[i].wx_str());
                    len = qspGameCodeWriteVal(buf, len, str, true, false);
                }
            }
            str = wxEmptyString;
            len = qspGameCodeWriteVal(buf, len, str, true, false);
        }
        str = container->GetLocationCode(idxLoc);
        if (str.Length())
        {
            lines = wxSplit(str, '\n');
            str = wxString(_("Location code:\n"));
            len = qspGameCodeWriteVal(buf, len, str, true, false);
            for (size_t i = 0; i < lines.GetCount(); ++i)
            {
                str = wxString::Format(wxT("\t%s"), lines[i].wx_str());
                len = qspGameCodeWriteVal(buf, len, str, true, false);
            }
            str = wxEmptyString;
            len = qspGameCodeWriteVal(buf, len, str, true, false);
        }
        str = wxString::Format(_("------------ End of location: \"%s\" ------------\n"),
                container->GetLocationName(idxLoc).wx_str());
        len = qspGameCodeWriteVal(buf, len, str, true, false);
    }
    return len * 2;
}

long qspExportTxt2Gam(const wxString &fileName, Controls *controls)
{
    wxString txt2gamPath = controls->GetSettings()->GetCurrentTxt2GamPath();
    wxString gamePath = controls->GetGamePath();
    wxString password = controls->GetGamePass();
    wxString commandLine = wxString::Format(wxT("\"%s\" \"%s\" \"%s\" \"p%s\" d"),
            txt2gamPath.wx_str(),
            gamePath.wx_str(),
            fileName.wx_str(),
            password.wx_str());

    if (!wxExecute(commandLine, wxEXEC_SYNC))
        return true;
    return false;
}

bool qspImportTxt2Gam(const wxString &fileName, Controls *controls)
{
    wxString txt2gamPath = controls->GetSettings()->GetCurrentTxt2GamPath();
    wxString gamePath = controls->GetGamePath();
    wxString commandLine = wxString::Format(wxT("\"%s\" \"%s\" \"%s\" c u"),
            txt2gamPath.wx_str(),
            fileName.wx_str(),
            gamePath.wx_str());

    if (!wxExecute(commandLine, wxEXEC_SYNC))
        return true;
    return false;
}

bool ParseConfigFile(DataContainer *container, wxXmlNode *node, long folder, long *locPos, long *folderPos, long *pos)
{
    wxString folderName;
    node = node->GetChildren();
    while (node)
    {
        if (node->GetName() == wxT("Folder"))
        {
            folderName = node->GetAttribute(wxT("name"));
            long curInd = container->FindFolderIndex(folderName);
            if (curInd < 0)
            {
                ++(*pos);
                ++(*folderPos);
                curInd = container->GetFoldersCount();
                container->AddFolder(folderName);
                container->SetFolderPos(curInd, *pos);
                container->MoveFolder(curInd, *folderPos);
                if (!ParseConfigFile(container, node, *folderPos, locPos, folderPos, pos))
                    return false;
            }
        }
        else if (node->GetName() == wxT("Location"))
        {
            long curInd = container->FindLocationIndex(node->GetAttribute(wxT("name")));
            if (curInd >= 0)
            {
                ++(*pos);
                ++(*locPos);
                container->SetLocFolder(curInd, folder);
                container->MoveLocationTo(curInd, *locPos);
            }
        }
        node = node->GetNext();
    }
    return true;
}

bool OpenConfigFile(DataContainer *container, const wxString &file)
{
    wxXmlDocument doc;
    if (!(wxFileExists(file) && doc.Load(file))) return false;
    wxXmlNode *root = doc.GetRoot();
    if (root == NULL || root->GetName() != wxT("QGen-project")) return false;
    wxXmlNode *structure = root->GetChildren();
    if (structure == NULL) return false;
    long locPos = -1, folderPos = -1, pos = -1;
    while (structure)
    {
        if (structure->GetName() == wxT("Structure"))
        {
            return ParseConfigFile(container, structure, -1, &locPos, &folderPos, &pos);
        }
        structure = structure->GetNext();
    }
    return false;
}

bool SaveConfigFile(DataContainer *container, const wxString &file)
{
    // Now config stores only folders structure that's why we can simply skip
    // saving file if there are no folders
    if (!container->GetFoldersCount())
    {
        // We must remove old file if such exists
        if (wxFileExists(file)) wxRemoveFile(file);
        return true;
    }
    wxXmlDocument doc;
    doc.SetVersion(wxT("1.0"));
    doc.SetFileEncoding(wxT("utf-8"));
    wxXmlNode *node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("QGen-project"));
    node->AddAttribute(wxT("version"), QGEN_VER);
    doc.SetRoot(node);
    node = new wxXmlNode(node, wxXML_ELEMENT_NODE, wxT("Structure"));
    size_t locsCount = container->GetLocationsCount();
    wxArrayInt locs;
    long oldPos = -1, pos = 0, folderIndex;
    wxXmlNode *structure = node;
    while (pos != oldPos)
    {
        oldPos = pos;
        folderIndex = container->FindFolderForPos(pos);
        if (folderIndex >= 0)
        {
            node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Folder"));
            node->AddAttribute(wxT("name"), container->GetFolderName(folderIndex));
            structure->AddChild(node);
            ++pos;
        }
        else
            node = structure;
        if (locs.GetCount() < locsCount)
        {
            for (size_t i = 0; i < locsCount; ++i)
            {
                if (locs.Index(i) < 0 && container->GetLocFolder(i) == folderIndex)
                {
                    if (folderIndex < 0)
                    {
                        if (container->FindFolderForPos(pos) >= 0)
                            break;
                    }
                    wxXmlNode *temp = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("Location"));
                    temp->AddAttribute(wxT("name"), container->GetLocationName(i));
                    node->AddChild(temp);
                    locs.Add(i);
                    ++pos;
                }
            }
        }
    }
    return doc.Save(file);
}
