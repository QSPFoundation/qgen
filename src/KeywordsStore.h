#ifndef _QGEN_KEYWORDS_STORE_H_
	#define _QGEN_KEYWORDS_STORE_H_
	
	#include <wx/wx.h>
	#include <wx/xml/xml.h>

	enum KeywordType
	{
		STATEMENT,
		EXPRESSION,
		VARIABLE,
	};

	struct Keyword
	{
		wxString	word;
		wxString	desc;
		KeywordType type;

		Keyword() { }
		Keyword(const wxString &word, const wxString &desc, KeywordType type)
		{
			this->word = word;
			this->desc = desc;
			this->type = type;
		}
	};

	WX_DECLARE_OBJARRAY(Keyword, KeywordsArray);

	class KeywordsStore
	{
	public:
		KeywordsStore() { }
		bool Load(const wxString &filename);
		wxString GetWords(KeywordType type) const;
		size_t GetCount();
		wxString GetWord(size_t index) const;
		wxString FindTip(const wxString &word) const;
	private:
		KeywordsArray	_keywords;
		
		void ParseKeywords(wxXmlNode *node, KeywordType type);
	};
#endif
