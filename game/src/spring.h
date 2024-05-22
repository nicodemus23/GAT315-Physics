#pragma once
#include "body.h"

struct ncBody;
typedef struct ncSpring_t {
	struct ncBody* body1;
	struct ncBody* body2;
	float restLength;
	float k; //stiffness

	struct ncSpring_t* next;
	struct ncSpring_t* prev;

}ncSpring_t;

extern ncSpring_t* ncSprings;


ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float rest, float k);
void AddSpring(ncSpring_t* spring);
void DestroySpring(ncSpring_t* spring);
void DestroyAllSprings(); // removes all bodies from the linked list and frees their memory // Declare DestroyAllBodies function


void ApplySpringForce(ncSpring_t* springs);
void ApplySpringForcePosition(Vector2 position, ncBody* body, float restLength, float k, float damping); // Apply a spring force to a body at a specific position
