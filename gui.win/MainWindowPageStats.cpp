/*
 * PtokaX - hub server for Direct Connect peer to peer network.

 * Copyright (C) 2002-2005  Ptaczek, Ptaczek at PtokaX dot org
 * Copyright (C) 2004-2010  Petr Kozelka, PPK at PtokaX dot org

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

//---------------------------------------------------------------------------
#include "../core/stdinc.h"
//---------------------------------------------------------------------------
#include "MainWindowPageStats.h"
//---------------------------------------------------------------------------
#include "../core/colUsers.h"
#include "../core/globalQueue.h"
#include "../core/LanguageManager.h"
#include "../core/ServerManager.h"
#include "../core/SettingManager.h"
#include "../core/UdpDebug.h"
#include "../core/User.h"
#include "../core/utility.h"
//---------------------------------------------------------------------------
#ifdef _WIN32
	#pragma hdrstop
#endif
//---------------------------------------------------------------------------
#include "LineDialog.h"
//---------------------------------------------------------------------------

MainWindowPageStats::MainWindowPageStats() {
    memset(&hWndPageItems, 0, (sizeof(hWndPageItems) / sizeof(hWndPageItems[0])) * sizeof(HWND));
}
//---------------------------------------------------------------------------

LRESULT MainWindowPageStats::MainWindowPageProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
        case WM_SETFOCUS:
            ::SetFocus(hWndPageItems[BTN_START_STOP]);

            return 0;
        case WM_COMMAND:
           switch(LOWORD(wParam))
                case BTN_START_STOP:
                    if(bServerRunning == false) {
                        if(ServerStart() == false) {
                            ::SetWindowText(hWndPageItems[LBL_STATUS_VALUE],
                                (string(LanguageManager->sTexts[LAN_READY], (size_t)LanguageManager->ui16TextsLens[LAN_READY])+".").c_str());
                        }
                    } else {
                        ServerStop();
                    }
                    return 0;
                case BTN_REDIRECT_ALL:
                    OnRedirectAll();
                    return 0;
                case BTN_MASS_MSG:
                    OnMassMessage();
                    return 0;
            break;
        case WM_WINDOWPOSCHANGED: {
            int iX = ((WINDOWPOS*)lParam)->cx;

            ::SetWindowPos(hWndPageItems[BTN_START_STOP], NULL, 0, 0, iX-10, 40, SWP_NOMOVE | SWP_NOZORDER);
            ::SetWindowPos(hWndPageItems[GB_STATS], NULL, 0, 0, iX-12, 138, SWP_NOMOVE | SWP_NOZORDER);
            ::SetWindowPos(hWndPageItems[LBL_STATUS_VALUE], NULL, 0, 0, iX-169, 14, SWP_NOMOVE | SWP_NOZORDER);
            ::SetWindowPos(hWndPageItems[LBL_JOINS_VALUE], NULL, 0, 0, iX-169, 14, SWP_NOMOVE | SWP_NOZORDER);
            ::SetWindowPos(hWndPageItems[LBL_PARTS_VALUE], NULL, 0, 0, iX-169, 14, SWP_NOMOVE | SWP_NOZORDER);
            ::SetWindowPos(hWndPageItems[LBL_ACTIVE_VALUE], NULL, 0, 0, iX-169, 14, SWP_NOMOVE | SWP_NOZORDER);
            ::SetWindowPos(hWndPageItems[LBL_ONLINE_VALUE], NULL, 0, 0, iX-169, 14, SWP_NOMOVE | SWP_NOZORDER);
            ::SetWindowPos(hWndPageItems[LBL_PEAK_VALUE], NULL, 0, 0, iX-169, 14, SWP_NOMOVE | SWP_NOZORDER);
            ::SetWindowPos(hWndPageItems[LBL_RECEIVED_VALUE], NULL, 0, 0, iX-169, 14, SWP_NOMOVE | SWP_NOZORDER);
            ::SetWindowPos(hWndPageItems[LBL_SENT_VALUE], NULL, 0, 0, iX-169, 14, SWP_NOMOVE | SWP_NOZORDER);
            ::SetWindowPos(hWndPageItems[BTN_REDIRECT_ALL], NULL, 0, 0, iX-10, 23, SWP_NOMOVE | SWP_NOZORDER);
            ::SetWindowPos(hWndPageItems[BTN_MASS_MSG], NULL, 0, 0, iX-10, 23, SWP_NOMOVE | SWP_NOZORDER);

            return 0;
        }
    }

	return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}
//------------------------------------------------------------------------------

bool MainWindowPageStats::CreateMainWindowPage(HWND hOwner) {
    CreateHWND(hOwner);

    RECT rcMain;
    ::GetClientRect(m_hWnd, &rcMain);

    hWndPageItems[BTN_START_STOP] = ::CreateWindowEx(0, WC_BUTTON, bServerRunning == false ? LanguageManager->sTexts[LAN_START_HUB] : LanguageManager->sTexts[LAN_STOP_HUB],
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, 5, 4, rcMain.right-10, 40, m_hWnd, (HMENU)BTN_START_STOP, g_hInstance, NULL);

    hWndPageItems[GB_STATS] = ::CreateWindowEx(WS_EX_TRANSPARENT, WC_BUTTON, "", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        6, 43, rcMain.right-12, 138, m_hWnd, NULL, g_hInstance, NULL);

    hWndPageItems[LBL_STATUS] = ::CreateWindowEx(0, WC_STATIC, (string(LanguageManager->sTexts[LAN_STATUS], (size_t)LanguageManager->ui16TextsLens[LAN_STATUS])+":").c_str(),
        WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP, 6, 12, 140, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_STATUS_VALUE] = ::CreateWindowEx(0, WC_STATIC, (string(LanguageManager->sTexts[LAN_READY], (size_t)LanguageManager->ui16TextsLens[LAN_READY])+".").c_str(),
        WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP, 151, 12, rcMain.right-169, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_JOINS] = ::CreateWindowEx(0, WC_STATIC,
        (string(LanguageManager->sTexts[LAN_ACCEPTED_CONNECTIONS], (size_t)LanguageManager->ui16TextsLens[LAN_ACCEPTED_CONNECTIONS])+":").c_str(),
        WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP, 6, 27, 140, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_JOINS_VALUE] = ::CreateWindowEx(0, WC_STATIC, "0", WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP,
        151, 27, rcMain.right-169, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_PARTS] = ::CreateWindowEx(0, WC_STATIC,
        (string(LanguageManager->sTexts[LAN_CLOSED_CONNECTIONS], (size_t)LanguageManager->ui16TextsLens[LAN_CLOSED_CONNECTIONS])+":").c_str(),
        WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP, 6, 42, 140, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_PARTS_VALUE] = ::CreateWindowEx(0, WC_STATIC, "0", WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP,
        151, 42, rcMain.right-169, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_ACTIVE] = ::CreateWindowEx(0, WC_STATIC,
        (string(LanguageManager->sTexts[LAN_ACTIVE_CONNECTIONS], (size_t)LanguageManager->ui16TextsLens[LAN_ACTIVE_CONNECTIONS])+":").c_str(),
        WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP, 6, 57, 140, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_ACTIVE_VALUE] = ::CreateWindowEx(0, WC_STATIC, "0", WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP,
        151, 57, rcMain.right-169, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_ONLINE] = ::CreateWindowEx(0, WC_STATIC,
        (string(LanguageManager->sTexts[LAN_USERS_ONLINE], (size_t)LanguageManager->ui16TextsLens[LAN_USERS_ONLINE])+":").c_str(),
        WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP, 6, 72, 140, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_ONLINE_VALUE] = ::CreateWindowEx(0, WC_STATIC, "0", WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP,
        151, 72, rcMain.right-169, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_PEAK] = ::CreateWindowEx(0, WC_STATIC,
        (string(LanguageManager->sTexts[LAN_USERS_PEAK], (size_t)LanguageManager->ui16TextsLens[LAN_USERS_PEAK])+":").c_str(),
        WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP, 6, 87, 140, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_PEAK_VALUE] = ::CreateWindowEx(0, WC_STATIC, "0", WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP,
        151, 87, rcMain.right-169, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_RECEIVED] = ::CreateWindowEx(0, WC_STATIC,
        (string(LanguageManager->sTexts[LAN_RECEIVED], (size_t)LanguageManager->ui16TextsLens[LAN_RECEIVED])+":").c_str(),
        WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP, 6, 102, 140, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_RECEIVED_VALUE] = ::CreateWindowEx(0, WC_STATIC, "0 B (0 B/s)", WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP,
        151, 102, rcMain.right-169, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_SENT] = ::CreateWindowEx(0, WC_STATIC,
        (string(LanguageManager->sTexts[LAN_SENT], (size_t)LanguageManager->ui16TextsLens[LAN_SENT])+":").c_str(),
        WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP, 6, 117, 140, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[LBL_SENT_VALUE] = ::CreateWindowEx(0, WC_STATIC, "0 B (0 B/s)", WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFTNOWORDWRAP,
        151, 117, rcMain.right-169, 14, hWndPageItems[GB_STATS], NULL, g_hInstance, NULL);

    hWndPageItems[BTN_REDIRECT_ALL] = ::CreateWindowEx(0, WC_BUTTON, LanguageManager->sTexts[LAN_REDIRECT_ALL_USERS], WS_CHILD | WS_VISIBLE | WS_DISABLED | WS_TABSTOP | BS_PUSHBUTTON,
        5, 186, rcMain.right-10, 23, m_hWnd, (HMENU)BTN_REDIRECT_ALL, g_hInstance, NULL);

    hWndPageItems[BTN_MASS_MSG] = ::CreateWindowEx(0, WC_BUTTON, LanguageManager->sTexts[LAN_MASS_MSG], WS_CHILD | WS_VISIBLE | WS_DISABLED | WS_TABSTOP | BS_PUSHBUTTON,
        5, 213, rcMain.right-10, 23, m_hWnd, (HMENU)BTN_MASS_MSG, g_hInstance, NULL);

    for(uint8_t ui8i = 0; ui8i < (sizeof(hWndPageItems) / sizeof(hWndPageItems[0])); ui8i++) {
        if(hWndPageItems[ui8i] == NULL) {
            return false;
        }

        ::SendMessage(hWndPageItems[ui8i], WM_SETFONT, (WPARAM)hfFont, MAKELPARAM(TRUE, 0));
    }

	return true;
}
//------------------------------------------------------------------------------

void MainWindowPageStats::UpdateLanguage() {
	if(bServerRunning == false) {
        ::SetWindowText(hWndPageItems[BTN_START_STOP], LanguageManager->sTexts[LAN_START_HUB]);
        ::SetWindowText(hWndPageItems[LBL_STATUS_VALUE], (string(LanguageManager->sTexts[LAN_READY], (size_t)LanguageManager->ui16TextsLens[LAN_READY])+".").c_str());
    } else {
        ::SetWindowText(hWndPageItems[BTN_START_STOP], LanguageManager->sTexts[LAN_STOP_HUB]);
        ::SetWindowText(hWndPageItems[LBL_STATUS_VALUE], (string(LanguageManager->sTexts[LAN_RUNNING], (size_t)LanguageManager->ui16TextsLens[LAN_RUNNING])+"...").c_str());
    }

    ::SetWindowText(hWndPageItems[LBL_STATUS], (string(LanguageManager->sTexts[LAN_STATUS], (size_t)LanguageManager->ui16TextsLens[LAN_STATUS])+":").c_str());
    ::SetWindowText(hWndPageItems[LBL_JOINS], (string(LanguageManager->sTexts[LAN_ACCEPTED_CONNECTIONS], (size_t)LanguageManager->ui16TextsLens[LAN_ACCEPTED_CONNECTIONS])+":").c_str());
    ::SetWindowText(hWndPageItems[LBL_PARTS], (string(LanguageManager->sTexts[LAN_CLOSED_CONNECTIONS], (size_t)LanguageManager->ui16TextsLens[LAN_CLOSED_CONNECTIONS])+":").c_str());
    ::SetWindowText(hWndPageItems[LBL_ACTIVE], (string(LanguageManager->sTexts[LAN_ACTIVE_CONNECTIONS], (size_t)LanguageManager->ui16TextsLens[LAN_ACTIVE_CONNECTIONS])+":").c_str());
    ::SetWindowText(hWndPageItems[LBL_ONLINE], (string(LanguageManager->sTexts[LAN_USERS_ONLINE], (size_t)LanguageManager->ui16TextsLens[LAN_USERS_ONLINE])+":").c_str());
    ::SetWindowText(hWndPageItems[LBL_PEAK], (string(LanguageManager->sTexts[LAN_USERS_PEAK], (size_t)LanguageManager->ui16TextsLens[LAN_USERS_PEAK])+":").c_str());
    ::SetWindowText(hWndPageItems[LBL_RECEIVED], (string(LanguageManager->sTexts[LAN_RECEIVED], (size_t)LanguageManager->ui16TextsLens[LAN_RECEIVED])+":").c_str());
    ::SetWindowText(hWndPageItems[LBL_SENT], (string(LanguageManager->sTexts[LAN_SENT], (size_t)LanguageManager->ui16TextsLens[LAN_SENT])+":").c_str());

    ::SetWindowText(hWndPageItems[BTN_REDIRECT_ALL], LanguageManager->sTexts[LAN_REDIRECT_ALL_USERS]);
    ::SetWindowText(hWndPageItems[BTN_MASS_MSG], LanguageManager->sTexts[LAN_MASS_MSG]);
}
//---------------------------------------------------------------------------

char * MainWindowPageStats::GetPageName() {
    return LanguageManager->sTexts[LAN_STATS];
}
//------------------------------------------------------------------------------

void OnRedirectAllOk(char * sLine, const int &iLen) {
    char *sMSG = (char *) HeapAlloc(hPtokaXHeap, HEAP_NO_SERIALIZE, iLen+16);
    if(sMSG == NULL) {
		string sDbgstr = "[BUF] Cannot allocate "+string(iLen+16)+
			" bytes of memory for sMSG in OnRedirectAllOk! "+string(HeapValidate(hPtokaXHeap, HEAP_NO_SERIALIZE, 0))+GetMemStat();
		AppendSpecialLog(sDbgstr);
        return;
    }

    int imsgLen = sprintf(sMSG, "$ForceMove %s|", sLine);
    if(CheckSprintf(imsgLen, iLen+16, "OnRedirectAllOk") == false) {
        return;
    }

    User *next = colUsers->llist;
    while(next != NULL) {
        User *curUser = next;
		next = curUser->next;
        UserSendChar(curUser, sMSG, imsgLen);
        // PPK ... close by hub needed !
        UserClose(curUser, true);
    }

    if(HeapFree(hPtokaXHeap, HEAP_NO_SERIALIZE, (void *)sMSG) == 0) {
		string sDbgstr = "[BUF] Cannot deallocate sMSG in OnRedirectAllOk! "+string((uint32_t)GetLastError())+" "+
			string(HeapValidate(hPtokaXHeap, HEAP_NO_SERIALIZE, 0));
		AppendSpecialLog(sDbgstr);
	}
}
//---------------------------------------------------------------------------

void MainWindowPageStats::OnRedirectAll() {
    UdpDebug->Broadcast("[SYS] Redirect All.");

	LineDialog * RedirectAllDlg = new LineDialog(&OnRedirectAllOk);
	RedirectAllDlg->DoModal(::GetParent(m_hWnd), LanguageManager->sTexts[LAN_REDIRECT_ALL_USERS_TO],
        SettingManager->sTexts[SETTXT_REDIRECT_ADDRESS] == NULL ? "" : SettingManager->sTexts[SETTXT_REDIRECT_ADDRESS]);
}
//---------------------------------------------------------------------------

void OnMassMessageOk(char * sLine, const int &iLen) {
    char *sMSG = (char *) HeapAlloc(hPtokaXHeap, HEAP_NO_SERIALIZE, iLen+256);
    if(sMSG == NULL) {
		string sDbgstr = "[BUF] Cannot allocate "+string(iLen+256)+
			" bytes of memory for sMSG in OnMassMessageOk! "+string(HeapValidate(hPtokaXHeap, HEAP_NO_SERIALIZE, 0))+GetMemStat();
		AppendSpecialLog(sDbgstr);
        return;
    }

    int imsgLen = sprintf(sMSG, "%s $<%s> %s|",
        SettingManager->bBools[SETBOOL_REG_BOT] == false ? SettingManager->sTexts[SETTXT_ADMIN_NICK] : SettingManager->sTexts[SETTXT_BOT_NICK],
        SettingManager->bBools[SETBOOL_REG_BOT] == false ? SettingManager->sTexts[SETTXT_ADMIN_NICK] : SettingManager->sTexts[SETTXT_BOT_NICK],
        sLine);
    if(CheckSprintf(imsgLen, iLen+256, "OnMassMessageOk") == false) {
        return;
    }

    QueueDataItem * newItem = globalQ->CreateQueueDataItem(sMSG, imsgLen, NULL, 0, globalqueue::PM2ALL);

    if(HeapFree(hPtokaXHeap, HEAP_NO_SERIALIZE, (void *)sMSG) == 0) {
		string sDbgstr = "[BUF] Cannot deallocate sMSG in OnMassMessageOk! "+string((uint32_t)GetLastError())+" "+
			string(HeapValidate(hPtokaXHeap, HEAP_NO_SERIALIZE, 0));
		AppendSpecialLog(sDbgstr);
	}

    globalQ->SingleItemsStore(newItem);
}

//---------------------------------------------------------------------------

void MainWindowPageStats::OnMassMessage() {
	LineDialog * MassMsgDlg = new LineDialog(&OnMassMessageOk);
	MassMsgDlg->DoModal(::GetParent(m_hWnd), LanguageManager->sTexts[LAN_MASS_MSG], "");
}
//---------------------------------------------------------------------------