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

#include "SyntaxTextBox.h"

IMPLEMENT_CLASS(SyntaxTextBox, wxStyledTextCtrl)

BEGIN_EVENT_TABLE(SyntaxTextBox, wxStyledTextCtrl)
	EVT_KEY_DOWN(SyntaxTextBox::OnKeyDown)
	EVT_RIGHT_DOWN(SyntaxTextBox::OnRightClick)
	EVT_STC_MARGINCLICK(wxID_ANY, SyntaxTextBox::OnMarginClicked)
	EVT_STC_CHARADDED(wxID_ANY, SyntaxTextBox::OnCharAdded)
	EVT_MOTION(SyntaxTextBox::OnMouseMove)
END_EVENT_TABLE()

SyntaxTextBox::SyntaxTextBox(wxWindow *owner, IControls *controls, wxStatusBar *statusBar, int style) :
	wxStyledTextCtrl(owner, wxID_ANY, wxDefaultPosition, wxSize(1, 1))
{
	_controls = controls;
	_statusBar = statusBar;
	_style = style;
	SetEOLMode(wxSTC_EOL_LF);
	SetWrapMode(wxSTC_WRAP_WORD);
	SetMarginWidth(1, 0);
	if (!(_style & SYNTAX_STYLE_SIMPLEMENU))
	{
		UsePopUp(false);
	}
	if (_style & SYNTAX_STYLE_COLORED)
	{
		SetScrollWidth(-1);
		SetScrollWidthTracking(true);

		SetLexer(wxSTC_LEX_VB);

		SetKeyWords(0, wxT("clear clr nl p pl msg wait act delact del cla cmdclear cmdclr cls set let killvar dynamic copyarr add obj addobj delobj killobj unselect unsel killall menu opengame openqst addqst killqst savegame refint settimer showacts showinput showobjs showstat if else end exit jump gosub gs goto gt xgoto xgt play close all view"));
		SetKeyWords(1, wxT("and or no desc iif input $desc $iif $input isplay max min $max $min rand rgb getobj $getobj dyneval $dyneval func $func arrpos arrsize instr isnum trim $trim ucase lcase $ucase $lcase len mid str $mid $str val arrcomp strcomp strfind $strfind strpos replace $replace"));
		SetKeyWords(2, wxT("countobj nosave disablescroll disablesubex msecscount rnd debug $curloc $qspver $selobj $selact $lastobj $user_text $usrtxt $maintxt $stattxt $counter $ongload $ongsave $onnewloc $onactsel $onobjsel $onobjadd $onobjdel $usercom usehtml bcolor fcolor lcolor fsize $fname $backimage args $args result $result"));

		//	SetViewEOL(true);
		//	SetViewWhiteSpace(true);
		SetIndentationGuides(true);
		SetProperty(wxT("fold"), wxT("1"));
		//	SetProperty(wxT("fold.compact"), wxT("0"));
		//	SetProperty(wxT("fold.comment"), wxT("1"));
		SetFoldFlags(wxSTC_FOLDLEVELBASE);

		SetMarginType(SYNTAX_FOLD_MARGIN, wxSTC_MARGIN_SYMBOL);
		SetMarginMask(SYNTAX_FOLD_MARGIN, wxSTC_MASK_FOLDERS);
		SetMarginWidth(SYNTAX_FOLD_MARGIN, 20);

		SetMarginType(SYNTAX_NUM_MARGIN, wxSTC_MARGIN_NUMBER);

		MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_PLUS);
		MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_MINUS);
		MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_EMPTY);
		MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
		MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_EMPTY);
		MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
		MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);

		SetFoldFlags(wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

		SetMarginSensitive(SYNTAX_FOLD_MARGIN, true);

		AutoCompSetChooseSingle(true);
		AutoCompSetIgnoreCase(true);
		AutoCompSetDropRestOfWord(true);

		FillKeywords(wxT("CLEAR CLR NL P PL MSG WAIT ACT DELACT DEL CLA CMDCLEAR CMDCLR CLS SET LET KILLVAR DYNAMIC COPYARR ADD OBJ ADDOBJ DELOBJ KILLOBJ UNSELECT UNSEL KILLALL MENU OPENGAME OPENQST ADDQST KILLQST SAVEGAME REFINT SETTIMER SHOWACTS SHOWINPUT SHOWOBJS SHOWSTAT IF ELSE END EXIT JUMP GOSUB GS GOTO GT XGOTO XGT PLAY CLOSE ALL VIEW AND OR NO DESC IIF INPUT $DESC $IIF $INPUT ISPLAY MAX MIN $MAX $MIN RAND RGB GETOBJ $GETOBJ DYNEVAL $DYNEVAL FUNC $FUNC ARRPOS ARRSIZE INSTR ISNUM TRIM $TRIM UCASE LCASE $UCASE $LCASE LEN MID STR REPLACE $REPLACE $MID $STR VAL ARRCOMP STRCOMP STRFIND $STRFIND STRPOS COUNTOBJ NOSAVE DISABLESCROLL DISABLESUBEX MSECSCOUNT RND DEBUG $CURLOC $QSPVER $SELOBJ $SELACT $LASTOBJ $USER_TEXT $USRTXT $COUNTER $ONGLOAD $ONGSAVE $ONNEWLOC $ONACTSEL $ONOBJSEL $ONOBJADD $ONOBJDEL $USERCOM USEHTML BCOLOR FCOLOR LCOLOR FSIZE $FNAME $BACKIMAGE ARGS $ARGS RESULT $RESULT"));
	}
	Update();
	if(_statusBar)
		LoadTips();

	_controls->GetSettings()->AddObserver(this);
}

