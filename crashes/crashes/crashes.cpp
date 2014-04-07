/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        multiplayer_sa/CMultiplayerSA.cpp
*  PURPOSE:     Multiplayer module class
*  DEVELOPERS:  Ed Lyons <eai@opencoding.net>
*               Christian Myhre Lundheim <>
*               Cecill Etheredge <ijsf@gmx.net>
*               Jax <>
*               Stanislav Bobrov <lil_toady@hotmail.com>
*               Alberto Alonso <rydencillo@gmail.com>
*               Peter <>
*               Sebas Lamers <sebasdevelopment@gmx.com>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <math.h>
#include "multiplayer_hooksystem.h"
#include "crashes.h"
#include "memory.h"
#include "log.h"
#include "RenderWare.h"
#include "StdInc.h"


void CPlayerPed__ProcessControl_Abort() { }

void _cdecl CrashAverted ( DWORD id )
{
    Log * log = new Log("crashes.log");
	char buf[256];
	sprintf(buf, "You were saved from crash #%d!", id);
	log->Write(buf);
	delete log;
}

#define CRASH_AVERTED(id) \
     \
	_asm pushfd \
    _asm pushad \
    _asm mov eax, id \
    _asm push eax \
    _asm call CrashAverted \
    _asm add esp, 4 \
    _asm popad \
	_asm popfd \
    _asm \
    


////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc1                              0x5D9A6E
#define HOOKSIZE_CrashFix_Misc1                             6
DWORD RETURN_CrashFix_Misc1 =                               0x5D9A74;
void _declspec(naked) HOOK_CrashFix_Misc1 ()
{
    _asm
    {
        mov     eax,dword ptr [esp+18h]
        test    eax,eax 
        je      cont

        mov     eax,dword ptr ds:[008D12CCh] 
        mov     ecx,dword ptr [eax+esi]     // If [eax+esi] is 0, it causes a crash
        test    ecx,ecx
        jne     cont
		{
			CRASH_AVERTED( 1 )
		}
        xor    ecx,ecx
    cont:
        jmp     RETURN_CrashFix_Misc1
    }
}


////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc2                              0x6B18B0
#define HOOKSIZE_CrashFix_Misc2                             9
DWORD RETURN_CrashFix_Misc2 =                               0x6B18B9;
DWORD RETURN_CrashFix_Misc2B =                              0x6B3775;
void _declspec(naked) HOOK_CrashFix_Misc2 ()
{
    _asm
    {
        test    eax,eax 
        je      cont        // Skip much code if eax is zero (vehicle has no colmodel)

        mov     eax,dword ptr [eax+2Ch] 

        test    eax,eax 
        je      cont        // Skip much code if eax is zero (colmodel has no coldata)

        mov     ebx,dword ptr [eax+10h] 

        test    ebx,ebx 
        je      cont        // Skip much code if ebx is zero (coldata has no suspension lines)

        mov     cl,byte ptr [esi+429h]
        jmp     RETURN_CrashFix_Misc2
    cont: 
		CRASH_AVERTED( 2 )
		jmp     RETURN_CrashFix_Misc2B
    }
}


////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc3                              0x645FD9
#define HOOKSIZE_CrashFix_Misc3                             6
DWORD RETURN_CrashFix_Misc3 =                               0x645FDF;
void _declspec(naked) HOOK_CrashFix_Misc3 ()
{
    _asm
    {
        test    ecx,ecx 
        je      cont        // Skip much code if ecx is zero (ped has no something)

        mov     edx,dword ptr [ecx+384h]
        jmp     RETURN_CrashFix_Misc3
    cont: 
		CRASH_AVERTED( 3 )
		jmp     CPlayerPed__ProcessControl_Abort
		
    }
}


////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc4                              0x4F02D2
#define HOOKSIZE_CrashFix_Misc4                             5
DWORD RETURN_CrashFix_Misc4 =                               0x4F02D7;
DWORD RETURN_CrashFix_Misc4B =                              0x4F0B07;
void _declspec(naked) HOOK_CrashFix_Misc4 ()
{
    _asm
    {
        test    ecx,ecx 
        je      cont        // Skip much code if ecx is zero (avoid divide by zero in soundmanager::service)

        cdq  
        idiv    ecx  
        add     edx, ebp  
        jmp     RETURN_CrashFix_Misc4
    cont: 
		CRASH_AVERTED( 4 )
		jmp     RETURN_CrashFix_Misc4B
		
    }
}


////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc5                              0x5DF949
#define HOOKSIZE_CrashFix_Misc5                             7
DWORD RETURN_CrashFix_Misc5 =                               0x5DF950;
DWORD RETURN_CrashFix_Misc5B =                              0x5DFCC4;
void _declspec(naked) HOOK_CrashFix_Misc5 ()
{
    _asm
    {
        mov     edi, dword ptr [ecx*4+0A9B0C8h]
        mov     edi, dword ptr [edi+5Ch]     
        test    edi, edi 
        je      cont        // Skip much code if edi is zero (ped has no model)

        mov     edi, dword ptr [ecx*4+0A9B0C8h]
        jmp     RETURN_CrashFix_Misc5
    cont: 
		CRASH_AVERTED( 5 )
		pop edi
		jmp     RETURN_CrashFix_Misc5B
		
    }
}


