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

#include "syntaxtextbox.h"
#include "mainframe.h"
#include "game.h"


IMPLEMENT_CLASS(SyntaxTextBox, wxStyledTextCtrl)

BEGIN_EVENT_TABLE(SyntaxTextBox, wxStyledTextCtrl)
	EVT_KEY_DOWN(SyntaxTextBox::OnKeyDown)
	EVT_KEY_UP(SyntaxTextBox::OnKeyUp)
	EVT_RIGHT_DOWN(SyntaxTextBox::OnRightClick)
	EVT_STC_MARGINCLICK(wxID_ANY, SyntaxTextBox::OnMarginClicked)
	EVT_STC_CHARADDED(wxID_ANY, SyntaxTextBox::OnCharAdded)
	EVT_MOTION(SyntaxTextBox::OnMouseMove)
END_EVENT_TABLE()

SyntaxTextBox::SyntaxTextBox(wxWindow *owner, IControls *controls, int style) :
	wxStyledTextCtrl(owner, wxID_ANY, wxDefaultPosition, wxSize(1, 1))
{
	_controls = controls;
	_keywordsStore = controls->GetKeywordsStore();
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
		SetKeyWords(0, _keywordsStore->GetWords(STATEMENT));
		SetKeyWords(1, _keywordsStore->GetWords(EXPRESSION));
		SetKeyWords(2, _keywordsStore->GetWords(VARIABLE));
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
	}
	Update();
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
	wxFont font;
	int tabSize;

	SetCaretForeground((backColour.Blue() << 16 | backColour.Green() << 8 | backColour.Red()) ^ 0xFFFFFF);
	SetSelBackground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
	SetSelForeground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));
	StyleSetForeground(wxSTC_STYLE_DEFAULT, settings->GetColour(SYNTAX_BASE));
	StyleSetBackground(wxSTC_STYLE_DEFAULT, backColour);
	font = settings->GetFont(SYNTAX_BASE);
	StyleSetFont(wxSTC_STYLE_DEFAULT, font);
	tabSize = settings->GetTabSize();
	SetTabWidth(tabSize);
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
		font = settings->GetFont(SYNTAX_STATEMENTS);
		StyleSetFont(wxSTC_B_KEYWORD, font);
		StyleSetForeground(wxSTC_B_KEYWORD, settings->GetColour(SYNTAX_STATEMENTS));
		font = settings->GetFont(SYNTAX_FUNCTIONS);
		StyleSetFont(wxSTC_B_KEYWORD2, font);
		StyleSetForeground(wxSTC_B_KEYWORD2, settings->GetColour(SYNTAX_FUNCTIONS));
		font = settings->GetFont(SYNTAX_SYS_VARIABLES);
		StyleSetFont(wxSTC_B_KEYWORD3, font);
		StyleSetForeground(wxSTC_B_KEYWORD3, settings->GetColour(SYNTAX_SYS_VARIABLES));
		// Строки
		font = settings->GetFont(SYNTAX_STRINGS);
		StyleSetFont(wxSTC_B_STRING, font);
		StyleSetForeground(wxSTC_B_STRING, settings->GetColour(SYNTAX_STRINGS));
		StyleSetFont(wxSTC_B_STRINGEOL, font);
		StyleSetForeground(wxSTC_B_STRINGEOL, settings->GetColour(SYNTAX_STRINGS));
		// Числа
		font = settings->GetFont(SYNTAX_NUMBERS);
		StyleSetFont(wxSTC_B_NUMBER, font);
		StyleSetForeground(wxSTC_B_NUMBER, settings->GetColour(SYNTAX_NUMBERS));
		// Операции
		font = settings->GetFont(SYNTAX_OPERATIONS);
		StyleSetFont(wxSTC_B_OPERATOR, font);
		StyleSetForeground(wxSTC_B_OPERATOR, settings->GetColour(SYNTAX_OPERATIONS));
		// Метки
		font = settings->GetFont(SYNTAX_LABELS);
		StyleSetFont(wxSTC_B_LABEL, font);
		StyleSetForeground(wxSTC_B_LABEL, settings->GetColour(SYNTAX_LABELS));
		// Комментарии
		font = settings->GetFont(SYNTAX_COMMENTS);
		StyleSetFont(wxSTC_B_COMMENT, font);
		StyleSetForeground(wxSTC_B_COMMENT, settings->GetColour(SYNTAX_COMMENTS));
		StyleSetFont(wxSTC_B_PREPROCESSOR, font);
		StyleSetForeground(wxSTC_B_PREPROCESSOR, settings->GetColour(SYNTAX_COMMENTS));
		StyleSetFont(wxSTC_B_DATE, font);
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
		wxString str;
		size_t count = _keywordsStore->GetCount();
		for (size_t i = 0; i < count; ++i)
		{
			str = _keywordsStore->GetWord(i).Upper();
			if (str.StartsWith(root)) words.Add(str);
		}
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
	if ((_style & SYNTAX_STYLE_COLORED) && event.GetKey() == '\n' && !_controls->IsInHotkeyExecution())
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
		menu.Append(ID_TEXT_UNDO, _("Undo"));
		menu.Append(ID_TEXT_REDO, _("Redo"));
		menu.AppendSeparator();
		menu.Append(ID_TEXT_CUT, _("Cut"));
		menu.Append(ID_TEXT_COPY, _("Copy"));
		menu.Append(ID_TEXT_PASTE, _("Paste"));
		menu.Append(ID_TEXT_DEL, _("Delete"));
		menu.AppendSeparator();
		menu.Append(ID_TEXT_SELALL, _("Select all"));
		if (_style & SYNTAX_STYLE_COLORED)
		{
			menu.AppendSeparator();
			menu.Append(ID_LOC_JUMPLOC, _("Go to selected location"));
		}
		_controls->UpdateMenuItems(&menu);
		PopupMenu(&menu);
	}
}

void SyntaxTextBox::SetValue( const wxString &str )
{
	SetText(str);
	SetModified(false);
	EmptyUndoBuffer();
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
	if (!(_style & SYNTAX_STYLE_NOHELPTIPS))
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
		_controls->SetStatusText(_keywordsStore->FindTip(str));
}

void SyntaxTextBox::OnKeyUp(wxKeyEvent& event)
{
	if (!(_style & SYNTAX_STYLE_NOHELPTIPS))
		Tip(GetCurrentPos());
	event.Skip();
}
