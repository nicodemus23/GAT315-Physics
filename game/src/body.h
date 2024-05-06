#pragma once
#include "raylib.h"
#include "raymath.h"


// body types
typedef enum
{   //<- enum is a user-defined data type that consists of integral constants
	BT_STATIC, // Static bodies are not affected by forces
	BT_KINEMATIC, // Kinematic bodies are not affected by forces but can be moved manually
	BT_DYNAMIC // Dynamic bodies are affected by forces
} ncBodyType;

typedef enum
{
	FM_FORCE,
	FM_IMPULSE,
	FM_VELOCITY
} ncForceMode; // unscoped enum (enum without a name)

typedef struct ncBody
{
	ncBodyType type;
	// acceleration -> velocity -> position (a updates v, v updates p)
	Vector2 position; 
	Vector2 velocity;
	Vector2 force;
	Vector2 acceleration;

	float mass;
	float inverseMass; // -> 1 / mass (static = 0)
	float gravityScale; /// typically 1 but can be adjusted here
	float damping; // damping factor (0 = no damping, 1 = total damping)
	float lifespan; // how long the body will exist (0 = infinite)
	float tracerlifespan; // how long the tracer will exist (0 = infinite)
	float alpha; // transparency of the body

	struct ncBody* next;
	struct ncBody* prev;
	Color color; // color of the body

} ncBody;

// adds force to a body
inline void ApplyForce(ncBody* body, Vector2 force, ncForceMode forceMode)
{
	if (body->type != BT_DYNAMIC) return;

	switch (forceMode)
	{
	case FM_FORCE:
		body->force = Vector2Add(body->force, force); //<- add force to body
		break;
	case FM_IMPULSE:
		// applies a suddden change in momentum 
		body->velocity = Vector2Scale(force, body->inverseMass); //<- impulse = force / mass (velocity = impulse / mass
		break;
	case FM_VELOCITY:
		body->velocity = force;
		break;
	}
};

// sets force of a body to zero
inline void ClearForce(ncBody* body)
{
	body->force = Vector2Zero(); //<- set force to zero
};

void Step(ncBody* body, float timestep); //<- updates body's position and velocity based on its force and a given timestep


