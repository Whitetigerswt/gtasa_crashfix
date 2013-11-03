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
#pragma once 

#include <stdint.h>
#include <Windows.h>

#define PATCHER_CHUNKS_MAX      16

int									memcpy_safe ( void *dest, const void *src, uint32_t len, int check = NULL, const void *checkdata = NULL);
int									memset_safe ( void *_dest, int c, uint32_t len );
int									memcmp_safe ( const void *_s1, const void *_s2, uint32_t len );

struct patch_chunk
{
        uint32_t        len;                    /* length of data @ ptr */
        void            *ptr;                   /* ptr to data */
        uint8_t         *data_cmp;              /* compare data against this */
        uint8_t         *data_rep;              /* replacement data @ ptr, if NULL NOPs are used */
        uint8_t         *data_orig;             /* original data @ ptr */
        int                     is_volatile;
};

struct patch_set
{
        const char                      *name;
        int                                     initialized;
        int                                     installed;
        struct patch_chunk      chunk[PATCHER_CHUNKS_MAX];
        int                                     failed; /* set if patcher_installed() failed to initialize; reset on patcher_remove() */
        int                                     has_volatile;

        /* these don't really belong here... but oh well. */
        int                                     ini_auto;       /* automatically enable patch; only used by the ini parser */
        int                                     ini_hotkey; /* hotkey for this patch; only used by the ini parser */
};


bool isBadPtr_readAny ( void *pointer, ULONG size );
bool isBadPtr_writeAny ( void *pointer, ULONG size );

static int __page_size_get ( void )
{
	static int	page_size = -1;
	SYSTEM_INFO si;

	if ( page_size == -1 )
	{
		GetSystemInfo( &si );
		page_size = (int)si.dwPageSize;
	}

	return page_size;
}

static int __page_write ( void *_dest, const void *_src, uint32_t len )
{
	static int		page_size = __page_size_get();
	uint8_t			*dest = (uint8_t *)_dest;
	const uint8_t	*src = (const uint8_t *)_src;
	DWORD			prot_prev = 0;
	int				prot_changed = 0;
	SIZE_T			write_len;
	int				ret = 1;

	while ( len > 0 )
	{
		int page_offset = (int)( (UINT_PTR) dest % page_size );
		int page_remain = page_size - page_offset;
		int this_len = len;

		if ( this_len > page_remain )
			this_len = page_remain;

		if ( isBadPtr_writeAny(dest, this_len) )
		{
			if ( !VirtualProtect((void *)dest, this_len, PAGE_EXECUTE_READWRITE, &prot_prev) )
				ret = 0;
			else
				prot_changed = 1;
		}

		if ( !WriteProcessMemory(GetCurrentProcess(), dest, (void *)src, this_len, &write_len) )
			write_len = 0;

		if ( prot_changed )
		{
			DWORD	dummy;
			if ( !VirtualProtect((void *)dest, this_len, prot_prev, &dummy) ) { }
				//Log( "__page_write() could not restore original permissions for ptr %p", dest );
		}

		if ( (int)write_len != this_len )
			ret = 0;

		dest += this_len;
		src += this_len;
		len -= this_len;
	}

	return ret;
}



static int __page_read ( void *_dest, const void *_src, uint32_t len )
{
	static int	page_size = __page_size_get();
	uint8_t		*dest = (uint8_t *)_dest;
	uint8_t		*src = (uint8_t *)_src;
	DWORD		prot_prev = 0;
	int			prot_changed = 0;
	SIZE_T		read_len;
	int			ret = 1;

	while ( len > 0 )
	{
		int page_offset = (int)( (UINT_PTR) src % page_size );
		int page_remain = page_size - page_offset;
		int this_len = len;

		if ( this_len > page_remain )
			this_len = page_remain;

		if ( isBadPtr_readAny(src, this_len) )
		{
			if ( !VirtualProtect((void *)src, this_len, PAGE_EXECUTE_READWRITE, &prot_prev) )
				ret = 0;
			else
				prot_changed = 1;
		}

		if ( !ReadProcessMemory(GetCurrentProcess(), src, dest, this_len, &read_len) )
			read_len = 0;

		if ( prot_changed )
		{
			DWORD	dummy;
			if ( !VirtualProtect((void *)src, this_len, prot_prev, &dummy) ) { }
				//Log( "__page_read() could not restore original permissions for ptr %p", src );
		}

		if ( (int)read_len != this_len )
		{
			memset( dest + read_len, 0, this_len - read_len );
			ret = 0;
		}

		dest += this_len;
		src += this_len;
		len -= this_len;
	}

	return ret;
}

int memcpy_safe ( void *_dest, const void *_src, uint32_t len, int check, const void *checkdata );



char			*dumb_hex_dump ( void *data, uint32_t len );
int             patcher_install ( struct patch_set *set );
int             patcher_remove ( struct patch_set *set );
int             is_patch_installed ( struct patch_set *patch );
void			patcher_free ( struct patch_set *set );
int             GTAPatchIDFinder ( DWORD patchChunk0Ptr );

static struct patch_set patch_EnableResolutions =
{
        "Enable All Supported Resolutions",
        0,
        0,
        {
                { 2, (void *)0x00745BC9, NULL, NULL, (uint8_t *)"\x7A\x6D" },
                { 2, (void *)0x007459E1, NULL, NULL, (uint8_t *)"\x7A\x71" }
        }
};

// thanks to MTA for this patch
static struct patch_set patch_DisableLoadingScreen =
{
        "Disable Loading Screen",
        0,
        0,
        {
                { 5, (void *)0x00748CF6, NULL, NULL, (uint8_t *)"\xE8\x15\xE7\xDB\xFF" },
                { 5, (void *)0x00590D7C, NULL, NULL, (uint8_t *)"\xE8\x3F\xFD\xFF\xFF" }, // uncommet this and our FPS is limited to 60.
                { 5, (void *)0x00590DB3, NULL, NULL, (uint8_t *)"\xE8\x08\xFD\xFF\xFF" },
                { 5, (void *)0x00590D9F, NULL, (uint8_t *)"\xC3\x90\x90\x90\x90", (uint8_t *)"\xE9\xCC\xF7\xFF\xFF" }
        }
};
//