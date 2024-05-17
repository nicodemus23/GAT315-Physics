#pragma once
#include "body_2.h"

extern ncBody* ncBodies;
extern int ncBodyCount;
extern Vector2 ncGravity;

ncBody* CreateBody(Vector2 position, float mass, ncBodyType type);
void AddBody(ncBody* body);
void DestroyBody(ncBody* body);