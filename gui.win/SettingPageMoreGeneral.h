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

//------------------------------------------------------------------------------
#ifndef SettingPageMoreGeneralH
#define SettingPageMoreGeneralH
//------------------------------------------------------------------------------
#include "SettingPage.h"
//---------------------------------------------------------------------------

class SettingPageMoreGeneral : public SettingPage {
public:
    bool bUpdateTextFiles, bUpdateRedirectAddress, bUpdateRegOnlyMessage, bUpdateShareLimitMessage, bUpdateSlotsLimitMessage, bUpdateHubSlotRatioMessage,
        bUpdateMaxHubsLimitMessage, bUpdateNoTagMessage, bUpdateTempBanRedirAddress, bUpdatePermBanRedirAddress, bUpdateNickLimitMessage;

    SettingPageMoreGeneral();
    ~SettingPageMoreGeneral() { };

    bool CreateSettingPage(HWND hOwner);

    void Save();
    void GetUpdates(bool & /*bUpdatedHubNameWelcome*/, bool & /*bUpdatedHubName*/, bool & /*bUpdatedTCPPorts*/, bool & /*bUpdatedUDPPort*/, bool & /*bUpdatedAutoReg*/,
        bool &/*bUpdatedMOTD*/, bool &/*bUpdatedHubSec*/, bool &bUpdatedRegOnlyMessage, bool &bUpdatedShareLimitMessage,
		bool &bUpdatedSlotsLimitMessage, bool &bUpdatedHubSlotRatioMessage, bool &bUpdatedMaxHubsLimitMessage, bool &bUpdatedNoTagMessage,
		bool &bUpdatedNickLimitMessage, bool &/*bUpdatedBotsSameNick*/, bool &/*bUpdatedBotNick*/, bool & /*bUpdatedBot*/, bool &/*bUpdatedOpChatNick*/,
		bool &/*bUpdatedOpChat*/, bool & /*bUpdatedLanguage*/, bool &bUpdatedTextFiles, bool &bUpdatedRedirectAddress, bool &bUpdatedTempBanRedirAddress,
        bool &bUpdatedPermBanRedirAddress, bool & /*bUpdatedSysTray*/, bool & /*bUpdatedScripting*/, bool & /*bUpdatedMinShare*/, bool & /*bUpdatedMaxShare*/);

    char * GetPageName();
    void FocusLastItem();
private:
    HWND hWndPageItems[21];
    
    enum enmPageItems {
        GB_TEXT_FILES,
        BTN_ENABLE_TEXT_FILES,
        BTN_SEND_TEXT_FILES_AS_PM,
        GB_PINGER,
        BTN_DONT_ALLOW_PINGER,
        BTN_REPORT_PINGER,
        GB_OWNER_EMAIL,
        EDT_OWNER_EMAIL,
        GB_MAIN_REDIR_ADDR,
        EDT_MAIN_REDIR_ADDR,
        BTN_REDIR_ALL,
        BTN_REDIR_HUB_FULL,
        GB_REG_ONLY_HUB,
        BTN_ALLOW_ONLY_REGS,
        GB_MSG_TO_NON_REGS,
        EDT_MSG_TO_NON_REGS,
        GB_NON_REG_REDIR,
        BTN_NON_REG_REDIR_ENABLE,
        EDT_NON_REG_REDIR_ADDR,
        GB_KILL_THAT_DUCK,
        BTN_KILL_THAT_DUCK
    };

    LRESULT SettingPageProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
//------------------------------------------------------------------------------

#endif