////////////////////////////////////////////////////////////////////////
// #5465 2/2
#define HOOKPOS_CrashFix_Misc6                              0x4D1750
#define HOOKSIZE_CrashFix_Misc6                             5
DWORD RETURN_CrashFix_Misc6 =                               0x4D1755;
DWORD RETURN_CrashFix_Misc6B =                              0x4D1A44;
void _declspec(naked) HOOK_CrashFix_Misc6 ()
{
    _asm
    {
        test    ecx, ecx 
        je      cont        // Skip much code if ecx is zero (ped has no anim something)

        mov     eax, dword ptr [ecx+10h]
        test    eax, eax
        jmp     RETURN_CrashFix_Misc6
    cont: 
		CRASH_AVERTED( 6 )
		jmp     RETURN_CrashFix_Misc6B
	}
}


////////////////////////////////////////////////////////////////////////
// #5466
#define HOOKPOS_CrashFix_Misc7                              0x417BF8
#define HOOKSIZE_CrashFix_Misc7                             5
DWORD RETURN_CrashFix_Misc7 =                               0x417BFD;
DWORD RETURN_CrashFix_Misc7B =                              0x417BFF;
void _declspec(naked) HOOK_CrashFix_Misc7 ()
{
    _asm
    {
        test    ecx, ecx 
        je      cont        // Skip much code if ecx is zero (no colmodel)

        mov     esi, dword ptr [ecx+2Ch] 
        test    esi, esi
        jmp     RETURN_CrashFix_Misc7
    cont: 
		CRASH_AVERTED( 7 )
		jmp     RETURN_CrashFix_Misc7B
	}
}


////////////////////////////////////////////////////////////////////////
// #5468  1/3
#define HOOKPOS_CrashFix_Misc8                              0x73485D
#define HOOKSIZE_CrashFix_Misc8                             5
DWORD RETURN_CrashFix_Misc8 =                               0x734862;
DWORD RETURN_CrashFix_Misc8B =                              0x734871;
void _declspec(naked) HOOK_CrashFix_Misc8 ()
{
    _asm
    {
        test    ecx, ecx 
        je      cont        // Skip much code if ecx is zero (no 2d effect plugin)

        mov     ecx, dword ptr [edx+ecx] 
        test    ecx, ecx 
        jmp     RETURN_CrashFix_Misc8
    cont: 
		CRASH_AVERTED( 8 )
		jmp     RETURN_CrashFix_Misc8B
    }
}


////////////////////////////////////////////////////////////////////////
// #5468  2/3
#define HOOKPOS_CrashFix_Misc9                              0x738B64
#define HOOKSIZE_CrashFix_Misc9                             6
DWORD RETURN_CrashFix_Misc9 =                               0x738B6A;
DWORD RETURN_CrashFix_Misc9B =                              0x73983A;
void _declspec(naked) HOOK_CrashFix_Misc9 ()
{
    _asm
    {
        test    esi, esi 
        je      cont        // Skip much code if esi is zero (invalid projectile)

        mov     eax, dword ptr [esi+40h] 
        test    ah, 1
        jmp     RETURN_CrashFix_Misc9
    cont: 
		CRASH_AVERTED( 9 )
		jmp     RETURN_CrashFix_Misc9B
    }
}


////////////////////////////////////////////////////////////////////////
// #5468  3/3
#define HOOKPOS_CrashFix_Misc10                             0x5334FE
#define HOOKSIZE_CrashFix_Misc10                            6
DWORD RETURN_CrashFix_Misc10 =                              0x533504;
DWORD RETURN_CrashFix_Misc10B =                             0x533539;
void _declspec(naked) HOOK_CrashFix_Misc10 ()
{
    _asm
    {
        cmp     ecx, 0x80
        jb      cont  // Skip much code if ecx is small (invalid vector pointer)

        mov     edx, dword ptr [ecx] 
        mov     dword ptr [esp], edx
        jmp     RETURN_CrashFix_Misc10
    cont: 
		CRASH_AVERTED( 10 )
		mov     ecx, dword ptr [esp+1Ch]
		mov     dword ptr [ecx],0
		mov     dword ptr [ecx+4],0
		mov     dword ptr [ecx+8],0
		jmp     RETURN_CrashFix_Misc10B
    }
}


////////////////////////////////////////////////////////////////////////
// #5576
#define HOOKPOS_CrashFix_Misc11                             0x4D2C62
#define HOOKSIZE_CrashFix_Misc11                            5
DWORD RETURN_CrashFix_Misc11 =                              0x4D2C67;
DWORD RETURN_CrashFix_Misc11B =                             0x4D2E03;
void _declspec(naked) HOOK_CrashFix_Misc11 ()
{
    _asm
    {
        test    ecx, ecx 
        je      cont  // Skip much code if ecx is zero (invalid anim somthing)

        mov     eax, dword ptr [ecx+10h] 
        test    eax, eax 
        jmp     RETURN_CrashFix_Misc11
    cont: 
		CRASH_AVERTED( 11 )
		jmp     RETURN_CrashFix_Misc11B
    }
}


////////////////////////////////////////////////////////////////////////
// #5530
#define HOOKPOS_CrashFix_Misc12                             0x4D41C5
#define HOOKSIZE_CrashFix_Misc12                            5
DWORD RETURN_CrashFix_Misc12 =                              0x4D41CA;
DWORD RETURN_CrashFix_Misc12B =                             0x4D4222;
void _declspec(naked) HOOK_CrashFix_Misc12 ()
{
    _asm
    {
        test    edi, edi 
        je      cont  // Skip much code if edi is zero (invalid anim somthing)

        mov     al, byte ptr [edi+0Bh] 
        test    al, al 
        jmp     RETURN_CrashFix_Misc12
    cont: 
		CRASH_AVERTED( 12 )
		jmp     RETURN_CrashFix_Misc12B
    }
}


