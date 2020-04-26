/*
Anti troll de veículos
18/03/2017 - Jelly23
*/

#if !defined FILTERSCRIPT

/*Uso de foreach*/
#define ATV_USAR_FOREACH (true)

/*Variavel e callbacks*/
static 
ATV_Timer;

forward ATV_TrollCheck();
forward OnPlayerTroll(playerid);

public OnGameModeInit()
{
	ATV_Timer = SetTimer("ATV_TrollCheck", 1000, true);
	#if defined JT_OnGameModeInit
	return JT_OnGameModeInit();
	#else
	return 1;
	#endif
}

public OnGameModeExit()
{
	KillTimer(ATV_Timer);
	#if defined JT_OnGameModeExit
	return JT_OnGameModeExit();
	#else
	return 1;
	#endif
}

public ATV_TrollCheck()
{
	#if ATV_USAR_FOREACH true
	foreach(new i: Player)
	{
		if(IsPlayerInAnyVehicle(i))
		{
			if(GetPlayerVehicleID(i) != GetPVarInt(i, "ATV_Carro"))
			{
				if(gettime() > GetPVarInt(i, "ATV_Tempo"))
				{
					OnPlayerTroll(i);
				}
			}
		}
	}
	#else
	for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if(IsPlayerConnected(i))
		{
			if(IsPlayerInAnyVehicle(i))
			{
				if(GetPlayerVehicleID(i) != GetPVarInt(i, "ATV_Carro"))
				{
					if(gettime() > GetPVarInt(i, "ATV_Tempo"))
					{
						OnPlayerTroll(i);
					}
				}
			}
		}
	}
	#endif
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(newstate == PLAYER_STATE_DRIVER)
	{
		if(GetPVarInt(playerid, "ATV_RJack") == 1)
		{
			OnPlayerTroll(playerid);
		}
		if(oldstate == PLAYER_STATE_PASSENGER)
		{
			OnPlayerTroll(playerid);
		}
	}

	#if defined JT_OnPlayerStateChange
	return JT_OnPlayerStateChange(playerid, newstate, oldstate);
	#else
	return 1;
	#endif
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	SetPVarInt(playerid, "ATV_Carro", vehicleid);
	if(!ispassenger)
	{
		SetPVarInt(playerid, "ATV_RJack", 1);
	}
	#if defined JT_OnPlayerEnterVehicle
	return JT_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	#else
	return 1;
	#endif
}

public OnPlayerUpdate(playerid)
{
	if(GetPVarInt(playerid, "ATV_RJack") == 1 && GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_ENTER_VEHICLE)
	{
		SetPVarInt(playerid, "ATV_RJack", 0);
		SetPVarInt(playerid, "ATV_Tempo", gettime()+3);
	}
	#if defined JT_OnPlayerUpdate
	return JT_OnPlayerUpdate(playerid);
	#else
	return 1;
	#endif
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
   	if(!passenger_seat && GetVehicleDistanceFromPoint(vehicleid, Pozicija_Vozila[vehicleid][0], Pozicija_Vozila[vehicleid][1], Pozicija_Vozila[vehicleid][2]) > 50.0)
	{
		new Float:Poz[3], string[144];
 		GetXYZInFrontOfPlayer(playerid, Poz[0], Poz[1], Poz[2], 5.0);
	 	if(GetVehicleDistanceFromPoint(vehicleid, Poz[0], Poz[1], Poz[2]) < 7.0)
	 	{
	 		format(string,sizeof(string), "[!] Zile's AC:{ffffff} Igrac %s(ID: %d) moguce da porta vozilo(VID:%d) do sebe, kick/ban ga ako spama!", GetName(playerid), playerid, vehicleid);
	 		SendMessageToAdmin(0xE74C3CFF, string); SetVehicleToRespawn(vehicleid);
	 		return 0;
     	}
	}
 	GetVehiclePos(vehicleid, Pozicija_Vozila[vehicleid][0], Pozicija_Vozila[vehicleid][1], Pozicija_Vozila[vehicleid][2]);

    if(floatround(floatsqroot(vel_x * vel_x + vel_y * vel_y) * 200, floatround_round) > 1100)
	{
		OnPlayerTroll(playerid);
		return 0;
	}
	if(GetVehicleDistanceFromPoint(vehicleid, new_x, new_y, new_z) > 15)
	{
		return 0;
	}
	#if defined JT_OnUnoccupiedVehicleUpdate
	return JT_OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z);
	#else
	return 1;
	#endif
}

#if defined _ALS_OnPlayerStateChange
#undef OnPlayerStateChange
#else
#define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange     JT_OnPlayerStateChange

#if defined JT_OnPlayerStateChange
forward JT_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

#if defined _ALS_OnPlayerEnterVehicle
#undef OnPlayerEnterVehicle
#else
#define _ALS_OnPlayerEnterVehicle
#endif
#define OnPlayerEnterVehicle     JT_OnPlayerEnterVehicle

#if defined JT_OnPlayerEnterVehicle
forward JT_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
#endif

#if defined _ALS_OnPlayerUpdate
#undef OnPlayerUpdate
#else
#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate     JT_OnPlayerUpdate

#if defined JT_OnPlayerUpdate
forward JT_OnPlayerUpdate(playerid);
#endif

#if defined _ALS_OnUnoccupiedVehicleUpdate
#undef OnUnoccupiedVehicleUpdate
#else
#define _ALS_OnUnoccupiedVehicleUpdate
#endif
#define OnUnoccupiedVehicleUpdate     JT_OnUnoccupiedVehicleUpdate

#if defined JT_OnUnoccupiedVehicleUpdate
forward JT_OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z);
#endif

#if defined _ALS_OnGameModeInit
#undef OnGameModeInit
#else
#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit     JT_OnGameModeInit

#if defined JT_OnGameModeInit
forward JT_OnGameModeInit();
#endif

#if defined _ALS_OnGameModeExit
#undef OnGameModeExit
#else
#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit     JT_OnGameModeExit

#if defined JT_OnGameModeExit
forward JT_OnGameModeExit();
#endif

#endif

stock JT_PutPlayerInVehicle(playerid, vehicleid, seatid)
{
	SetPVarInt(playerid, "ATV_Tempo", gettime()+3);
	SetPVarInt(playerid, "ATV_Carro", vehicleid);
	SetPVarInt(playerid, "ATV_RJack", 0);
	return PutPlayerInVehicle(playerid, vehicleid, seatid);
}

stock JT_RemovePlayerFromVehicle(playerid)
{
	SetPVarInt(playerid, "ATV_Tempo", gettime()+3);
	SetPVarInt(playerid, "ATV_Carro", 0);
	SetPVarInt(playerid, "ATV_RJack", 0);
	return RemovePlayerFromVehicle(playerid);
}

#if defined _ALS_RemovePlayerFromVehicle
#undef RemovePlayerFromVehicle
#else
#define _ALS_RemovePlayerFromVehicle
#endif

#define RemovePlayerFromVehicle JT_RemovePlayerFromVehicle

#if defined _ALS_PutPlayerInVehicle
#undef PutPlayerInVehicle
#else 
#define _ALS_PutPlayerInVehicle
#endif

#define PutPlayerInVehicle JT_PutPlayerInVehicle
