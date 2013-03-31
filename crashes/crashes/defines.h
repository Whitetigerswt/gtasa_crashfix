#include "windows.h"

unsigned long HOOKPOS_FindPlayerCoors;
unsigned long HOOKPOS_FindPlayerCentreOfWorld;
unsigned long HOOKPOS_FindPlayerHeading;
unsigned long HOOKPOS_CStreaming_Update_Caller;
unsigned long HOOKPOS_CHud_Draw_Caller;
unsigned long HOOKPOS_CRunningScript_Process;
unsigned long HOOKPOS_CExplosion_AddExplosion;
unsigned long HOOKPOS_CRealTimeShadowManager__ReturnRealTimeShadow;
unsigned long HOOKPOS_CCustomRoadsignMgr__RenderRoadsignAtomic;
unsigned long HOOKPOS_Trailer_BreakTowLink;
unsigned long HOOKPOS_CRadar__DrawRadarGangOverlay;
unsigned long HOOKPOS_CTaskComplexJump__CreateSubTask;
unsigned long HOOKPOS_CTrain_ProcessControl_Derail;
unsigned long HOOKPOS_CVehicle_SetupRender;
unsigned long HOOKPOS_CVehicle_ResetAfterRender;
unsigned long HOOKPOS_CObject_Render;
unsigned long HOOKPOS_EndWorldColors;
unsigned long HOOKPOS_CWorld_ProcessVerticalLineSectorList;
unsigned long HOOKPOS_ComputeDamageResponse_StartChoking;
unsigned long HOOKPOS_CAutomobile__ProcessSwingingDoor;

unsigned long FUNC_CStreaming_Update;
unsigned long FUNC_CAudioEngine__DisplayRadioStationName;
unsigned long FUNC_CHud_Draw;

unsigned long ADDR_CursorHiding;
unsigned long ADDR_GotFocus;

unsigned long FUNC_CPlayerInfoBase;

#define HOOKPOS_FxManager_CreateFxSystem                    0x4A9BE0
#define HOOKPOS_FxManager_DestroyFxSystem                   0x4A9810

DWORD RETURN_FxManager_CreateFxSystem =                     0x4A9BE8;
DWORD RETURN_FxManager_DestroyFxSystem =                    0x4A9817;

#define HOOKPOS_CCam_ProcessFixed                           0x51D470
#define HOOKPOS_CTaskSimplePlayerOnFoot_ProcessPlayerWeapon 0x6859a0
#define HOOKPOS_CPed_IsPlayer                               0x5DF8F0

DWORD RETURN_CCam_ProcessFixed =                            0x51D475;
DWORD RETURN_CTaskSimplePlayerOnFoot_ProcessPlayerWeapon =  0x6859A7;
DWORD RETURN_CPed_IsPlayer =                                0x5DF8F6;

#define VAR_CollisionStreamRead_ModelInfo                   0x9689E0
#define HOOKPOS_CollisionStreamRead                         0x41B1D0
DWORD RETURN_CollisionStreamRead =                          0x41B1D6;

#define CALL_Render3DStuff                                  0x53EABF
#define FUNC_Render3DStuff                                  0x53DF40

#define CALL_CRenderer_Render                               0x53EA12
#define FUNC_CRenderer_Render                               0x727140

#define CALL_CBike_ProcessRiderAnims                        0x6BF425   // @ CBike::ProcessDrivingAnims

#define CALL_CTrafficLights_GetPrimaryLightState 			0x49DB5F
#define CALL_CTrafficLights_GetSecondaryLightState 			0x49DB6D

#define HOOKPOS_CGame_Process                               0x53C095
DWORD RETURN_CGame_Process =                                0x53C09F;

#define HOOKPOS_Idle                                        0x53E981
DWORD RETURN_Idle =                                         0x53E98B;

DWORD FUNC_CBike_ProcessRiderAnims =                        0x6B7280;
DWORD FUNC_CEntity_Render =                                 0x534310;

