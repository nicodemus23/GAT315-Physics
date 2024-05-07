#pragma once //<- include guard // says to only include this file once
//#include "body.h"
#include "raylib.h"

// forward declarations
typedef struct ncBody ncBody; // Declare ncBody struct as a forward declaration 

// external variables
extern ncBody* ncBodies; //<- points to first body in linked list // Declare bodies as a pointer to a Body struct // says it's externally defined
extern int ncBodyCount; //<- count of bodies in the world // Declare bodyCount as an integer // says it's externally defined
extern Vector2 ncGravity; //<- gravity force to be applied to all bodies // Declare gravity as a Vector2 // says it's externally defined
extern float LIFESPAN; //<- lifespan of a body // Declare LIFESPAN as a float // says it's externally defined
extern float MAX_BODIES; //<- maximum number of bodies allowed // Declare MAX_BODIES as a float // says it's externally defined

// function prototypes
ncBody* CreateBody(); // creates a new body and adds it ot the linked list // Declare CreateBody function that returns a Body pointer
void UpdateBody(ncBody* body, float dt); // updates body properties // Declare UpdateBody function that takes a Body pointer and a float as arguments
void DestroyBody(ncBody* body); // removes body from the linked list and frees its memory // Declare DestroyBody function that takes a Body pointer as an argument
void DestroyAllBodies(); // removes all bodies from the linked list and frees their memory // Declare DestroyAllBodies function