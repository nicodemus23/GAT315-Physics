#pragma once
#include "raylib.h"

typedef struct ncContact ncContact_t;
typedef struct ncBody ncBody;

void CreateContacts(ncBody* bodies, ncContact_t** contacts);
ncContact_t* GenerateContact(ncBody* body1, ncBody* body2);

void SeparateContacts(ncContact_t* contacts);
void ResolveContacts(ncContact_t* contacts);