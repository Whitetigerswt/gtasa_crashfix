/*

        PROJECT:                mod_sa
        LICENSE:                See LICENSE in the top level directory
        COPYRIGHT:              Copyright we_sux

        mod_sa is available from http://code.google.com/p/m0d-s0beit-sa/

        mod_sa is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        mod_sa is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with mod_sa.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "patcher.h"

int memset_safe ( void *_dest, int c, uint32_t len )
{
	uint8_t *dest = (uint8_t *)_dest;
	uint8_t buf[4096];

	memset( buf, c, (len > 4096) ? 4096 : len );

	for ( ;; )
	{
		if ( len > 4096 )
		{
			if ( !memcpy_safe(dest, buf, 4096) )
				return 0;
			dest += 4096;
			len -= 4096;
		}
		else
		{
			if ( !memcpy_safe(dest, buf, len) )
				return 0;
			break;
		}
	}

	return 1;
}

int memcmp_safe ( const void *_s1, const void *_s2, uint32_t len )
{
	const uint8_t	*s1 = (const uint8_t *)_s1;
	const uint8_t	*s2 = (const uint8_t *)_s2;
	uint8_t			buf[4096];

	for ( ;; )
	{
		if ( len > 4096 )
		{
			if ( !memcpy_safe(buf, s1, 4096) )
				return 0;
			if ( memcmp(buf, s2, 4096) )
				return 0;
			s1 += 4096;
			s2 += 4096;
			len -= 4096;
		}
		else
		{
			if ( !memcpy_safe(buf, s1, len) )
				return 0;
			if ( memcmp(buf, s2, len) )
				return 0;
			break;
		}
	}

	return 1;
}

bool isBadPtr_handlerAny ( void *pointer, ULONG size, DWORD dwFlags )
{
	DWORD						dwSize;
	MEMORY_BASIC_INFORMATION	meminfo;

	if ( NULL == pointer )
		return true;

	memset( &meminfo, 0x00, sizeof(meminfo) );
	dwSize = VirtualQuery( pointer, &meminfo, sizeof(meminfo) );

	if ( 0 == dwSize )
		return true;

	if ( MEM_COMMIT != meminfo.State )
		return true;

	if ( 0 == (meminfo.Protect & dwFlags) )
		return true;

	if ( size > meminfo.RegionSize )
		return true;

	if ( (unsigned)((char *)pointer - (char *)meminfo.BaseAddress) > (unsigned)(meminfo.RegionSize - size) )
		return true;

	return false;
}

bool isBadPtr_readAny ( void *pointer, ULONG size )
{
	return isBadPtr_handlerAny( pointer, size, PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ |
								PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY );
}

bool isBadPtr_writeAny ( void *pointer, ULONG size )
{
	return isBadPtr_handlerAny( pointer, size,
								PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY );
}

int memcpy_safe ( void *_dest, const void *_src, uint32_t len, int check, const void *checkdata )
{
	static int		page_size = __page_size_get();
	static int		recurse_ok = 1;
	uint8_t			buf[4096];
	uint8_t			*dest = (uint8_t *)_dest;
	const uint8_t	*src = (const uint8_t *)_src;
	int				ret = 1;

	if (check && checkdata)
	{
		if (!memcmp_safe(checkdata, _dest, len))
			return 0;
	}

	while ( len > 0 )
	{
		uint32_t	this_len = sizeof( buf );

		if ( this_len > len )
			this_len = len;

		if ( !__page_read(buf, src, this_len) )
			ret = 0;

		if ( !__page_write(dest, buf, this_len) )
			ret = 0;

		len -= this_len;
		src += this_len;
		dest += this_len;
	}

	return ret;
}

char *dumb_hex_dump ( void *data, uint32_t len )
{
        static char buf[1024];
        int                     i;

        if ( len > 500 )
                len = 500;

        for ( i = 0; i < (int)len; i++ )
                sprintf_s( buf + i * 2, sizeof(buf) - i * 2, "%02X", *((uint8_t *)data + i) );

        return buf;
}



int patcher_install ( struct patch_set *patch )
{

        int i;

        /* previous initialization failed; always return error. */
        if ( !patch->initialized && patch->failed )
                return 0;

        if ( !patch->initialized )
        {
                patch->failed = 1;

                for ( i = 0; i < PATCHER_CHUNKS_MAX; i++ )
                {
                        if ( patch->chunk[i].ptr == NULL )
                                break;

                        if ( patch->chunk[i].data_cmp != NULL
                         &&      !memcmp_safe((uint8_t *)patch->chunk[i].ptr, patch->chunk[i].data_cmp, patch->chunk[i].len) )
                        {
                                void    *mem = malloc( patch->chunk[i].len );

        
                                if ( mem != NULL )
                                {
                                        if ( memcpy_safe(mem, patch->chunk[i].ptr, patch->chunk[i].len) )
										{ }
                                        free( mem );
                                }

                                return 0;
                        }

                        if ( patch->chunk[i].data_orig == NULL )
                                patch->chunk[i].data_orig = (uint8_t *)malloc( patch->chunk[i].len );
                        if ( patch->chunk[i].data_orig == NULL )
                                continue;

                        if ( !memcpy_safe(patch->chunk[i].data_orig, patch->chunk[i].ptr, patch->chunk[i].len) )
                        {
                                //Log( "Failed to copy original data for patch '%s' chunk %d @ 0x%p", patch->name, i, patch->chunk[i].ptr );
                        }
                }

                patch->initialized = 1;
                patch->failed = 0;

                //Log("Initialized patch '%s'.", patch->name);
        }

        if ( !patch->installed || patch->has_volatile )
        {
                for ( i = 0; i < PATCHER_CHUNKS_MAX; i++ )
                {
                        if ( patch->chunk[i].ptr == NULL )
                                break;

                        if ( patch->installed && !patch->chunk[i].is_volatile )
                                break;

                        if ( patch->chunk[i].data_rep == NULL )
                        {
                                if ( !memset_safe((uint8_t *)patch->chunk[i].ptr, 0x90, patch->chunk[i].len) )
								{} //Log( "Failed to install patch '%s' chunk %d", patch->name, i );
                        }
                        else
                        {
                                if ( !memcpy_safe((uint8_t *)patch->chunk[i].ptr, patch->chunk[i].data_rep, patch->chunk[i].len) )
								{} //Log( "Failed to install patch '%s' chunk %d", patch->name, i );
                        }
                }

                if ( !patch->installed )
                {
                        patch->installed = 1;

                        //Log("Installed patch '%s'.", patch->name);
                }
        }

        return 1;
}

int patcher_remove ( struct patch_set *patch )
{


        int i;

        if ( patch->failed )
        {
                patch->failed = 0;
                return 1;
        }

        if ( patch->installed )
        {
                for ( i = 0; i < PATCHER_CHUNKS_MAX; i++ )
                {
                        if ( patch->chunk[i].ptr == NULL )
                                break;

                        if ( !memcpy_safe((uint8_t *)patch->chunk[i].ptr, patch->chunk[i].data_orig, patch->chunk[i].len) )
						{} //Log( "Failed to restore patch '%s' chunk %d", patch->name, i );
                }

                patch->installed = 0;

                //Log("Removed patch '%s'.", patch->name);
        }

        return 1;
}

int is_patch_installed ( struct patch_set *patch )
{
        if ( patch->installed )
                return 1;
        else
                return 0;
}

void patcher_free ( struct patch_set *patch )
{
        int i;

        for ( i = 0; i < PATCHER_CHUNKS_MAX; i++ )
        {
                if ( patch->chunk[i].ptr == NULL )
                        break;

                if ( patch->chunk[i].data_orig != NULL )
                        free( patch->chunk[i].data_orig );
        }
}
