#if defined _INC_FIX_SCOPE
	#endinput
#endif
#define _INC_FIX_SCOPE
#include <a_samp>

#if !defined PRESSING
	#define PRESSING(%0,%1) (%0 & (%1))
#endif	

#if !defined RELEASED
	#define RELEASED(%0) (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))	
#endif

enum E_FIX_SCOPE {

	fs_index,
	fs_modelid,
	fs_bone,
	Float: fs_offsetX,
	Float: fs_offsetY,
	Float: fs_offsetZ,
	Float: fs_rotX,
	Float: fs_rotY,
	Float: fs_rotZ,
	Float: fs_scaleX,
	Float: fs_scaleY,
	Float: fs_scaleZ,
	fs_materialcolor1,
	fs_materialcolor2
};
static FS_AttachData[MAX_PLAYERS][MAX_PLAYER_ATTACHED_OBJECTS][E_FIX_SCOPE];
static bool: Checked[MAX_PLAYERS];
stock FS_H_RemovePlayerAttachedObject(playerid, index) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return false;
	}
	if (index < 0 || index >= MAX_PLAYER_ATTACHED_OBJECTS) {
		return false;
	}
	if(!Checked[playerid]) {
		FS_AttachData[playerid][index][fs_offsetX] = 
		FS_AttachData[playerid][index][fs_offsetY] = 
		FS_AttachData[playerid][index][fs_offsetZ] = 
		FS_AttachData[playerid][index][fs_rotX] = 
		FS_AttachData[playerid][index][fs_rotY] = 
		FS_AttachData[playerid][index][fs_rotZ] = 
		FS_AttachData[playerid][index][fs_scaleX] = 
		FS_AttachData[playerid][index][fs_scaleY] = 
		FS_AttachData[playerid][index][fs_scaleZ] = 
		FS_AttachData[playerid][index][fs_materialcolor1] = 
		FS_AttachData[playerid][index][fs_materialcolor2] = 
		FS_AttachData[playerid][index][fs_index] = 
		FS_AttachData[playerid][index][fs_modelid] = 
		FS_AttachData[playerid][index][fs_bone]  = -1;
	}
	Checked[playerid] = false;
	return RemovePlayerAttachedObject(playerid, index);
}
#if defined _ALS_RemovePlayerAttachedObject
	#undef RemovePlayerAttachedObject
#else
	#define _ALS_RemovePlayerAttachedObject
#endif
#define RemovePlayerAttachedObject FS_H_RemovePlayerAttachedObject

stock FS_RemovePlayerAttachedObject(playerid, index) //No hook 
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return false;
	}
	if (index < 0 || index >= MAX_PLAYER_ATTACHED_OBJECTS) {
		return false;
	}
	if(FS_AttachData[playerid][index][fs_index] == index)
	{
		if(FS_AttachData[playerid][index][fs_bone] == 2) // if bone is head
		{		
			Checked[playerid] = true;	
			return RemovePlayerAttachedObject(playerid, index);
		}
	}	
	return false;	
}
stock FS_SetPlayerAttachedObject(playerid, index, modelid, bone, Float:fOffsetX = 0.0, Float:fOffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0, materialcolor1 = 0, materialcolor2 = 0)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return false;
	}
	if (index < 0 || index >= MAX_PLAYER_ATTACHED_OBJECTS) {
		return false;
	}	
	if(bone == 2) // if bone is head
	{		
		FS_AttachData[playerid][index][fs_index] = index;
		FS_AttachData[playerid][index][fs_modelid] = modelid;
		FS_AttachData[playerid][index][fs_bone]  = bone;
		FS_AttachData[playerid][index][fs_offsetX] = fOffsetX;
		FS_AttachData[playerid][index][fs_offsetY] = fOffsetY;
		FS_AttachData[playerid][index][fs_offsetZ] = fOffsetZ;
		FS_AttachData[playerid][index][fs_rotX] = fRotX;
		FS_AttachData[playerid][index][fs_rotY] = fRotY;
		FS_AttachData[playerid][index][fs_rotZ] = fRotZ;
		FS_AttachData[playerid][index][fs_scaleX] = fScaleX;
		FS_AttachData[playerid][index][fs_scaleY] = fScaleY;
		FS_AttachData[playerid][index][fs_scaleZ] = fScaleZ;
		FS_AttachData[playerid][index][fs_materialcolor1] = materialcolor1;
		FS_AttachData[playerid][index][fs_materialcolor2] = materialcolor2;
	}
	return SetPlayerAttachedObject(playerid, index, modelid, bone, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ, materialcolor1, materialcolor2);
}
#if defined _ALS_SetPlayerAttachedObject
	#undef SetPlayerAttachedObject