SyntaxTextBox::~SyntaxTextBox()
{
	_controls->GetSettings()->RemoveObserver(this);
}

void SyntaxTextBox::Update(bool isFromObservable)
{
	Settings *settings = _controls->GetSettings();
	wxColour backColour = settings->GetTextBackColour();

	SetCaretForeground((backColour.Blue() << 16 | backColour.Green() << 8 | backColour.Red()) ^ 0xFFFFFF);
	SetSelBackground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
	SetSelForeground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));
	StyleSetForeground(wxSTC_STYLE_DEFAULT, settings->GetColour(SYNTAX_BASE));
	StyleSetBackground(wxSTC_STYLE_DEFAULT, backColour);
	StyleSetFont(wxSTC_STYLE_DEFAULT, settings->GetFont(SYNTAX_BASE));
	SetTabWidth(settings->GetTabSize());
	StyleClearAll();

	if (_style & SYNTAX_STYLE_COLORED)
	{
		SetWrapMode(settings->GetWrapLines() ? wxSTC_WRAP_WORD : wxSTC_WRAP_NONE);
		SetFoldMarginColour(true, backColour);
		// Нумерация строк
		SetMarginWidth(SYNTAX_NUM_MARGIN, settings->GetShowLinesNums() ? 40 : 0);
		// Ключевые слова
		StyleSetFont(wxSTC_B_KEYWORD, settings->GetFont(SYNTAX_STATEMENTS));
		StyleSetForeground(wxSTC_B_KEYWORD, settings->GetColour(SYNTAX_STATEMENTS));
		StyleSetFont(wxSTC_B_KEYWORD2, settings->GetFont(SYNTAX_FUNCTIONS));
		StyleSetForeground(wxSTC_B_KEYWORD2, settings->GetColour(SYNTAX_FUNCTIONS));
		StyleSetFont(wxSTC_B_KEYWORD3, settings->GetFont(SYNTAX_SYS_VARIABLES));
		StyleSetForeground(wxSTC_B_KEYWORD3, settings->GetColour(SYNTAX_SYS_VARIABLES));
		// Строки
		StyleSetFont(wxSTC_B_STRING, settings->GetFont(SYNTAX_STRINGS));
		StyleSetForeground(wxSTC_B_STRING, settings->GetColour(SYNTAX_STRINGS));
		StyleSetFont(wxSTC_B_STRINGEOL, settings->GetFont(SYNTAX_STRINGS));
		StyleSetForeground(wxSTC_B_STRINGEOL, settings->GetColour(SYNTAX_STRINGS));
		// Числа
		StyleSetFont(wxSTC_B_NUMBER, settings->GetFont(SYNTAX_NUMBERS));
		StyleSetForeground(wxSTC_B_NUMBER, settings->GetColour(SYNTAX_NUMBERS));
		// Операции
		StyleSetFont(wxSTC_B_OPERATOR, settings->GetFont(SYNTAX_OPERATIONS));
		StyleSetForeground(wxSTC_B_OPERATOR, settings->GetColour(SYNTAX_OPERATIONS));
		// Метки
		StyleSetFont(wxSTC_B_LABEL, settings->GetFont(SYNTAX_LABELS));
		StyleSetForeground(wxSTC_B_LABEL, settings->GetColour(SYNTAX_LABELS));
		// Комментарии
		StyleSetFont(wxSTC_B_COMMENT, settings->GetFont(SYNTAX_COMMENTS));
		StyleSetForeground(wxSTC_B_COMMENT, settings->GetColour(SYNTAX_COMMENTS));
		StyleSetFont(wxSTC_B_PREPROCESSOR, settings->GetFont(SYNTAX_COMMENTS));
		StyleSetForeground(wxSTC_B_PREPROCESSOR, settings->GetColour(SYNTAX_COMMENTS));
		StyleSetFont(wxSTC_B_DATE, settings->GetFont(SYNTAX_COMMENTS));
		StyleSetForeground(wxSTC_B_DATE, settings->GetColour(SYNTAX_COMMENTS));
	}
}

