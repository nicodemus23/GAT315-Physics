#pragma once
#include "raylib.h"

typedef struct ncContact ncContact_t;
typedef struct Body Body;

void CreateContacts(Body* bodies, ncContact_t** contacts);
ncContact_t* GenerateContact(Body* body1, Body* body2);

void SeparateContacts(ncContact_t* contacts);
void ResolveContacts(ncContact_t* contacts);