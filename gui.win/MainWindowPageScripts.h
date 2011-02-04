/*
 * PtokaX - hub server for Direct Connect peer to peer network.

 * Copyright (C) 2002-2005  Ptaczek, Ptaczek at PtokaX dot org
 * Copyright (C) 2004-2011  Petr Kozelka, PPK at PtokaX dot org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3
 * as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//------------------------------------------------------------------------------
#ifndef MainWindowPageScriptsH
#define MainWindowPageScriptsH
//------------------------------------------------------------------------------
#include "MainWindowPage.h"
//------------------------------------------------------------------------------
class ScriptEditorDialog;
//------------------------------------------------------------------------------

class MainWindowPageScripts : public MainWindowPage {
public:
    HWND hWndPageItems[8];

    enum enmPageItems {
        GB_SCRIPTS_ERRORS,
        REDT_SCRIPTS_ERRORS,
        BTN_OPEN_SCRIPT_EDITOR,
        BTN_REFRESH_SCRIPTS,
        LV_SCRIPTS,
        BTN_MOVE_UP,
        BTN_MOVE_DOWN,
        BTN_RESTART_SCRIPTS
    };

    MainWindowPageScripts();
    ~MainWindowPageScripts();

    bool CreateMainWindowPage(HWND hOwner);
    void UpdateLanguage();
    char * GetPageName();

    void ClearMemUsageAll();
    void UpdateMemUsage();
    void MoveScript(uint8_t ui8ScriptId, const bool &bUp);
    void AddScriptsToList(const bool &bDelete);
    void ScriptToList(const uint8_t &ui8ScriptId, const bool &bInsert, const bool &bSelected);
    void UpdateCheck(const uint8_t &ui8ScriptId);
private:
    bool bIgnoreItemChanged;

    LRESULT MainWindowPageProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void OnContextMenu(HWND hWindow, LPARAM lParam);
    ScriptEditorDialog * OpenScriptEditor();
    void RefreshScripts();
    void OnItemChanged(const LPNMLISTVIEW &pListView);
    void OnDoubleClick(const LPNMITEMACTIVATE &pItemActivate);
    void MoveUp();
    void MoveDown();
    void RestartScripts();
    void UpdateUpDown();
    void OpenInExternalEditor();
    void OpenInScriptEditor();
    void DeleteScript();
    void ClearMemUsage(uint8_t ui8ScriptId);
};
//------------------------------------------------------------------------------
extern MainWindowPageScripts *pMainWindowPageScripts;
//---------------------------------------------------------------------------

#endif