#define HOOKPOS_VehicleCamStart                             0x5245ED
DWORD RETURN_VehicleCamStart_success =                      0x5245F3;
DWORD RETURN_VehicleCamStart_failure =                      0x525E43;
#define HOOKPOS_VehicleCamTargetZTweak                      0x524A68
DWORD RETURN_VehicleCamTargetZTweak =                       0x524AA4;
#define HOOKPOS_VehicleCamLookDir1                          0x524DF1
DWORD RETURN_VehicleCamLookDir1 =                           0x524DF6;
#define HOOKPOS_VehicleCamLookDir2                          0x525B0E
DWORD RETURN_VehicleCamLookDir2 =                           0x525B73;
#define HOOKPOS_VehicleCamHistory                           0x525C56
DWORD RETURN_VehicleCamHistory =                            0x525D4A;
#define HOOKPOS_VehicleCamColDetect                         0x525D8D
DWORD RETURN_VehicleCamColDetect =                          0x525D92;
#define CALL_VehicleCamUp                                   0x525E1F
#define HOOKPOS_VehicleCamEnd                               0x525E3C
DWORD RETURN_VehicleCamEnd =                                0x525E42;

#define HOOKPOS_VehicleLookBehind                           0x5207E3
DWORD RETURN_VehicleLookBehind =                            0x520891;
#define CALL_VehicleLookBehindUp                            0x520E2A
#define HOOKPOS_VehicleLookAside                            0x520F70
DWORD RETURN_VehicleLookAside =                             0x520FDC;
#define CALL_VehicleLookAsideUp                             0x5211E0

#define HOOKPOS_OccupiedVehicleBurnCheck                    0x570C84
DWORD RETURN_OccupiedVehicleBurnCheck =                     0x570C8A;
#define HOOKPOS_UnoccupiedVehicleBurnCheck                  0x6A76DC
DWORD RETURN_UnoccupiedVehicleBurnCheck =                   0x6A76E4;
#define HOOKPOS_ApplyCarBlowHop                             0x6B3816
DWORD RETURN_ApplyCarBlowHop =                              0x6B3831;

#define HOOKPOS_CPhysical_ApplyGravity                      0x543081
DWORD RETURN_CPhysical_ApplyGravity =                       0x543093;

#define HOOKPOS_CWorld_SetWorldOnFire                       0x56B983
DWORD RETURN_CWorld_SetWorldOnFire =                        0x56B989;
#define HOOKPOS_CTaskSimplePlayerOnFire_ProcessPed          0x6336DA
DWORD RETURN_CTaskSimplePlayerOnFire_ProcessPed =           0x6336E0;
#define HOOKPOS_CFire_ProcessFire                           0x53AC1A
DWORD RETURN_CFire_ProcessFire =                            0x53AC1F;
#define HOOKPOS_CExplosion_Update                           0x7377D3
DWORD RETURN_CExplosion_Update =                            0x7377D8;
#define HOOKPOS_CWeapon_FireAreaEffect                      0x73EBFE
DWORD RETURN_CWeapon_FireAreaEffect =                       0x73EC03;

#define CALL_RenderScene_Plants                             0x53E103
#define HOOKPOS_RenderScene_end                             0x53E159
#define HOOKPOS_CPlantMgr_Render                            0x5DBC4C
DWORD RETURN_CPlantMgr_Render_success =                     0x5DBC52;
DWORD RETURN_CPlantMgr_Render_fail =                        0x5DBDAA;

#define HOOKPOS_CEventHandler_ComputeKnockOffBikeResponse   0x4BA06F
DWORD RETURN_CEventHandler_ComputeKnockOffBikeResponse =    0x4BA076;

#define HOOKPOS_CAnimManager_AddAnimation                   0x4d3aa0
DWORD RETURN_CAnimManager_AddAnimation =                    0x4D3AAA;
#define HOOKPOS_CAnimManager_BlendAnimation                 0x4D4610
DWORD RETURN_CAnimManager_BlendAnimation =                  0x4D4617;

#define HOOKPOS_CPed_GetWeaponSkill                         0x5e3b60
DWORD RETURN_CPed_GetWeaponSkill =                          0x5E3B68;

#define HOOKPOS_CPed_AddGogglesModel                        0x5E3ACB
DWORD RETURN_CPed_AddGogglesModel =                         0x5E3AD4;

#define FUNC_CWorld_Remove                                  0x563280
#define FUNC_CTagManager_ShutdownForRestart                 0x49CC60
unsigned int* VAR_NumTags                                   = (unsigned int *)0xA9AD70;
DWORD** VAR_TagInfoArray                                    = (DWORD **)0xA9A8C0;

#define HOOKPOS_CPhysical_ProcessCollisionSectorList        0x54BB93
DWORD RETURN_CPhysical_ProcessCollisionSectorList =         0x54BB9A;


