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
        wxString    word;
        wxString    desc;
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
        KeywordsArray _keywords;

        void ParseKeywords(wxXmlNode *node, KeywordType type);
    };
#endif
