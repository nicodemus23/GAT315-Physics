#include "world.h"
#include "body.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
//#include "GUI_var.h"


// DEFINES EXTERNAL VARIABLES ncBodies and ncBodyCount declared in world.h
ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity = { 0, 9.8f }; //<- gravity vector
float LIFESPAN = 3.0f;
//float MAX_BODIES = 100000;
//float guiBodyMass = 1.0f;
//float guiBodyGravityScale = 1.0f;
//float guiBodyDamping = 0.2f;
//float guiBodyLifespan = 0.0f;
//float guiBodyAlpha = 1.0f;
//float guiBodyOuterRadius = 5.0f;
//float guiBodyInnerRadius = 4.5f;
//int guiBodyNumPoints = 5;
Color guiBodyColor;

ncBody* CreateBody(Vector2 position, float outerRadius, float innerRadius, int numPoints, Color color) // (!) was just () // allocates memory for a new body, initializes its properties and adds it to the linked list
{
   
	ncBody* body = (ncBody*)malloc(sizeof(ncBody)); //<- allocate memory for a new body
    assert(body != NULL);

	memset(body, 0, sizeof(ncBody)); //<- sets all bytes of body to 0

    body->type = BT_DYNAMIC;
	body->position = position; // (!) was Vector2Zero()
    body->velocity = Vector2Zero();
    body->force = Vector2Zero();
    body->acceleration = Vector2Zero();
    body->randomTwinkleOffset = GetRandomFloatValue(0, 2 * PI);
    body->color = guiBodyColor;

    body->outerRadius = outerRadius;
	body->innerRadius = innerRadius;
	body->numPoints = numPoints;


    body->prev = NULL;
	body->next = NULL; // was ncBodies; (!)
    //body->next = ncBodies; (!)

    if (ncBodies != NULL)
        ncBodies->prev = body;

    ncBodies = body;
    ncBodyCount++;

    // initialize lifespan
	body->lifespan = LIFESPAN;
	body->alpha = 1.0f; // start off at full opacity

   /* printf("Created body at (%.2f, %.2f)\n", body->position.x, body->position.y);
    printf("Mass: %.2f, Inverse Mass: %.2f\n", body->mass, body->inverseMass);
    printf("Velocity: (%.2f, %.2f)\n", body->velocity.x, body->velocity.y);
    printf("Force: (%.2f, %.2f)\n", body->force.x, body->force.y);
    printf("Acceleration: (%.2f, %.2f)\n", body->acceleration.x, body->acceleration.y);*/

    return body;
}

AddBody(ncBody* body)
{
	assert(body != NULL);

    // add element to linked list
	body->prev = NULL;
	body->next = ncBodies;

	if (ncBodies != NULL) ncBodies->prev = body;

	ncBodies = body;
	ncBodyCount++;
}

void UpdateBody(ncBody* body, float dt)
{
    if (body->lifespan > 0)
    {
        body->lifespan -= dt;
		body->alpha = fmaxf(body->lifespan / LIFESPAN, 0.0f); //<- set alpha to 0 if lifespan is less than 0
		body->lifespan = fmaxf(body->lifespan, 0.0f); //<- set lifespan to 0 if it is less than 0
    }
    else
    {
        body->lifespan = 3.0f; 
        body->alpha = 1.0f;
    }

    // Shrink over time
    float shrinkRate = 0.95f;
    body->mass *= shrinkRate;

    if (isnan(body->position.x) || isnan(body->position.y))
    {
		//printf("Body at (%.2f, %.2f) has NaN position\n", body->position.x, body->position.y);
		//DestroyBody(body);
		return;
	}

    //printf("Body at (%.2f, %.2f) updated: lifespan = %.2f, alpha = %.2f\n", body->position.x, body->position.y, body->lifespan, body->alpha);
    
   /* printf("Before update:\n");
    printf("Position: (%.2f, %.2f)\n", body->position.x, body->position.y);
    printf("Velocity: (%.2f, %.2f)\n", body->velocity.x, body->velocity.y);
    printf("Force: (%.2f, %.2f)\n", body->force.x, body->force.y);
    printf("Acceleration: (%.2f, %.2f)\n", body->acceleration.x, body->acceleration.y);*/

    Step(body, dt);

   /* printf("After update:\n");
    printf("Position: (%.2f, %.2f)\n", body->position.x, body->position.y);
    printf("Velocity: (%.2f, %.2f)\n", body->velocity.x, body->velocity.y);
    printf("Force: (%.2f, %.2f)\n", body->force.x, body->force.y);
    printf("Acceleration: (%.2f, %.2f)\n", body->acceleration.x, body->acceleration.y);*/
}


void DestroyBody(ncBody* body) // removes body from the linked list and frees its memory
{
    assert(body != NULL);

    //printf("Destroying body at (%.2f, %.2f)\n", body->position.x, body->position.y);


    if (body->prev != NULL)
        body->prev->next = body->next;

    if (body->next != NULL)
        body->next->prev = body->prev;

    if (body == ncBodies)
        ncBodies = body->next;

	body->next = NULL; // <- set next pointer to NULL to avoid dangling pointers
	body->prev = NULL; // <- set prev pointer to NULL to avoid dangling pointers

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