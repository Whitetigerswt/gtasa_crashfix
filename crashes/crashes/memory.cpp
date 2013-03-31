#include <windows.h>
#include "memory.h"

void MemSet ( void* dwDest, int cValue, int uiAmount )
{
    memset ( dwDest, cValue, uiAmount );
}

void MemCpy ( void* dwDest, const void* dwSrc, int uiAmount )
{
    memcpy ( dwDest, dwSrc, uiAmount );
}