#else
	#define _ALS_SetPlayerAttachedObject
#endif
#define SetPlayerAttachedObject FS_SetPlayerAttachedObject

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (PRESSING(newkeys, KEY_HANDBRAKE)) { //RMB (Right Mouse Button)
		if (WEAPON_SNIPER <= GetPlayerWeapon(playerid) <= WEAPON_HEATSEEKER) { //Weapons
			for (new index; index < MAX_PLAYER_ATTACHED_OBJECTS; index++) { //Loop all slots
				if (IsPlayerAttachedObjectSlotUsed(playerid, index)) {
					FS_RemovePlayerAttachedObject(playerid, index);
				}
			}	
		}
	}
	else if (RELEASED(KEY_HANDBRAKE)) { //RMB (Right Mouse Button)
		if (WEAPON_SNIPER <= GetPlayerWeapon(playerid) <= WEAPON_HEATSEEKER) { //Weapon
			for (new index; index < MAX_PLAYER_ATTACHED_OBJECTS; index++) { //Loop all slots
				SetPlayerAttachedObject(playerid, 
				FS_AttachData[playerid][index][fs_index],
				FS_AttachData[playerid][index][fs_modelid],
				FS_AttachData[playerid][index][fs_bone],
				FS_AttachData[playerid][index][fs_offsetX],
				FS_AttachData[playerid][index][fs_offsetY],
				FS_AttachData[playerid][index][fs_offsetZ],
				FS_AttachData[playerid][index][fs_rotX],
				FS_AttachData[playerid][index][fs_rotY],
				FS_AttachData[playerid][index][fs_rotZ],
				FS_AttachData[playerid][index][fs_scaleX],
				FS_AttachData[playerid][index][fs_scaleY],
				FS_AttachData[playerid][index][fs_scaleZ],
				FS_AttachData[playerid][index][fs_materialcolor1],
				FS_AttachData[playerid][index][fs_materialcolor2]);
			}
		}
	}
#if defined FS_OnPlayerKeyStateChange
	return FS_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#else
	return true;
#endif
}
public OnPlayerConnect(playerid)
{
	Checked[playerid] = false;
	for (new index; index < MAX_PLAYER_ATTACHED_OBJECTS; index++) //Loop all slots
		FS_AttachData[playerid][index][fs_offsetX] = 
		FS_AttachData[playerid][index][fs_offsetY] = 
		FS_AttachData[playerid][index][fs_offsetZ] = 
		FS_AttachData[playerid][index][fs_rotX] = 
		FS_AttachData[playerid][index][fs_rotY] = 
		FS_AttachData[playerid][index][fs_rotZ] = 
		FS_AttachData[playerid][index][fs_scaleX] = 
		FS_AttachData[playerid][index][fs_scaleY] = 
		FS_AttachData[playerid][index][fs_scaleZ] = 
		FS_AttachData[playerid][index][fs_materialcolor1] = 
		FS_AttachData[playerid][index][fs_materialcolor2] = 
		FS_AttachData[playerid][index][fs_index] = 
		FS_AttachData[playerid][index][fs_modelid] = 
		FS_AttachData[playerid][index][fs_bone]  = -1;
#if defined FS_OnPlayerConnect
	return FS_OnPlayerConnect(playerid);
#else
	return true;
#endif
}
//ALS HOOKS
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect FS_OnPlayerConnect
#if defined FS_OnPlayerConnect
	forward FS_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange FS_OnPlayerKeyStateChange
#if defined FS_OnPlayerKeyStateChange
	forward FS_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif