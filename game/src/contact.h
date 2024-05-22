#pragma once
#include "raylib.h"

typedef struct ncContact
{
	struct ncBody* body1;
	struct ncBody* body2;

	float restitution;
	float depth;
	Vector2 normal;

	struct ncContact* next;
} ncContact_t;

void AddContact(ncContact_t* contact, ncContact_t** contacts);
void DestroyAllContacts(ncContact_t** contacts);