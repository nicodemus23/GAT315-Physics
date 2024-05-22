#include "world.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int jgBodyCount = 0;
Vector2 jgGravity;

ncBody* CreateBody(Vector2 position, float mass, bodyType bt) {
	ncBody* body = (ncBody*)malloc(sizeof(ncBody));
	assert(body);

	memset(body, 0, sizeof(ncBody));

	body->position = position;
	body->mass = mass;
	body->type = bt;
	body->inverseMass = (bt == BT_DYNAMIC) ? 1 / mass : 0;


	return body;
}

void AddBody(ncBody* body) {
	assert(body);
	body->prev = NULL;
	body->next = ncBodies;
	if (ncBodies != NULL) {
		ncBodies->prev = body;
	}
	jgBodyCount++;
	ncBodies = body;
}

void DestroyBody(ncBody* body) {
	assert(body);
	if (body->prev != NULL) body->prev->next = body->next;
	if (body->next != NULL) body->next->prev = body->prev;
	if (ncBodies == body) ncBodies = body->next;
	jgBodyCount--;
	free(body);
}