////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc13                             0x4D464E
#define HOOKSIZE_CrashFix_Misc13                            6
DWORD RETURN_CrashFix_Misc13 =                              0x4D4654;
DWORD RETURN_CrashFix_Misc13B =                             0x4D4764;
void _declspec(naked) HOOK_CrashFix_Misc13 ()
{
    _asm
    {
        cmp     eax, 0x2480
        jb      cont  // Skip much code if eax is less than 0x480 (invalid anim)

        mov     al, byte ptr [eax+0Ah] 
        shr     al, 5
        jmp     RETURN_CrashFix_Misc13
    cont: 
		CRASH_AVERTED( 13 )
		jmp     RETURN_CrashFix_Misc13B
    }
}


////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc14                             0x4DD4B5
#define HOOKSIZE_CrashFix_Misc14                            6
DWORD RETURN_CrashFix_Misc14 =                              0x4DD4BB;
void _declspec(naked) HOOK_CrashFix_Misc14 ()
{
    _asm
    {
        mov     eax, dword ptr ds:[0BD00F8h]
        cmp     eax, 0
        je      cont  // Skip much code if eax is zero ( Audio event volumes table not initialized )

        sub     esp, 0D4h
        jmp     RETURN_CrashFix_Misc14
    cont: 
		CRASH_AVERTED( 14 )
		add     esp, 12
		retn    12
    }
}


////////////////////////////////////////////////////////////////////////
void _cdecl DoWait ( HANDLE hHandle )
{
    DWORD dwWait = 4000;
    DWORD dwResult = WaitForSingleObject ( hHandle, dwWait );
    if ( dwResult == WAIT_TIMEOUT )
    {
        // This thread lock bug in GTA will have to be fixed one day.
        // Until then, a 5 second freeze should be long enough for the loading thread to have finished it's job.
#if 0
        _wassert ( _CRT_WIDE("\
            Press IGNORE  ('I' Key) \n\
            Press IGNORE  ('I' Key) \n\
            Press IGNORE  ('I' Key) \n\
            Press IGNORE  ('I' Key) \n\
            ")
         , _CRT_WIDE(__FILE__), __LINE__);
#endif
        dwResult = WaitForSingleObject ( hHandle, 1000 );
    }
}

// hook info
#define HOOKPOS_FreezeFix_Misc15_US                         0x156CDAE
#define HOOKSIZE_FreezeFix_Misc15_US                        6
#define HOOKPOS_FreezeFix_Misc15_EU                         0x156CDEE
#define HOOKSIZE_FreezeFix_Misc15_EU                        6
DWORD RETURN_FreezeFix_Misc15_US =                          0x156CDB4;
DWORD RETURN_FreezeFix_Misc15_EU =                          0x156CDF4;
DWORD RETURN_FreezeFix_Misc15_BOTH =                        0;
void _declspec(naked) HOOK_FreezeFix_Misc15 ()
{
    _asm
    {
        pop eax
        pop edx
        pushad

        push eax
        call DoWait
        add esp, 4

        popad
        jmp     RETURN_FreezeFix_Misc15_BOTH
    }
}


////////////////////////////////////////////////////////////////////////
// Handle RpAnimBlendClumpGetFirstAssociation returning NULL
#define HOOKPOS_CrashFix_Misc16                             0x5E5815
#define HOOKSIZE_CrashFix_Misc16                            6
DWORD RETURN_CrashFix_Misc16 =                              0x5E581B;
void _declspec(naked) HOOK_CrashFix_Misc16 ()
{
#if TEST_CRASH_FIXES
    SIMULATE_ERROR_BEGIN( 10 )
        _asm
        {
            mov     eax, 0
        }
    SIMULATE_ERROR_END
#endif

    _asm
    {
        cmp     eax, 0
        je      cont  // Skip much code if eax is zero ( RpAnimBlendClumpGetFirstAssociation returns NULL )

        // continue standard path
        movsx   ecx, word ptr [eax+2Ch]
        xor     edi, edi
        jmp     RETURN_CrashFix_Misc16

    cont: 
		CRASH_AVERTED( 16 )
		add     esp, 96
		retn
    }
}


////////////////////////////////////////////////////////////////////////
// Handle RwFrameSetStaticPluginsSize having NULL member at 0x90
#define HOOKPOS_CrashFix_Misc17_US                          0x7F120E
#define HOOKSIZE_CrashFix_Misc17_US                         6
#define HOOKPOS_CrashFix_Misc17_EU                          0x7F124E
#define HOOKSIZE_CrashFix_Misc17_EU                         6
DWORD RETURN_CrashFix_Misc17_US =                           0x7F1214;
DWORD RETURN_CrashFix_Misc17_EU =                           0x7F1254;
DWORD RETURN_CrashFix_Misc17_BOTH =                         0;
DWORD RETURN_CrashFix_Misc17B_US =                          0x7F1236;
DWORD RETURN_CrashFix_Misc17B_EU =                          0x7F1276;
DWORD RETURN_CrashFix_Misc17B_BOTH =                        0;
void _declspec(naked) HOOK_CrashFix_Misc17 ()
{
    _asm
    {
        cmp     eax, 0
        je      cont  // Skip much code if eax is zero

        // continue standard path
        mov     eax, [eax+90h]
        jmp     RETURN_CrashFix_Misc17_BOTH

    cont: 
		CRASH_AVERTED( 17 )
		jmp     RETURN_CrashFix_Misc17B_BOTH
    }
}