bool SyntaxTextBox::StartAutoComplete()
{
	int curLineIndex = GetCurrentLine();
	wxString line = GetLine(curLineIndex);
	int caretChar = GetCharIndexFromPosition(PositionFromLine(curLineIndex), GetCurrentPos());
	int startword = caretChar - 1;
	while (startword >= 0)
		if (QGEN_STRCHR(QGEN_DELIMS, line[startword]))
			break;
		else
			--startword;
	++startword;
	wxString root = line.Mid(startword, caretChar - startword).Upper();
	if (!root.IsEmpty())
	{
		wxArrayString words;
		for (size_t i = 0; i < keywords.GetCount(); ++i)
			if (keywords[i].StartsWith(root)) words.Add(keywords[i]);
		if (words.GetCount() > 0)
		{
			AutoCompShow(root.Length(), GetArrayAsString(words));
			return true;
		}
	}
	return false;
}

void SyntaxTextBox::OnMarginClicked( wxStyledTextEvent &event )
{
	switch (event.GetMargin())
	{
	case SYNTAX_FOLD_MARGIN:
		ToggleFold(LineFromPosition(event.GetPosition()));
		break;
	}
}

void SyntaxTextBox::OnCharAdded( wxStyledTextEvent &event )
{
	if ((_style & SYNTAX_STYLE_COLORED) && event.GetKey() == '\n')
	{
		int curLine = GetCurrentLine();
		if (curLine > 0 && GetLineLength(curLine) <= 2)
		{
			wxString line = GetLine(curLine - 1);
			for (wxString::iterator i = line.begin(); i != line.end(); ++i)
				if (*i != wxT(' ') && *i != wxT('\t'))
				{
					line.erase(i, line.end());
					break;
				}
			ReplaceSelection(line);
		}
	}
}

void SyntaxTextBox::OnKeyDown( wxKeyEvent& event )
{
	if (_style & SYNTAX_STYLE_COLORED)
	{
		switch (event.GetKeyCode())
		{
		case WXK_SPACE:
			if (event.ControlDown())
			{
				StartAutoComplete();
				return;
			}
			break;
		}
	}
	if (_style & SYNTAX_STYLE_NOHOTKEYS)
		event.Skip();
	else
	{
		if (!_controls->ExecuteHotkey(event.GetKeyCode(), event.GetModifiers()))
			event.Skip();
	}
}

void SyntaxTextBox::OnRightClick(wxMouseEvent& event)
{
	if (_style & SYNTAX_STYLE_SIMPLEMENU)
		event.Skip();
	else
	{
		wxMenu menu;
		menu.Append(UNDO_TEXT, wxT("Отменить"));
		menu.Append(REDO_TEXT, wxT("Повторить"));
		menu.AppendSeparator();
		menu.Append(CUT_TEXT, wxT("Вырезать"));
		menu.Append(COPY_TEXT, wxT("Копировать"));
		menu.Append(PASTE_TEXT, wxT("Вставить"));
		menu.Append(DEL_TEXT, wxT("Удалить"));
		menu.AppendSeparator();
		menu.Append(SELALL_TEXT, wxT("Выделить всё"));
		if (_style & SYNTAX_STYLE_COLORED)
		{
			menu.AppendSeparator();
			menu.Append(LOC_JUMP_LOC, wxT("Перейти на выбранную локацию"));
		}
		_controls->UpdateMenuItems(&menu);
		PopupMenu(&menu);
	}
}

void SyntaxTextBox::SetValue( const wxString &str )
{
	SetText(str);
	SetModified(false);
}

void SyntaxTextBox::FillKeywords( const wxString &str )
{
	int cur, beg = 0;
	keywords.Clear();
	while ((cur = str.find(wxT(' '), beg)) >= 0)
	{
		keywords.Add(str.substr(beg, cur - beg));
		beg = cur + 1;
	}
	if (beg > 0) keywords.Add(str.substr(beg, str.length() - beg));
	keywords.Sort();
}

wxString SyntaxTextBox::GetArrayAsString( const wxArrayString &arr ) const
{
	wxString res;
	if (arr.GetCount() > 0)
	{
		res += arr[0];
		for (size_t i = 1; i < arr.GetCount(); ++i)
			res += wxT(" ") + arr[i];
	}
	return res;
}

int SyntaxTextBox::GetCharPosition( int startPos, int chars )
{
	while (chars--)
		startPos = PositionAfter(startPos);
	return startPos;
}

