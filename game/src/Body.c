#pragma once
#include "body.h"
#include "world.h"
#include "integrator.h"

void Step(ncBody* body, float timestep) //<- updates body's position and velocity based on its force and a given timestep
{
	/*printf("Before Step:\n");
	printf("Position: (%.2f, %.2f)\n", body->position.x, body->position.y);
	printf("Velocity: (%.2f, %.2f)\n", body->velocity.x, body->velocity.y);
	printf("Force: (%.2f, %.2f)\n", body->force.x, body->force.y);
	printf("Mass: %.2f, Inverse Mass: %.2f\n", body->mass, body->inverseMass);
	printf("Gravity Scale: %.2f\n", body->gravityScale);
	printf("Damping: %.2f\n", body->damping);
	printf("Timestep: %.2f\n", timestep);*/

	body->force = Vector2Add(body->force, Vector2Scale(Vector2Scale(ncGravity, body->gravityScale), body->mass)); // F = m * g
	//printf("Updated Force: (%.2f, %.2f)\n", body->force.x, body->force.y);

	body->acceleration = Vector2Scale(body->force, body->inverseMass); // a = F / m
	//printf("Updated Acceleration: (%.2f, %.2f)\n", body->acceleration.x, body->acceleration.y);

	SemiImplicitEuler(body, timestep);
	/*printf("After SemiImplicitEuler:\n");
	printf("Position: (%.2f, %.2f)\n", body->position.x, body->position.y);
	printf("Velocity: (%.2f, %.2f)\n", body->velocity.x, body->velocity.y);*/

	// apply damping
	float damping = 1 / (1 + (body->damping * timestep));
	body->velocity = Vector2Scale(body->velocity, damping); // velocity = velocity * 1.0f / (1.0f + dt + damping)	
	//printf("Damped Velocity: (%.2f, %.2f)\n", body->velocity.x, body->velocity.y);


	ClearForce(body);
}