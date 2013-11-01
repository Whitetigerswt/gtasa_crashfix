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
		Log* log = new Log("crashes.log");
		char err[256];
		sprintf_s(err, sizeof(err), "%s has failed to open!", szFilename);
		int lastIdx = strlen(szFilename) - 1;
		if(szFilename[lastIdx] == 'b' && szFilename[lastIdx-1] == '.') {
			delete log;
		}
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

// ---------------------------------------------------
// ---------------------------------------------------
// ---------------------------------------------------
#define COLLISION_IGNORE_NUM 1828
int     collision_ignore_array[COLLISION_IGNORE_NUM] = {
// levelXREF
2885, 2890, 2892, 2893, 2899, 2900, 2903, 2904, 2909, 2911, 2912, 2918, 2920, 2954, 2955, 
2956, 2957, 2959, 2960, 2963, 2964, 2965, 2968, 2969, 2970, 2971, 2972, 2973, 2974, 2975, 
2990, 2991, 2994, 3005, 3006, 3013, 3014, 3015, 3016, 3029, 3030, 3031, 3032, 3033, 3035, 
3036, 3037, 3038, 3039, 3040, 3041, 3042, 3043, 3046, 3049, 3050, 3052, 3053, 3054, 3055, 
3057, 3058, 3061, 3062, 3066, 3067, 3073, 3075, 3077, 3078, 3080, 3083, 3084, 3089, 3091, 
3093, 3094, 3095, 3107, 3109, 3110, 3113, 3114, 3115, 3117, 3134, 
// countryXREF
3294, 3295, 3296, 3297, 3298, 3299, 3300, 3301, 3302, 3352, 
// VEGASxref
3447, 3451, 3465, 
// LAxref
3603, 3604, 3605, 3606, 3607, 3610, 3611, 3612, 3613, 3615, 3616, 3617, 
// Levelmap
3898, 3921, 3922, 3926, 3927, 3929, 3930, 3931, 3932, 3935, 3936, 3937, 3939, 3940, 3941, 
3944, 3945, 3946, 3948, 
// LAn
3979, 3980, 3986, 3988, 4002, 4003, 4004, 4006, 4011, 4015, 4016, 4019, 4021, 4022, 4023, 
4034, 4048, 4060, 4102, 4106, 4112, 4113, 4114, 4117, 4120, 4121, 4123, 4141, 4170, 4171, 
4183, 4185, 4188, 4189, 4190, 4193, 4195, 4196, 4201, 4202, 4205, 4227, 4231, 
// LAn2
4560, 4563, 4564, 4574, 4594, 4602, 4605, 4609, 4638, 4639, 4640, 4641, 4642, 4711, 4714, 
4718, 4724, 
// LAs
4826, 4828, 4832, 4848, 4850, 4853, 4857, 4882, 4886, 4981, 4982, 4983, 4984, 4987, 4990, 
5001, 5005, 5006, 5007, 5010, 5011, 5017, 5024, 5030, 5032, 5043, 5044, 5051, 5056, 5060, 
5061, 5062, 5063, 
// LAs2
5126, 5129, 5132, 5148, 5152, 5153, 5154, 5155, 5158, 5160, 5166, 5167, 5170, 5171, 5175, 
5190, 5243, 5244, 5259, 5260, 5261, 5262, 5266, 5268, 5269, 5291, 5298, 5299, 5300, 5302, 
5304, 5305, 5306, 5309, 5310, 5312, 5338, 5340, 5358, 5359, 5360, 5363, 5364, 
// LAe
5394, 5400, 5401, 5405, 5409, 5410, 5414, 5415, 5416, 5417, 5421, 5422, 5463, 5464, 5465, 
5469, 5470, 5473, 5475, 5476, 5520, 5565, 5626, 5627, 5630, 5631, 5632, 5633, 5634, 5635, 
5636, 5637, 5638, 5641, 5644, 5654, 5659, 5660, 5676, 5677, 5678, 5681, 
// LAwn
5710, 5711, 5712, 5713, 5714, 5715, 5727, 5737, 5742, 5762, 5770, 5776, 5779, 5792, 5811, 
5814, 5816, 5817, 5820, 5821, 5822, 5837, 5838, 5839, 5840, 5848, 5849, 5855, 5856, 5873, 
5874, 5877, 5882, 5884, 5885, 5888, 5893, 5926, 5935, 5971, 5983, 5986, 5989, 5993, 6002, 
6005, 
// LAw
6046, 6049, 6051, 6052, 6056, 6062, 6066, 6068, 6110, 6189, 6191, 6223, 6228, 6230, 6243, 
6247, 6264, 
// LAw2
6283, 6287, 6288, 6289, 6295, 6298, 6299, 6342, 6344, 6370, 6400, 6405, 6407, 6413, 6415, 
6422, 6451, 6456, 6457, 6458, 6459, 6460, 6461, 6462, 6463, 6465, 6466, 6475, 6489, 6516, 
6517, 6522, 6523, 6524, 6829, 
// VEGASn
6867, 6868, 6871, 6874, 6875, 6907, 6908, 6909, 6910, 6946, 6954, 6955, 6957, 6958, 6960, 
6961, 6962, 6963, 6967, 6968, 6969, 6970, 6977, 6994, 7027, 7033, 7034, 7038, 7039, 7040, 
7089, 7093, 7094, 7095, 7102, 7103, 7104, 7105, 7153, 7172, 7186, 7187, 7188, 7189, 7190, 
7191, 7192, 7196, 7197, 7198, 7200, 7201, 7202, 7204, 7205, 7209, 7210, 7212, 7213, 7223, 
7224, 7227, 7228, 7229, 7231, 7234, 7236, 7238, 7243, 7269, 7273, 7276, 7277, 7287, 7291, 
7292, 7295, 7296, 7297, 7298, 7299, 7311, 7312, 7317, 7319, 7344, 7347, 7349, 7350, 7351, 
7352, 7361, 7367, 7368, 7369, 7370, 7371, 7377, 7378, 7379, 7380, 7381, 
// VEGASw
7415, 7418, 7423, 7424, 7425, 7426, 7488, 7489, 7492, 7493, 7494, 7495, 7497, 7504, 7505, 
7518, 7520, 7521, 7522, 7524, 7525, 7526, 7527, 7528, 7529, 7531, 7532, 7538, 7539, 7540, 
7554, 7560, 7561, 7583, 7584, 7586, 7592, 7593, 7595, 7596, 7597, 7599, 7606, 7610, 7617, 
7618, 7619, 7620, 7621, 7622, 7623, 7624, 7625, 7627, 7628, 7650, 7651, 7657, 7658, 7659, 
7662, 7663, 7664, 7665, 7666, 7692, 7707, 7708, 7709, 7832, 7834, 7836, 7837, 7838, 7839, 
7840, 7841, 7842, 7891, 7893, 7894, 7917, 7918, 7919, 7920, 7921, 7922, 7923, 7924, 7926, 
7927, 7930, 7931, 7933, 7939, 7947, 7948, 7949, 7950, 7952, 7953, 7954, 
// VEGASs
7979, 7980, 7981, 7982, 8034, 8038, 8041, 8042, 8044, 8057, 8058, 8059, 8060, 8061, 8062, 
8063, 8064, 8073, 8074, 8075, 8076, 8077, 8078, 8128, 8130, 8131, 8132, 8147, 8148, 8149, 
8150, 8151, 8152, 8153, 8154, 8155, 8163, 8165, 8167, 8169, 8170, 8206, 8207, 8208, 8209, 
8210, 8229, 8237, 8240, 8247, 8249, 8251, 8253, 8254, 8255, 8260, 8262, 8263, 8300, 8311, 
8313, 8314, 8315, 8320, 8335, 8337, 8339, 8341, 8342, 8345, 8369, 8375, 8378, 8391, 8394, 
8396, 8397, 8399, 8400, 8406, 8407, 8408, 8410, 8412, 8416, 8421, 8424, 8426, 8429, 8430, 
8431, 8434, 8435, 8436, 8437, 8481, 8489, 8491, 8492, 8494, 8498, 8499, 8501, 8503, 8504, 
8505, 8506, 8507, 8508, 8509, 8516, 8526, 8527, 8528, 8530, 8534, 8535, 8536, 8537, 8545, 
8546, 8549, 8550, 8551, 8556, 8557, 8558, 8559, 8563, 8591, 8597, 8618, 8620, 8635, 8636, 
8639, 8640, 8641, 8642, 8645, 8646, 8647, 8648, 8649, 8650, 8651, 8652, 8653, 8656, 8657, 
8658, 8659, 8662, 8668, 8669, 8670, 8673, 8674, 8675, 8676, 8680, 8681, 8682, 8683, 8684, 
8685, 8686, 8687, 8825, 8826, 8827, 8828, 8833, 8834, 8838, 8839, 8841, 8842, 8849, 8870, 
8871, 8872, 8873, 8875, 8877, 8879, 8881, 8882, 8883, 8884, 8885, 8886, 8889, 8947, 8948, 
8952, 8953, 8957, 8979, 8980, 8982, 8984, 8985, 8986, 8987, 8988, 8992, 8993, 8994, 8995, 
8996, 8997, 8998, 8999, 9029, 9030, 9031, 9032, 9033, 9041, 9043, 9054, 9055, 9070, 9071, 
9072, 9093, 9099, 9106, 9108, 9109, 9110, 9111, 9112, 9113, 9114, 9132, 9134, 9164, 9165, 
9166, 9167, 9168, 9169, 9171, 
// SFn
9237, 9238, 9245, 9258, 9270, 9271, 9272, 9273, 9274, 9275, 9278, 9279, 9280, 9281, 9282, 
9283, 9292, 9293, 9294, 9295, 9296, 9297, 9298, 9302, 9303, 9310, 9317, 9318, 9321, 9322, 
9323, 9325, 9326, 9330, 9331, 9332, 9333, 9334, 9335, 9336, 9337, 9339, 9340, 9343, 9344, 
9347, 9348, 9349, 9350, 9361, 9362, 9390, 
// SFw
9482, 9526, 9528, 9587, 9588, 9589, 9604, 9623, 9625, 9690, 9694, 9761, 9818, 9819, 9820, 
9821, 9822, 9837, 9838, 
// SFe
9949, 9950, 9951, 9958, 10024, 10029, 10145, 10166, 10182, 10226, 10227, 10229, 10246, 10248, 10270, 
10309, 
// SFs
10357, 10369, 10379, 10380, 10381, 10388, 10390, 10391, 10392, 10396, 10397, 10398, 10401, 10402, 10437, 
10442, 10445, 10558, 10575, 10610, 10611, 10624, 10626, 10630, 10631, 10633, 10634, 10671, 10675, 10683, 
10716, 
// SFse
10757, 10762, 10764, 10773, 10774, 10775, 10776, 10777, 10789, 10806, 10807, 10808, 10809, 10810, 10811, 
10814, 10825, 10828, 10829, 10830, 10831, 10832, 10835, 10836, 10837, 10838, 10839, 10840, 10843, 10844, 
10845, 10847, 10873, 10874, 10875, 10885, 10889, 10890, 10932, 10947, 10954, 10955, 10982, 10984, 10985, 
10986, 10987, 10992, 10994, 10999, 11008, 11009, 11010, 11011, 11014, 11015, 11081, 11087, 11089, 11090, 
11091, 11092, 11093, 11101, 11102, 11103, 11147, 11148, 11225, 11244, 11246, 11247, 11290, 11292, 11293, 
11295, 11313, 11314, 11319, 11327, 11334, 11352, 11353, 11359, 11360, 11383, 11390, 11391, 11393, 11395, 
11416, 
// countryN
11422, 11423, 11424, 11425, 11426, 11427, 11428, 11449, 11450, 11452, 11453, 11454, 11455, 11456, 11457, 
11458, 11459, 11461, 11462, 11463, 11469, 11470, 11471, 11472, 11473, 11474, 11475, 11479, 11480, 11481, 
11482, 11483, 11489, 11490, 11491, 11492, 11494, 11497, 11498, 11499, 11500, 11501, 11502, 11503, 11504, 
11505, 11543, 11544, 11546, 11547, 11551, 11556, 11558, 11566, 11567, 11568, 11572, 11609, 11611, 11623, 
11631, 11664, 11665, 11666, 11674, 
// countryE
12805, 12807, 12808, 12821, 12839, 12840, 12842, 12845, 12846, 12848, 12854, 12858, 12859, 12860, 12861, 
12911, 12912, 12913, 12914, 12915, 12917, 12918, 12921, 12922, 12927, 12929, 12930, 12932, 12933, 12934, 
12935, 12936, 12937, 12938, 12940, 12943, 12950, 12954, 12955, 12956, 12957, 12958, 12961, 12972, 12977, 
12985, 12986, 12987, 12992, 13002, 13003, 13004, 13005, 13011, 13016, 13018, 13024, 13025, 13028, 13029, 
13033, 13034, 13045, 13053, 13054, 13057, 13060, 13061, 13097, 13098, 13108, 13109, 13118, 13132, 13140, 
13141, 13143, 13153, 13187, 13188, 13190, 13198, 13206, 13296, 13360, 13367, 13369, 13435, 13436, 13438, 
13445, 13450, 13489, 
// STADINT
13590, 13591, 13592, 13593, 13594, 13596, 13597, 13598, 13599, 13600, 13601, 13604, 13605, 13606, 13609, 
13623, 13625, 13628, 13629, 13630, 13631, 13632, 13653, 13656, 13666, 13667, 
// LAhills
13681, 13694, 13695, 13696, 13697, 13699, 13705, 13712, 13714, 13722, 13725, 13728, 13729, 13731, 13743, 
13744, 13747, 13748, 13754, 13755, 13758, 13759, 13817, 13831, 13855, 13861, 13862, 13863, 13864, 13865, 
13866, 
// int_LA
14386, 14391, 14392, 14396, 14397, 14398, 14399, 14401, 14403, 14405, 14413, 14433, 14434, 14435, 14436, 
14437, 14440, 14445, 14446, 14448, 14449, 14450, 14451, 14452, 14454, 14455, 14458, 14459, 14463, 14464, 
14467, 14477, 14480, 14481, 14482, 14483, 14486, 14490, 14491, 14493, 14494, 14497, 14502, 14503, 14504, 
14505, 14507, 14508, 14509, 14510, 14513, 14516, 14517, 14518, 14519, 14524, 
// int_SF
14532, 14535, 14540, 14543, 14544, 14547, 14549, 14550, 14551, 14552, 14556, 14558, 
// int_VEGAS
14560, 14562, 14565, 14566, 14567, 14568, 14569, 14571, 14573, 14574, 14575, 14578, 14582, 14583, 14584, 
14585, 14586, 14587, 14589, 14599, 14600, 14604, 14608, 14613, 14616, 14617, 14618, 14619, 14620, 14622, 
14632, 14633, 14635, 14638, 14640, 
// int_country
14651, 14653, 14657, 
// GEN_INT1
14660, 14666, 14673, 14676, 14678, 14679, 14683, 14685, 14693, 
// GEN_INT2
14704, 14705, 14715, 14716, 
// GEN_INTb
14739, 14741, 14745, 14757, 14764, 
// GEN_INT3
14770, 14772, 14780, 14781, 14782, 14786, 14790, 14791, 14792, 14796, 14797, 14799, 14800, 14802, 14804, 
14805, 14806, 14807, 14809, 14810, 14811, 14813, 14816, 14817, 14819, 14820, 14826, 14828, 14830, 14831, 
14832, 14835, 14836, 14837, 14839, 14840, 
// GEN_INT4
14843, 14844, 14850, 14854, 14855, 14856, 14861, 14866, 14867, 14869, 14870, 14872, 14873, 14875, 14878, 
14880, 14883, 14888, 14891, 14895, 
// SAVEHOUSE
15025, 15026, 15032, 15035, 15036, 15037, 15039, 15050, 15052, 15064, 
// countryN2
16000, 16001, 16002, 16003, 16004, 16005, 16006, 16010, 16011, 16012, 16016, 16017, 16018, 16019, 16020, 
16021, 16023, 16051, 16052, 16053, 16054, 16060, 16061, 16062, 16064, 16065, 16066, 16067, 16081, 16083, 
16086, 16087, 16088, 16089, 16090, 16091, 16092, 16094, 16095, 16096, 16098, 16100, 16101, 16104, 16105, 
16106, 16107, 16108, 16121, 16123, 16125, 16126, 16129, 16130, 16131, 16132, 16134, 16135, 16136, 16138, 
16139, 16140, 16141, 16142, 16143, 16144, 16145, 16146, 16151, 16152, 16154, 16281, 16285, 16287, 16308, 
16313, 16314, 16315, 16320, 16321, 16322, 16324, 16327, 16328, 16329, 16335, 16340, 16341, 16342, 16343, 
16345, 16346, 16347, 16349, 16351, 16353, 16359, 16360, 16362, 16365, 16366, 16367, 16369, 16370, 16377, 
16378, 16385, 16386, 16387, 16396, 16398, 16399, 16400, 16401, 16402, 16404, 16405, 16406, 16410, 16411, 
16434, 16436, 16437, 16438, 16439, 16442, 16446, 16475, 16477, 16479, 16480, 16481, 16500, 16501, 16503, 
16530, 16531, 16532, 16533, 16534, 16535, 16562, 16563, 16564, 16568, 16599, 16601, 16602, 16605, 16613, 
16641, 16646, 16650, 16662, 16664, 16666, 16667, 16668, 16669, 16670, 16671, 16673, 16675, 16689, 16690, 
16732, 16766, 16767, 16769, 16770, 16773, 16774, 16775, 
// countryW
17026, 17027, 17028, 17029, 17030, 17034, 17035, 17036, 17037, 17038, 17039, 17040, 17041, 17042, 17044, 
17045, 17049, 17050, 17051, 17052, 17053, 17054, 17055, 17057, 17059, 17060, 17063, 17064, 17065, 17066, 
17067, 17069, 17071, 17072, 17074, 17076, 17110, 17111, 17112, 17120, 17293, 17294, 17295, 17298, 17299, 
17307, 17309, 17323, 17324, 17335, 17454, 17457, 17472, 
// LAe2
17501, 17502, 17510, 17514, 17516, 17522, 17523, 17525, 17527, 17528, 17532, 17539, 17540, 17542, 17543, 
17544, 17550, 17551, 17565, 17566, 17573, 17575, 17576, 17583, 17585, 17699, 17804, 17805, 17840, 17851, 
17857, 17860, 17863, 17872, 17874, 17875, 17876, 17886, 17887, 17898, 17911, 17912, 17913, 17924, 17936, 
17937, 17938, 17939, 17940, 17941, 17942, 17947, 17949, 17951, 17968, 
// GEN_INT5
18003, 18005, 18012, 18014, 18016, 18017, 18019, 18032, 18034, 18035, 18040, 18041, 18042, 18043, 18044, 
18047, 18050, 18051, 18055, 18057, 18059, 18060, 18061, 18064, 18067, 18069, 18070, 18074, 18077, 18081, 
18083, 18089, 18090, 18092, 18104, 18105, 18109, 
// countryS
18201, 18202, 18230, 18232, 18233, 18234, 18235, 18236, 18238, 18257, 18258, 18259, 18260, 18263, 18264, 
18283, 18284, 18286, 18287, 18288, 18289, 18290, 18291, 18292, 18365, 18366, 18367, 18368, 18451, 18452, 
18551, 18552, 18561, 18563, 18565, 18566, 18567, 18568, 18569, 18608, 18609, 
};