int SyntaxTextBox::GetCharIndexFromPosition( int fromPos, int pos )
{
	int index = 0;
	while (pos != fromPos)
	{
		pos = PositionBefore(pos);
		++index;
	}
	return index;
}

void SyntaxTextBox::SetSelection( long from, long to )
{
	int start = GetCharPosition(0, from);
	int end = GetCharPosition(start, to - from);
	EnsureVisibleEnforcePolicy(LineFromPosition(start));
	GotoPos(end);
	GotoPos(start);
	wxStyledTextCtrl::SetSelection(start, end);
}

void SyntaxTextBox::Replace( long from, long to, const wxString &str )
{
	int start = GetCharPosition(0, from);
	int end = GetCharPosition(start, to - from);
	EnsureVisibleEnforcePolicy(LineFromPosition(start));
	GotoPos(end);
	GotoPos(start);
	wxStyledTextCtrl::Replace(start, end, str);
}

long SyntaxTextBox::GetInsertionPoint() const
{
	return const_cast<SyntaxTextBox *>(this)->GetCharIndexFromPosition(0, GetCurrentPos());
}

wxString SyntaxTextBox::GetStringSelection() const
{
	return const_cast<SyntaxTextBox *>(this)->GetSelectedText();
}

void SyntaxTextBox::RemoveSelection()
{
	wxStyledTextCtrl::Clear();
}

void SyntaxTextBox::Clear()
{
	ClearAll();
	SetModified(false);
}

void SyntaxTextBox::Expand(int &line, bool doExpand, bool force, int visLevels, int level)
{
	int lineMaxSubord = GetLastChild(line, level & wxSTC_FOLDLEVELNUMBERMASK);
	++line;
	while (line <= lineMaxSubord)
	{
		if (force)
		{
			if (visLevels > 0)
				ShowLines(line, line);
			else
				HideLines(line, line);
		}
		else if (doExpand)
				ShowLines(line, line);
		int levelLine = level;
		if (levelLine == -1)
			levelLine = GetFoldLevel(line);
		if (levelLine & wxSTC_FOLDLEVELHEADERFLAG)
		{
			if (force)
			{
				SetFoldExpanded(line, visLevels > 1);
				Expand(line, doExpand, force, visLevels - 1);
			}
			else
			{
				if (!GetFoldExpanded(line))
					SetFoldExpanded(line, true);
				Expand(line, doExpand, force, visLevels - 1);
			}
		}
		else
			line++;
	}
}

void SyntaxTextBox::ExpandCollapseAll( bool isExpanded )
{
	int linesCount = GetLineCount();
	SetProperty(wxT("fold.all"), wxT("1"));
	wxStyledTextCtrl::Update();
	SetProperty(wxT("fold.all"), wxT("0"));
	Freeze();
	for (int line = 0; line < linesCount; line++)
	{
		int level = GetFoldLevel(line);
		if ((level & wxSTC_FOLDLEVELHEADERFLAG) &&
			(wxSTC_FOLDLEVELBASE == (level & wxSTC_FOLDLEVELNUMBERMASK)))
		{
			if (isExpanded)
			{
				SetFoldExpanded(line, true);
				Expand(line, true, false, 0, level);
				line--;
			}
			else
			{
				int lineMaxSubord = GetLastChild(line, -1);
				SetFoldExpanded(line, false);
				if (lineMaxSubord > line)
					HideLines(line + 1, lineMaxSubord);
			}
		}
	}
	Thaw();
}

void SyntaxTextBox::OnMouseMove(wxMouseEvent& evt)
{
	wxStyledTextCtrl::OnMouseMove(evt);
	if(_statusBar)
		Tip(PositionFromPoint(evt.GetPosition()));	
}

wxString SyntaxTextBox::GetWordFromPos(int pos)
{	
	wxString str;
	int beginPos, lastPos;

	int lineInd = LineFromPosition(pos);
	int realPos = GetCharIndexFromPosition(PositionFromLine(lineInd), pos);
	wxString lineStr = GetLine(lineInd);

	if (str.IsEmpty() && !lineStr.IsEmpty())
	{
		if (realPos == lineStr.Length()) --realPos;
		beginPos = realPos;
		lastPos = realPos;
		while (beginPos >= 0)
			if (QGEN_STRCHR(QGEN_DELIMS, lineStr[beginPos]))
				break;
			else
				--beginPos;
		while ((size_t)lastPos < lineStr.Length())
			if (QGEN_STRCHR(QGEN_DELIMS, lineStr[lastPos]))
				break;
			else
				++lastPos;
		if (lastPos > beginPos) str = lineStr.Mid(beginPos + 1, lastPos - beginPos - 1);
	}

	return str;
}

