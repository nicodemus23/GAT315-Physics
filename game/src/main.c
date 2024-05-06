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
	Vector2 ncGravity = (Vector2){ 0, -9.8f };

	// create render texture to store rendered scene before applying post-processing effects
	RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	RenderTexture2D brightTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	RenderTexture2D blurTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

	// Load post-processing shaders
	//Shader postProcessingShader = LoadShader(0, TextFormat("resources/shaders/glsl%i.fs", 330)); //<- load shader
	//Shader postProcessingShader = LoadShader(0, "resources/shaders/PPS_bloom.frag"); //<- load shader
	Shader brightPassShader = LoadShader(0, "resources/shaders/PPS_bright.frag");
	Shader blurPassShader = LoadShader(0, "resources/shaders/PPS_blur.frag");
	Shader combinePassShader = LoadShader(0, "resources/shaders/PPS_combine.frag");


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

		// begin rendering to texture
		BeginTextureMode(target);
		ClearBackground(BLACK);

		// draw bodies
		body = ncBodies;
		while (body)
		{

			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, body->color);
			body = body->next;
		}

		// end rendering to texture
		EndTextureMode();

		//bright pass
		BeginTextureMode(brightTexture);
		BeginShaderMode(brightPassShader);
		float threshold = 0.5f;
		SetShaderValue(brightPassShader, GetShaderLocation(brightPassShader, "threshold"), &threshold, SHADER_UNIFORM_FLOAT);
		DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
		EndShaderMode();
		EndTextureMode();

		//blur pass (horizontal)
		BeginTextureMode(blurTexture);
		BeginShaderMode(blurPassShader);
		Vector2 horizontal = (Vector2){ 1.0f, 0.0f }; //<- horizontal blur
		SetShaderValue(blurPassShader, GetShaderLocation(blurPassShader, "blurDirection"), &horizontal, SHADER_UNIFORM_VEC2);
		DrawTextureRec(brightTexture.texture, (Rectangle) { 0, 0, (float)brightTexture.texture.width, (float)-brightTexture.texture.height }, (Vector2) { 0, 0 }, WHITE);
		EndShaderMode();
		EndTextureMode();

		// blur pass (vertical)
		BeginTextureMode(target);
		BeginShaderMode(blurPassShader);
		Vector2 vertical = { 0.0f, 1.0f };
		SetShaderValue(blurPassShader, GetShaderLocation(blurPassShader, "blurDirection"), &vertical, SHADER_UNIFORM_VEC2);
		DrawTextureRec(blurTexture.texture, (Rectangle) { 0, 0, (float)blurTexture.texture.width, (float)-blurTexture.texture.height }, (Vector2) { 0, 0 }, WHITE);
		EndShaderMode();
		EndTextureMode();

		// combine pass
		BeginShaderMode(combinePassShader);
		SetShaderValueTexture(combinePassShader, GetShaderLocation(combinePassShader, "originalTexture"), target.texture);
		SetShaderValueTexture(combinePassShader, GetShaderLocation(combinePassShader, "blurredTexture"), blurTexture.texture);
		float intensity = 1.0f;
		SetShaderValue(combinePassShader, GetShaderLocation(combinePassShader, "intensity"), &intensity, SHADER_UNIFORM_FLOAT);
		DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
		EndShaderMode();

		// set shader uniform
		/*float screenWidth = (float)GetScreenWidth();
		float screenHeight = (float)GetScreenHeight();
		float threshold = 0.5f; 
		float intensity = 1.0f; 

		SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "screenWidth"), &screenWidth, SHADER_UNIFORM_FLOAT);
		SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "screenHeight"), &screenHeight, SHADER_UNIFORM_FLOAT);
		SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "threshold"), &threshold, SHADER_UNIFORM_FLOAT);
		SetShaderValue(postProcessingShader, GetShaderLocation(postProcessingShader, "intensity"), &intensity, SHADER_UNIFORM_FLOAT);*/

		TraceLog(LOG_INFO, "Shader uniform values - Threshold: %.2f, Intensity: %.2f", threshold, intensity);

		// draw render texture
		DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);

		// end shader mode
		EndShaderMode();

		// render
		BeginDrawing();
		ClearBackground(BLACK);

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


	CloseWindow();

	return 0;
}

// correct render order with post-processing: 
//BeginDrawing
//BeginShaderMode
//Set shader uniforms
//Draw your scene
//EndShaderMode
//EndDrawing





