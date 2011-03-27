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
#ifndef RangeBansDialogH
#define RangeBansDialogH
//------------------------------------------------------------------------------
struct RangeBanItem;
//------------------------------------------------------------------------------

class RangeBansDialog {
public:
    HWND hWndWindowItems[8];

    enum enmWindowItems {
        WND_THIS,
        BTN_ADD_RANGE_BAN,
        LV_RANGE_BANS,
        GB_FILTER,
        EDT_FILTER,
        CB_FILTER,
        BTN_CLEAR_RANGE_TEMP_BANS,
        BTN_CLEAR_RANGE_PERM_BANS
    };

    RangeBansDialog();
    ~RangeBansDialog();

    static LRESULT CALLBACK StaticRangeBansDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static int CompareRangeBans(const void * pItem, const void * pOtherItem);
    static int CALLBACK SortCompareRangeBans(LPARAM lParam1, LPARAM lParam2, LPARAM /*lParamSort*/);

	void DoModal(HWND hWndParent);
	void FilterRangeBans();
	void AddRangeBan(const RangeBanItem * pRangeBan);
	void RemoveRangeBan(const RangeBanItem * pRangeBan);
private:
    string sFilterString;

    int iFilterColumn, iSortColumn;

    bool bSortAscending;

    LRESULT RangeBansDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void AddAllRangeBans();
    void OnColumnClick(const LPNMLISTVIEW &pListView);
    void RemoveRangeBans();
    void OnContextMenu(HWND hWindow, LPARAM lParam);
    bool FilterRangeBan(const RangeBanItem * pRangeBan);
};
//------------------------------------------------------------------------------
extern RangeBansDialog * pRangeBansDialog;
//------------------------------------------------------------------------------

#endif