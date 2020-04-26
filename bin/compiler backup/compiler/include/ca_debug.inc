/*
---------------------------------------

	ColAndreas Draw Debug Line
	@author: Ino
	@version: 1.0.0
	@release: 12/01/2017
	@build: 1
	@download: github.com/Ino42O/CADrawDebugLine

---------------------------------------

	Functions list:

		* CA_DebugDrawLine(Float:StartX, Float:StartY, Float:StartZ, Float:EndX, Float:EndY, Float:EndZ, bool:StopAtCollision, Color = 0xFFFF0000);
		* CA_DestroyDebugDrawLine(line);
		* IsValidDebugDrawLine(line)



 	Changelog:

 		20-12-2016:
 			* Initial release



 	Thanks to:
 		ColAndreas Team - For ColAndreas
 		Southclaw - For math stuffs from 'line.inc'
 		Y_Less - Iterators
 		incognito - Streamer

---------------------------------------
*/

#include <a_samp>

/*#if !defined _Y_ITERATE_LOCAL_VERSION
	#tryinclude <YSI\y_iterate>
#endif*/

#if !defined STREAMER_TYPE_OBJECT
	#tryinclude <streamer>

	#if !defined STREAMER_TYPE_OBJECT
		#error The Streamer plugin is required, get it here: github.com/samp-incognito/samp-streamer-plugin
	#endif
#endif

#include <streamer>

#if !defined COLANDREAS && !defined COLANDREAS_VERSION
	#tryinclude <colandreas>

	#if !defined COLANDREAS && !defined COLANDREAS_VERSION
		#error The ColAndreas plugin is required, get it here: github.com/Pottus/ColAndreas
	#endif
#endif


#define CA_DEBUG_LINE // For other includes


#define MAX_RAYCAST_DEBUG 		(50)
#define MAX_OBJECTS_DEBUG		(1000)

enum E_LINEINFO
{
	 bool:cad_used,
		  cad_objCount,
		  cad_color,
	Float:cad_posX,
	Float:cad_posY,
	Float:cad_posZ,
	Float:cad_rotX,
	Float:cad_rotY,
	Float:cad_rotZ,
	Float:cad_dstX,
	Float:cad_dstY,
	Float:cad_dstZ
}
static LineInfo[MAX_RAYCAST_DEBUG][E_LINEINFO];


static LineObject[MAX_RAYCAST_DEBUG][MAX_OBJECTS_DEBUG];

#if defined _Y_ITERATE_LOCAL_VERSION
	static Iterator:LineIter<MAX_RAYCAST_DEBUG>;
#endif


/*----------------------------------------------------------------------------

	Function:
		CA_DebugDrawLine

	Params:
		@Float:StartX
		@Float:StartY
		@Float:StartZ
		@Float:EndX
		@Float:EndY
		@Float:EndZ
		@bool:StopAtCollision
		@Color = ARGB

	Notes:
		Create ColAndreas Debug Draw Line

----------------------------------------------------------------------------*/
stock CA_DebugDrawLine(Float:StartX, Float:StartY, Float:StartZ, Float:EndX, Float:EndY, Float:EndZ, bool:StopAtCollision, Color = 0xFFFF0000)
{
	new id = -1;

	#if defined _Y_ITERATE_LOCAL_VERSION
		id = Iter_Free(LineIter);
	#else
		for (new i = 0; i < MAX_RAYCAST_DEBUG; i++)
		{
			if (!LineInfo[i][cad_used])
			{
				id = i;
				break;
			}
		}
	#endif

	if(id == -1) return -1;

	LineInfo[id][cad_posX] = StartX;
	LineInfo[id][cad_posY] = StartY;
	LineInfo[id][cad_posZ] = StartZ;

	if (!StopAtCollision)
	{
		LineInfo[id][cad_dstX] = EndX;
		LineInfo[id][cad_dstY] = EndY;
		LineInfo[id][cad_dstZ] = EndZ;
	}
	else
	{
		if(!CA_RayCastLine(StartX, StartY, StartZ, EndX, EndY, EndZ, LineInfo[id][cad_dstX], LineInfo[id][cad_dstY], LineInfo[id][cad_dstZ]))
		{
			LineInfo[id][cad_dstX] = EndX;
			LineInfo[id][cad_dstY] = EndY;
			LineInfo[id][cad_dstZ] = EndZ;
		}
	}

	LineInfo[id][cad_color] = Color;

	LineInfo[id][cad_used] = true;

	#if defined _Y_ITERATE_LOCAL_VERSION
		Iter_Add(LineIter, id);
	#endif

	// Generate and show line from array
	ShowLine(id);

	return id;
}


