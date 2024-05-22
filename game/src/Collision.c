#include "collision.h"
#include "contact.h"
#include "body.h"
#include "stdlib.h"
#include "assert.h"

bool Intersects(ncBody* body1, ncBody* body2) {
	// if distance is less than radius

	float distance = Vector2Distance(body1->position, body2->position);
	float radius = body1->mass + body2->mass;

	return distance <= radius;
}

void CreateContacts(ncBody* bodies, ncContact_t** contacts) {
	for (ncBody* body1 = bodies; body1; body1 = body1->next) {
		for (ncBody* body2 = body1->next; body2; body2 = body2->next) {
			if (body1 == body2) continue;
			if (body1->type != BT_DYNAMIC && body2->type != BT_DYNAMIC) continue;

			if (Intersects(body1, body2)) {
				ncContact_t* contact = GenerateContact(body1, body2);
				AddContact(contact, contacts);
			}
		}
	}
}

ncContact_t* GenerateContact(ncBody* body1, ncBody* body2) {
	ncContact_t* contact = (ncContact_t*)malloc(sizeof(ncContact_t));
	assert(contact);

	memset(contact, 0, sizeof(ncContact_t));

	contact->body1 = body1;
	contact->body2 = body2;

	Vector2 direction = Vector2Subtract(body1->position, body2->position);
	float distance = Vector2Length(direction);
	if (distance == 0)
	{
		direction = (Vector2){ GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f) };
	}

	float radius = (body1->mass + body2->mass);

	contact->depth = radius - distance;
	contact->normal = Vector2Normalize(direction);
	contact->restitution = (body1->restitution + body2->restitution) * 0.5f;

	return contact;
}

// Method to separate contacts between two bodies to resolve overlapping.
void SeparateContacts(ncContact_t* contacts) {
	// Iterate through each contact in the contact list.
	for (ncContact_t* contact = contacts; contact; contact = contact->next) {
		// Calculate total inverse mass of the two bodies involved in the contact.
		float totalInverseMass = (contact->body1->inverseMass + contact->body2->inverseMass);
		// Calculate the separation vector based on the contact normal and depth.
		Vector2 separation = Vector2Scale(contact->normal, (contact->depth / totalInverseMass));
		// Move body1 away from body2 proportionally to its inverse mass.
		contact->body1->position = Vector2Add(contact->body1->position, Vector2Scale(separation, contact->body1->inverseMass));
		// Move body2 away from body1 proportionally to its inverse mass, with opposite direction.
		contact->body2->position = Vector2Add(contact->body2->position, Vector2Scale(separation, -contact->body2->inverseMass));
	}
}

// Method to resolve contacts by applying impulses to bodies to separate them.
void ResolveContacts(ncContact_t* contacts) {
	// Iterate through each contact in the contact list.
	for (ncContact_t* contact = contacts; contact; contact = contact->next) {
		// Calculate the relative velocity between the two bodies.
		Vector2 rv = Vector2Subtract(contact->body1->Velocity, contact->body2->Velocity);
		// Calculate the normal velocity.
		float nv = Vector2DotProduct(rv, contact->normal);
		// If normal velocity is positive, the bodies are moving away from each other, no action needed.
		if (nv > 0) continue;
		// Calculate the total inverse mass of the two bodies.
		float totalInverseMass = (contact->body1->inverseMass + contact->body2->inverseMass);
		// Calculate the impulse magnitude using coefficient of restitution and normal velocity.
		float impulseMagnitude = -((1 + contact->restitution) * nv / totalInverseMass);
		// Calculate the impulse vector based on the contact normal and impulse magnitude.
		Vector2 impulse = Vector2Scale(contact->normal, impulseMagnitude);

		// Apply the impulse to body1.
		ApplyForce(contact->body1, impulse, FM_IMPULSE);
		// Apply the opposite impulse to body2.
		ApplyForce(contact->body2, Vector2Negate(impulse), FM_IMPULSE);
	}
}