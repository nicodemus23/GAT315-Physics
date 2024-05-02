#include "world.h"
#include "body.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// DEFINES EXTERNAL VARIABLES ncBodies and ncBodyCount declared in world.h
ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity = { 0, 9.8f };

ncBody* CreateBody() // allocates memory for a new body, initializes its properties and adds it to the linked list
{
    ncBody* body = (ncBody*)malloc(sizeof(ncBody));
    assert(body != NULL);

    memset(body, 0, sizeof(ncBody));

    body->prev = NULL;
    body->next = ncBodies;

    if (ncBodies != NULL)
        ncBodies->prev = body;

    ncBodies = body;
    ncBodyCount++;

    return body;
}

void DestroyBody(ncBody* body) // removes body from the linked list and frees its memory
{
    assert(body != NULL);

    if (body->prev != NULL)
        body->prev->next = body->next;

    if (body->next != NULL)
        body->next->prev = body->prev;

    if (body == ncBodies)
        ncBodies = body->next;

    ncBodyCount--;
    free(body);
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