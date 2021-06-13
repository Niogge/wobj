#pragma once
#include "wobj.h"
#define EPSILON 0.000001f

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
static wobj_float3 wobj_float3_cross(wobj_float3* A, wobj_float3* B){
	wobj_float3 res;
	res.x = A->y * B->z - A->z * B->y;
	res.y= A->z * B->x - A->x* B->z;
	res.z = A->x * B->y - A->y*B->x;
	return res;
}
static float wobj_float3_dot(wobj_float3* A, wobj_float3* B)
{
	return A->x*B->x +A->y*B->y + A->z *B->z;
}
static wobj_float3 wobj_float3_sum(wobj_float3* A, wobj_float3* B)
{
	wobj_float3 res;
	res.x = A->x+B->x;
	res.y = A->y+B->y;
	res.z = A->z+B->z;
	return res;
}
static wobj_float3 wobj_float3_sub(wobj_float3* A, wobj_float3* B)
{
	wobj_float3 res;
	res.x = A->x-B->x;
	res.y = A->y-B->y;
	res.z = A->z-B->z;
	return res;
}