////////////////////////////////////////////////////////////////////////
// Handle GetWheelPosition having wrong data
#define HOOKPOS_CrashFix_Misc18                             0x4C7DAD
#define HOOKSIZE_CrashFix_Misc18                            7
DWORD RETURN_CrashFix_Misc18 =                              0x4C7DB4;
void _declspec(naked) HOOK_CrashFix_Misc18 ()
{
#if TEST_CRASH_FIXES
    SIMULATE_ERROR_BEGIN( 10 )
        _asm
        {
            mov     ebp, 0
        }
    SIMULATE_ERROR_END
#endif

    _asm
    {
        cmp     ebp, 0
        je      cont  // Skip much code if ebp is zero

        // continue standard path
        mov         edx,dword ptr [ebp+40h] 
        mov         eax,dword ptr [esp+10h] 
        jmp     RETURN_CrashFix_Misc18

    cont:
		CRASH_AVERTED( 18 )
		mov         edx,0 
		mov         eax,dword ptr [esp+10h]
		mov         dword ptr [eax],edx 
		mov         dword ptr [eax+4],edx 
		pop         esi  
		mov         dword ptr [eax+8],edx 
		pop         ebp  
		ret         0Ch  
    }
}



////////////////////////////////////////////////////////////////////////
// Handle RwFrameCloneHierarchy having wrong data
#define HOOKPOS_CrashFix_Misc19_US                          0x7F0BF7
#define HOOKSIZE_CrashFix_Misc19_US                         6
#define HOOKPOS_CrashFix_Misc19_EU                          0x7F0C37
#define HOOKSIZE_CrashFix_Misc19_EU                         6
DWORD RETURN_CrashFix_Misc19_US =                           0x7F0BFD;
DWORD RETURN_CrashFix_Misc19_EU =                           0x7F0C3D;
DWORD RETURN_CrashFix_Misc19_BOTH =                         0;
DWORD RETURN_CrashFix_Misc19B_US =                          0x7F0C20;
DWORD RETURN_CrashFix_Misc19B_EU =                          0x7F0C60;
DWORD RETURN_CrashFix_Misc19B_BOTH =                        0;
void _declspec(naked) HOOK_CrashFix_Misc19 ()
{
    _asm
    {
        cmp     esi, 0
        je      cont  // Skip much code if esi is zero

        // continue standard path
        mov     eax, [esi+98h] 
        jmp     RETURN_CrashFix_Misc19_BOTH

    cont: 
		CRASH_AVERTED( 19 )
		mov         edx,dword ptr [ecx+98h] 
		test        edx,edx
		jmp     RETURN_CrashFix_Misc19B_BOTH
    }
}


////////////////////////////////////////////////////////////////////////
// Handle CPlaceable::RemoveMatrix having wrong data
#define HOOKPOS_CrashFix_Misc20                             0x54F3B0
#define HOOKSIZE_CrashFix_Misc20                            6
DWORD RETURN_CrashFix_Misc20 =                              0x54F3B6;
void _declspec(naked) HOOK_CrashFix_Misc20 ()
{
#if TEST_CRASH_FIXES
    SIMULATE_ERROR_BEGIN( 10 )
        _asm
        {
            mov     ecx, 0
        }
    SIMULATE_ERROR_END
#endif
    _asm
    {
        cmp     ecx, 0
        je      cont        // Skip much code if ecx is zero

        // continue standard path
        sub     esp, 10h 
        mov     eax, [ecx+14h] 
        jmp     RETURN_CrashFix_Misc20

    cont: 
		CRASH_AVERTED( 20 )
		retn
    }
}


////////////////////////////////////////////////////////////////////////
// Handle CTaskSimpleCarFallOut::FinishAnimFallOutCB having wrong data
#define HOOKPOS_CrashFix_Misc21                             0x648EF6
#define HOOKSIZE_CrashFix_Misc21                            6
DWORD RETURN_CrashFix_Misc21 =                              0x648EFC;
void _declspec(naked) HOOK_CrashFix_Misc21 ()
{
#if TEST_CRASH_FIXES
    SIMULATE_ERROR_BEGIN( 10 )
        _asm
        {
            mov     ecx, 0x10
        }
    SIMULATE_ERROR_END
#endif

    _asm
    {
        cmp     ecx, 0x480
        jb      cont  // Skip much code if ecx is low

        // continue standard path
        mov     edx, [ecx+590h]
        jmp     RETURN_CrashFix_Misc21

    cont:
		CRASH_AVERTED( 21 )
		retn
    }
}


////////////////////////////////////////////////////////////////////////
// Handle CAnimBlendAssociation::Init having wrong data
#define HOOKPOS_CrashFix_Misc22                             0x4CEF08
#define HOOKSIZE_CrashFix_Misc22                            6
DWORD RETURN_CrashFix_Misc22 =                              0x4CEF25;
void _declspec(naked) HOOK_CrashFix_Misc22 ()
{
    _asm
    {
        mov         edx,dword ptr [edi+0Ch] 
    }

#if TEST_CRASH_FIXES
    SIMULATE_ERROR_BEGIN( 10 )
        _asm
        {
            mov     edx, 0x10
        }
    SIMULATE_ERROR_END
#endif

    _asm
    {
        cmp     edx, 0x480
        jb      altcode  // Fill output with zeros if edx is low

        // do standard code
    lp1:
        mov         edx,dword ptr [edi+0Ch] 
        mov         edx,dword ptr [edx+eax*4] 
        mov         ebx,dword ptr [esi+10h] 
        mov         dword ptr [ebx+ecx+10h],edx 
        mov         edx,dword ptr [esi+10h] 
        mov         dword ptr [edx+ecx+14h],esi 
        movsx       edx,word ptr [esi+0Ch] 
        inc         eax  
        add         ecx,18h 
        cmp         eax,edx 
        jl          lp1 
        jmp     RETURN_CrashFix_Misc22

        // do alternate code
    altcode: 
		CRASH_AVERTED( 22 )
		mov     edx,0
		mov         ebx,dword ptr [esi+10h] 
		mov         dword ptr [ebx+ecx+10h],edx 
		mov         dword ptr [ebx+ecx+14h],edx 
		movsx       edx,word ptr [esi+0Ch] 
		inc         eax  
		add         ecx,18h 
		cmp         eax,edx 
		jl          altcode 
		jmp     RETURN_CrashFix_Misc22
    }
}


