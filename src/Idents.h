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
#ifndef _ID_MENU_H
	#define _ID_MENU_H

	enum
	{
		ID_TOGGLE_TOOLBAR = 10001,
		ID_TOGGLE_STATUSBAR,
		ID_TOGGLE_LISTBOX,
		NEW_QUEST,
		OPEN_QUEST,
		JOIN_QUEST,
		SAVE_QUEST,
		SAVE_QUESTAS,
		EXPORT_QUEST_TXT,
		EXPORT_QUEST_TXT2GAM,
		IMPORT_QUEST,
		PLAY_QUEST,
		EXIT_QGEN,
		UTIL_FIND,
		UTIL_INF,
		UTIL_OPTIONS,
		LOC_CREAT,
		LOC_RENAME,
		LOC_DEL,
		LOC_COPY,
		LOC_REPLACE,
		LOC_PASTE,
		LOC_PASTE_NEW,
		LOC_CLEAR,
		LOC_SORT_ASC,
		LOC_SORT_DESC,
		LOC_JUMP_LOC,
		LOC_EXPAND,
		LOC_COLLAPSE,
		CREATE_ACTION,
		RENAME_ACTION,
		DEL_ACTION,
		DEL_ALL_ACTIONS,
		HELP_HELP,
		HELP_SEARCH_INDEX,
		HELP_ABOUT,
		UNDO_TEXT,
		REDO_TEXT,
		CUT_TEXT,
		COPY_TEXT,
		PASTE_TEXT,
		DEL_TEXT,
		SELALL_TEXT,
		LIST_WINDOWS,
		SELECT_LOCATION,
		SUB_MENU_ACT,
		OPEN_PICT,
		FIND_TEXT,
		REPL_TEXT,
		FIND_NEXT,
		FIND_REPL,
		FIND_REPLALL,
		FIND_ANEW,
		FIND_SKIPLOC,
		ID_MENUCLOSEALLTABS,
		ID_MENUCLOSEEXCEPTSELECTED,
		ID_MENUCLOSESELECTED,
		ID_LOCDESCVISIBLE,
		ID_LOCACTVISIBLE
	};

	enum
	{
		QGEN_MSG_EXISTS =100,
		QGEN_MSG_EXISTS_HKEY,
		QGEN_MSG_EMPTYDATA,
		QGEN_MSG_WRONGPASSWORD,
		QGEN_MSG_CANTSAVEGAME,
		QGEN_MSG_NOTFOUND,
		QGEN_MSG_SEARCHENDED,
		QGEN_MSG_WRONGFORMAT,
		QGEN_MSG_MAXACTIONSCOUNTREACHED,
		QGEN_MSG_TOOLONGLOCATIONNAME,
		QGEN_MSG_TOOLONGACTIONNAME
	};

	#ifdef _UNICODE
		typedef wchar_t QGEN_CHAR;
		#define QGEN_FMT2(x) L##x
		#define QGEN_FMT(x) QGEN_FMT2(x)
	#else
		typedef char QGEN_CHAR;
		#define QGEN_FMT(x) x
	#endif

	#ifdef _UNICODE
		#define QGEN_STRCPY wcscpy
		#define QGEN_STRNCPY wcsncpy
		#define QGEN_STRLEN wcslen
		#define QGEN_STRSTR wcsstr
		#define QGEN_STRCHR wcschr
		#define QGEN_STRTOL wcstol
		#define QGEN_CHRLWR towlower
		#define QGEN_CHRUPR towupper
		#define QGEN_STRCMP wcscmp
		#define QGEN_STRCOLL wcscmp
		#define QGEN_STRPBRK wcspbrk
		#define QGEN_ISDIGIT iswdigit
		#define QGEN_WCSTOMBSLEN(a) wcstombs(0, a, 0)
		#define QGEN_WCSTOMBS wcstombs
		#define QGEN_MBTOSB(a) ((a) % 256)
		#define QGEN_FROM_OS_CHAR(a) qspReverseConvertUC(a, qspCP1251ToUnicodeTable)
		#define QGEN_TO_OS_CHAR(a) qspDirectConvertUC(a, qspCP1251ToUnicodeTable)
		#define QGEN_WCTOB
		#define QGEN_BTOWC
	#else
		#define QGEN_STRCPY strcpy
		#define QGEN_STRNCPY strncpy
		#define QGEN_STRLEN strlen
		#define QGEN_STRSTR strstr
		#define QGEN_STRCHR strchr
		#define QGEN_STRTOL strtol
		#define QGEN_STRCMP strcmp
		#define QGEN_STRPBRK strpbrk
		#define QGEN_ISDIGIT isdigit
		#define QGEN_WCSTOMBSLEN strlen
		#define QGEN_WCSTOMBS strncpy
		#define QGEN_MBTOSB
		#ifdef _WINDOWS
			#define QGEN_FROM_OS_CHAR
			#define QGEN_TO_OS_CHAR
			#define QGEN_WCTOB(a) qspReverseConvertUC(a, qspCP1251ToUnicodeTable)
			#define QGEN_BTOWC(a) qspDirectConvertUC(a, qspCP1251ToUnicodeTable)
			#define QGEN_CHRLWR(a) qspCP1251ToLowerTable[(unsigned char)(a)]
			#define QGEN_CHRUPR(a) qspCP1251ToUpperTable[(unsigned char)(a)]
			#define QGEN_STRCOLL(a, b) qspStrCmpSB(a, b, qspCP1251OrderTable)
		#else
			#define QGEN_FROM_OS_CHAR(a) qspReverseConvertSB(a, qspCP1251ToKOI8RTable)
			#define QGEN_TO_OS_CHAR(a) qspDirectConvertSB(a, qspCP1251ToKOI8RTable)
			#define QGEN_WCTOB(a) qspReverseConvertUC(a, qspKOI8RToUnicodeTable)
			#define QGEN_BTOWC(a) qspDirectConvertUC(a, qspKOI8RToUnicodeTable)
			#define QGEN_CHRLWR(a) qspKOI8RToLowerTable[(unsigned char)(a)]
			#define QGEN_CHRUPR(a) qspKOI8RToUpperTable[(unsigned char)(a)]
			#define QGEN_STRCOLL(a, b) qspStrCmpSB(a, b, qspKOI8ROrderTable)
		#endif
	#endif
	#ifdef _WINDOWS
		#define QGEN_PATHDELIM QGEN_FMT("\\")
	#else
		#define QGEN_PATHDELIM QGEN_FMT("/")
	#endif

	#define QGEN_LEN(x) (sizeof(x) / sizeof(QGEN_CHAR) - 1)

	#define QGEN_CODREMOV 5
	#define QGEN_SPACES QGEN_FMT(" \t")
	#define QGEN_STRSDELIM QGEN_FMT("\r\n")
	#define QGEN_GAMEID QGEN_FMT("QSPGAME")
	#define QGEN_VER QGEN_FMT("4.0.0 alpha 13")
	#define QGEN_NAME QGEN_FMT("QGEN")
	#define QGEN_PASSWD QGEN_FMT("No")
	#define QGEN_MAXACTIONS 50
	#define QGEN_MAXLOCATIONNAMELEN 100
	#define QGEN_MAXACTIONNAMELEN 500
	#define QGEN_DELIMS QGEN_FMT("():=+-*/<>!&,[] '\"\t{}%@#^;.?~\\|\r\n")

#endif
