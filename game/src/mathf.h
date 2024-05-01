#pragma once
#include "raylib.h"
#include <stdlib.h>

inline float GetRandomFloatValue01()
{
	return rand() / (float)RAND_MAX; // Return a random float value between 0 and 1 or you'll get zero all the time
}

inline float GetRandomFloatValue(float min, float max)
{
	return min + (max - min) * GetRandomFloatValue01(); // Return a random float value between min and max
}

inline Vector2 CreateVector2(float x, float y) 
{
	return (Vector2){ x, y }; // Return a vector with x and y values 
}