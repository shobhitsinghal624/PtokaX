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
#ifndef BansDialogH
#define BansDialogH
//------------------------------------------------------------------------------
struct BanItem;
//------------------------------------------------------------------------------

class BansDialog {
public:
    HWND m_hWnd;

    HWND hWndWindowItems[7];

    enum enmWindowItems {
        BTN_ADD_BAN,
        LV_BANS,
        GB_FILTER,
        EDT_FILTER,
        CB_FILTER,
        BTN_CLEAR_TEMP_BANS,
        BTN_CLEAR_PERM_BANS
    };

    BansDialog();
    ~BansDialog();

    static LRESULT CALLBACK StaticBansDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static int CompareBans(const void * pItem, const void * pOtherItem);
    static int CALLBACK SortCompareBans(LPARAM lParam1, LPARAM lParam2, LPARAM /*lParamSort*/);

	void DoModal(HWND hWndParent);
	void FilterBans();
	void AddBan(const BanItem * pBan);
	void RemoveBan(const BanItem * pBan);
private:
    string sFilterString;

    int iFilterColumn, iSortColumn;

    bool bSortAscending;

    LRESULT BansDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void AddAllBans();
    void OnColumnClick(const LPNMLISTVIEW &pListView);
    void RemoveBans();
    void OnContextMenu(HWND hWindow, LPARAM lParam);
    bool FilterBan(const BanItem * pBan);
};
//------------------------------------------------------------------------------
extern BansDialog *pBansDialog;
//------------------------------------------------------------------------------

#endif