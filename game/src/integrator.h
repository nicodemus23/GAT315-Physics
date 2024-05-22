#pragma once
#include "Body.h"
#include "raymath.h"

inline void ExplicitEuler(ncBody* body, float timestep) {
	body->position = Vector2Add(body->position, Vector2Scale(body->Velocity, timestep));
	body->Velocity = Vector2Add(body->Velocity, Vector2Scale(body->Acceleration, timestep));
}

inline void SemiImplicitEuler(ncBody* body, float timestep) {
	body->Velocity = Vector2Add(body->Velocity, Vector2Scale(body->Acceleration, timestep));
	body->position = Vector2Add(body->position, Vector2Scale(body->Velocity, timestep));
}