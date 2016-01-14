#pragma once
#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>

#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <stdafx.h>
#include <Communication.h>

//LPTSTR SlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");
// HANDLE hFile = NULL;

BOOL WriteSlot(interior_ptr<HANDLE>  hSlot, char* lpszMessage);

int inithandle(bool force, LPTSTR slotName,  interior_ptr<HANDLE>  hFile);

void updateSpeed(LPTSTR slotname, interior_ptr<HANDLE>   hFile, long x);