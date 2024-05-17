#include "body.h"
#include "integrator.h"
#include "world.h"

void Step(Body* body, float timestep) {
	body->Force = Vector2Add(body->Force, Vector2Scale(Vector2Scale(jgGravity, body->gravityScale), body->mass));
	body->Acceleration = Vector2Scale(body->Force, body->inverseMass);
	SemiImplicitEuler(body, timestep);
	//apply damping
	float damping = 1 / (1 + (body->damping * timestep));
	body->Velocity = Vector2Scale(body->Velocity, damping);

	ClearForce(body);
}