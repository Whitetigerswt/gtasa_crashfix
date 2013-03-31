#include <Windows.h>

void MemSet ( void* dwDest, int cValue, int uiAmount );
void MemCpy ( void* dwDest, const void* dwSrc, int uiAmount );

template < class T, class U >
void MemPut ( U ptr, const T value )
{
    if ( *(T*)ptr != value )
        MemCpy ( (void*)ptr, &value, sizeof ( T ) );
}

//
// Use Mem*Fast for the remaining memory regions
//

inline
void MemCpyFast ( void* dwDest, const void* dwSrc, int uiAmount )
{
    memcpy ( dwDest, dwSrc, uiAmount );
}

inline
void MemSetFast ( void* dwDest, int cValue, int uiAmount )
{
    memset ( dwDest, cValue, uiAmount );
}

template < class T, class U >
void MemPutFast ( U ptr, const T value )
{
    *(T*)ptr = value;
}

template < class T, class U >
void MemSubFast ( U ptr, const T value )
{
    *(T*)ptr -= value;
}