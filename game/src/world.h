#pragma once
#include "Body.h"

extern ncBody* ncBodies;
extern int jgBodyCount;
extern Vector2 jgGravity;

ncBody* CreateBody(Vector2 position, float mass, bodyType bt);
void AddBody(ncBody* body);
void DestroyBody(ncBody* body);