#pragma once
#include "raylib.h"
#include <raymath.h>

typedef enum {
	BT_STATIC,
	BT_KINEMATIC,
	BT_DYNAMIC
} bodyType;

typedef enum {
	FM_FORCE,
	FM_IMPULSE,
	FM_VELOCITY
} jgForceMode;

typedef struct Body
{
	bodyType type;
	Vector2 Position;
	Vector2 Velocity;
	Vector2 Acceleration;
	Vector2 Force;

	float mass;
	float inverseMass;
	float gravityScale;
	float damping;
	float restitution;

	Color color;

	struct Body* next;
	struct Body* prev;
} Body;

inline void ApplyForce(Body* body, Vector2 force, jgForceMode fm) {
	if (body->type != BT_DYNAMIC) return;

	switch (fm) {
	case FM_FORCE:
		body->Force = Vector2Add(body->Force, force);
		break;
	case FM_IMPULSE:
		//applies a sudden change in momentum
		body->Velocity = Vector2Add(Vector2Scale(force, body->inverseMass), body->Velocity);
		break;
	case FM_VELOCITY:
		body->Velocity = force;
		break;
	}

}

inline void ClearForce(Body* body) {
	body->Force = Vector2Zero();
}

void Step(Body* body, float timestep);