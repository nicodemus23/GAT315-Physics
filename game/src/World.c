#include "world.h"
#include "body.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// DEFINES EXTERNAL VARIABLES ncBodies and ncBodyCount declared in world.h
ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity = { 0, 9.8f }; //<- gravity vector
float LIFESPAN = 1.3f;
//float MAX_BODIES = 100000;

ncBody* CreateBody() // allocates memory for a new body, initializes its properties and adds it to the linked list
{
   
    ncBody* body = (ncBody*)malloc(sizeof(ncBody));
    assert(body != NULL);

	memset(body, 0, sizeof(ncBody)); //<- sets all bytes of body to 0

    body->prev = NULL;
    body->next = ncBodies;

    if (ncBodies != NULL)
        ncBodies->prev = body;

    ncBodies = body;
    ncBodyCount++;

    // initialize lifespan
	body->lifespan = LIFESPAN;
	body->alpha = 1.0f; // start off at full opacity

    return body;
}

void UpdateBody(ncBody* body, float dt)
{
    if (body->lifespan > 0)
    {
        body->lifespan -= dt;
        body->alpha = body->lifespan / LIFESPAN; // Ratio of remaining lifespan to max lifespan
        body->lifespan = fmaxf(body->lifespan, 0.0f); // Clamp lifespan to 0 or higher
    }
    else
    {
		body->lifespan = 0.0f;
		body->alpha = 0.0f;
    }

    // shrink over time
    float shrinkRate = 0.95f;
	body->mass *= shrinkRate;

    printf("Body at (%.2f, %.2f) updated: lifespan = %.2f, alpha = %.2f\n", body->position.x, body->position.y, body->lifespan, body->alpha);

	Step(body, dt); //<- updates body's position and velocity based on its force and a given timestep
}


void DestroyBody(ncBody* body) // removes body from the linked list and frees its memory
{
    assert(body != NULL);

    printf("Destroying body at (%.2f, %.2f)\n", body->position.x, body->position.y);


    if (body->prev != NULL)
        body->prev->next = body->next;

    if (body->next != NULL)
        body->next->prev = body->prev;

    if (body == ncBodies)
        ncBodies = body->next;

    free(body);
    ncBodyCount--;
}

void DestroyAllBodies() // removes all bodies from the linked list and frees their memory
{
	ncBody* body = ncBodies;
	while (body != NULL)
	{
		ncBody* next = body->next; //<- store next body before freeing current body
		free(body); //<- free current body
		body = next; //<- move to next body
	}

	ncBodies = NULL;
	ncBodyCount = 0;
}