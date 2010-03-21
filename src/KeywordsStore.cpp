#include "KeywordsStore.h"
#include <wx/arrimpl.cpp>

WX_DEFINE_OBJARRAY(KeywordsArray);

int CmpWordsAsc( Keyword **first, Keyword **second )
{
	return (*first)->word.CmpNoCase((*second)->word);
}

bool KeywordsStore::Load( const wxString &filename )
{
	wxXmlDocument doc;
	wxXmlNode *node;
	if (!(wxFileExists(filename) && doc.Load(filename))) return false;
	node = doc.GetRoot();
	if (node == NULL || node->GetName() != wxT("QGen-keywords")) return false;
	node = node->GetChildren();
	if (node == NULL) return false;
	while (node)
	{
		if (node->GetName() == wxT("Statements"))
		{
			ParseKeywords(node, STATEMENT);
		} 
		else if (node->GetName() == wxT("Expressions"))
		{
			ParseKeywords(node, EXPRESSION);
		} 
		else if (node->GetName() == wxT("Variables"))
		{
			ParseKeywords(node, VARIABLE);
		}
		node = node->GetNext();
	}
	_keywords.Sort(CmpWordsAsc);
	return true;
}

void KeywordsStore::ParseKeywords( wxXmlNode *node, KeywordType type )
{
	Keyword *keyword;

	node = node->GetChildren();
	while (node)
	{
		if (node->GetName() == wxT("Keyword"))
		{
			keyword = new Keyword();
			keyword->word = node->GetAttribute(wxT("name")).Lower();
			keyword->desc = node->GetAttribute(wxT("desc"));
			keyword->type = type;
			_keywords.Add(keyword);
		}
		node = node->GetNext();
	}
}

wxString KeywordsStore::GetWords( KeywordType type ) const
{
	wxString result;

	size_t count = _keywords.GetCount(); 
	for (size_t idx = 0; idx < count; ++idx)
	{
		if (_keywords[idx].type == type)
			result += _keywords[idx].word + wxT(' ');
	}
	return result.Trim();
}

size_t KeywordsStore::GetCount()
{
	return _keywords.GetCount();
}

wxString KeywordsStore::GetWord( size_t index ) const
{
	if (_keywords.GetCount() == 0) return wxEmptyString;
	return _keywords[index].word;
}

wxString KeywordsStore::FindTip( const wxString &word ) const
{
	wxString result;
	int idx, first = 0;
	size_t count = _keywords.GetCount();
	if (count == 0) return result;
	int last = count - 1;
	wxString w = word.Lower();
	wxString str;
	do 
	{
		idx = first + (last - first) / 2;
		str = _keywords[idx].word;
		if(w.CmpNoCase(str) > 0)
			first = idx + 1;
		else if(w.CmpNoCase(str) < 0)
			last = idx - 1;
		else
		{
			result = _keywords[idx].desc;
			break;
		}
	} while (first <= last);
	return result;
}
