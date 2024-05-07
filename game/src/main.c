#include "raylib.h"
#include "body.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"
#include "raymath.h"
#include "force.h"

#include <stdlib.h>
#include <assert.h>

#define TRACERLIFESPAN 20.0f
#define DECAY_RATE 0.99f


int main(void)
{
	// Initialization of first Raylib window
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	// initialize world
	Vector2 ncGravity = (Vector2){ 0, -9.8f };

	// create render texture to store rendered scene before applying post-processing effects
	RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
	RenderTexture2D brightTexture = LoadRenderTexture(screenWidth, screenHeight);
	RenderTexture2D blurTexture = LoadRenderTexture(screenWidth, screenHeight);
	RenderTexture2D tracerTexture = LoadRenderTexture(screenWidth, screenHeight); //<- tracer texture to keep track of previous frames

	// Load post-processing shaders
	Shader brightPassShader = LoadShader(0, "resources/shaders/PPS_bright.fs");
	Shader blurPassShader = LoadShader(0, "resources/shaders/PPS_blur.fs");
	Shader combinePassShader = LoadShader(0, "resources/shaders/PPS_combine.fs");

	float decay = 1.0f;
	bool firstFrame = true;

	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();

		if (IsMouseButtonDown(0))
		{
			int numBodies = 50; //<- number of bodies to create per click

			for (int i = 0; i < numBodies; i++)
			{
				// initialize body properties
				ncBody* body = CreateBody();
				if (body != NULL)
				{	// debug
					printf("Created body at position (%.2f, %.2f) with mass %.2f, lifespan %.2f, and alpha %.2f\n", body->position.x, body->position.y, body->mass, body->lifespan, body->alpha);

					body->position = position;
					//body->velocity = CreateVector2(GetRandomFloatValue(-50, 50), GetRandomFloatValue(-50, 50));
					body->mass = GetRandomFloatValue(1, 20);
					body->inverseMass = 1.0f / body->mass;
					body->type = BT_DYNAMIC;
					body->damping = 2.5f;
					body->gravityScale = 20.0f;
					ApplyForce(body, (Vector2) { GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
					TraceLog(LOG_INFO, "Created body at position (%.2f, %.2f)", position.x, position.y);
					body->lifespan = LIFESPAN;
					//body->tracerlifespan = TRACERLIFESPAN;
					body->alpha = 1.0f; // start off at full opacity

					// Assign random bright colors
					float hue = GetRandomFloatValue(0, 10);
					float saturation = 0.5f;
					float brightness = 1.0f; //<- brightness percentage
					body->color = ColorFromHSV(hue * 360.0f, saturation, brightness); // this uses ColorFromHSV function to generate colors from HSV values
				}
			}
		}

		// apply force
		// update bodies
		ncBody* body = ncBodies;
		while (body)
		{
			ncBody* nextBody = body->next;
			UpdateBody(body, dt);
			if (body->lifespan == 0.0f && body->alpha == 0.0f)
			{
				DestroyBody(body);
			}
			body = nextBody;
		}

		// update bodies
		/*for (ncBody* body = ncBodies; body; body = body->next)
		{
			Step(body, dt);
		}*/

		body = ncBodies;
		int bodyCount = 0;
		while (body)
		{
			Step(body, dt);
			body = body->next;
			bodyCount++;
		}
		//TraceLog(LOG_INFO, "Number of bodies drawn: %d", bodyCount);

		//------------------------------------------------------------------------------------------------------------------------------

		BeginDrawing();
		ClearBackground(BLACK);

		// begin rendering to texture
		BeginTextureMode(target);
		ClearBackground(BLACK);

		// draw bodies
		body = ncBodies;
		while (body)
		{
			printf("Rendering body at (%.2f, %.2f) with alpha %.2f\n", body->position.x, body->position.y, body->alpha);

			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, body->color, body->alpha);
			body = body->next;
		}

		// end rendering to texture
		EndTextureMode();

		//------------------------------------------------------------------------------------------------------------------------------

		// POST-PROCESSING

		//bright pass
		BeginTextureMode(brightTexture);
		BeginShaderMode(brightPassShader);
		float threshold = 0.6f;
		SetShaderValue(brightPassShader, GetShaderLocation(brightPassShader, "threshold"), &threshold, SHADER_UNIFORM_FLOAT);
		DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
		EndShaderMode();
		EndTextureMode();

		// Blur Pass (horizontal and vertical)
		for (int i = 0; i < 2; i++)
		{
			RenderTexture2D src = (i % 2 == 0) ? brightTexture : blurTexture; //<- alternate between bright and blur textures
			RenderTexture2D dst = (i % 2 == 0) ? blurTexture : brightTexture; //<- alternate between bright and blur textures

			Vector2 blurDirection = (i % 2 == 0) ? (Vector2) { 2.0f, 0.0f } : (Vector2) { 0.0f, 2.0f };

			BeginTextureMode(dst); //<- render to destination texture
			BeginShaderMode(blurPassShader);
			SetShaderValue(blurPassShader, GetShaderLocation(blurPassShader, "blurDirection"), &blurDirection, SHADER_UNIFORM_VEC2); //<- set blur direction
			DrawTextureRec(src.texture, (Rectangle) { 0, 0, src.texture.width, -src.texture.height }, (Vector2) { 0, 0 }, WHITE); 
			EndShaderMode();
			EndTextureMode();
		}


		// draw tracers (trail effect)
		BeginTextureMode(tracerTexture);
		ClearBackground(BLACK);
		BeginShaderMode(combinePassShader);

		// apply decay
		if (!firstFrame)
		{
			DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, decay));
		}
		else
		{
			firstFrame = false;
		}

		// draw the bodies
		body = ncBodies;
		while (body)
		{
			float tracerAlpha = body->alpha * decay; //<- set tracer alpha to body alpha
			Color colorWithAlpha = Fade(body->color, tracerAlpha);

			BeginBlendMode(BLEND_ADDITIVE);
			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, colorWithAlpha);
			EndBlendMode();

			body = body->next;
		}

		EndShaderMode();
		EndTextureMode();

		// WITHOUT USING SHADER: 
		//BeginTextureMode(tracerTexture);
		//ClearBackground(BLACK);

		//// apply decay
		//if (!firstFrame)
		//{
		//	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, decay));
		//}
		//else
		//{
		//	firstFrame = false;
		//}

		//// draw the bodies
		//body = ncBodies;
		//while (body)
		//{
		//	Color colorWithAlpha = Fade(body->color, body->alpha);
		//	DrawCircle((int)body->position.x, (int)body->position.y, body->mass, colorWithAlpha);
		//	body = body->next;
		//}

		//EndTextureMode();
		//BeginTextureMode(tracerTexture);
		//BeginShaderMode(combinePassShader);

		//// clear background on first frame only
		//if (firstFrame)
		//{
		//	ClearBackground(BLACK);
		//	firstFrame = false;
		//}
		//else
		//{
		//	decay *= DECAY_RATE; // only decay after the first frame
		//	//DrawRectangle(0, 0, tracerTexture.texture.width, tracerTexture.texture.height, Fade(BLACK, decay)); //<- fade previous frame
		//	DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, decay));

		//}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// reduce tracer alpha for all bodies
		//body = ncBodies;
		//while (body != NULL) {
		//	float tracerAlpha = body->alpha; //<- set tracer alpha to body alpha
		//	if (body->lifespan > 0.0f) {

		//		printf("Rendering tracer for body at (%.2f, %.2f) with lifespan %.2f and tracerAlpha %.2f\n", body->position.x, body->position.y, body->lifespan, tracerAlpha);

		//		// Draw on top without clearing
		//		Color colorWithAlpha = Fade(body->color, tracerAlpha); //<- fade color based on lifespan
		//		DrawCircle((int)body->position.x, (int)body->position.y, body->mass, colorWithAlpha);
		//	}
		//	body = body->next;
		//}
		//EndShaderMode();  // Stop shader
		//EndTextureMode(); // End rendering to texture

		// Debug: Draw the tracer texture
		//DrawTextureRec(tracerTexture.texture, (Rectangle) { 0, 0, (float)tracerTexture.texture.width, (float)-tracerTexture.texture.height }, (Vector2) { 0, 0 }, WHITE);

		// combine pass
		BeginTextureMode(target);
		BeginShaderMode(combinePassShader);

		SetShaderValueTexture(combinePassShader, GetShaderLocation(combinePassShader, "originalTexture"), target.texture);
		SetShaderValueTexture(combinePassShader, GetShaderLocation(combinePassShader, "blurredTexture"), blurTexture.texture);
		SetShaderValueTexture(combinePassShader, GetShaderLocation(combinePassShader, "tracerTexture"), tracerTexture.texture);

		float intensity = 5.5f;
		SetShaderValue(combinePassShader, GetShaderLocation(combinePassShader, "intensity"), &intensity, SHADER_UNIFORM_FLOAT);

		DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);

		EndShaderMode();
		EndTextureMode();

		// Draw final output
		DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);

		// Reset decay rate after each frame
		decay = 1.0f;

		// stats (HUD)
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME); //<- frames per second
		DrawText(TextFormat("FRAME: (%.4f)", dt), 10, 30, 20, LIME); //<- frame time

		// cursor position
		//DrawCircle((int)position.x, (int)position.y, 20, RED); //<- draw cursor

		EndDrawing();
	}

	// unload shader and render texture
	//UnloadShader(postProcessingShader);
	//UnloadRenderTexture(target);

	UnloadShader(brightPassShader);
	UnloadShader(blurPassShader);
	UnloadShader(combinePassShader);
	UnloadRenderTexture(target);
	UnloadRenderTexture(brightTexture);
	UnloadRenderTexture(blurTexture);
	UnloadRenderTexture(tracerTexture);

	CloseWindow();

	return 0;
}


// correct render order with post-processing: 
//BeginDrawing
//BeginShaderMode
//Set shader uniforms
//Draw scene
//EndShaderMode
//EndDrawing





