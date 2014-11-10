#include <Windows.h>
#include "fixes.h"
#include "multiplayer_hooksystem.h"


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

//
// Volumetric shadow hooks
//

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

#define HOOKPOS_FixClimbBug								0x05E3054
#define SECOND_HOOKPOS_FixClimbBug						0x05E2F8F
#define HOOKSIZE_FixClimbBug                            5

DWORD	RETURN_FixClimbBug								= 0x5E3059;
DWORD	dwSendDmgToServer								= 0x04b5ac0;
DWORD	dwDontSendDmgToServer							= dwSendDmgToServer + 0x6;

DWORD	g_dwPlayer										= 0;

DWORD	dwTmp											= 0;
DWORD	dwTmpEcx										= 0;
DWORD	dwTmpEax										= 0;

void _declspec(naked) HOOK_FixClimbBug () { 

	_asm mov [dwTmp],edx
	_asm mov [dwTmpEcx],ecx
	_asm mov [dwTmpEax],eax

	g_dwPlayer = *(int*)0xB6F5F0;

	if(*(BYTE*)(g_dwPlayer + 0x46D) == 34 || *(BYTE*)(g_dwPlayer + 0x15C) != 0) {
		_asm {
			mov		edx,[dwTmp]
			mov		eax,[dwTmpEax]
			mov		ecx,[dwTmpEcx]
			call	dwDontSendDmgToServer
			jmp		RETURN_FixClimbBug
		}
	}


	_asm {
		mov		edx,[dwTmp]
		mov		eax,[dwTmpEax]
		mov		ecx,[dwTmpEcx]
		call	dwSendDmgToServer
		jmp		RETURN_FixClimbBug
	}
}

#define HOOKPOS_FixClimbBug2                            0x04B5AE5
#define HOOKSIZE_FixClimbBug2                           6
#define FixClimbBug_alt									0x04B5CDC
#define RETURN_FixClimbBug2								0x04B5AEB
DWORD	dwRETURN_FixClimbBug2							= 0x04B5AEB;
DWORD	dwfixClimbBugAlt								= 0x04B5CDC;

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
			mov		ecx,[dwTmpEcx]
			mov		eax,[dwTmpEax]
			jmp		dwRETURN_FixClimbBug2
		}
	}


	g_dwPlayer = *(int*)0xB6F5F0;

	if(*(BYTE*)(g_dwPlayer + 0x46D) == 34 || *(BYTE*)(g_dwPlayer + 0x15C) != 0) {
		_asm {
			jmp dwfixClimbBugAlt
		}
	}

	_asm {
		mov		ecx,[dwTmpEcx]
		mov		eax,[dwTmpEax]
		jmp		dwRETURN_FixClimbBug2
	}
}

/*
DWORD ClimbConstructorRet = 0x04039B2;
void _declspec(naked) HOOK_ClimbConstructor () { 

	_asm pushad
	lastClimbed = GetTickCount() + 10000;
	_asm {
		popad
		push -1
		jmp ClimbConstructorRet
	}

}*/

//////////////////////////////////////////////////////////////////////////////////////////
//
// Fx_AddBulletImpact
//
// Modify bullet impact effect type
//
// 1 = sparks
// 2 = sand
// 3 = wood
// 4 = dust
//
//////////////////////////////////////////////////////////////////////////////////////////
DWORD lTicks;
int OnMY_Fx_AddBulletImpact(int iType)
{
	// Limit sand or dust effect due to performance issues
	if (iType == 2 || iType == 4)
	{
		if (lTicks < GetTickCount())
			lTicks = GetTickCount() + 500;
		else
			iType = 1;                  // Otherwise replace with spark
	}
	return iType;
}

// Hook info
#define HOOKPOS_Fx_AddBulletImpact                         0x049F3E8
#define HOOKSIZE_Fx_AddBulletImpact                        5
DWORD RETURN_Fx_AddBulletImpact = 0x049F3ED;

void _declspec(naked) HOOK_Fx_AddBulletImpact(void)
{
	_asm
	{
		pushad
			push    eax
			call    OnMY_Fx_AddBulletImpact
			mov[esp + 0], eax         // Put result temp
			add     esp, 4 * 1
			popad

			mov     esi, [esp - 32 - 4 * 1]    // Get result temp
			mov     eax, ds:0x0B6F03C
			jmp     RETURN_Fx_AddBulletImpact
	}
}

void InitHooks_Fixes ()
{
	HookInstall ( HOOKPOS_GetFxQuality, (DWORD)HOOK_GetFxQuality, 5 );
    HookInstall ( HOOKPOS_StoreShadowForVehicle, (DWORD)HOOK_StoreShadowForVehicle, 9 );

	EZHookInstall ( FixClimbBug );
	EZHookInstall ( FixClimbBug2 );
	HookInstall( SECOND_HOOKPOS_FixClimbBug, (DWORD)HOOK_FixClimbBug, 5);
	EZHookInstall(Fx_AddBulletImpact);
	//HookInstall( 0x67A110, (DWORD)HOOK_ClimbConstructor, 7);
}