/*----------------------------------------------------------------------------

	Function:
		CA_DestroyDebugDrawLine

	Params:
		line

	Notes:
		Destroy ColAndreas Debug Draw Line

----------------------------------------------------------------------------*/
stock CA_DestroyDebugDrawLine(line)
{
	if (!IsValidDebugDrawLine(line)) return 0;

	for (new i = 0; i < LineInfo[line][cad_objCount]; i++)
	{
		if (IsValidDynamicObject(LineObject[line][i]))
		{
			DestroyDynamicObject(LineObject[line][i]);
			LineObject[line][i] = INVALID_OBJECT_ID;
		}
	}

	LineInfo[line][cad_used] = false;

	#if defined _Y_ITERATE_LOCAL_VERSION
		Iter_Remove(LineIter, line);
	#endif

	return 1;
}

/*----------------------------------------------------------------------------

	Function:
		IsValidDebugDrawLine

	Params:
		line

	Notes:
		Check if line exist

----------------------------------------------------------------------------*/
stock IsValidDebugDrawLine(line)
{
	#if defined _Y_ITERATE_LOCAL_VERSION
		if (!Iter_Contains(LineIter, line)) return 0;
	#else
		if (!LineInfo[line][cad_used]) return 0;
	#endif

	return 1;
}



/*--------------------------------------------------------------------------*/
static stock ShowLine(line)
{
	new
		Float:vecX = LineInfo[line][cad_dstX] - LineInfo[line][cad_posX],
		Float:vecY = LineInfo[line][cad_dstY] - LineInfo[line][cad_posY],
		Float:vecZ = LineInfo[line][cad_dstZ] - LineInfo[line][cad_posZ],
		Float:rotX,
		Float:rotY,
		Float:rotZ,
		Float:temp,
		Float:distance,
			  count
	;

	rotZ = -(atan2(vecY, vecX)-90.0);
	rotX = -(floatabs(atan2(floatsqroot(floatpower(vecX, 2.0) + floatpower(vecY, 2.0)), vecZ))-90.0);

	distance = floatsqroot( (vecX * vecX) + (vecY * vecY) + (vecZ * vecZ) );
	count = floatround(distance / 2.46193, floatround_ceil) + 1;

	for (new i = 0, c = count; i < c; i++)
	{
		if(i == 0) temp = (2.46193 / 2.0) + (-1.230965);
		else if(i == count - 1)	temp = (distance - (2.46193 / 2.0)) + (-1.230965);
		else temp = (2.46193 * (i - 1)) + (2.46193 / 2.0) + (-1.230965);

		if(temp > distance) break;

		if(!IsValidDynamicObject(LineObject[line][i]))
		{
			LineObject[line][i] = CreateDynamicObject(19087,
				LineInfo[line][cad_posX] + ( temp * floatsin(rotZ, degrees) * floatcos(rotX, degrees) ),
				LineInfo[line][cad_posY] + ( temp * floatcos(rotZ, degrees) * floatcos(rotX, degrees) ),
				LineInfo[line][cad_posZ] + ( temp * floatsin(rotX, degrees) ),
				rotX + 90.0, rotY, -rotZ
			);

			SetDynamicObjectMaterial(LineObject[line][i], 0, 14590, "mafcastopfoor", "cop_ceiling1", LineInfo[line][cad_color]);
		}
		else
		{
			SetDynamicObjectPos(LineObject[line][i],
				LineInfo[line][cad_posX] + ( temp * floatsin(rotZ, degrees) * floatcos(rotX, degrees) ),
				LineInfo[line][cad_posY] + ( temp * floatcos(rotZ, degrees) * floatcos(rotX, degrees) ),
				LineInfo[line][cad_posZ] + ( temp * floatsin(rotX, degrees) )
			);

			SetDynamicObjectRot(LineObject[line][i],
				rotX + LineInfo[line][cad_rotX],
				rotY + LineInfo[line][cad_rotY],
				-rotZ + LineInfo[line][cad_rotZ]
			);
		}

		if(temp > 0xFFFF) break;
	}

	if(count < LineInfo[line][cad_objCount])
	{
		for (new i = count; i < MAX_OBJECTS_DEBUG; i++)
		{
		    if(IsValidDynamicObject(LineObject[line][i]))
		    {
		    	DestroyDynamicObject(LineObject[line][i]);
		    	LineObject[line][i] = INVALID_OBJECT_ID;
		    }
		}
	}

	LineInfo[line][cad_objCount] = count;

	return 1;
}