#define HOOKPOS_CheckAnimMatrix_US                          0x7C5A5C
#define HOOKPOS_CheckAnimMatrix_EU                          0x7C5A9C
DWORD RETURN_CheckAnimMatrix_US =                           0x7C5A61;
DWORD RETURN_CheckAnimMatrix_EU =                           0x7C5AA1;
DWORD RETURN_CheckAnimMatrix_BOTH =                         0;

#define HOOKPOS_VehColCB                                    0x04C838D
DWORD RETURN_VehColCB =                                     0x04C83AA;

#define HOOKPOS_VehCol                                      0x06D6603
DWORD RETURN_VehCol =                                       0x06D660C;

// Handling fix - driveType is per model
#define HOOKPOS_CHandlingData_isNotRWD              0x6A048C
DWORD RETURN_CHandlingData_isNotRWD =               0x6A0493;
#define HOOKPOS_CHandlingData_isNotFWD              0x6A04BC
DWORD RETURN_CHandlingData_isNotFWD =               0x6A04C3;
// end of handling fix
#define CALL_CAutomobile_ProcessEntityCollision             0x6AD053
#define CALL_CBike_ProcessEntityCollision1                  0x6BDF82
#define CALL_CBike_ProcessEntityCollision2                  0x6BE0D1
#define CALL_CMonsterTruck_ProcessEntityCollision           0x6C8B9E
DWORD RETURN_ProcessEntityCollision =                             0x4185C0;

#define HOOKPOS_PreFxRender                                     0x049E650
DWORD RETURN_PreFxRender_US =                                   0x0404D1E;
DWORD RETURN_PreFxRender_EU =                                   0x0405855;
DWORD RETURN_PreFxRender_BOTH =                                 0;

#define HOOKPOS_PreHUDRender                                      0x053EAD8
DWORD RETURN_PreHUDRender =                                       0x053EADD;

#define HOOKPOS_LoadIPLInstance                                    0x4061E8
DWORD CALL_LoadIPLInstance   =                                     0x538090;
DWORD RETURN_LoadIPLInstance =                                     0x04061ED;

#define HOOKPOS_CWorld_LOD_SETUP                                  0x406224
#define HOOKPOS_CWorld_LOD_SETUP2                                 0x406326
DWORD CALL_CWorld_LODSETUP   =                                    0x404C90;

#define HOOKPOS_CBuilding_DTR                                     0x404180
DWORD JMP_CBuilding_DTR   =                                       0x535E90;

#define HOOKPOS_CDummy_DTR                                        0x532566
DWORD JMP_CDummy_DTR   =                                          0x535E90;

#define HOOKPOS_CObject_DTR                                       0x59F680
DWORD JMP_CObject_DTR  =                                          0x59F686;

#define HOOKPOS_AddBuildingInstancesToWorld_CWorldAdd             0x5B5348
DWORD JMP_CWorld_Add_AddBuildingInstancesToWorld_CALL_CWorldAdd = 0x563220;
DWORD RETURN_AddBuildingInstancesToWorld_CWorldAdd =              0x5B534D;


#define HOOKPOS_CWorld_Remove_CPopulation_ConvertToDummyObject    0x6146F8

#define HOOKPOS_CWorld_ADD_CPopulation_ConvertToRealObject              0x6145C7
DWORD JMP_CWorld_Add_CPopulation_ConvertToRealObject_Retn =             0x6145CC;
DWORD JMP_CWorld_Add_CPopulation_ConvertToRealObject_CallCWorldAdd =    0x563220;

#define HOOKPOS_ConvertToObject_CPopulationManageDummy            0x616091
DWORD CALL_Convert_To_Real_Object_CPopulation_ManageDummy = 0x614580;
DWORD JMP_RETN_Called_CPopulation_ManageDummy = 0x616097;
DWORD JMP_RETN_Cancel_CPopulation_ManageDummy = 0x616098;

#define HOOKPOS_CWorld_ADD_CPopulation_ConvertToDummyObject       0x61470C
DWORD CALL_CWorld_Add_CPopulation_ConvertToDummyObject = 0x563220;
DWORD JMP_RETN_Called_CPopulation_ConvertToDummyObject = 0x614712;
DWORD JMP_RETN_Cancelled_CPopulation_ConvertToDummyObject = 0x614715;

