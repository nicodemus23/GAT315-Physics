#pragma once
#include "body.h"


// performs the Explicit Euler integration method to update the position and velocity of a body based on its force and a given timestep.
inline void ExplicitEuler(ncBody* body, float timestep) 
{
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep)); // x(t + dt) = x(t) + v(t) * dt
	body->velocity = Vector2Add(body->velocity, Vector2Scale(body->acceleration, timestep)); // v(t + dt) = v(t) + (F / m) * dt
}

inline void SemiImplicitEuler(ncBody* body, float timestep)
{	// velocity is updated first, then position (ExplicitEuler updates position first, then velocity)
	body->velocity = Vector2Add(body->velocity, Vector2Scale(body->acceleration, timestep)); // v(t + dt) = v(t) + (F / m) * dt
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep)); // x(t + dt) = x(t) + v(t + dt) * dt
}