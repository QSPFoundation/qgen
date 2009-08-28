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
	EVT_KEY_UP(SyntaxTextBox::OnKeyUp)
	EVT_RIGHT_DOWN(SyntaxTextBox::OnRightClick)
	EVT_STC_MARGINCLICK(wxID_ANY, SyntaxTextBox::OnMarginClicked)
	EVT_STC_CHARADDED(wxID_ANY, SyntaxTextBox::OnCharAdded)
	EVT_MOTION(SyntaxTextBox::OnMouseMove)
END_EVENT_TABLE()

wxArrayString SyntaxTextBox::_keywords;
std::list<HelpTip> SyntaxTextBox::_tooltips;

SyntaxTextBox::SyntaxTextBox(wxWindow *owner, IControls *controls, wxStatusBar *statusBar, int style) :
	wxStyledTextCtrl(owner, wxID_ANY, wxDefaultPosition, wxSize(1, 1))
{
	_controls = controls;
	_statusBar = statusBar;
	_style = style;
	SetEOLMode(wxSTC_EOL_LF);
	SetWrapMode(wxSTC_WRAP_WORD);
	SetMarginWidth(1, 0);
	if (_style & SYNTAX_STYLE_NOSCROLLBARS)
	{
		SetUseHorizontalScrollBar(false);
		SetUseVerticalScrollBar(false);
	}
	if (!(_style & SYNTAX_STYLE_SIMPLEMENU))
	{
		UsePopUp(false);
	}
	if (_style & SYNTAX_STYLE_COLORED)
	{
		SetScrollWidth(-1);
		SetScrollWidthTracking(true);

		SetLexer(wxSTC_LEX_VB);

		SetKeyWords(0, wxT("clear clr nl p pl msg wait act delact del cla cmdclear cmdclr cls set ") \
					   wxT("let killvar dynamic copyarr add obj addobj delobj killobj unselect unsel ") \
					   wxT("killall menu opengame openqst addqst killqst savegame refint settimer showacts ") \
					   wxT("showinput showobjs showstat if else end exit jump gosub gs goto gt xgoto xgt play close all view"));

		SetKeyWords(1, wxT("and or no desc iif input $desc $iif $input isplay max min $max $min rand rgb ") \
					   wxT("getobj $getobj dyneval $dyneval func $func arrpos arrsize instr isnum trim $trim ") \
					   wxT("ucase lcase $ucase $lcase len mid str $mid $str val arrcomp strcomp strfind $strfind ") \
					   wxT("strpos replace $replace user_text $user_text usrtxt $usrtxt maintxt $maintxt ") \
					   wxT("stattxt $stattxt countobj msecscount rnd curloc $curloc qspver $qspver selobj $selobj ") \
					   wxT("selact $selact curacts $curacts"));

		SetKeyWords(2, wxT("nosave disablescroll disablesubex debug $counter $ongload $ongsave ") \
					   wxT("$onnewloc $onactsel $onobjsel $onobjadd $onobjdel $usercom usehtml bcolor ") \
					   wxT("fcolor lcolor fsize $fname $backimage args $args result $result"));

		//	SetViewEOL(true);
		//	SetViewWhiteSpace(true);
		SetIndentationGuides(true);
		if (!(_style & SYNTAX_STYLE_NOMARGINS))
		{
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
		}
		AutoCompSetChooseSingle(true);
		AutoCompSetIgnoreCase(true);
		AutoCompSetDropRestOfWord(true);

		FillKeywords(wxT("CLEAR CLR NL P PL MSG WAIT ACT DELACT DEL CLA CMDCLEAR CMDCLR CLS SET LET ") \
					 wxT("KILLVAR DYNAMIC COPYARR ADD OBJ ADDOBJ DELOBJ KILLOBJ UNSELECT UNSEL KILLALL ") \
					 wxT("MENU OPENGAME OPENQST ADDQST KILLQST SAVEGAME REFINT SETTIMER SHOWACTS SHOWINPUT ") \
					 wxT("SHOWOBJS SHOWSTAT IF ELSE END EXIT JUMP GOSUB GS GOTO GT XGOTO XGT PLAY ") \
					 wxT("CLOSE ALL VIEW AND OR NO DESC IIF INPUT $DESC $IIF $INPUT ISPLAY MAX MIN ") \
					 wxT("$MAX $MIN RAND RGB GETOBJ $GETOBJ DYNEVAL $DYNEVAL FUNC $FUNC ARRPOS ARRSIZE ") \
					 wxT("INSTR ISNUM TRIM $TRIM UCASE LCASE $UCASE $LCASE LEN MID STR REPLACE $REPLACE ") \
					 wxT("$MID $STR VAL ARRCOMP STRCOMP STRFIND $STRFIND STRPOS COUNTOBJ NOSAVE DISABLESCROLL ") \
					 wxT("DISABLESUBEX MSECSCOUNT RND DEBUG CURLOC $CURLOC QSPVER $QSPVER SELOBJ $SELOBJ ") \
					 wxT("SELACT $SELACT USER_TEXT $USER_TEXT USRTXT $USRTXT MAINTXT $MAINTXT ") \
					 wxT("STATTXT $STATTXT $COUNTER $ONGLOAD $ONGSAVE $ONNEWLOC $ONACTSEL $ONOBJSEL ") \
					 wxT("$ONOBJADD $ONOBJDEL $USERCOM USEHTML BCOLOR FCOLOR LCOLOR FSIZE $FNAME ") \
					 wxT("$BACKIMAGE ARGS $ARGS RESULT $RESULT CURACTS $CURACTS"));
	}
	Update();
	if (_statusBar) LoadTips();

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
		if (!(_style & SYNTAX_STYLE_NOMARGINS))
		{
			SetFoldMarginColour(true, backColour);
			// Нумерация строк
			SetMarginWidth(SYNTAX_NUM_MARGIN, settings->GetShowLinesNums() ? 40 : 0);
		}
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
		for (size_t i = 0; i < _keywords.GetCount(); ++i)
			if (_keywords[i].StartsWith(root)) words.Add(_keywords[i]);
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
	if (_keywords.empty())
	{
		int cur, beg = 0;
		while ((cur = str.find(wxT(' '), beg)) >= 0)
		{
			_keywords.Add(str.substr(beg, cur - beg));
			beg = cur + 1;
		}
		if (beg > 0) _keywords.Add(str.substr(beg, str.length() - beg));
		_keywords.Sort();
	}
}

