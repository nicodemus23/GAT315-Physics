#pragma once
#include "body.h"

extern Body* bodies; // Declare bodies as a pointer to a Body struct // says it's externally defined
extern int bodyCount; // Declare bodyCount as an integer // says it's externally defined

Body* CreateBody();
void DestroyBody(Body* body); // Declare DestroyBody function that takes a Body pointer as an argument