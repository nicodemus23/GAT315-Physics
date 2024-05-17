#pragma once
#include "Body.h"

extern Body* jgBodies;
extern int jgBodyCount;
extern Vector2 jgGravity;

Body* CreateBody(Vector2 position, float mass, bodyType bt);
void AddBody(Body* body);
void DestroyBody(Body* body);