#define HOOKPOS_CEntity_IsOnScreen_FixObjectsScale      0x534575
DWORD JMP_CEntity_IsOnScreen_FixObjectsScale = 0x53457C;

#define HOOKPOS_CEventVehicleDamageCollision                    0x6A7657
DWORD JMP_CEventVehicleDamageCollision_RETN = 0x6A765D;

#define HOOKPOS_CEventVehicleDamageCollision_Plane              0x6CC4B3
DWORD JMP_CEventVehicleDamageCollision_Plane_RETN = 0x6CC4B8;

#define HOOKPOS_CEventVehicleDamageCollision_Bike               0x6B8EC6
DWORD JMP_CEventVehicleDamageCollision_Bike_RETN = 0x6B8ECC;

#define HOOKPOS_CClothes_RebuildPlayer                      0x5A82C0
DWORD RETURN_CClothes_RebuildPlayera                        = 0x5A82C8;
DWORD RETURN_CClothes_RebuildPlayerb                        = 0x5A837F;

#define HOOKPOS_CProjectileInfo_FindPlayerPed               0x739321
#define HOOKPOS_CProjectileInfo_FindPlayerVehicle           0x739570

#define HOOKPOS_CHeli_ProcessHeliKill                       0x6DB201
DWORD RETURN_CHeli_ProcessHeliKill_RETN_Cancel = 0x6DB9E0;
DWORD RETURN_CHeli_ProcessHeliKill_RETN_Cont_Zero = 0x6DB207;
DWORD RETURN_CHeli_ProcessHeliKill_6DB437h = 0x6DB437;

FLOAT fFalseHeading;
bool bSetCenterOfWorld;
DWORD dwVectorPointer;
bool bInStreamingUpdate;
bool bHideRadar;
bool bHasProcessedScript;
float fX, fY, fZ;
DWORD RoadSignFixTemp;
DWORD dwEAEG = 0;
bool m_bExplosionsDisabled;
float fGlobalGravity = 0.008f;
float fLocalPlayerGravity = 0.008f;
float fLocalPlayerCameraRotation = 0.0f;
bool bCustomCameraRotation = false;
unsigned char ucTrafficLightState = 0;
bool bTrafficLightsBlocked = false;
bool bInteriorSoundsEnabled = true;

bool bUsingCustomSkyGradient = false;
BYTE ucSkyGradientTopR = 0;
BYTE ucSkyGradientTopG = 0;
BYTE ucSkyGradientTopB = 0;
BYTE ucSkyGradientBottomR = 0;
BYTE ucSkyGradientBottomG = 0;
BYTE ucSkyGradientBottomB = 0;
bool bUsingCustomWaterColor = false;
float fWaterColorR = 0.0F;
float fWaterColorG = 0.0F;
float fWaterColorB = 0.0F;
float fWaterColorA = 0.0F;

