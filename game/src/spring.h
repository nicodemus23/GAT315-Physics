#pragma once


struct Body;
typedef struct jgSpring_t {
	struct Body* body1;
	struct Body* body2;
	float restLength;
	float k; //stiffness

	struct jgSpring_t* next;
	struct jgSpring_t* prev;

}jgSpring_t;

extern jgSpring_t* jgsprings;

jgSpring_t* CreateSpring(struct Body* body1, struct Body* body2, float rest, float k);
void AddSpring(jgSpring_t* body);
void DestroySpring(jgSpring_t* body);

void ApplySpringForce(jgSpring_t* spring);