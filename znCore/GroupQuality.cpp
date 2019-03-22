#include "stdafx.h"

// General
#include "GroupQuality.h"

CGroupQuality::CGroupQuality()
{

}

CGroupQuality::~CGroupQuality()
{

}

void CGroupQuality::InitDefault()
{
	// Distances
	ADT_MCNK_Distance = C_ADT_MCNK_Distance;
	ADT_MCNK_HighRes_Distance = C_ADT_MCNK_HighRes_Distance;
	ADT_MDX_Distance = C_ADT_MDX_Distance;
	ADT_WMO_Distance = C_ADT_WMO_Distance;
	WMO_MODD_Distance = C_WMO_MODD_Distance;


	// Drawing
	draw_map_mccv = true;
	WMO_MOCV = false;
	WMO_AmbColor = false;

	// Disables
	draw_mcnk = true;
	draw_mcnk_low = false;
	draw_map_wmo = true;
	draw_wmo_doodads = true;
	draw_map_m2 = true;
	draw_water = true;
	draw_wmo_water = true;

	drawfog = false;
	timeEnable = false;
}

void CGroupQuality::UpdateByFog(float _fogDist)
{
	ADT_MCNK_Distance = (_fogDist < C_ADT_MCNK_Distance) ? _fogDist : /*C_ADT_MCNK_Distance*/ _fogDist;
	ADT_MCNK_HighRes_Distance = (_fogDist < C_ADT_MCNK_HighRes_Distance) ? _fogDist : C_ADT_MCNK_HighRes_Distance;
	ADT_MDX_Distance = (_fogDist < C_ADT_MDX_Distance) ? _fogDist : C_ADT_MDX_Distance;
	ADT_WMO_Distance = (_fogDist < C_ADT_WMO_Distance) ? _fogDist : C_ADT_WMO_Distance;
	WMO_MODD_Distance = (_fogDist < C_WMO_MODD_Distance) ? _fogDist : C_WMO_MODD_Distance;

	if (ADT_MDX_Distance > ADT_MCNK_Distance)
	{
		ADT_MDX_Distance = ADT_MCNK_Distance;
	}

	if (ADT_WMO_Distance > ADT_MCNK_Distance)
	{
		ADT_WMO_Distance = ADT_MCNK_Distance;
	}
}
