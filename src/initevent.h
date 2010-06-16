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

#ifndef _QGEN_INITEVENT_H_
	#define _QGEN_INITEVENT_H_

	#include <wx/wx.h>

	class InitEvent : public wxEvent
	{
	public:
		// C-tors / D-tor
		InitEvent();
		InitEvent(const InitEvent& event);

		// Accessors
		wxString GetInitString() const { return _initString; }
		void SetInitString(const wxString& str) { _initString = str; }

		// Overloaded methods
		virtual wxEvent *Clone() const
		{
			return new InitEvent(*this);
		}
	protected:
		// Fields
		wxString _initString;
	private:
		DECLARE_DYNAMIC_CLASS(InitEvent)
	};

	DECLARE_EVENT_TYPE(EVT_INIT, 0)

	typedef void (wxEvtHandler::*InitEventFunction)(InitEvent&);

	#define InitEventHandler(func) \
		(wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(InitEventFunction, &func)

	#define EVT_INIT(func) \
		wx__DECLARE_EVT0(EVT_INIT, InitEventHandler(func))

#endif
