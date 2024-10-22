// Copyright (C) 2005-2012
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

#include "application.h"

IMPLEMENT_APP(Application)

bool Application::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    wxLog::EnableLogging(true);
    delete wxLog::SetActiveTarget(new wxLogStderr());

    wxInitAllImageHandlers();
    wxRegisterId(20000);
    _controls = new Controls();
    _controls->UpdateLocale(_controls->GetSettings()->GetLangId());
    _controls->GetSettings()->PostInitLocaleSettings();
    MainFrame *mainFrame = new MainFrame(_controls);
    _controls->SetMainFrame(mainFrame);
    _controls->SetLocListBox(mainFrame->GetLocListBox());
    _controls->SetNotebook(mainFrame->GetNotebook());
    _controls->NewGame();
    mainFrame->UpdateTitle();
    mainFrame->Show();

    InitEvent initEvent;
    if (!_gameFile.IsEmpty())
    {
        wxFileName path(_gameFile);
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

void Application::OnInitCmdLine(wxCmdLineParser &parser)
{
    wxApp::OnInitCmdLine(parser);

    parser.AddParam("game file",
                    wxCMD_LINE_VAL_STRING,
                    wxCMD_LINE_PARAM_OPTIONAL);
}

bool Application::OnCmdLineParsed(wxCmdLineParser &parser)
{
    if (!wxApp::OnCmdLineParsed(parser))
        return false;

    if (parser.GetParamCount() > 0)
        _gameFile = parser.GetParam();
    return true;
}