wxString SyntaxTextBox::GetArrayAsString( const wxArrayString &arr )
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

void SyntaxTextBox::OnMouseMove(wxMouseEvent& event)
{
	wxStyledTextCtrl::OnMouseMove(event);
	if (_statusBar)
		Tip(PositionFromPoint(event.GetPosition()));
}

wxString SyntaxTextBox::GetWordFromPos(int pos)
{
	wxString str;
	int beginPos, lastPos;
	int lineInd = LineFromPosition(pos);
	int realPos = GetCharIndexFromPosition(PositionFromLine(lineInd), pos);
	wxString lineStr = GetLine(lineInd).Trim();
	if (!lineStr.IsEmpty())
	{
		if (realPos >= lineStr.Length())
			realPos = lineStr.Length() - 1;
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
	bool tipFound = false;
	wxString str = GetWordFromPos(pos).Lower();
	if (!str.IsEmpty())
	{
		for (std::list<HelpTip>::const_iterator i = _tooltips.begin(); i != _tooltips.end(); ++i)
		{
			if (str == i->word)
			{
				_statusBar->SetStatusText(i->tip);
				tipFound = true;
				break;
			}
		}
	}
	if (!tipFound)
		_statusBar->SetStatusText(wxEmptyString);
}

void SyntaxTextBox::LoadTips()
{
	if (_tooltips.empty())
	{
		// Statements
		_tooltips.push_back(HelpTip(wxT("pl"), wxT("PL [выражение] / *PL [выражение] - вывод текста в дополнительное / основное окно описания, затем переход на новую строку")));
		_tooltips.push_back(HelpTip(wxT("clear"), wxT("CLEAR / *CLEAR - очистка дополнительного / основного окна описания")));
		_tooltips.push_back(HelpTip(wxT("clr"), wxT("CLR / *CLR - очистка дополнительного / основного окна описания")));
		_tooltips.push_back(HelpTip(wxT("p"), wxT("P [выражение] / *P [выражение] - вывод текста в дополнительное / основное окно описания")));
		_tooltips.push_back(HelpTip(wxT("nl"), wxT("NL [выражение] / *NL [выражение] - переход на новую строку, затем вывод текста в дополнительное / основное окно описания")));
		_tooltips.push_back(HelpTip(wxT("msg"), wxT("MSG [выражение] - вывод сообщения в информационном окне")));
		_tooltips.push_back(HelpTip(wxT("wait"), wxT("WAIT [#выражение] - остановка выполнения программы на заданное количество миллисекунд")));
		_tooltips.push_back(HelpTip(wxT("act"), wxT("ACT [$название],[$путь к файлу изображения]:[оператор] & [оператор] & ... - добавление действия")));
		_tooltips.push_back(HelpTip(wxT("delact"), wxT("DELACT [$название] / DEL ACT [$название] - удаление действия")));
		_tooltips.push_back(HelpTip(wxT("cla"), wxT("CLA - очистка списка действий")));
		_tooltips.push_back(HelpTip(wxT("cmdclear"), wxT("CMDCLEAR - очистка строки ввода")));
		_tooltips.push_back(HelpTip(wxT("cmdclr"), wxT("CMDCLR - очистка строки ввода")));
		_tooltips.push_back(HelpTip(wxT("cls"), wxT("CLS - очищает экран, оставляя только список предметов")));
		_tooltips.push_back(HelpTip(wxT("menu"), wxT("MENU [$выражение] - показ меню с заданным названием")));
		_tooltips.push_back(HelpTip(wxT("settimer"), wxT("SETTIMER [#выражение] - задает интервал таймера для локации-счетчика")));
		_tooltips.push_back(HelpTip(wxT("dynamic"), wxT("DYNAMIC [$строка кода] - динамическое выполнение кода")));
		_tooltips.push_back(HelpTip(wxT("set"), wxT("SET [название переменной]=[выражение] - установка значения переменной")));
		_tooltips.push_back(HelpTip(wxT("let"), wxT("LET [название переменной]=[выражение] - установка значения переменной")));
		_tooltips.push_back(HelpTip(wxT("killvar"), wxT("KILLVAR [$название],[#индекс] - удаление всех переменных / указанной переменной")));
		_tooltips.push_back(HelpTip(wxT("copyarr"), wxT("COPYARR [$массив-приемник],[$массив-источник] - копирование содержимого массива в другой массив")));
		_tooltips.push_back(HelpTip(wxT("addobj"), wxT("ADDOBJ [$название],[$путь к файлу изображения] / ADD OBJ [$название],[$путь к файлу изображения] - добавление предмета")));
		_tooltips.push_back(HelpTip(wxT("delobj"), wxT("DELOBJ [$название] / DEL OBJ [$название] - удаление предмета")));
		_tooltips.push_back(HelpTip(wxT("killobj"), wxT("KILLOBJ - удаление всех предметов")));
		_tooltips.push_back(HelpTip(wxT("unselect"), wxT("UNSELECT - отмена выбора предмета")));
		_tooltips.push_back(HelpTip(wxT("unsel"), wxT("UNSEL - отмена выбора предмета")));
		_tooltips.push_back(HelpTip(wxT("killall"), wxT("KILLALL - удаление всех переменных и предметов")));
		_tooltips.push_back(HelpTip(wxT("opengame"), wxT("OPENGAME [$выражение] - загрузка состояния игры")));
		_tooltips.push_back(HelpTip(wxT("openqst"), wxT("OPENQST [$выражение] - открытие и запуск файла игры")));
		_tooltips.push_back(HelpTip(wxT("addqst"), wxT("ADDQST [$выражение] - добавление локаций из указанного файла")));
		_tooltips.push_back(HelpTip(wxT("killqst"), wxT("KILLQST - удаление всех добавленных через ADDQST локаций")));
		_tooltips.push_back(HelpTip(wxT("savegame"), wxT("SAVEGAME [$выражение] - сохранение состояния игры")));
		_tooltips.push_back(HelpTip(wxT("refint"), wxT("REFINT - обновление интерфейса")));
		_tooltips.push_back(HelpTip(wxT("showacts"), wxT("SHOWACTS [#выражение] - показ / скрытие списка действий")));
		_tooltips.push_back(HelpTip(wxT("showinput"), wxT("SHOWINPUT [#выражение] - показ / скрытие строки ввода")));
		_tooltips.push_back(HelpTip(wxT("showobjs"), wxT("SHOWOBJS [#выражение] - показ / скрытие списка предметов")));
		_tooltips.push_back(HelpTip(wxT("showstat"), wxT("SHOWSTAT [#выражение] - показ / скрытие дополнительного окна описания")));
		_tooltips.push_back(HelpTip(wxT("if"), wxT("IF [#выражение]:[оператор1] & [оператор2] & ... ELSE [оператор3] & [оператор4] & ... - условие")));
		_tooltips.push_back(HelpTip(wxT("else"), wxT("IF [#выражение]:[оператор1] & [оператор2] & ... ELSE [оператор3] & [оператор4] & ... - условие")));
		_tooltips.push_back(HelpTip(wxT("exit"), wxT("EXIT - завершение выполнения текущего кода")));
		_tooltips.push_back(HelpTip(wxT("end"), wxT("END - завершение многострочной формы IF / ACT")));
		_tooltips.push_back(HelpTip(wxT("jump"), wxT("JUMP [$выражение] - переход на указанную метку")));
		_tooltips.push_back(HelpTip(wxT("gosub"), wxT("GOSUB [$выражение],[параметр1],[параметр2], ... - обработка указанной локации")));
		_tooltips.push_back(HelpTip(wxT("gs"), wxT("GS [$выражение],[параметр1],[параметр2], ... - обработка указанной локации")));
		_tooltips.push_back(HelpTip(wxT("goto"), wxT("GOTO [$выражение],[параметр1],[параметр2], ... - переход на указанную локацию")));
		_tooltips.push_back(HelpTip(wxT("gt"), wxT("GT [$выражение],[параметр1],[параметр2], ... - переход на указанную локацию")));
		_tooltips.push_back(HelpTip(wxT("xgoto"), wxT("XGOTO [$выражение],[параметр1],[параметр2], ... - переход на указанную локацию без очистки основного окна описания")));
		_tooltips.push_back(HelpTip(wxT("xgt"), wxT("XGT [$выражение],[параметр1],[параметр2], ... - переход на указанную локацию без очистки основного окна описания")));
		_tooltips.push_back(HelpTip(wxT("play"), wxT("PLAY [$путь к звуковому файлу],[#громкость] - воспроизведение звукового файла с заданной громкостью")));
		_tooltips.push_back(HelpTip(wxT("close"), wxT("CLOSE [$путь к звуковому файлу] / CLOSE ALL - остановка проигрывания звукового файла / всех звуковых файлов")));
		_tooltips.push_back(HelpTip(wxT("view"), wxT("VIEW [$путь к графическому файлу] - просмотр изображения")));
		// Functions
		_tooltips.push_back(HelpTip(wxT("desc"), wxT("DESC([$выражение]) - возвращает текст базового описания локации")));
		_tooltips.push_back(HelpTip(wxT("$desc"), wxT("$DESC([$выражение]) - возвращает текст базового описания локации")));
		_tooltips.push_back(HelpTip(wxT("iif"), wxT("IIF([#выражение],[выражение_да],[выражение_нет]) - возвращает одно из выражений по условию")));
		_tooltips.push_back(HelpTip(wxT("$iif"), wxT("$IIF([#выражение],[выражение_да],[выражение_нет]) - возвращает одно из выражений по условию")));
		_tooltips.push_back(HelpTip(wxT("input"), wxT("INPUT([выражение]) - показывает окно ввода текста и возвращает введенное значение")));
		_tooltips.push_back(HelpTip(wxT("$input"), wxT("$INPUT([выражение]) - показывает окно ввода текста и возвращает введенное значение")));
		_tooltips.push_back(HelpTip(wxT("isplay"), wxT("ISPLAY([$выражение]) - проверяет, проигрывается ли в текущий момент указанный файл")));
		_tooltips.push_back(HelpTip(wxT("max"), wxT("MAX([выражение 1],[выражение 2], ...) - возвращает максимальное из значений аргументов")));
		_tooltips.push_back(HelpTip(wxT("$max"), wxT("$MAX([выражение 1],[выражение 2], ...) - возвращает максимальное из значений аргументов")));
		_tooltips.push_back(HelpTip(wxT("min"), wxT("MIN([выражение 1],[выражение 2], ...) - возвращает минимальное из значений аргументов")));
		_tooltips.push_back(HelpTip(wxT("$min"), wxT("$MIN([выражение 1],[выражение 2], ...) - возвращает минимальное из значений аргументов")));
		_tooltips.push_back(HelpTip(wxT("rand"), wxT("RAND([#выражение 1],[#выражение 2]) - возвращает случайное число между заданными")));
		_tooltips.push_back(HelpTip(wxT("rgb"), wxT("RGB([#красный],[#зеленый],[#синий]) - возвращает код цвета на основе 3-х составляющих")));
		_tooltips.push_back(HelpTip(wxT("getobj"), wxT("GETOBJ([#выражение]) - возвращает название предмета, расположенного в заданной позиции")));
		_tooltips.push_back(HelpTip(wxT("$getobj"), wxT("$GETOBJ([#выражение]) - возвращает название предмета, расположенного в заданной позиции")));
		_tooltips.push_back(HelpTip(wxT("dyneval"), wxT("DYNEVAL([$выражение]) - возвращает значение динамически вычисленного выражения")));
		_tooltips.push_back(HelpTip(wxT("$dyneval"), wxT("$DYNEVAL([$выражение]) - возвращает значение динамически вычисленного выражения")));
		_tooltips.push_back(HelpTip(wxT("func"), wxT("FUNC([$выражение],[параметр1],[параметр2], ...) - обработка указанной локации как функции")));
		_tooltips.push_back(HelpTip(wxT("$func"), wxT("$FUNC([$выражение],[параметр1],[параметр2], ...) - обработка указанной локации как функции")));
		_tooltips.push_back(HelpTip(wxT("arrsize"), wxT("ARRSIZE([$выражение]) - возвращает число элементов в указанном массиве")));
		_tooltips.push_back(HelpTip(wxT("arrpos"), wxT("ARRPOS([#начальный индекс],[$имя массива],[значение]) - поиск в массиве элемента с заданным значением")));
		_tooltips.push_back(HelpTip(wxT("instr"), wxT("INSTR([#начальная позиция],[$текст],[$искомый текст]) - поиск вхождения текста")));
		_tooltips.push_back(HelpTip(wxT("isnum"), wxT("ISNUM([$выражение]) - проверяет, является ли указанная строка числом")));
		_tooltips.push_back(HelpTip(wxT("trim"), wxT("TRIM([$выражение]) - удаляет из текста прилегающие пробелы и символы табуляции")));
		_tooltips.push_back(HelpTip(wxT("$trim"), wxT("$TRIM([$выражение]) - удаляет из текста прилегающие пробелы и символы табуляции")));
		_tooltips.push_back(HelpTip(wxT("ucase"), wxT("UCASE([$выражение]) - преобразует маленькие буквы текста в большие")));
		_tooltips.push_back(HelpTip(wxT("$ucase"), wxT("$UCASE([$выражение]) - преобразует маленькие буквы текста в большие")));
		_tooltips.push_back(HelpTip(wxT("lcase"), wxT("LCASE([$выражение]) - преобразует большие буквы текста в маленькие")));
		_tooltips.push_back(HelpTip(wxT("$lcase"), wxT("$LCASE([$выражение]) - преобразует большие буквы текста в маленькие")));
		_tooltips.push_back(HelpTip(wxT("len"), wxT("LEN([$выражение]) - возвращает длину указанной строки")));
		_tooltips.push_back(HelpTip(wxT("mid"), wxT("MID([$текст],[#начало],[#длина]) - вырезает из текста строку указанной длины начиная с заданной позиции")));
		_tooltips.push_back(HelpTip(wxT("$mid"), wxT("$MID([$текст],[#начало],[#длина]) - вырезает из текста строку указанной длины начиная с заданной позиции")));
		_tooltips.push_back(HelpTip(wxT("replace"), wxT("REPLACE([$текст],[$искомый текст],[$текст для замены]) - заменяет в тексте заданную строку")));
		_tooltips.push_back(HelpTip(wxT("$replace"), wxT("$REPLACE([$текст],[$искомый текст],[$текст для замены]) - заменяет в тексте заданную строку")));
		_tooltips.push_back(HelpTip(wxT("str"), wxT("STR([#выражение]) - переводит число в строку")));
		_tooltips.push_back(HelpTip(wxT("$str"), wxT("$STR([#выражение]) - переводит число в строку")));
		_tooltips.push_back(HelpTip(wxT("val"), wxT("VAL([$выражение]) - переводит строку в число")));
		_tooltips.push_back(HelpTip(wxT("arrcomp"), wxT("ARRCOMP([#начальный индекс],[$имя массива],[$шаблон]) - поиск в массиве элемента, соответствующего регулярному выражению")));
		_tooltips.push_back(HelpTip(wxT("strcomp"), wxT("STRCOMP([$выражение],[$шаблон]) - проверяет заданный текст на соответствие регулярному выражению")));
		_tooltips.push_back(HelpTip(wxT("strfind"), wxT("STRFIND([$выражение],[$шаблон],[#номер]) - возвращает подстроку, соответствующую группе с номером [#номер] регулярного выражения")));
		_tooltips.push_back(HelpTip(wxT("$strfind"), wxT("$STRFIND([$выражение],[$шаблон],[#номер]) - возвращает подстроку, соответствующую группе с номером [#номер] регулярного выражения")));
		_tooltips.push_back(HelpTip(wxT("strpos"), wxT("STRPOS([$выражение],[$шаблон],[#номер]) - возвращает позицию подстроки, соответствующей группе с номером [#номер] регулярного выражения")));
		_tooltips.push_back(HelpTip(wxT("countobj"), wxT("COUNTOBJ - возвращает текущее число предметов")));
		_tooltips.push_back(HelpTip(wxT("msecscount"), wxT("MSECSCOUNT - возвращает количество миллисекунд, прошедших с момента начала игры")));
		_tooltips.push_back(HelpTip(wxT("rnd"), wxT("RND - возвращает случайное значение от 1 до 1000")));
		_tooltips.push_back(HelpTip(wxT("curloc"), wxT("CURLOC - возвращает название текущей локации")));
		_tooltips.push_back(HelpTip(wxT("$curloc"), wxT("$CURLOC - возвращает название текущей локации")));
		_tooltips.push_back(HelpTip(wxT("qspver"), wxT("QSPVER - возвращает версию интерпретатора")));
		_tooltips.push_back(HelpTip(wxT("$qspver"), wxT("$QSPVER - возвращает версию интерпретатора")));
		_tooltips.push_back(HelpTip(wxT("selobj"), wxT("SELOBJ - возвращает название выделенного предмета")));
		_tooltips.push_back(HelpTip(wxT("$selobj"), wxT("$SELOBJ - возвращает название выделенного предмета")));
		_tooltips.push_back(HelpTip(wxT("selact"), wxT("SELACT - возвращает название выделенного действия")));
		_tooltips.push_back(HelpTip(wxT("$selact"), wxT("$SELACT - возвращает название выделенного действия")));
		_tooltips.push_back(HelpTip(wxT("curacts"), wxT("CURACTS - возвращает в виде кода все текущие действия")));
		_tooltips.push_back(HelpTip(wxT("$curacts"), wxT("$CURACTS - возвращает в виде кода все текущие действия")));
		_tooltips.push_back(HelpTip(wxT("user_text"), wxT("USER_TEXT - возвращает текст в строке ввода")));
		_tooltips.push_back(HelpTip(wxT("$user_text"), wxT("$USER_TEXT - возвращает текст в строке ввода")));
		_tooltips.push_back(HelpTip(wxT("usrtxt"), wxT("USRTXT - возвращает текст в строке ввода")));
		_tooltips.push_back(HelpTip(wxT("$usrtxt"), wxT("$USRTXT - возвращает текст в строке ввода")));
		_tooltips.push_back(HelpTip(wxT("maintxt"), wxT("MAINTXT - возвращает текст в основном окне описания")));
		_tooltips.push_back(HelpTip(wxT("$maintxt"), wxT("$MAINTXT - возвращает текст в основном окне описания")));
		_tooltips.push_back(HelpTip(wxT("stattxt"), wxT("STATTXT - возвращает текст в дополнительном окне описания")));
		_tooltips.push_back(HelpTip(wxT("$stattxt"), wxT("$STATTXT - возвращает текст в дополнительном окне описания")));
		// Variables
		_tooltips.push_back(HelpTip(wxT("disablescroll"), wxT("DISABLESCROLL - если переменная не равна 0, то запрещает автопрокрутку текста при выводе")));
		_tooltips.push_back(HelpTip(wxT("disablesubex"), wxT("DISABLESUBEX - если переменная не равна 0, то запрещает использование 'подвыражений' в тексте")));
		_tooltips.push_back(HelpTip(wxT("nosave"), wxT("NOSAVE - если переменная не равна 0, то сохранение состояния игры пользователем невозможно")));
		_tooltips.push_back(HelpTip(wxT("debug"), wxT("DEBUG - если переменная не равна 0, то отключается проверка идентификатора игры при загрузке состояния")));
		_tooltips.push_back(HelpTip(wxT("$counter"), wxT("$COUNTER - переменная содержит название локации-счётчика")));
		_tooltips.push_back(HelpTip(wxT("$ongload"), wxT("$ONGLOAD - переменная содержит название локации-обработчика загрузки состояния")));
		_tooltips.push_back(HelpTip(wxT("$ongsave"), wxT("$ONGSAVE - переменная содержит название локации-обработчика сохранения состояния")));
		_tooltips.push_back(HelpTip(wxT("$onnewloc"), wxT("$ONNEWLOC - переменная содержит название локации-обработчика перехода на новую локацию")));
		_tooltips.push_back(HelpTip(wxT("$onactsel"), wxT("$ONACTSEL - переменная содержит название локации-обработчика выбора действия")));
		_tooltips.push_back(HelpTip(wxT("$onobjsel"), wxT("$ONOBJSEL - переменная содержит название локации-обработчика выбора предмета")));
		_tooltips.push_back(HelpTip(wxT("$onobjadd"), wxT("$ONOBJADD - переменная содержит название локации-обработчика добавления предмета")));
		_tooltips.push_back(HelpTip(wxT("$onobjdel"), wxT("$ONOBJDEL - переменная содержит название локации-обработчика удаления предмета")));
		_tooltips.push_back(HelpTip(wxT("$usercom"), wxT("$USERCOM - переменная содержит название локации-обработчика строки ввода")));
		_tooltips.push_back(HelpTip(wxT("usehtml"), wxT("USEHTML - если переменная не равна 0, то включает возможность использования HTML")));
		_tooltips.push_back(HelpTip(wxT("bcolor"), wxT("BCOLOR - переменная содержит цвет фона")));
		_tooltips.push_back(HelpTip(wxT("fcolor"), wxT("FCOLOR - переменная содержит основной цвет шрифта")));
		_tooltips.push_back(HelpTip(wxT("lcolor"), wxT("LCOLOR - переменная содержит основной цвет ссылок")));
		_tooltips.push_back(HelpTip(wxT("fsize"), wxT("FSIZE - переменная содержит основной размер шрифта")));
		_tooltips.push_back(HelpTip(wxT("$fname"), wxT("$FNAME - переменная содержит название основного шрифта")));
		_tooltips.push_back(HelpTip(wxT("$backimage"), wxT("$BACKIMAGE - переменная содержит путь к файлу изображения локации")));
	}
}

void SyntaxTextBox::OnKeyUp(wxKeyEvent& event)
{
	if (_statusBar)
		Tip(GetCurrentPos());
	event.Skip();
}
