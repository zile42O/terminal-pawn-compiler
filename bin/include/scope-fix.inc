#if defined _INC_FIX_SCOPE
	#endinput
#endif
#define _INC_FIX_SCOPE

#if !defined CHAIN_FORWARD
	#define CHAIN_FORWARD:%0_%2(%1)=%3; \
		forward %0_%2(%1); \
		public %0_%2(%1) <_ALS : _ALS_x0, _ALS : _ALS_x1> { return (%3); } \
		public %0_%2(%1) <> { return (%3); }
#endif	

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

stock FS_SetPlayerAttachedObject(playerid, index, modelid, bone, Float:fOffsetX = 0.0, Float:fOffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0, materialcolor1 = 0, materialcolor2 = 0)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return false;
	}
	if (index < 0 || index >= MAX_PLAYER_ATTACHED_OBJECTS) {
		return false;
	}
	FS_ClearBeforeSet(playerid, index);
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
	return SetPlayerAttachedObject(playerid, index, modelid, bone, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ, materialcolor1, materialcolor2);
}
#if defined _ALS_SetPlayerAttachedObject
	#undef SetPlayerAttachedObject
#else
	#define _ALS_SetPlayerAttachedObject
#endif
#define SetPlayerAttachedObject FS_SetPlayerAttachedObject

stock FS_ClearBeforeSet(playerid, index)
{	
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
	return true;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (PRESSING(newkeys, KEY_HANDBRAKE)) { //RMB (Right Mouse Button)
		if (GetPlayerWeapon(playerid) == 34) { //Weapon Sniper Rifle
	    	for (new index; index < MAX_PLAYER_ATTACHED_OBJECTS; index++) { //Loop all slots
	        	if (IsPlayerAttachedObjectSlotUsed(playerid, index)) {
	        	   RemovePlayerAttachedObject(playerid, index);
	        	}
	        }	
		}
	}
	else if (RELEASED(KEY_HANDBRAKE)) { //RMB (Right Mouse Button)
	    if (GetPlayerWeapon(playerid) == 34) { //Weapon Sniper Rifle
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
	return FS_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange(%0) CHAIN_PUBLIC:FS_OnPlayerKeyStateChange(%0)
CHAIN_FORWARD:FS_OnPlayerKeyStateChange(playerid, newkeys, oldkeys) = 1;