void SyntaxTextBox::Tip(int pos)
{
	wxString str = GetWordFromPos(pos);

	wxString tip;
	
	for (int i=0; i<110; i++)
	{
		if (!str.IsEmpty())
			if (str.Lower() == tooltips[i].word)
			{
				_statusBar->SetStatusText(tooltips[i].tip);
				break;
			} else {
				_statusBar->SetStatusText(wxEmptyString);
			}
		else
			_statusBar->SetStatusText(wxEmptyString);
	}
}

void SyntaxTextBox::LoadTips()
{
	tooltips[0].word = "pl"; tooltips[0].tip = "PL [выражение] / *PL [выражение] - вывод текста, затем переход на новую строку в дополнительном / основном окне описаний.";
	tooltips[1].word = "clear"; tooltips[1].tip = "*CLEAR / CLEAR - очистка основного окна описаний / окна пользователя.";
	tooltips[2].word = "clr"; tooltips[2].tip = "*CLR / CLR - очистка основного окна описаний / окна пользователя.";
	tooltips[3].word = "p"; tooltips[3].tip = "*P [выражение] / P [выражение] - вывод текста в основное окно описаний / окно пользователя.";
	tooltips[4].word = "nl"; tooltips[4].tip = "*NL [выражение] / NL [выражение] - переход на новую строку, затем вывод текста в основном окне описаний / окне пользователя.";
	tooltips[5].word = "msg"; tooltips[5].tip = "MSG [выражение] - вывод заданного сообщения в информационном окне.";
	tooltips[6].word = "wait"; tooltips[6].tip = "WAIT [#выражение] - остановка выполнения программы на заданное количество миллисекунд.";
	tooltips[7].word = "act"; tooltips[7].tip = "ACT [$название],[$путь к файлу изображения]:[оператор] & [оператор] & ... - добавление действия к существующим на локации.";
	tooltips[8].word = "delact"; tooltips[8].tip = "DEL ACT [$название] или DELACT [$название] - удаляет действие из списка действий на локации.";
	tooltips[9].word = "cla"; tooltips[9].tip = "CLA - очистка списка текущих действий.";
	tooltips[10].word = "cmdclear"; tooltips[10].tip = "CMDCLEAR или CMDCLR - очистка строки ввода.";
	tooltips[11].word = "cmdclr"; tooltips[11].tip = "CMDCLEAR или CMDCLR - очистка строки ввода.";
	tooltips[12].word = "cls"; tooltips[12].tip = "CLS - эквивалентен конструкции 'CLEAR & *CLEAR & CLA & CMDCLEAR', т.е. очищает всё, кроме списка предметов.";
	tooltips[13].word = "menu"; tooltips[13].tip = "MENU [$выражение] - вызов меню с заданным названием.";
	tooltips[14].word = "settimer"; tooltips[14].tip = "SETTIMER [#выражение] - задает интервал таймера для локации-счётчика.";
	tooltips[15].word = "dynamic"; tooltips[15].tip = "DYNAMIC [$строка кода] - выполнение строки кода.";
	tooltips[16].word = "set"; tooltips[16].tip = "SET [название переменной]=[выражение] - установка значения переменной.";
	tooltips[17].word = "let"; tooltips[17].tip = "LET [название переменной]=[выражение] - установка значения переменной.";
	tooltips[18].word = "killvar"; tooltips[18].tip = "KILLVAR - удаление всех переменных.";
	tooltips[19].word = "copyarr"; tooltips[19].tip = "COPYARR [$массив-приемник],[$массив-источник] - копирование содержимого массива в другой массив.";
	tooltips[20].word = "addobj"; tooltips[20].tip = "ADD OBJ [$название],[$путь к файлу изображения] или ADDOBJ [$название],[$путь к файлу изображения] - добавление предмета с заданным изображением в рюкзак.";
	tooltips[21].word = "delobj"; tooltips[21].tip = "DEL OBJ [$название] или DELOBJ [$название] - удаление предмета из рюкзака, если таковой имеется.";
	tooltips[22].word = "killobj"; tooltips[22].tip = "KILLOBJ - очистка рюкзака.";
	tooltips[23].word = "unselect"; tooltips[23].tip = "UNSELECT или UNSEL - отмена выбора предмета.";
	tooltips[24].word = "unsel"; tooltips[24].tip = "UNSELECT или UNSEL - отмена выбора предмета.";
	tooltips[25].word = "killall"; tooltips[25].tip = "KILLALL - эквивалентен конструкции 'KILLVAR & KILLOBJ'.";
	tooltips[26].word = "opengame"; tooltips[26].tip = "OPENGAME [$выражение] - если [$выражение] равно '' (пустая строка) или отсутствует, то вызов окна загрузки состояния игры, иначе загрузка состояния из указанного файла.";
	tooltips[27].word = "openqst"; tooltips[27].tip = "OPENQST [$выражение] - открытие и запуск заданного файла игры.";
	tooltips[28].word = "addqst"; tooltips[28].tip = "ADDQST [$выражение] - из заданного файла игры добавляет все локации, названия которых отсутствуют среди текущих игровых локаций.";
	tooltips[29].word = "killqst"; tooltips[29].tip = "KILLQST - удаляет все локации, добавленные с помощью оператора 'ADDQST'.";
	tooltips[30].word = "savegame"; tooltips[30].tip = "SAVEGAME [$выражение] - если [$выражение] равно '' (пустая строка) или отсутствует, то вызов окна сохранения состояния игры, иначе сохранение состояния в указанный файл.";
	tooltips[31].word = "refint"; tooltips[31].tip = "REFINT - обновление интерфейса.";
	tooltips[32].word = "showacts"; tooltips[32].tip = "SHOWACTS [#выражение] - если значение выражения отлично от 0, то показывает список действий, иначе скрывает его.";
	tooltips[33].word = "showinput"; tooltips[33].tip = "SHOWINPUT [#выражение] - если значение выражения отлично от 0, то показывает строку ввода, иначе скрывает её.";
	tooltips[34].word = "showobjs"; tooltips[34].tip = "SHOWOBJS [#выражение] - если значение выражения отлично от 0, то показывает список предметов, иначе скрывает его.";
	tooltips[35].word = "showstat"; tooltips[35].tip = "SHOWSTAT [#выражение] - если значение выражения отлично от 0, то показывает дополнительное описание, иначе скрывает его.";
	tooltips[36].word = "if"; tooltips[36].tip = "IF [#выражение]:[оператор1] & [оператор2] & ... ELSE [оператор3] & [оператор4] & ... ";
	tooltips[37].word = "else"; tooltips[37].tip = "IF [#выражение]:[оператор1] & [оператор2] & ... ELSE [оператор3] & [оператор4] & ... ";
	tooltips[38].word = "exit"; tooltips[38].tip = "EXIT - завершение выполнения текущего кода.";
	tooltips[39].word = "jump"; tooltips[39].tip = "JUMP [$выражение] - переход в текущем коде.";
	tooltips[40].word = "gosub"; tooltips[40].tip = "GOSUB [$выражение],[параметр1],[параметр2], ... или GS [$выражение],[параметр1],[параметр2], ... - обработка локации с названием [$выражение].";
	tooltips[41].word = "gs"; tooltips[41].tip = "GOSUB [$выражение],[параметр1],[параметр2], ... или GS [$выражение],[параметр1],[параметр2], ... - обработка локации с названием [$выражение].";
	tooltips[42].word = "goto"; tooltips[42].tip = "GOTO [$выражение],[параметр1],[параметр2], ... или GT [$выражение],[параметр1],[параметр2], ... - переход на локацию с названием [$выражение]. ";
	tooltips[43].word = "gt"; tooltips[43].tip = "GOTO [$выражение],[параметр1],[параметр2], ... или GT [$выражение],[параметр1],[параметр2], ... - переход на локацию с названием [$выражение]. ";
	tooltips[44].word = "xgoto"; tooltips[44].tip = "XGOTO [$выражение],[параметр1],[параметр2], ... или XGT [$выражение],[параметр1],[параметр2], ... - отличается от 'GOTO / GT' тем, что при переходе не очищается поле основного описания локации.";
	tooltips[45].word = "xgt"; tooltips[45].tip = "XGOTO [$выражение],[параметр1],[параметр2], ... или XGT [$выражение],[параметр1],[параметр2], ... - отличается от 'GOTO / GT' тем, что при переходе не очищается поле основного описания локации.";
	tooltips[46].word = "play"; tooltips[46].tip = "PLAY [$путь к звуковому файлу],[#громкость] - проигрывание звукового файла с заданным названием и громкостью.";
	tooltips[47].word = "close"; tooltips[47].tip = "CLOSE [$путь к звуковому файлу] / CLOSE ALL - остановка проигрывания звукового файла с заданным названием / всех звуковых файлов.";
	tooltips[48].word = "view"; tooltips[48].tip = "VIEW [$путь к графическому файлу] - просмотр картинки из указанного файла.";
	tooltips[49].word = "desc"; tooltips[49].tip = "DESC([$выражение]) - возвращает текст базового описания локации с заданным в [$выражение] названием.";
	tooltips[50].word = "iif"; tooltips[50].tip = "IIF([#выражение],[выражение_да],[выражение_нет]) - возвращает значение выражения [выражение_да], если [#выражение] верно, иначе значение выражения [выражение_нет].";
	tooltips[51].word = "input"; tooltips[51].tip = "INPUT([выражение]) - выводит окно ввода с приглашением [выражение].";
	tooltips[52].word = "isplay"; tooltips[52].tip = "ISPLAY([$выражение]) - проверяет, проигрывается ли файл с заданным названием в текущий момент.";
	tooltips[53].word = "max"; tooltips[53].tip = "MAX([выражение 1],[выражение 2]) - возвращает максимальное из значений выражений-аргументов.";
	tooltips[54].word = "min"; tooltips[54].tip = "MIN([выражение 1],[выражение 2]) - возвращает минимальное из значений выражений-аргументов.";
	tooltips[55].word = "rand"; tooltips[55].tip = "RAND([#выражение 1],[#выражение 2]) - возвращает случайное число между числами [#выражение 1] и [#выражение 2].";
	tooltips[56].word = "rgb"; tooltips[56].tip = "RGB([#выражение 1],[#выражение 2],[#выражение 3]) - возвращает код цвета на основе 3-х числовых аргументов.";
	tooltips[57].word = "getobj"; tooltips[57].tip = "GETOBJ([#выражение]) - возвращает название предмета в рюкзаке, расположенного в заданной позиции.";
	tooltips[58].word = "dyneval"; tooltips[58].tip = "DYNEVAL([$выражение]) - возвращает значение указанного выражения.";
	tooltips[59].word = "func"; tooltips[59].tip = "FUNC([$выражение],[параметр1],[параметр2], ...) - обработка локации с названием [$выражение].";
	tooltips[60].word = "arrsize"; tooltips[60].tip = "ARRSIZE([$выражение]) - возвращает число элементов в массиве с названием [$выражение].";
	tooltips[61].word = "arrpos"; tooltips[61].tip = "ARRPOS([#выражение 1],[$выражение 2],[выражение 3]) - возвращает индекс элемента массива с названием [$выражение 2], равного значению выражения [выражение 3].";
	tooltips[62].word = "instr"; tooltips[62].tip = "INSTR([#выражение 1],[$выражение 2],[$выражение 3]) - возвращает номер позиции символа, с которого начинается вхождение строки [$выражение 3] в строку [$выражение 2].";
	tooltips[63].word = "isnum"; tooltips[63].tip = "ISNUM([$выражение]) - функция проверяет, все ли символы в строке являются цифрами.";
	tooltips[64].word = "trim"; tooltips[64].tip = "TRIM([$выражение]) - удаляет прилегающие пробелы и символы табуляции из [$выражение].";
	tooltips[65].word = "ucase"; tooltips[65].tip = "UCASE([$выражение]) - возвращает строку больших букв, полученную изменением регистра букв исходной строки [$выражение].";
	tooltips[66].word = "lcase"; tooltips[66].tip = "LCASE([$выражение]) - возвращает строку маленьких букв, полученную изменением регистра букв исходной строки [$выражение].";
	tooltips[67].word = "len"; tooltips[67].tip = "LEN([$выражение]) - возвращает длину строки [$выражение].";
	tooltips[68].word = "mid"; tooltips[68].tip = "MID([$выражение],[#выражение 1],[#выражение 2]) - вырезает из строки [$выражение] строку, которая начинается с символа номер [#выражение 1] и имеет длину [#выражение 2].";
	tooltips[69].word = "replace"; tooltips[69].tip = "REPLACE([$выражение 1],[$выражение 2],[$выражение 3]) - заменяет в строке [$выражение 1] все вхождения строки [$выражение 2] строкой [$выражение 3].";
	tooltips[70].word = "str"; tooltips[70].tip = "STR([#выражение]) - переводит число (числовое выражение) [#выражение] в соответствующую строку.";
	tooltips[71].word = "val"; tooltips[71].tip = "VAL([$выражение]) - переводит строку цифр [$выражение] в соответствующее число.";
	tooltips[72].word = "arrcomp"; tooltips[72].tip = "ARRCOMP([#выражение 1],[$выражение 2],[$выражение 3]) - возвращает индекс элемента массива с названием [$выражение 2], соответствующего регулярному выражению [$выражение 3].";
	tooltips[73].word = "strcomp"; tooltips[73].tip = "STRCOMP([$выражение],[$шаблон]) - проводит сравнение строки [$выражение] на соответствие регулярному выражению [$шаблон].";
	tooltips[74].word = "strfind"; tooltips[74].tip = "STRFIND([$выражение],[$шаблон],[#номер]) - возвращает подстроку в строке [$выражение], соответствующую группе с номером [#номер] регулярного выражения [$шаблон].";
	tooltips[75].word = "strpos"; tooltips[75].tip = "STRPOS([$выражение],[$шаблон],[#номер]) - возвращает позицию символа, с которого начинается вхождение подстроки в строке [$выражение], соответствующей группе с номером [#номер] регулярного выражения [$шаблон].";
	tooltips[76].word = "countobj"; tooltips[76].tip = "COUNTOBJ - имеет значение, равное числу предметов в рюкзаке.";
	tooltips[77].word = "nosave"; tooltips[77].tip = "NOSAVE - если её значение отлично от 0, то сохранение состояния игры пользователем невозможно.";
	tooltips[78].word = "disablescroll"; tooltips[78].tip = "DISABLESCROLL - если значение переменной не равно 0, то запрещает автопрокрутку текста при его выводе в основное или дополнительное окно описания локации.";
	tooltips[79].word = "disablesubex"; tooltips[79].tip = "DISABLESUBEX - если значение переменной не равно 0, то запрещает использование 'подвыражений' в тексте.";
	tooltips[80].word = "msecscount"; tooltips[80].tip = "MSECSCOUNT - содержит количество миллисекунд, прошедших с момента начала игры.";
	tooltips[81].word = "rnd"; tooltips[81].tip = "RND - имеет случайное значение от 1 до 1000.";
	tooltips[82].word = "debug"; tooltips[82].tip = "DEBUG - если значение переменной не равно 0, то отключается проверка идентификатора игры при загрузке состояния.";
	tooltips[83].word = "curloc"; tooltips[83].tip = "$CURLOC - содержит название текущей локации.";
	tooltips[84].word = "qspver"; tooltips[84].tip = "$QSPVER - содержит версию интерпретатора в формате 'X.Y.Z'.";
	tooltips[85].word = "selobj"; tooltips[85].tip = "$SELOBJ - содержит название выделенного предмета.";
	tooltips[86].word = "setact"; tooltips[86].tip = "$SELACT - содержит название выделенного действия.";
	tooltips[87].word = "lastobj"; tooltips[87].tip = "$LASTOBJ - содержит название последнего добавленного / удалённого предмета.";
	tooltips[88].word = "user_text"; tooltips[88].tip = "$USER_TEXT и $USRTXT - содержат текст, находящийся в строке ввода.";
	tooltips[89].word = "usrtxt"; tooltips[89].tip = "$USER_TEXT и $USRTXT - содержат текст, находящийся в строке ввода.";
	tooltips[90].word = "maintxt"; tooltips[90].tip = "$MAINTXT - содержит текст, находящийся в основном окне описаний.";
	tooltips[91].word = "stattxt"; tooltips[91].tip = "$STATTXT - содержит текст, находящийся в окне пользователя.";
	tooltips[92].word = "counter"; tooltips[92].tip = "$COUNTER - содержит название локации-счётчика.";
	tooltips[93].word = "ongload"; tooltips[93].tip = "$ONGLOAD - содержит название локации-обработчика загрузки состояния.";
	tooltips[94].word = "ongsave"; tooltips[94].tip = "$ONGSAVE - содержит название локации-обработчика сохранения состояния.";
	tooltips[95].word = "onnewloc"; tooltips[95].tip = "$ONNEWLOC - содержит название локации-обработчика перехода на новую локацию.";
	tooltips[96].word = "onactsel"; tooltips[96].tip = "$ONACTSEL - содержит название локации-обработчика выбора действия.";
	tooltips[97].word = "onobjsel"; tooltips[97].tip = "$ONOBJSEL - содержит название локации-обработчика выбора предмета.";
	tooltips[98].word = "onobjadd"; tooltips[98].tip = "$ONOBJADD - содержит название локации-обработчика добавления предмета.";
	tooltips[99].word = "onobjdel"; tooltips[99].tip = "$ONOBJDEL - содержит название локации-обработчика удаления предмета.";
	tooltips[100].word = "usercom"; tooltips[100].tip = "$USERCOM - содержит название локации-обработчика строки ввода.";
	tooltips[101].word = "usehtml"; tooltips[101].tip = "USEHTML - если отлична от 0, включает возможность использования HTML в описании локации, в дополнительном описании, в списках действий и предметов, а также в диалоге ввода текста, вызываемого функцией 'INPUT'.";
	tooltips[102].word = "bcolor"; tooltips[102].tip = "BCOLOR - содержит цвет текущего фона.";
	tooltips[103].word = "fcolor"; tooltips[103].tip = "FCOLOR - содержит цвет используемого в данный момент шрифта.";
	tooltips[104].word = "lcolor"; tooltips[104].tip = "LCOLOR - содержит текущий цвет ссылок.";
	tooltips[105].word = "fsize"; tooltips[105].tip = "FSIZE - содержит размер используемого в данный момент шрифта.";
	tooltips[106].word = "fname"; tooltips[106].tip = "$FNAME - содержит название используемого в данный момент шрифта.";
	tooltips[107].word = "backimage"; tooltips[107].tip = "$BACKIMAGE - содержит путь к файлу изображения локации.";
}