bool ignoreColWithObjectID ( int iModelID )
{
        if ( iModelID <= 9500 )
        {
                // Vegepart
                if ( iModelID <= 792 && iModelID >= 615 )
                        return true;

                // ProcObj
                if ( iModelID <= 906 && iModelID >= 800 )
                        return true;

                // propExt
                if ( iModelID <= 964 && iModelID >= 910 )
                        return true;

                // Barriers
                if ( iModelID <= 998 && iModelID >= 966 )
                        return true;

                // Dynamic
                if ( iModelID <= 1325 && iModelID >= 1207 )
                        return true;

                // Dynamic2
                if ( iModelID <= 1572 && iModelID >= 1327 )
                        return true;

                // MultiObj
                if ( iModelID <= 1698 && iModelID >= 1574 )
                        return true;

                // props & props2
                if ( iModelID <= 2882 && iModelID >= 1700 )
                        return true;

                // levelXREF
                if ( iModelID <= 2938 && iModelID >= 2923 )
                        return true;

                if ( iModelID <= 2952 && iModelID >= 2941 )
                        return true;

                if ( iModelID <= 2988 && iModelID >= 2977 )
                        return true;

                // countryXREF
                if ( iModelID <= 3279 && iModelID >= 3167 )
                        return true;

                if ( iModelID <= 3291 && iModelID >= 3281 )
                        return true;

                if ( iModelID <= 3350 && iModelID >= 3330 )
                        return true;

                if ( iModelID <= 3374 && iModelID >= 3354 )
                        return true;

                if ( iModelID <= 3398 && iModelID >= 3376 )
                        return true;

                if ( iModelID <= 3424 && iModelID >= 3403 )
                        return true;

                // VEGASxref
                if ( iModelID <= 3441 && iModelID >= 3430 )
                        return true;

                if ( iModelID <= 3463 && iModelID >= 3454 )
                        return true;

                if ( iModelID <= 3482 && iModelID >= 3467 )
                        return true;

                if ( iModelID <= 3500 && iModelID >= 3489 )
                        return true;

                // LAxref
                if ( iModelID <= 3598 && iModelID >= 3502 )
                        return true;

                if ( iModelID <= 3740 && iModelID >= 3619 )
                        return true;

                if ( iModelID <= 3763 && iModelID >= 3742 )
                        return true;

                if ( iModelID <= 3783 && iModelID >= 3765 )
                        return true;

                // SFxref
                if ( iModelID <= 3819 && iModelID >= 3785 )
                        return true;

                if ( iModelID <= 3888 && iModelID >= 3850 )
                        return true;

                // LAn
                if ( iModelID <= 4100 && iModelID >= 4084 )
                        return true;

                // seabed
                if ( iModelID <= 4527 && iModelID >= 4504 )
                        return true;

                // LAs
                if ( iModelID <= 5082 && iModelID >= 5066 )
                        return true;

                // VEGASn
                if ( iModelID <= 6934 && iModelID >= 6919 )
                        return true;

                if ( iModelID <= 7025 && iModelID >= 7014 )
                        return true;

                // VEGASw
                if ( iModelID <= 7516 && iModelID >= 7507 )
                        return true;

                // VEGASs
                if ( iModelID <= 8196 && iModelID >= 8173 )
                        return true;

        }else{
                // SFse
                if ( iModelID <= 11242 && iModelID >= 11233 )
                        return true;

                // countryN
                if ( iModelID <= 11447 && iModelID >= 11431 )
                        return true;

                // STADINT
                if ( iModelID <= 13650 && iModelID >= 13634 )
                        return true;

                // GEN_INT2
                if ( iModelID <= 14728 && iModelID >= 14719 )
                        return true;

                // countryN2
                if ( iModelID <= 16079 && iModelID >= 16069 )
                        return true;

                if ( iModelID <= 16119 && iModelID >= 16110 )
                        return true;

                if ( iModelID <= 16279 && iModelID >= 16267 )
                        return true;

                if ( iModelID <= 16305 && iModelID >= 16293 )
                        return true;

                if ( iModelID <= 16637 && iModelID >= 16627 )
                        return true;

                // countryW
                if ( iModelID <= 17011 && iModelID >= 17000 )
                        return true;

                if ( iModelID <= 17024 && iModelID >= 17013 )
                        return true;

                // countryS
                if ( iModelID <= 18228 && iModelID >= 18216 )
                        return true;

                if ( iModelID <= 18255 && iModelID >= 18240 )
                        return true;

                if ( iModelID <= 18281 && iModelID >= 18267 )
                        return true;

                if ( iModelID <= 18446 && iModelID >= 18432 )
                        return true;

        }

        int i = (iModelID >= 9500) ? 908 : 0;
        int maxNum = (i == 0) ? 908 : COLLISION_IGNORE_NUM;
        for ( ; i < maxNum; i++ )
        {
                if ( collision_ignore_array[i] == iModelID )
                        return true;
        }
        return false;
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
}
