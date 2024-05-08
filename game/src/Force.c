#include "force.h"
#include "world.h"
#include "math.h"
#include "body.h"

void ApplyGravitation(ncBody* bodies, float strength)
{
	for (ncBody* body1 = bodies; body1; body1 = body1->next) // <-iterate through all bodies
	{
		for (ncBody* body2 = bodies; body2; body2 = body2->next) // 
		{
			if (body1 == body2) continue; // <- skip if the two bodies are the same

			Vector2 direction = Vector2Subtract(body2->position, body1->position); // <- calculate the direction vector between the two bodies
			float distance = Vector2Length(direction); // <- calculate the distance between the two bodies

			distance = fmaxf(distance, 1.0f); // <- clamp the distance to 1.0f or higher to avoid division by zero
			// Newton's law of universal gravitation
			float force = body1->mass * body2->mass / (distance * distance) * strength; // <- calculate the force of attraction between the two bodies // F = G * (m1 * m2) / r^2

			// scale the direction vector by the force magnitude
			direction = Vector2Scale(direction, force); // <- scale the direction vector by the force magnitude
			direction = Vector2Normalize(direction); // <- scale the direction vector by the force magnitude to get the unit vector
		
			// apply the force to the bodies
			ApplyForce(body1, Vector2Negate(direction), FM_FORCE); // <- apply the force to the first body in the opposite direction of the unit vector (repulsion) 
			ApplyForce(body2, direction, FM_FORCE); // <- apply the force to the second body in the direction of the unit vector (attraction)
		}
	}
}
