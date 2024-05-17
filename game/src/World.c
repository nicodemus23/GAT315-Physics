#include "world.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

Body* jgBodies = NULL;
int jgBodyCount = 0;
Vector2 jgGravity;

Body* CreateBody(Vector2 position, float mass, bodyType bt) {
	Body* body = (Body*)malloc(sizeof(Body));
	assert(body);

	memset(body, 0, sizeof(Body));

	body->Position = position;
	body->mass = mass;
	body->type = bt;
	body->inverseMass = (bt == BT_DYNAMIC) ? 1 / mass : 0;


	return body;
}

void AddBody(Body* body) {
	assert(body);
	body->prev = NULL;
	body->next = jgBodies;
	if (jgBodies != NULL) {
		jgBodies->prev = body;
	}
	jgBodyCount++;
	jgBodies = body;
}

void DestroyBody(Body* body) {
	assert(body);
	if (body->prev != NULL) body->prev->next = body->next;
	if (body->next != NULL) body->next->prev = body->prev;
	if (jgBodies == body) jgBodies = body->next;
	jgBodyCount--;
	free(body);
}