void HOOK_FindPlayerCoors();
void HOOK_FindPlayerCentreOfWorld();
void HOOK_FindPlayerHeading();
void HOOK_CStreaming_Update_Caller();
void HOOK_CHud_Draw_Caller();
void HOOK_CRunningScript_Process();
void HOOK_CExplosion_AddExplosion();
void HOOK_CRealTimeShadowManager__ReturnRealTimeShadow();
void HOOK_CCustomRoadsignMgr__RenderRoadsignAtomic();
void HOOK_Trailer_BreakTowLink();
void HOOK_CRadar__DrawRadarGangOverlay();
void HOOK_CTaskComplexJump__CreateSubTask();
void HOOK_CBike_ProcessRiderAnims();
void HOOK_FxManager_CreateFxSystem ();
void HOOK_FxManager_DestroyFxSystem ();
void HOOK_CCam_ProcessFixed ();
void HOOK_Render3DStuff ();
void HOOK_CTaskSimplePlayerOnFoot_ProcessPlayerWeapon ();
void HOOK_CPed_IsPlayer ();
void HOOK_CTrain_ProcessControl_Derail ();
void HOOK_CVehicle_SetupRender ();
void HOOK_CVehicle_ResetAfterRender();
void HOOK_CObject_Render ();
void HOOK_EndWorldColors ();
void HOOK_CWorld_ProcessVerticalLineSectorList ();
void HOOK_ComputeDamageResponse_StartChoking ();
void HOOK_CollisionStreamRead ();
void HOOK_CPhysical_ApplyGravity ();
void HOOK_VehicleCamStart ();
void HOOK_VehicleCamTargetZTweak ();
void HOOK_VehicleCamLookDir1 ();
void HOOK_VehicleCamLookDir2 ();
void HOOK_VehicleCamHistory ();
void HOOK_VehicleCamUp ();
void HOOK_VehicleCamEnd ();
void HOOK_VehicleLookBehind ();
void HOOK_VehicleLookAside ();
void HOOK_OccupiedVehicleBurnCheck ();
void HOOK_UnoccupiedVehicleBurnCheck ();
void HOOK_ApplyCarBlowHop ();
void HOOK_CWorld_SetWorldOnFire ();
void HOOK_CTaskSimplePlayerOnFire_ProcessPed ();
void HOOK_CFire_ProcessFire ();
void HOOK_CExplosion_Update ();
void HOOK_CWeapon_FireAreaEffect ();
void HOOK_CGame_Process ();
void HOOK_Idle ();
void HOOK_RenderScene_Plants ();
void HOOK_RenderScene_end ();
void HOOK_CPlantMgr_Render ();
void HOOK_CEventHandler_ComputeKnockOffBikeResponse ();
void HOOK_CAnimManager_AddAnimation ();
void HOOK_CAnimManager_BlendAnimation ();
void HOOK_CPed_GetWeaponSkill ();
void HOOK_CPed_AddGogglesModel ();
void HOOK_CPhysical_ProcessCollisionSectorList ();
void HOOK_CrashFix_Misc1 ();
void HOOK_CrashFix_Misc2 ();
void HOOK_CrashFix_Misc3 ();
void HOOK_CrashFix_Misc4 ();
void HOOK_CrashFix_Misc5 ();
void HOOK_CrashFix_Misc6 ();
void HOOK_CrashFix_Misc7 ();
void HOOK_CrashFix_Misc8 ();
void HOOK_CrashFix_Misc9 ();
void HOOK_CrashFix_Misc10 ();
void HOOK_CrashFix_Misc11 ();
void HOOK_CrashFix_Misc12 ();
void HOOK_CrashFix_Misc13 ();
void HOOK_CrashFix_Misc14 ();
void HOOK_FreezeFix_Misc15 ();
void HOOK_CrashFix_Misc16 ();
void HOOK_CrashFix_Misc17 ();
void HOOK_CrashFix_Misc18 ();
void HOOK_CrashFix_Misc19 ();
void HOOK_CrashFix_Misc20 ();
void HOOK_CrashFix_Misc21 ();
void HOOK_CrashFix_Misc22 ();
void HOOK_CrashFix_Misc23 ();
void HOOK_CrashFix_Misc24 ();
void HOOK_CheckAnimMatrix ();
void HOOK_VehColCB ();
void HOOK_VehCol ();
void HOOK_isVehDriveTypeNotRWD ();
void HOOK_isVehDriveTypeNotFWD ();
void HOOK_PreFxRender ();
void HOOK_PreHUDRender ();

void HOOK_CTrafficLights_GetPrimaryLightState ();
void HOOK_CTrafficLights_GetSecondaryLightState ();

void HOOK_CAutomobile__ProcessSwingingDoor ();

void vehicle_lights_init ();

void HOOK_LoadIPLInstance ();

void HOOK_CWorld_LOD_SETUP ();

void Hook_AddBuildingInstancesToWorld ( );

void HOOK_CWorld_Remove_CPopulation_ConvertToDummyObject ( );

void HOOK_CWorld_Add_CPopulation_ConvertToDummyObject ( );

void Hook_CWorld_ADD_CPopulation_ConvertToRealObject ( );

void HOOK_ConvertToObject_CPopulationManageDummy ( );

void Hook_CBuilding_DTR ( );

void Hook_CDummy_DTR ( );

void Hook_CObject_DTR ( );

void HOOK_CEntity_IsOnScreen_FixObjectScale ();

void HOOK_CEventVehicleDamageCollision ( );

void HOOK_CEventVehicleDamageCollision_Plane ( );

void HOOK_CEventVehicleDamageCollision_Bike ( );

void HOOK_CClothes_RebuildPlayer ();

void HOOK_CProjectileInfo_Update_FindLocalPlayer_FindLocalPlayerVehicle ();

void HOOK_CHeli_ProcessHeliKill ();