////////////////////////////////////////////////////////////////////////
// Handle CVehicleAnimGroup::ComputeAnimDoorOffsets having wrong door index
#define HOOKPOS_CrashFix_Misc23                             0x6E3D10
#define HOOKSIZE_CrashFix_Misc23                            7
DWORD RETURN_CrashFix_Misc23 =                              0x6E3D17;
void _declspec(naked) HOOK_CrashFix_Misc23 ()
{
#if TEST_CRASH_FIXES
    SIMULATE_ERROR_BEGIN( 10 )
        _asm
        {
            mov     edx,0xffff0000
            mov     [esp+8], edx
        }
    SIMULATE_ERROR_END
#endif

    _asm
    {
        // Ensure door index is reasonable
        mov     edx, [esp+8]
        cmp     edx,16
        jb      ok

        // zero if out of range
        mov     edx,0
        mov     [esp+8], edx
		{
			CRASH_AVERTED( 23 )
		}

    ok:
        // continue standard path
        mov     edx, [esp+8]
        lea     eax, [edx+edx*2]
        jmp     RETURN_CrashFix_Misc23
    }
}


////////////////////////////////////////////////////////////////////////
// Handle _RwFrameForAllChildren being called with NULL
#define HOOKPOS_CrashFix_Misc24_US                          0x7F0DC8
#define HOOKSIZE_CrashFix_Misc24_US                         6
#define HOOKPOS_CrashFix_Misc24_EU                          0x7F0E08
#define HOOKSIZE_CrashFix_Misc24_EU                         6
DWORD RETURN_CrashFix_Misc24_US =                           0x7F0DCE;
DWORD RETURN_CrashFix_Misc24_EU =                           0x7F0E0E;
DWORD RETURN_CrashFix_Misc24_BOTH =                         0;
void _declspec(naked) HOOK_CrashFix_Misc24 ()
{
    _asm
    {
        cmp     ebp, 0x480
        jb      cont  // Skip code if ebp is low

        // continue standard path
        mov     eax, [ebp+98h]
        jmp     RETURN_CrashFix_Misc24_BOTH

    cont:
        CRASH_AVERTED( 24 )
        mov     ebp, 0
        mov     eax, 0
        jmp     RETURN_CrashFix_Misc24_BOTH
    }
}


////////////////////////////////////////////////////////////////////////
// Handle CTaskSimpleCarOpenDoorFromOutside::FinishAnimCarOpenDoorFromOutsideCB having zero pointer to vehicle
#define HOOKPOS_CrashFix_Misc25                             0x646026
#define HOOKSIZE_CrashFix_Misc25                            5
DWORD RETURN_CrashFix_Misc25 =                              0x64602B;
void _declspec(naked) HOOK_CrashFix_Misc25 ()
{
#if TEST_CRASH_FIXES
    SIMULATE_ERROR_BEGIN( 10 )
        _asm
        {
            mov     eax, 0
            mov     [esi+0x10], eax
        }
    SIMULATE_ERROR_END
#endif

    _asm
    {
        // Check for zero pointer to vehicle
        mov     eax, [esi+0x10]
        cmp     eax, 0
        jz      fix

        // Continue standard path
        lea     eax,[esp+10h]
        push    eax
        jmp     RETURN_CrashFix_Misc25

    fix:
        CRASH_AVERTED( 25 )
        // Do special thing
        pop     esi
        pop     ecx
        retn
    }
}


////////////////////////////////////////////////////////////////////////
// Handle CShotInfo::Update having invalid item
#define HOOKPOS_CrashFix_Misc26                             0x739FA0
#define HOOKSIZE_CrashFix_Misc26                            6
DWORD RETURN_CrashFix_Misc26 =                              0x739FA6;
void _declspec(naked) HOOK_CrashFix_Misc26 ()
{
#if TEST_CRASH_FIXES
    SIMULATE_ERROR_BEGIN( 10 )
        _asm
        {
            mov     ebx, 130h
        }
    SIMULATE_ERROR_END
#endif

    _asm
    {
        // Check for incorrect pointer
        cmp     ebx, 130h
        jz      fix

        // Continue standard path
        mov     edi,dword ptr [ebx+ebp*4] 
        dec     ebp  
        test    edi,edi 
        jmp     RETURN_CrashFix_Misc26

    fix:
        CRASH_AVERTED( 26 )
        // Do special thing
        mov     edi, 0 
        dec     ebp  
        test    edi,edi 
        jmp     RETURN_CrashFix_Misc26
    }
}

