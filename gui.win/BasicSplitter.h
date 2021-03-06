/*
 * PtokaX - hub server for Direct Connect peer to peer network.

 * Copyright (C) 2004-2015  Petr Kozelka, PPK at PtokaX dot org

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

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef BasicSplitterH
#define BasicSplitterH
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class BasicSplitter {
public:
    RECT rcSplitter;

    int iSplitterPos, iPercentagePos;

    BasicSplitter();
    virtual ~BasicSplitter() { }

    bool BasicSplitterProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void SetSplitterRect(const LPRECT &lpRect);
private:
    bool bUpdatePercentagePos;

    BasicSplitter(const BasicSplitter&);
    const BasicSplitter& operator=(const BasicSplitter&);

    virtual HWND GetWindowHandle() = NULL;
    virtual void UpdateSplitterParts() = NULL;

	bool OnMouseMove(WPARAM wParam, LPARAM lParam);
	void OnLButtonDown(LPARAM lParam);
	static void OnLButtonUp();

    void SetSplitterPosition(int iPos, const bool &bUpdate = true);
	bool IsCursorOverSplitter(const int &iX, const int &iY) const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif
