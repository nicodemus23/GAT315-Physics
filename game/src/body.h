#pragma once
#include "raylib.h"

typedef struct Body
{
	// force -> velocity -> position
	Vector2 position; 
	Vector2 velocity;
	Vector2 force;
	Vector2 acceleration;
	float mass;

} Body;