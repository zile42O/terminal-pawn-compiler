/*
	---------------------------------------
	IMPORTANT

		_ Camera Fix Lookat under MAP _

		@author: Zile & Kanada ( 42O )
		@version: 1.0.0
		@release: 21/01/2019

	---------------------------------------
 	TODO
			NOTHING
	---------------------------------------		

*/
#if defined _INC_VEHICLE_CAMERA_FIX
	#endinput
#endif
#define _INC_VEHICLE_CAMERA_FIX // For other includes


#if !defined _streamer_included
	#tryinclude <streamer>
	#if !defined _streamer_included
		#error The Streamer is required for camera-fix-vehicle.inc, get it here: https://github.com/samp-incognito/samp-streamer-plugin
	#endif
#endif


#if !defined COLANDREAS
	#tryinclude <colandreas>
	#if !defined COLANDREAS
		#error The ColAndreas is required for camera-fix-vehicle.inc, get it here: https://github.com/Pottus/ColAndreas
	#endif
#endif


#if !defined CAMVEH_FIX_DEBUG
	#define CAMVEH_FIX_DEBUG 			false 
#endif 

#if !defined CAMVEH_BLOCK_OBJECT_ID
	#define CAMVEH_BLOCK_OBJECT_ID 		19337
#endif 

#if !defined CAMVEH_OFFSET_Z
	#define CAMVEH_OFFSET_Z 			-29.5
#endif
#if !defined CAMVEH_STREAMDIS
	#define CAMVEH_STREAMDIS 			29.7
#endif	

#if !defined CAMVEH_DRAWDIS	
	#define CAMVEH_DRAWDIS 				29.7
#endif
new player_VehCamFixTimer[MAX_PLAYERS];
new	player_VehCameraFixObject[MAX_PLAYERS];


stock Float:GetDistance(Float:x1,Float:y1,Float:z1, Float:x2,Float:y2,Float:z2)
{
	return floatsqroot(floatpower(floatabs(floatsub(x2,x1)),2)+floatpower(floatabs(floatsub(y2,y1)),2)+floatpower(floatabs(floatsub(z2,z1)),2));
}

forward OnFixPlayerCamInVehicle(playerid);
public OnFixPlayerCamInVehicle(playerid)
{
	if(!IsPlayerInAnyVehicle(playerid))
	{
		KillTimer(player_VehCamFixTimer[playerid]);
		return false;
	}	
	new
	Float:CamX,
	Float:CamY,
	Float:CamZ,
	Float:HitX,
	Float:HitY,
	Float:HitZ;
	GetPlayerCameraPos(playerid, CamX, CamY, CamZ);	
	CA_RayCastLine(CamX, CamY, CamZ, CamX, CamY, CamZ-100.0, HitX, HitY, HitZ);
	new Float:DisFromCamToHit = GetDistance(CamX, CamY, CamZ , HitX, HitY, HitZ);

	if(HitZ < 1) DisFromCamToHit = 0;

	if(DisFromCamToHit > 0)
	{
		
		SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 0, -1, "none", "none", 16711680);
		SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 1, -1, "none", "none", 16711680);
		SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 2, -1, "none", "none", 16711680);
		SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 3, -1, "none", "none", 16711680);
	}	
	else
	{
		SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 0, 10817, "airportgnd_sfse", "black64", 0);
		SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 1, 10817, "airportgnd_sfse", "black64", 0);
		SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 2, 10817, "airportgnd_sfse", "black64", 0);
		SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 3, 10817, "airportgnd_sfse", "black64", 0);
	}
	
	#if CAMVEH_FIX_DEBUG == true

		new buff[180];
		format(buff,sizeof(buff),"Camera-Fix-Vehicle [debug]:{ffffff} The ground X: %f Y: %f Z: %f",  HitX, HitY, HitZ);
		SendClientMessage(playerid, 0x42f448AA, buff);
		format(buff,sizeof(buff),"Camera-Fix-Vehicle [debug]:{ffffff} The from cam to ground distance: %f ", DisFromCamToHit);
		SendClientMessage(playerid, 0x42f448AA, buff);

	#endif
	return true;

}

stock CamFixVeh_IsInvalidVehicle(id)
{	
	//Air
    if(id == 417 ||
    id == 425 ||  
    id == 447 ||
    id == 460 ||
    id == 469 ||
    id == 476 ||
    id == 487 ||
    id == 488 ||
    id == 497 ||
    id == 511 ||
    id == 512 ||
    id == 513 ||
    id == 519 ||
    id == 520 ||
    id == 548 ||
    id == 553 ||
    id == 563 ||
    id == 577 ||
    id == 592 ||
    id == 592 ||
    //Boats
    id == 430 ||
    id == 446 ||
    id == 452 ||
    id == 453 ||
    id == 454 ||
    id == 472 ||
    id == 473 ||
    id == 484 ||
    id == 493 ||
    id == 595 ||
    //RC CamFixVeh
    id == 464 ||
    id == 465 ||
    id == 501 ) return true;    
    return false;	
}

public OnPlayerDisconnect(playerid, reason)
{	
	KillTimer(player_VehCamFixTimer[playerid]);
#if defined CamFixVeh_OnPlayerDisconnect
    return CamFixVeh_OnPlayerDisconnect(playerid, reason);
#else
    return true;
#endif
}	
#if defined _ALS_OnPlayerDisconnect
   	#undef OnPlayerDisconnect
#else
   	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect CamFixVeh_OnPlayerDisconnect
#if defined CamFixVeh_OnPlayerDisconnect
    forward CamFixVeh_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(newstate == PLAYER_STATE_DRIVER && oldstate == PLAYER_STATE_ONFOOT)
	{
		new vehicleid,
			modelid;
		vehicleid = GetPlayerVehicleID(playerid);
		modelid = GetVehicleModel(vehicleid);
		if(!CamFixVeh_IsInvalidVehicle(modelid))
		{	
	
			player_VehCamFixTimer[playerid] = SetTimerEx("OnFixPlayerCamInVehicle", 100, true, "i", playerid);
			player_VehCameraFixObject[playerid] = CA_CreateDynamicObject_SC(CAMVEH_BLOCK_OBJECT_ID, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, playerid, .drawdistance = CAMVEH_DRAWDIS, .streamdistance = CAMVEH_STREAMDIS);
			SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 0, 10817, "airportgnd_sfse", "black64", 0);
			SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 1, 10817, "airportgnd_sfse", "black64", 0);
			SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 2, 10817, "airportgnd_sfse", "black64", 0);
			SetDynamicObjectMaterial(player_VehCameraFixObject[playerid], 3, 10817, "airportgnd_sfse", "black64", 0);
   			AttachDynamicObjectToVehicle(player_VehCameraFixObject[playerid], vehicleid, 0.0, 0.0, CAMVEH_OFFSET_Z, 0.0, 0.0, 0.0);		
		}	
	}	
	if(newstate ==  PLAYER_STATE_ONFOOT && oldstate == PLAYER_STATE_DRIVER)
	{
		DestroyDynamicObject(player_VehCameraFixObject[playerid]);
	}
#if defined CamFixVeh_OnPlayerStateChange
    return CamFixVeh_OnPlayerStateChange(playerid, newstate, oldstate);
#else
    return true;
#endif
}	
#if defined _ALS_OnPlayerStateChange
   	#undef OnPlayerStateChange
#else
   	#define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange CamFixVeh_OnPlayerStateChange
#if defined CamFixVeh_OnPlayerStateChange
    forward CamFixVeh_OnPlayerStateChange(playerid, newstate, oldstate);
#endif