////////////////////////////////////////////////////////////////////////
// Handle CObject::ProcessGarageDoorBehaviour object with no dummy
#define HOOKPOS_CrashFix_Misc28                             0x44A4FD
#define HOOKSIZE_CrashFix_Misc28                            6
DWORD RETURN_CrashFix_Misc28 =                              0x44A503;
DWORD RETURN_CrashFix_Misc28B =                             0x44A650;
void _declspec(naked) HOOK_CrashFix_Misc28 ()
{
    _asm
    {
        // Execute replaced code
        mov     eax, [esi+170h]
    }

#if TEST_CRASH_FIXES
    SIMULATE_ERROR_BEGIN( 50 )
        _asm
        {
            mov     eax, 0
        }
    SIMULATE_ERROR_END
#endif

    _asm
    {
        // Check if dummy pointer is zero
        test    eax, eax
        jne     cont

        // Skip much code
        jmp     RETURN_CrashFix_Misc28B

cont:
        // Continue standard path
        jmp     RETURN_CrashFix_Misc28
    }
}

void OnMY_Rtl_fopen_Post( FILE* fh, DWORD calledFrom, const char* szFilename, const char* szMode )
{
    // Check for file open error
    if ( !fh )
    {
		int lastIdx = strlen(szFilename) - 1;
		if(szFilename[lastIdx] == 'b' && szFilename[lastIdx-1] == '.') {
			return;
		}
		Log* log = new Log("crashes.log");
		char err[256];
		sprintf_s(err, sizeof(err), "%s has failed to open!", szFilename);
		
		
		log->Write(err);
		delete log;
    }
}

// Hook info
#define HOOKPOS_Rtl_fopen_US                         0x8232D8
#define HOOKSIZE_Rtl_fopen_US                        6
#define HOOKPOS_Rtl_fopen_EU                         0x823318
#define HOOKSIZE_Rtl_fopen_EU                        6
DWORD RETURN_Rtl_fopen_US =                          0x8232DE;
DWORD RETURN_Rtl_fopen_EU =                          0x82331E;
DWORD RETURN_Rtl_fopen_BOTH =                        0;
void _declspec(naked) HOOK_Rtl_fopen()
{
    _asm
    {
        push    [esp+4*3]
        push    [esp+4*3]
        push    [esp+4*3]
        call inner
        add     esp, 4*3

        pushad
        push    [esp+32+4*2]
        push    [esp+32+4*2]
        push    [esp+32+4*2]
        push    eax
        call    OnMY_Rtl_fopen_Post
        add     esp, 4*2+4+4
        popad
        retn

inner:
        push    40h
        push    [esp+0x0c] 
        jmp     RETURN_Rtl_fopen_BOTH
    }
}

////////////////////////////////////////////////////////////////////////
// Handle CTaskComplexDieInCar::ControlSubTask ped with no vehicle
#define HOOKPOS_CrashFix_Misc27                             0x6377FB
#define HOOKSIZE_CrashFix_Misc27                            7
DWORD RETURN_CrashFix_Misc27 =                              0x637802;
void _declspec(naked) HOOK_CrashFix_Misc27 ()
{
    _asm
    {
        // Execute replaced code
        cmp     byte ptr [edi+484h], 2
        je      cont
	
        // Check if veh pointer is zero
        mov     ecx, [edi+58Ch]
        test    ecx, ecx
        jne     cont
        CRASH_AVERTED( 27 )

cont:
        // Continue standard path
        jmp     RETURN_CrashFix_Misc27
    }
}

