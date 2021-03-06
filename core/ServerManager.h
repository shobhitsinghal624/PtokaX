/*
 * PtokaX - hub server for Direct Connect peer to peer network.

 * Copyright (C) 2002-2005  Ptaczek, Ptaczek at PtokaX dot org
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

//---------------------------------------------------------------------------
#ifndef ServerManagerH
#define ServerManagerH
//---------------------------------------------------------------------------
class ServerThread;
//---------------------------------------------------------------------------

class clsServerManager {
public:
    static string sPath, sScriptPath;

#ifdef _WIN32
	static string sLuaPath, sOS;
#endif

    static double daCpuUsage[60], dCpuUsage;

    static uint64_t ui64ActualTick, ui64TotalShare;
    static uint64_t ui64BytesRead, ui64BytesSent, ui64BytesSentSaved;
    static uint64_t ui64LastBytesRead, ui64LastBytesSent;
    static uint64_t ui64Mins, ui64Hours, ui64Days;

#ifdef _WIN32
	static HANDLE hConsole, hLuaHeap, hPtokaXHeap, hRecvHeap, hSendHeap;
#endif

#ifdef __MACH__
	static clock_serv_t csMachClock;
#endif

	static time_t tStartTime;

#if defined(_WIN32) && !defined(_WIN_IOT)
    static UINT_PTR sectimer;
    static UINT_PTR regtimer;
	#ifdef _BUILD_GUI
        static HINSTANCE hInstance;
        static HWND hWndActiveDialog;
	#endif
#endif

	static ServerThread * pServersS;

    static char * pGlobalBuffer;

	static size_t szGlobalBufferSize;

#ifndef _WIN32
    static uint32_t ui32CpuCount;
#endif

    static uint32_t ui32Joins, ui32Parts, ui32Logged, ui32Peak;
    static uint32_t ui32aUploadSpeed[60], ui32aDownloadSpeed[60];
    static uint32_t ui32ActualBytesRead, ui32ActualBytesSent;
    static uint32_t ui32AverageBytesRead, ui32AverageBytesSent;

    static bool bServerRunning, bServerTerminated, bIsRestart, bIsClose;

#ifdef _WIN32
	#ifndef _BUILD_GUI
        static bool bService;
	#endif
#else
    static bool bDaemon;
#endif

    static bool bCmdAutoStart, bCmdNoAutoStart, bCmdNoTray, bUseIPv4, bUseIPv6, bIPv6DualStack;

    static char sHubIP[16], sHubIP6[40];

    static uint8_t ui8SrCntr, ui8MinTick;

	static void OnSecTimer();
    static void OnRegTimer();

    static void Initialize();

    static void FinalStop(const bool &bDeleteServiceLoop);
    static void ResumeAccepts();
    static void SuspendAccepts(const uint32_t &ui32Time);
    static void UpdateAutoRegState();

    static bool Start();
    static void UpdateServers();
    static void Stop();
    static void FinalClose();
    static void CreateServerThread(const int &iAddrFamily, const uint16_t &ui16PortNumber, const bool &bResume = false);

	static void CommandLineSetup();

	static bool ResolveHubAddress(const bool &bSilent = false);
};
//--------------------------------------------------------------------------- 

#endif
