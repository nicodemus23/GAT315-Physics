#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <assert.h>
#include <world.h>
#include "integrator.h"
#include "force.h"
#include "render.h"
#include "editor.h"
#include "spring.h"
#include "collision.h"
#include "contact.h"


#define MAX_BODIES 1000


int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	//initialize world
	jgGravity = (Vector2){ 0,-10 };

	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;

	Vector2 prevPos = Vector2Zero();

	// fixed time step logic
	double timeAccumulator = 0.0; //accumulates delta time
	double fixedTimeStep = 1.0f / 50.0f; //rate that physics simulator will run at



	//game loop
	while (!WindowShouldClose())
	{
		//update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.25f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10.0f);
		UpdateEditor(position);

		// handle user input and selection
		selectedBody = GetBodyIntersect(ncBodies, position);
		if (selectedBody) {
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5f) + 5, YELLOW);
		}

		// create a new body on left mouse click
		if (IsMouseButtonPressed(0)) {
			ncBody* newbody = CreateBody(ConvertScreenToWorld(position), GetRandomFloatValue(state.MassMinValue, state.MassMaxValue), state.BodyTypeActive);
			AddBody(newbody);
			newbody->gravityScale = state.GravityScaleValue;
			newbody->restitution = 1.0f;
			int num = (rand() % (3 - 1 + 1)) + 1;
			switch (num)
			{
			case 1:
				newbody->color = RED;
				break;
			case 2:
				newbody->color = GREEN;
				break;
			case 3:
				newbody->color = BLUE;
				break;
			}
			//ApplyForce(newbody, (Vector2) { GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
			//newbody->Velocity = );
		}

		// connect bodies with spring on right mouse click
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) {
			connectBody = selectedBody;
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) {
			DrawLineBodyToPosition(connectBody, position);
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody) {
			if (selectedBody && selectedBody != connectBody) {
				ncSpring_t* spring = CreateSpring(selectedBody, connectBody, 1, 10);
				AddSpring(spring);
			}
		}

		// add delta time to time accumulator
		timeAccumulator += dt;

		// create a while loop that loops while the time accumulator is greater than or equal to the fixed time step
		while (timeAccumulator >= fixedTimeStep) 
		{
			// subtract fixed time step from time accumulator
			timeAccumulator -= fixedTimeStep;
			// apply gravitation and other forces
			//ApplyGravitation(ncBodies, 0.1f); 
			ApplySpringForce(ncSprings);
			// step each body, make sure to pass in the fixed time step for the timestep
			for (ncBody* body = ncBodies; body; body = body->next) {
				Step(body, fixedTimeStep);
				// destroy all contacts, create contacts, separate contacts, resolve contacts (collision)
				ncContact_t* contacts = NULL;
				DestroyAllContacts(&contacts);
				CreateContacts(ncBodies, &contacts);
				SeparateContacts(contacts);
				ResolveContacts(contacts);

			}
		}

		// ===== Physics Update =====
		ncBody* currentbody = ncBodies;
		//apply gravitation
		//ApplyGravitation(ncBodies, 2); 

		//ApplySpringForce(ncSprings);

		for (ncBody* body = ncBodies; body; body = body->next) {
			Step(body, dt);
		}

		//collision
		ncContact_t* contacts = NULL;
		/*CreateContacts(ncBodies, &contacts);
		SeparateContacts(contacts);
		ResolveContacts(contacts);*/

		currentbody = ncBodies;
		while (currentbody) {


			currentbody = currentbody->next;
		}


		//draw
		BeginDrawing();
		ClearBackground(BLACK);
		//stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: (%.4f)", dt), 10, 30, 20, LIME);

		//DrawCircle((int)position.x, (int)position.y, 10, RED);
		//draw bodies
		currentbody = ncBodies;
		for (ncBody* body = ncBodies; body; body = body->next) {
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
			currentbody = currentbody->next;
		}

		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next) {
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine(screen1.x, screen1.y, screen2.x, screen2.y, LIME);

		}

		for (ncContact_t* contact = contacts; contact; contact = contact->next) {
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(contact->body2->position);
			DrawCircleLines((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
			DrawCircleLines((int)screen2.x, (int)screen2.y, ConvertWorldToPixel(contact->body2->mass * 0.5f), RED);
		}
		DrawEditor(position);
		EndDrawing();
		prevPos = position;
	}
	CloseWindow();
	
	return 0;
}

// FIXED TIME STEP LOGIC
// In the main function, we have a fixed time step logic that accumulates delta time and runs the physics simulation at a fixed rate:
//Outside the main while loop
//Define a fixedTimestep(rate that physics simulator will run at) set it to 1.0f / 50 (or number of frames you want it to run per second)
//Define a timeAccumulator(accumulates delta time)
//Before the physics update
//Add delta time to time accumulator
//Create a while () loop that loops while the time accumulator is greater than or equal to the fixed time step
//Subtract fixed time step from time accumulator
//Apply Gravitation and other forces
//Step() each body, make sure to pass in the fixed time step for the timestep
//Destroy all contacts
//Create contacts
//Separate contacts
//Resolve contacts