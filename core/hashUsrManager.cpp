/*
 * PtokaX - hub server for Direct Connect peer to peer network.

 * Copyright (C) 2002-2005  Ptaczek, Ptaczek at PtokaX dot org
 * Copyright (C) 2004-2012  Petr Kozelka, PPK at PtokaX dot org

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
#include "stdinc.h"
//---------------------------------------------------------------------------
#include "hashUsrManager.h"
//---------------------------------------------------------------------------
#include "User.h"
#include "utility.h"
//---------------------------------------------------------------------------
#ifdef _WIN32
	#pragma hdrstop
#endif
//---------------------------------------------------------------------------
hashMan *hashManager = NULL;
//---------------------------------------------------------------------------

hashMan::hashMan() {
    for(uint32_t ui32i = 0; ui32i < 65536; ui32i++) {
        nicktable[ui32i] = NULL;
        iptable[ui32i] = NULL;
    }

    //Memo("hashManager created");
}
//---------------------------------------------------------------------------

hashMan::~hashMan() {
    //Memo("hashManager destroyed");
    for(uint32_t ui32i = 0; ui32i < 65536; ui32i++) {
		IpTableItem * next = iptable[ui32i];
        
        while(next != NULL) {
            IpTableItem * cur = next;
            next = cur->next;
        
            delete cur;
		}
    }
}
//---------------------------------------------------------------------------

void hashMan::Add(User * u) {
    uint16_t ui16dx = 0;
    memcpy(&ui16dx, &u->ui32NickHash, sizeof(uint16_t));

    if(nicktable[ui16dx] != NULL) {
        nicktable[ui16dx]->hashtableprev = u;
        u->hashtablenext = nicktable[ui16dx];
    }

    nicktable[ui16dx] = u;

    ui16dx = *((uint16_t *)(u->ui128IpHash+13));

    if(iptable[ui16dx] == NULL) {
        iptable[ui16dx] = new IpTableItem();

        if(iptable[ui16dx] == NULL) {
            u->ui32BoolBits |= User::BIT_ERROR;
            UserClose(u);

            AppendDebugLog("%s - [MEM] Cannot allocate IpTableItem in hashMan::Add\n", 0);
            return;
        }

        iptable[ui16dx]->next = NULL;
        iptable[ui16dx]->prev = NULL;

        iptable[ui16dx]->FirstUser = u;
		iptable[ui16dx]->ui16Count = 1;

        return;
    }

    IpTableItem * next = iptable[ui16dx];

    while(next != NULL) {
        IpTableItem * cur = next;
        next = cur->next;

        if(memcmp(cur->FirstUser->ui128IpHash, u->ui128IpHash, 16) == 0) {
            cur->FirstUser->hashiptableprev = u;
            u->hashiptablenext = cur->FirstUser;
            cur->FirstUser = u;
			cur->ui16Count++;

            return;
        }
    }

    IpTableItem * cur = new IpTableItem();

    if(cur == NULL) {
        u->ui32BoolBits |= User::BIT_ERROR;
        UserClose(u);

		AppendDebugLog("%s - [MEM] Cannot allocate IpTableItem2 in hashMan::Add\n", 0);
		return;
    }

    cur->FirstUser = u;
	cur->ui16Count = 1;

    cur->next = iptable[ui16dx];
    cur->prev = NULL;

    iptable[ui16dx]->prev = cur;
    iptable[ui16dx] = cur;
}
//---------------------------------------------------------------------------

void hashMan::Remove(User * u) {
    if(u->hashtableprev == NULL) {
        uint16_t ui16dx = 0;
        memcpy(&ui16dx, &u->ui32NickHash, sizeof(uint16_t));

        if(u->hashtablenext == NULL) {
            nicktable[ui16dx] = NULL;
        } else {
            u->hashtablenext->hashtableprev = NULL;
            nicktable[ui16dx] = u->hashtablenext;
        }
    } else if(u->hashtablenext == NULL) {
        u->hashtableprev->hashtablenext = NULL;
    } else {
        u->hashtableprev->hashtablenext = u->hashtablenext;
        u->hashtablenext->hashtableprev = u->hashtableprev;
    }

    u->hashtableprev = NULL;
    u->hashtablenext = NULL;

	uint16_t ui16dx = *((uint16_t *)(u->ui128IpHash+13));

	if(u->hashiptableprev == NULL) {
        IpTableItem * next = iptable[ui16dx];
    
        while(next != NULL) {
            IpTableItem * cur = next;
            next = cur->next;

            if(memcmp(cur->FirstUser->ui128IpHash, u->ui128IpHash, 16) == 0) {
				cur->ui16Count--;

                if(u->hashiptablenext == NULL) {
                    if(cur->prev == NULL) {
                        if(cur->next == NULL) {
                            delete cur;
                            iptable[ui16dx] = NULL;
                        } else {
                            cur->next->prev = NULL;
                            iptable[ui16dx] = cur->next;
                        }
                    } else if(cur->next == NULL) {
                        cur->prev->next = NULL;
                    } else {
                        cur->prev->next = cur->next;
                        cur->next->prev = cur->prev;
                    }
                } else {
                    u->hashiptablenext->hashiptableprev = NULL;
                    cur->FirstUser = u->hashiptablenext;
                }

                u->hashiptableprev = NULL;
                u->hashiptablenext = NULL;

                return;
            }
        }
    } else if(u->hashiptablenext == NULL) {
        u->hashiptableprev->hashiptablenext = NULL;
    } else {
        u->hashiptableprev->hashiptablenext = u->hashiptablenext;
        u->hashiptablenext->hashiptableprev = u->hashiptableprev;
    }

    u->hashiptableprev = NULL;
    u->hashiptablenext = NULL;

    IpTableItem * next = iptable[ui16dx];

    while(next != NULL) {
        IpTableItem * cur = next;
        next = cur->next;

        if(memcmp(cur->FirstUser->ui128IpHash, u->ui128IpHash, 16) == 0) {
			cur->ui16Count--;

            return;
        }
    }
}
//---------------------------------------------------------------------------

User * hashMan::FindUser(char * sNick, const size_t &szNickLen) {
    uint32_t ui32Hash = HashNick(sNick, szNickLen);

    uint16_t ui16dx = 0;
    memcpy(&ui16dx, &ui32Hash, sizeof(uint16_t));

    User *next = nicktable[ui16dx];

    // pointer exists ? Then we need look for nick 
    if(next != NULL) {
        while(next != NULL) {
            User *cur = next;
            next = cur->hashtablenext;

            // we are looking for duplicate string
			if(cur->ui32NickHash == ui32Hash && cur->ui8NickLen == szNickLen && strcasecmp(cur->sNick, sNick) == 0) {
                return cur;
            }
        }            
    }
    
    // no equal hash found, we dont have the nick in list
    return NULL;
}
//---------------------------------------------------------------------------

User * hashMan::FindUser(User * u) {
    uint16_t ui16dx = 0;
    memcpy(&ui16dx, &u->ui32NickHash, sizeof(uint16_t));

    User *next = nicktable[ui16dx];  

    // pointer exists ? Then we need look for nick
    if(next != NULL) { 
        while(next != NULL) {
            User *cur = next;
            next = cur->hashtablenext;

            // we are looking for duplicate string
			if(cur->ui32NickHash == u->ui32NickHash && cur->ui8NickLen == u->ui8NickLen && strcasecmp(cur->sNick, u->sNick) == 0) {
                return cur;
            }
        }            
    }
    
    // no equal hash found, we dont have the nick in list
    return NULL;
}
//---------------------------------------------------------------------------

User * hashMan::FindUser(const uint8_t * ui128IpHash) {
    uint16_t ui16dx = *((uint16_t *)(ui128IpHash+13));

	IpTableItem * next = iptable[ui16dx];

    while(next != NULL) {
		IpTableItem * cur = next;
        next = cur->next;

        if(memcmp(cur->FirstUser->ui128IpHash, ui128IpHash, 16) == 0) {
            return cur->FirstUser;
        }
    }

    return NULL;
}
//---------------------------------------------------------------------------

uint32_t hashMan::GetUserIpCount(User * u) {
    uint16_t ui16dx = *((uint16_t *)(u->ui128IpHash+13));

	IpTableItem * next = iptable[ui16dx];

	while(next != NULL) {
		IpTableItem * cur = next;
		next = cur->next;

        if(memcmp(cur->FirstUser->ui128IpHash, u->ui128IpHash, 16) == 0) {
            return cur->ui16Count;
        }
	}

	return 0;
}
//---------------------------------------------------------------------------
