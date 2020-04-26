/*
	---------------------------------------
	IMPORTANT

		Custom 3D Text label

		@author:  Zile
		@version: 1.0.0
		@release: 28/01/2019

	---------------------------------------
 	CHANGELOG
			28.1.2019:
			    * Initial release 
			28.1.2019 
				* Fixed bugs with rotation
			28.1.2019
				* Adding types of object effect


	---------------------------------------		

*/
#if defined _INC_CUSTOM_3D_LABEL_FIX
	#endinput
#endif
#define _INC_CUSTOM_3D_LABEL_FIX // For other includes

#if !defined _streamer_included
	#tryinclude <streamer>
	#if !defined _streamer_included
		#error The Streamer is required for custom-label.inc, github.com/samp-incognito/samp-streamer-plugin
	#endif
#endif

#define MAX_CUSTOM3DLABELS			0x99999
#define INVALID_CUSTOM3DLABEL 		0xFFFF
#define MAX_CUSTOM3DLABEL_NAME		16
#define OBJECT_CUSTOM3DLABEL_B 		19477 // BILBORD 
#define OBJECT_CUSTOM3DLABEL_C 		1317  // CYLINDAR
#define CUSTOM3DLABEL_STREAM_DIS	5

const TYPE_CUSTOMLABEL_BIL = 0,
	  TYPE_CUSTOMLABEL_CYL = 1;

enum E_CUSTOM_LABELS
{
	cl_name[MAX_CUSTOM3DLABEL_NAME],
	bool:cl_exists = false,
	cl_object_1,
	cl_object_2,
}
new cl_Data[MAX_CUSTOM3DLABELS][E_CUSTOM_LABELS];

stock cl_FindFreeID() 
{
	new cl_id = -1;

	for(new i = 0; i < MAX_CUSTOM3DLABELS; i++)
	{
		if(!cl_Data[i][cl_exists])
		{
			cl_id = i;
			break;
		}
	}
	return (cl_id != -1) ? cl_id : -1;
}


stock cl_IsValid(cl_id)
	return (cl_Data[_:cl_id][cl_exists]);

stock CreateCustom3DLabel(text[], type = TYPE_CUSTOMLABEL_BIL, Float:x, Float:y, Float:z, distance = CUSTOM3DLABEL_STREAM_DIS)
{
	new cl_id = cl_FindFreeID();
	if(cl_id == -1)
		return print("(error) CustomLabels: I can't create labels!");
	if(strlen(text) > MAX_CUSTOM3DLABEL_NAME)
		return printf("(error) CustomLabels: I can't create label cl_id %i, string is too long! (THE MAX IS 16 CHAR)", cl_id);

	cl_Data[cl_id][cl_exists] = true;

	switch(type)
	{
		case 0:
		{
			cl_Data[cl_id][cl_object_1] = CreateDynamicObject(OBJECT_CUSTOM3DLABEL_B,  0,  0, 0, 0.000, 0.000, 0.000, -1, -1, -1, distance, distance);
			SetDynamicObjectMaterialText(cl_Data[cl_id][cl_object_1], 0, text, 90, "Segoe Keycaps", 32, 1, -10040320, 0, 1);
			cl_Data[cl_id][cl_object_2] = CreateDynamicObject(1276,  x,  y,  z - 2.5, 0.000, 0.000, 0.000, -1, -1, -1, distance, distance);
			AttachDynamicObjectToObject(cl_Data[cl_id][cl_object_1], cl_Data[cl_id][cl_object_2], 0, 0, 2, 0.000, 0.000, 90.000);

		}
		case 1:
		{
			cl_Data[cl_id][cl_object_1] = CreateDynamicObject(OBJECT_CUSTOM3DLABEL_C,  0,  0, 0, 0.000, 0.000, 0.000, -1, -1, -1, distance, distance);
			SetDynamicObjectMaterialText(cl_Data[cl_id][cl_object_1], 0, text, 90, "Segoe Keycaps", 32, 1, -10040320, 0, 1);
			cl_Data[cl_id][cl_object_2] = CreateDynamicObject(1276,  x,  y,  z - 2.5, 0.000, 0.000, 0.000, -1, -1, -1, distance, distance);
			AttachDynamicObjectToObject(cl_Data[cl_id][cl_object_1], cl_Data[cl_id][cl_object_2], 0, 0, 2, 0.000, 0.000, 90.000);
		}
	}
//	print("The label created!");
	return true;
}

stock DestroyCustom3DLabel(cl_id)
{
	if (cl_IsValid(cl_id))
		return (false);
	cl_Data[_:cl_id][cl_exists] = false;
	DestroyDynamicObject(cl_Data[cl_id][cl_object_1]);
	DestroyDynamicObject(cl_Data[cl_id][cl_object_2]);	
	return true;
}


stock DestroyAllCustom3DLabels()
{
	for(new i = 0; i < MAX_CUSTOM3DLABELS; i++)
	{
		if (cl_IsValid(i))
		{
			cl_Data[_:i][cl_exists] = false;
			DestroyDynamicObject(cl_Data[i][cl_object_1]);
			DestroyDynamicObject(cl_Data[i][cl_object_2]);	
		}
	}	
}