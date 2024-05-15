#include "spring.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
//#include "GUI_var.h"

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k) // creates a new body and adds it ot the linked list // Declare CreateBody function that returns a Body pointer
{

	ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t)); //<- allocate memory for a new body
	assert(spring);

	memset(spring, 0, sizeof(ncSpring_t)); //<- sets all bytes of body to 0
	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;

	spring->prev = NULL;
	spring->next = ncSprings;

	return spring;
}

void AddSpring(ncSpring_t* spring) // adds a body to the linked list
{
	assert(spring); //<- check if spring is not NULL

	// add element to linked list
	spring->prev = NULL;
	spring->next = ncSprings;

	if (ncSprings) ncSprings->prev = spring;

	// set head of elements to new element
	ncSprings = spring;
}

void DestroySpring(ncSpring_t* spring)
{
	assert(spring);

	if (spring->prev) spring->prev->next = spring->next;
	if (spring->next) spring->next->prev = spring->prev;

	if (spring == ncSprings) ncSprings = spring->next;

	free(spring);
}

void DestroyAllSprings() // removes all bodies from the linked list and frees their memory
{
	ncSpring_t* spring = ncSprings;
	while (spring)
	{
		ncSpring_t* next = spring->next;
		DestroySpring(spring);
		spring = next;
	}
}

void ApplySpringForce(ncSpring_t* springs)
{
	for (ncSpring_t* spring = springs; spring; spring = spring->next)
	{
		// get direction vector from body2 to body1
		Vector2 direction = Vector2Subtract(spring->body2->position, spring->body1->position); // <- calculate the direction vector between the two bodies
		if (direction.x == 0 && direction.y == 0) continue; // <- skip if the two bodies are at the same position (division by zero error)

		float length = Vector2Length(direction); // <- calculate the distance between the two bodies
		// compute displacement from current length to resting length
		float x = length - spring->restLength;
		// compute force using product of displacement sand stiffness (f = -kx) k = stiffness constant, x = displacement from resting length 
		float force = -spring->k * x;

		// get direction normal
		Vector2 ndirection = Vector2Normalize(direction); // <- scale the direction vector by the force magnitude to get the unit vector

		ApplyForce(spring->body2, direction, FM_FORCE); // <- apply the force to the second body in the direction of the unit vector (attraction)
		ApplyForce(spring->body1, Vector2Negate(direction), FM_FORCE); // <- apply the force to the first body in the opposite direction of the unit vector (repulsion) 
	}
	
}
