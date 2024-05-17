#include "world_2.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity;

ncBody* CreateBody(Vector2 position, float mass, ncBodyType type) {
	ncBody* body = (ncBody*)malloc(sizeof(ncBody));
	assert(body);

	memset(body, 0, sizeof(ncBody));

	body->position = position;
	body->mass = mass;
	body->type = type;
	body->inverseMass = (type == BT_DYNAMIC) ? 1 / mass : 0;


	return body;
}

void AddBody(ncBody* body) {
	assert(body);
	body->prev = NULL;
	body->next = ncBodies;
	if (ncBodies != NULL) {
		ncBodies->prev = body;
	}
	ncBodyCount++;
	ncBodies = body;
}

void DestroyBody(ncBody* body) {
	assert(body);
	if (body->prev != NULL) body->prev->next = body->next;
	if (body->next != NULL) body->next->prev = body->prev;
	if (ncBodies == body) ncBodies = body->next;
	ncBodyCount--;
	free(body);
}