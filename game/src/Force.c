#include "force.h"
#include "body.h"
void ApplyGravitation(Body* bodies, float strength)
{
	for (Body* body1 = bodies; body1; body1 = body1->next)
	{
		for (Body* body2 = bodies; body2; body2 = body2->next)
		{
			//<if bodies are the same, continue>
			if (body1 == body2) continue;

			//Vector2 direction = < calculate direction(body position - body position)
			Vector2 direction = Vector2Subtract(body1->Position, body2->Position);
			//float distance = <get length of vector>
			float distance = Vector2Length(direction);

			//distance = <clamp distance to a minumum of 1>
			distance = (distance < 1) ? 1 : distance;
			//float force = <(mass1 * mass2 / (distance * distance)) * strength>;
			float force = (body1->mass * body2->mass / (distance * distance)) * strength;

			//direction = <scale normalized direction vector by force>;
			direction = Vector2Scale(Vector2Normalize(direction), force);

			//ApplyForce(<apply force to body 1, one of them is negative>);
			ApplyForce(body1, direction, FM_FORCE);
			//ApplyForce(<apply force to body 2, one of them is negative>);
			ApplyForce(body2, Vector2Invert(direction), FM_FORCE);
		}
	}
}