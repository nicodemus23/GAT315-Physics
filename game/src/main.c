#include "raylib.h"
#include "body.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

#define TRACERLIFESPAN 10.0f
#define DECAY_RATE 0.9f


int main(void)
{
	// Initialization of first Raylib window
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	// initialize world
	Vector2 ncGravity = (Vector2){ 0, -9.8f };

	// create render texture to store rendered scene before applying post-processing effects
	RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	RenderTexture2D brightTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	RenderTexture2D blurTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	RenderTexture2D tracerTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight()); //<- tracer texture to keep track of previous frames

	// Load post-processing shaders
	//Shader postProcessingShader = LoadShader(0, TextFormat("resources/shaders/glsl%i.fs", 330)); //<- load shader
	//Shader postProcessingShader = LoadShader(0, "resources/shaders/PPS_bloom.frag"); //<- load shader
	Shader brightPassShader = LoadShader(0, "resources/shaders/PPS_bright.fs");
	Shader blurPassShader = LoadShader(0, "resources/shaders/PPS_blur.fs");
	Shader combinePassShader = LoadShader(0, "resources/shaders/PPS_combine.fs");
	//Shader bloomShader = LoadShader(0, "resources/shaders/bloom.fs");

	float decay = 1.0f;

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
			ApplyForce(body, (Vector2) { GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
			TraceLog(LOG_INFO, "Created body at position (%.2f, %.2f)", position.x, position.y);
			body->lifespan = LIFESPAN;
			body->tracerlifespan = TRACERLIFESPAN;
			body->alpha = 1.0f; // start off at full opacity
			

			// Assign random bright colors
			float hue = GetRandomFloatValue(0, 10);
			float saturation = 0.5f;
			float brightness = 1.0f; //<- brightness percentage
			body->color = ColorFromHSV(hue * 360.0f, saturation, brightness); // this uses ColorFromHSV function to generate colors from HSV values
		}

		// apply force
		ncBody* body = ncBodies;
		while (body)
		{
			//ApplyForce(body, CreateVector2(0, 9.8f), FM_FORCE); // try values other than 9.8f (gravity)
			UpdateBody(body, dt);
			body = body->next;
		}

		// update bodies
		body = ncBodies;
		int bodyCount = 0;
		while (body)
		{
			Step(body, dt);
			body = body->next;
			bodyCount++;
		}
		TraceLog(LOG_INFO, "Number of bodies drawn: %d", bodyCount);

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
		float threshold = 0.5f;
		SetShaderValue(brightPassShader, GetShaderLocation(brightPassShader, "threshold"), &threshold, SHADER_UNIFORM_FLOAT);
		DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
		EndShaderMode();
		EndTextureMode();

		// Blur Pass (horizontal and vertical)
		for (int i = 0; i < 2; i++)
		{
			RenderTexture2D src = (i % 2 == 0) ? brightTexture : blurTexture;
			RenderTexture2D dst = (i % 2 == 0) ? blurTexture : brightTexture;

			Vector2 blurDirection = (i % 2 == 0) ? (Vector2) { 2.0f, 0.0f } : (Vector2) { 0.0f, 2.0f };

			BeginTextureMode(dst);
			BeginShaderMode(blurPassShader);
			SetShaderValue(blurPassShader, GetShaderLocation(blurPassShader, "blurDirection"), &blurDirection, SHADER_UNIFORM_VEC2);
			DrawTextureRec(src.texture, (Rectangle) { 0, 0, src.texture.width, -src.texture.height }, (Vector2) { 0, 0 }, WHITE);
			EndShaderMode();
			EndTextureMode();
		}

		decay *= DECAY_RATE;

		// draw tracers (trail effect)
		BeginTextureMode(tracerTexture);
		ClearBackground(BLACK);

		body = ncBodies;
		ncBody* nextBody = NULL;
		while (body != NULL) {
			nextBody = body->next;
			float alphaDecay = body->tracerlifespan / TRACERLIFESPAN;
			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, Fade(body->color, alphaDecay));

			body->tracerlifespan -= dt;
			if (body->tracerlifespan <= 0) {
				DestroyBody(body);
			}

			body = nextBody;
		}
		EndTextureMode();

		// combine pass
		BeginTextureMode(target);
		BeginShaderMode(combinePassShader);
		SetShaderValueTexture(combinePassShader, GetShaderLocation(combinePassShader, "originalTexture"), target.texture);
		SetShaderValueTexture(combinePassShader, GetShaderLocation(combinePassShader, "blurredTexture"), blurTexture.texture);
		SetShaderValueTexture(combinePassShader, GetShaderLocation(combinePassShader, "tracerTexture"), tracerTexture.texture);
		float intensity = 1.5f;
		SetShaderValue(combinePassShader, GetShaderLocation(combinePassShader, "intensity"), &intensity, SHADER_UNIFORM_FLOAT);
		DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
		EndShaderMode();
		EndTextureMode();

		// Draw final output
		DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);

		// Reset decay rate after each frame
		decay = 1.0f;

		// RENDER FINAL OUTPUT
		DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);


		// stats (HUD)
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME); //<- frames per second
		DrawText(TextFormat("FRAME: (%.4f)", dt), 10, 30, 20, LIME); //<- frame time

		// cursor position
		DrawCircle((int)position.x, (int)position.y, 20, RED); //<- draw cursor

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





