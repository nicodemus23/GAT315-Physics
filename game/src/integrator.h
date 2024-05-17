#pragma once
#include "Body.h"
#include "raymath.h"

inline void ExplicitEuler(Body* body, float timestep) {
	body->Position = Vector2Add(body->Position, Vector2Scale(body->Velocity, timestep));
	body->Velocity = Vector2Add(body->Velocity, Vector2Scale(body->Acceleration, timestep));
}

inline void SemiImplicitEuler(Body* body, float timestep) {
	body->Velocity = Vector2Add(body->Velocity, Vector2Scale(body->Acceleration, timestep));
	body->Position = Vector2Add(body->Position, Vector2Scale(body->Velocity, timestep));
}