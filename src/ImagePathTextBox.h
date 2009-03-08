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

#ifndef _IMAGEPATH_TEXTBOX_H
	#define _IMAGEPATH_TEXTBOX_H

	#include <wx/wx.h>
	#include "ILocationPage.h"
	#include "IControls.h"

	class ImagePathTextBox : public wxTextCtrl, public IObserver
	{
		DECLARE_CLASS(ImagePathTextBox)
		DECLARE_EVENT_TABLE()
	private:
		ILocationPage *_locPage;
		IControls *_controls;

		void OnLostFocus(wxFocusEvent &event);
		void OnKeyDown(wxKeyEvent& event);
	public:
		ImagePathTextBox( wxWindow *owner, wxWindowID id, ILocationPage *locPage, IControls *controls );
		~ImagePathTextBox();

		void Update(bool isFromObservable = false);
	};

#endif
