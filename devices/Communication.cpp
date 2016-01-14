#include <windows.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <stdafx.h>
#include <Communication.h>

//LPTSTR SlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");
// HANDLE hFile = NULL;

BOOL WriteSlot(interior_ptr<HANDLE> hSlot, char* lpszMessage)
{
	BOOL fResult;
	DWORD cbWritten;

	fResult = WriteFile(*hSlot,
		lpszMessage,
		(DWORD)(strlen(lpszMessage) + 1)*sizeof(char),
		&cbWritten,
		(LPOVERLAPPED)NULL);

	if (!fResult)
	{
		printf("WriteFile failed with %d.\n", GetLastError());
		return FALSE;
	}

	printf("Slot written to successfully.\n");

	return TRUE;
}


int inithandle(bool force, LPTSTR slotName, interior_ptr<HANDLE>  hFile)
{

	if (!force) {
		if (*hFile != NULL && *hFile != INVALID_HANDLE_VALUE)
		{
			return TRUE;
		}
	}


	CloseHandle(*hFile);
	
	HANDLE z = CreateFile(TEXT("\\\\.\\mailslot\\sample_mailslot"),
		GENERIC_WRITE,
		FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);


	if (z == INVALID_HANDLE_VALUE)
	{
		char buf[10000];
		sprintf(buf, "CreateFile failed with %d \n", GetLastError(),&hFile);
		return FALSE;
	}

	*hFile = z;


	return TRUE;
}

void updateSpeed(LPTSTR slotname, interior_ptr<HANDLE>  hFile, long x) {
	inithandle(true,slotname,hFile);

	char buf[300]; // where you put result
	sprintf(buf, "%d", (int)x);
	WriteSlot(hFile, buf);

}