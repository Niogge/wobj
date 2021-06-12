#pragma once
#include "wobj.h"

static float wobj_float3_get(unsigned int i, wobj_float3* f3)
{
	if (i < 3)
	{
		switch (i)
		{
		case 0:
			return f3->x;
		case 1:
			return f3->y;
		case 2:
			return f3->z;
		default:
			return 0.0f;
		}
	}
	return 0.0f;
}
static float wobj_float3_set(unsigned int i, wobj_float3* f3, float value)
{
	if (i < 3)
	{
		switch (i)
		{
		case 0:
			f3->x = value;
			return f3->x;
		case 1:
			f3->y = value;
			return f3->y;
		case 2:
			f3->z = value;
			return f3->z;
		default:
			return 0.0f;
		}
	}
	return 0.0f;
}