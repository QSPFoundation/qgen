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

#include "Application.h"

IMPLEMENT_APP(Application)

bool Application::OnInit()
{
	InitEvent initEvent;
	wxInitAllImageHandlers();
	wxFileName appPath(wxStandardPaths::Get().GetExecutablePath());
	wxRegisterId(20000);
	_controls = new Controls(appPath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
	_controls->UpdateLocale(_controls->GetSettings()->GetLangId());
	MainFrame *mainFrame = new MainFrame(_controls);
	_controls->SetMainFrame(mainFrame);
	_controls->SetLocListBox(mainFrame->GetLocListBox());
	_controls->SetNotebook(mainFrame->GetNotebook());
	_controls->NewGame();
	mainFrame->UpdateTitle();
	mainFrame->Show();
	wxCmdLineParser cmdParser(argc, argv);
	if (argc > 1)
	{
		cmdParser.AddParam();
		cmdParser.Parse(false);
		wxFileName path(cmdParser.GetParam());
		path.MakeAbsolute();
		initEvent.SetInitString(path.GetFullPath());
		wxPostEvent(mainFrame, initEvent);
	}
	else
	{
		Settings *settings = _controls->GetSettings();
		if (settings->GetOpenLastGame())
		{
			initEvent.SetInitString(settings->GetLastGamePath());
			wxPostEvent(mainFrame, initEvent);
		}
	}
	return true;
}

int Application::OnExit()
{
	Settings *settings = _controls->GetSettings();
	settings->SetLastGamePath(_controls->GetGamePath());
	settings->SaveSettings();
	delete _controls;
	return wxApp::OnExit();
}
