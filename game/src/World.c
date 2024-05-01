#include "world.h"
#include <stdlib.h>
#include <assert.h>

Body* bodies = NULL;
int bodyCount = 0;

Body* CreateBody()
{
    Body* newBody = (Body*)malloc(sizeof(Body));
    assert(newBody != NULL);

    newBody->prev = NULL;
    newBody->next = bodies;

    if (bodies != NULL)
        bodies->prev = newBody;

    bodies = newBody;
    bodyCount++;

    return newBody;
}

void DestroyBody(Body* body)
{
    assert(body != NULL);

    if (body->prev != NULL)
        body->prev->next = body->next;

    if (body->next != NULL)
        body->next->prev = body->prev;

    if (body == bodies)
        bodies = body->next;

    bodyCount--;
    free(body);
}