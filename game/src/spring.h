#pragma once

typedef struct ncSpring
{
	struct ncBody* body1;
	struct ncBody* body2;
	float restLength;
	float k; // stiffness

	struct ncSpring* next;
	struct ncSpring* prev;


} ncSpring_t; // _t mean s it's a data type

extern ncSpring_t* ncSprings;

// function prototypes
ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k); // creates a new body and adds it ot the linked list // Declare CreateBody function that returns a Body pointer
void AddSpring(ncSpring_t* spring); // adds a body to the linked list // Declare AddBody function that takes a Body pointer as an argument
void DestroySpring(ncSpring_t* spring); // removes body from the linked list and frees its memory // Declare DestroyBody function that takes a Body pointer as an argument
void DestroyAllSprings(); // removes all bodies from the linked list and frees their memory // Declare DestroyAllBodies function

void ApplySpringForce(ncSpring_t* springs);