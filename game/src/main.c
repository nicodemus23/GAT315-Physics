#include "raylib.h"
#include "body.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

int main(void)
{
	// Initialization of first Raylib window
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60); 

	// initialize world
	ncGravity = (Vector2){ 0, 9.8f };


	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		if (IsMouseButtonDown(0))
		{
			ncBody* body = CreateBody();
			body->position = position;
			//body->velocity = CreateVector2(GetRandomFloatValue(-50, 50), GetRandomFloatValue(-50, 50));
			body->mass = GetRandomFloatValue(1, 20);
			body->inverseMass = 1.0f / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 2.5f;
			body->gravityScale = 20.0f;
			ApplyForce(body, (Vector2) { GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) },  FM_VELOCITY);
		}

		// apply force
		ncBody* body = ncBodies;
		while (body)
		{
			//ApplyForce(body, CreateVector2(0, 9.8f), FM_FORCE); // try values other than 9.8f (gravity)
			body = body->next;
		}

		// update bodies
		body = ncBodies;
		while (body)
		{
			Step(body, dt);
			body = body->next;
		}


		// render
		BeginDrawing();
		ClearBackground(BLACK);

		// stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: (%.4f)", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 20, RED); 

		// draw bodies
		// Clean up bodies
		body = ncBodies;
		while (body)
		{
			//ncBody* nextBody = body->next;
			//DestroyBody(body);
			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, RED);
			body = body->next;
		}

		EndDrawing();
	}



	CloseWindow();

	return 0;
}