////////////////////////////////////////////////////////////////////////
// Handle CAEPCBankLoader::IsSoundBankLoaded with invalid argument
#define HOOKPOS_CrashFix_Misc29                             0x4E022C
#define HOOKSIZE_CrashFix_Misc29                            5
DWORD RETURN_CrashFix_Misc29 =                              0x4E0231;
DWORD RETURN_CrashFix_Misc29B =                             0x4E0227;
void _declspec(naked) HOOK_CrashFix_Misc29 ()
{
    _asm
    {
        // Execute replaced code
        movsx   eax,word ptr [esp+8]
    }


    _asm
    {
        // Check word being -1
        cmp     al, 0xffff
        jz      cont

        // Continue standard path
        jmp     RETURN_CrashFix_Misc29

cont:
        // Skip much code
        jmp     RETURN_CrashFix_Misc29B
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// CClumpModelInfo::GetFrameFromId
//
//////////////////////////////////////////////////////////////////////////////////////////
RwFrame* OnMY_CClumpModelInfo_GetFrameFromId_Post ( RwFrame* pFrameResult, DWORD _ebx, DWORD _esi, DWORD _edi, DWORD calledFrom, RpClump* pClump, int id )
{
    if ( pFrameResult )
        return pFrameResult;

    // Don't check frame if call can legitimately return NULL
    if ( calledFrom == 0x6D308F       // CVehicle::SetWindowOpenFlag
        || calledFrom == 0x6D30BF     // CVehicle::ClearWindowOpenFlag
        || calledFrom == 0x4C7DDE     // CVehicleModelInfo::GetOriginalCompPosition
        || calledFrom == 0x4C96BD )   // CVehicleModelInfo::CreateInstance
        return NULL;

    // Ignore external calls
    if ( calledFrom < 0x401000 || calledFrom > 0x801000 )
        return NULL;

    // Now we have a NULL frame which will cause a crash
    // (Probably due to a custom vehicle with missing frames)

    // See if we can get the model id for reporting
    DWORD pVehicle = NULL;

    if ( calledFrom == 0x6D3847 )       // CVehicle::AddReplacementUpgrade
        pVehicle = _ebx;
    else
    if ( calledFrom == 0x6DFA61         // CVehicle::AddUpgrade
        || calledFrom == 0x6D3A62 )     // CVehicle::GetReplacementUpgrade
        pVehicle = _edi;
    else
    if ( calledFrom == 0x06AC740        // CAutomobile::PreRender (Forklift)
        || calledFrom == 0x6D39F3       // CVehicle::RemoveReplacementUpgrade
        || calledFrom == 0x6D3A32 )     // CVehicle::RemoveReplacementUpgrade2
        pVehicle = _esi;


    // Need a valid frame to replace the missing one - Find nearest other id     
    for ( int i = 2 ; i < 40 ; i++ )
    {
        RwFrame* pNewFrameResult = NULL;
        int uiNewId = id + ( i / 2 ) * ( ( i & 1 ) ? -1 : 1 );
        DWORD dwFunc = 0x4C53C0;    // CClumpModelInfo::GetFrameFromId
        _asm
        {
            push    uiNewId
            push    pClump
            call    dwFunc
            add     esp, 8
            mov     pNewFrameResult,eax
        }

        if ( pNewFrameResult )
        {
            Log * log = new Log("crashes.log");
			char buf[256];
			sprintf(buf, "Averted invalid vehicle mod crash - Called from: %d Vehicle: %d", calledFrom, pVehicle);
			log->Write(buf);
			delete log;
            return pNewFrameResult;
        }
    }

    // Couldn't find a replacement frame id

    return NULL;
}

// Hook info
#define HOOKPOS_CClumpModelInfo_GetFrameFromId                      0x4C53C0
#define HOOKSIZE_CClumpModelInfo_GetFrameFromId                     7
DWORD RETURN_CClumpModelInfo_GetFrameFromId =                       0x4C53C7;
void _declspec(naked) HOOK_CClumpModelInfo_GetFrameFromId ()
{
    _asm
    {
        push    [esp+4*2]
        push    [esp+4*2]
        call    inner
        add     esp, 4*2

        pushad
        push    [esp+32+4*2]
        push    [esp+32+4*2]
        push    [esp+32+4*2]
        push    edi
        push    esi
        push    ebx
        push    eax
        call    OnMY_CClumpModelInfo_GetFrameFromId_Post
        mov     [esp+0],eax
        add     esp, 4*7
        popad
        mov     eax,[esp-32-4*7]
        retn

inner:
        sub     esp,8 
        mov     eax,dword ptr [esp+10h] 
        jmp     RETURN_CClumpModelInfo_GetFrameFromId
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
//
// Setup hooks for CrashFixHacks
//
//////////////////////////////////////////////////////////////////////////////////////////

int getGameVersion() {
	unsigned char ucA = *reinterpret_cast < unsigned char* > ( 0x748ADD );
    unsigned char ucB = *reinterpret_cast < unsigned char* > ( 0x748ADE );
    if ( ucA == 0xFF && ucB == 0x53 )
    {
        return 1; // usa
    }
    else if ( ucA == 0x0F && ucB == 0x84 )
    {
        return 2; // eu
    }
    else if ( ucA == 0xFE && ucB == 0x10 )
    {
        return 3;
    }
    else
    {
        return -1;
    }

    return -1;
}

//
// Volumetric shadow hooks
//

#define HOOKPOS_GetFxQuality                0x49EA50
void HOOK_GetFxQuality ();

#define HOOKPOS_StoreShadowForVehicle       0x70BDA0
DWORD RETURN_StoreShadowForVehicle =        0x70BDA9;
void HOOK_StoreShadowForVehicle ();

DWORD dwFxQualityValue = 0;
WORD usCallingForVehicleModel = 0;
bool volumetricShadows = false;

void setVolumetricShadows(bool value) {
	volumetricShadows = value;
}

void _cdecl MaybeAlterFxQualityValue ( DWORD dwAddrCalledFrom )
{
    // Handle all calls from CVolumetricShadowMgr
    if ( dwAddrCalledFrom > 0x70F990 && dwAddrCalledFrom < 0x711EB0 )
    {
        // Force blob shadows if volumetric shadows are not enabled
        if ( !volumetricShadows )
            dwFxQualityValue = 0;

        // These vehicles seem to have problems with volumetric shadows, so force blob shadows
        switch ( usCallingForVehicleModel )
        {
            case 460:   // Skimmer
            case 511:   // Beagle
            case 572:   // Mower
            case 590:   // Box Freight
            case 592:   // Andromada
                dwFxQualityValue = 0;
        }
        usCallingForVehicleModel = 0;
    }
    else
    // Handle all calls from CPed::PreRenderAfterTest
    if ( dwAddrCalledFrom > 0x5E65A0 && dwAddrCalledFrom < 0x5E7680 )
    {
        // Always use blob shadows for peds as realtime shadows are disabled in MTA (context switching issues)
        dwFxQualityValue = 0;
    }
}

// Hooked from 0x49EA50
void _declspec(naked) HOOK_GetFxQuality ()
{
    _asm
    {
        pushad
        mov     eax, [ecx+054h]         // Current FxQuality setting
        mov     dwFxQualityValue, eax

        mov     eax, [esp+32]           // Address GetFxQuality was called from
        push    eax                     
        call    MaybeAlterFxQualityValue
        add     esp, 4
    }

    _asm
    {
        popad
    }

    _asm
    {
        mov     eax, dwFxQualityValue
        retn
    }
}

// Hook to discover what vehicle will be calling GetFxQuality
void _declspec(naked) HOOK_StoreShadowForVehicle ()
{
    _asm
    {
        // Hooked from 0x70BDA0  5 bytes
        mov     eax, [esp+4]        // Get vehicle
        mov     ax, [eax+34]       // pEntity->m_nModelIndex
        mov     usCallingForVehicleModel, ax
        sub     esp, 44h 
        push    ebx
        mov     eax, 0x70F9B0   // CVolumetricShadowMgr::IsAvailable
        call    eax
        jmp     RETURN_StoreShadowForVehicle
    }
}

//#define HOOKPOS_FixClimbBug                             0x04B5AE5
#define HOOKPOS_FixClimbBug								0x05E3054
#define SECOND_HOOKPOS_FixClimbBug						0x05E2F8F
#define HOOKSIZE_FixClimbBug                            5
DWORD RETURN_FixClimbBug =								0x5E3059;
DWORD calltakedmg = 0x04b5ac0;
DWORD ignoretakedmg = 0x04b5ac6;
DWORD dmg = 0x04B5CDC;
//DWORD fixClimbBugAlt_DWORD = 005E3054;

DWORD g_iPlayer = 0;

DWORD dwTmp = 0;
void _declspec(naked) HOOK_FixClimbBug () { 

	_asm mov dwTmp,edx

	g_iPlayer = *(int*)0xB6F5F0;

	if(*(BYTE*)(g_iPlayer + 0x46D) == 34 || *(BYTE*)(g_iPlayer + 0x15C) != 0) {
		_asm {
			mov eax,28F934h
			mov ecx,28F934h
			mov edx,dwTmp
			call ignoretakedmg
			jmp RETURN_FixClimbBug
		}
	}


	_asm {
		mov eax,28F934h
		mov ecx,28F934h
		mov edx,dwTmp
		call calltakedmg
		jmp RETURN_FixClimbBug
	}
}

#define HOOKPOS_FixClimbBug2                             0x04B5AE5
#define HOOKSIZE_FixClimbBug2                            6
#define FixClimbBug_alt									0x04B5CDC
#define RETURN_FixClimbBug2								0x04B5AEB
DWORD dwRETURN_FixClimbBug2 =								0x04B5AEB;
DWORD fixClimbBugAlt_DWORD = 0x04B5CDC;

DWORD dwTmpEcx = 0;
DWORD dwTmpEax = 0;

void _declspec(naked) HOOK_FixClimbBug2 () { 

	/*asm {
		jnz FixClimbBug_alt
		cmp edi,1
		jne RETURN_FixClimbBug2
	}*/

	_asm {
		jnz FixClimbBug_alt
		mov [dwTmp],edi
		mov [dwTmpEcx],ecx
		mov [dwTmpEax],eax
	}

	if(dwTmp != 1) {
		_asm {
			mov ecx,[dwTmpEcx]
			mov eax,[dwTmpEax]
			jmp dwRETURN_FixClimbBug2
		}
	}


	g_iPlayer = *(int*)0xB6F5F0;

	if(*(BYTE*)(g_iPlayer + 0x46D) == 34 || *(BYTE*)(g_iPlayer + 0x15C) != 0) {
		_asm {
			jmp fixClimbBugAlt_DWORD
		}
	}

	_asm {
		mov ecx,[dwTmpEcx]
		mov eax,[dwTmpEax]
		jmp dwRETURN_FixClimbBug2
	}
}

void InitHooks_CrashFixHacks ()
{
    EZHookInstall ( CrashFix_Misc1 );
    EZHookInstall ( CrashFix_Misc2 );
    EZHookInstall ( CrashFix_Misc3 );
    EZHookInstall ( CrashFix_Misc4 );
    EZHookInstall ( CrashFix_Misc5 );
    EZHookInstall ( CrashFix_Misc6 );
    EZHookInstall ( CrashFix_Misc7 );
    EZHookInstall ( CrashFix_Misc8 );
    EZHookInstall ( CrashFix_Misc9 );
    EZHookInstall ( CrashFix_Misc10 );
    EZHookInstall ( CrashFix_Misc11 );
    EZHookInstall ( CrashFix_Misc12 );
    EZHookInstall ( CrashFix_Misc13 );
    EZHookInstall ( CrashFix_Misc14 );
    EZHookInstall ( FreezeFix_Misc15 );
    EZHookInstall ( CrashFix_Misc16 );
    EZHookInstall ( CrashFix_Misc17 );
    EZHookInstall ( CrashFix_Misc18 );
    EZHookInstall ( CrashFix_Misc19 );
    EZHookInstall ( CrashFix_Misc20 );
    EZHookInstall ( CrashFix_Misc21 );
    EZHookInstall ( CrashFix_Misc22 );
    EZHookInstall ( CrashFix_Misc23 );
    EZHookInstall ( CrashFix_Misc24 );
    EZHookInstall ( CrashFix_Misc25 );
    EZHookInstall ( CrashFix_Misc26 );
	EZHookInstall ( CrashFix_Misc27 );
	EZHookInstall ( CrashFix_Misc28 );
	EZHookInstall ( CrashFix_Misc29 );
	HookInstall ( HOOKPOS_GetFxQuality, (DWORD)HOOK_GetFxQuality, 5 );
    HookInstall ( HOOKPOS_StoreShadowForVehicle, (DWORD)HOOK_StoreShadowForVehicle, 9 );
	EZHookInstall ( CClumpModelInfo_GetFrameFromId );
	EZHookInstall ( Rtl_fopen );
	EZHookInstall ( FixClimbBug );
	EZHookInstall ( FixClimbBug2 );
	HookInstall( SECOND_HOOKPOS_FixClimbBug, (DWORD)HOOK_FixClimbBug, 5);
}