#pragma once
#include "raylib.h"
#include "body.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"
#include "raymath.h"
#include "force.h"
#include "editor.h"
#include "shapes.h"
//#define RAYGUI_IMPLEMENTATION

#include "C:\Users\Nic\Documents\Neumont\6_Spring 2024\GAT315\GAT315-Physics\raygui\src\raygui.h"
//#include "GUI_var.h"

#include <stdlib.h>
#include <assert.h>

#define TRACERLIFESPAN 1.0f
#define DECAY_RATE 1.0f
#define MAX_LIGHTS 100
#define BRIGHTNESS_THRESHOLD 5000.0f //<- brightness threshold for considering a body a light source

int main(void)
{
	// Initialization of first Raylib window
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	float ncScreenZoom;
	Vector2 size = { screenWidth, screenHeight };

	// initialize world
	Vector2 ncGravity = (Vector2){ 0, -9.8f }; //<- gravity vector (0, -9.8f) set to 0, 0 temporarily for exercise

	// create render texture to store rendered scene before applying post-processing effects
	RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

	RenderTexture2D tracerTexture = LoadRenderTexture(screenWidth, screenHeight); //<- tracer texture to keep track of previous frames
	RenderTexture2D bloomTexture = LoadRenderTexture(screenWidth, screenHeight);


	Shader bloomShader = LoadShader("resources/shaders/base.vs", "resources/shaders/PPS_bloom.fs");
	Shader lensFlareShader = LoadShader("resources/shaders/base.vs", "resources/shaders/lens_flare.fs");

	// set bloom shader uniforms
	//SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "size"), &size, SHADER_UNIFORM_VEC2);
	//float samples = 1.0f;
	//float quality = 200.5f;
	//SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "samples"), &samples, SHADER_UNIFORM_FLOAT);
	//SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "quality"), &quality, SHADER_UNIFORM_FLOAT);

	// set lens flare shader uniforms
	SetShaderValue(lensFlareShader, GetShaderLocation(lensFlareShader, "resolution"), &size, SHADER_UNIFORM_VEC2);
	Vector2 lightPositions[MAX_LIGHTS];
	int numLights = 0;

	float decay = DECAY_RATE;
	bool firstFrame = true;

	float sphereRadius = 3.0f;
	float initialSpeed = 2000.0f;

	float scaleFactor = 5.0f;

	// GUI variables
	/*float guiBodyMass = 1.0f;
	float guiBodyGravityScale = 1.0f;
	float guiBodyDamping = 0.2f;
	float guiBodyLifespan = 0.0f;
	float guiBodyAlpha = 1.0f;
	float guiBodyOuterRadius = 5.0f;
	float guiBodyInnerRadius = 4.5f;
	int guiBodyNumPoints = 5;
	Color guiBodyColor = WHITE;*/

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		float fps = (float)GetFPS();
		Vector2 position = GetMousePosition();
		//ncScreenZoom += GetMouseWheelMove() * 0.2f;
		//ncScreenZoom = Clamp(ncScreenZoom, 0.f, 10);

		// Create a GUI window
	//	Rectangle windowBounds = { 10, 60, 300, 400 };
	////	GuiWindow(windowBounds, "Body Settings");

	//	// Create the GUI controls
	//	guiBodyMass = GuiSlider((Rectangle) { 20, 100, 280, 20 }, "Mass", TextFormat("%.1f", guiBodyMass), &guiBodyMass, 0.1f, 20.0f);
	//	guiBodyGravityScale = GuiSlider((Rectangle) { 20, 130, 280, 20 }, "Gravity Scale", TextFormat("%.1f", guiBodyGravityScale), &guiBodyGravityScale, 0.0f, 5.0f);
	//	guiBodyDamping = GuiSlider((Rectangle) { 20, 160, 280, 20 }, "Damping", TextFormat("%.2f", guiBodyDamping), &guiBodyDamping, 0.0f, 1.0f);
	//	guiBodyLifespan = GuiSlider((Rectangle) { 20, 190, 280, 20 }, "Lifespan", TextFormat("%.1f", guiBodyLifespan), &guiBodyLifespan, 0.0f, 10.0f);
	//	guiBodyAlpha = GuiSlider((Rectangle) { 20, 220, 280, 20 }, "Alpha", TextFormat("%.2f", guiBodyAlpha), &guiBodyAlpha, 0.0f, 1.0f);
	//	guiBodyOuterRadius = GuiSlider((Rectangle) { 20, 250, 280, 20 }, "Outer Radius", TextFormat("%.1f", guiBodyOuterRadius), &guiBodyOuterRadius, 1.0f, 20.0f);
	//	guiBodyInnerRadius = GuiSlider((Rectangle) { 20, 280, 280, 20 }, "Inner Radius", TextFormat("%.1f", guiBodyInnerRadius), &guiBodyInnerRadius, 0.5f, 19.5f);
	//	guiBodyNumPoints = GuiSlider((Rectangle) { 20, 310, 280, 20 }, "Num Points", TextFormat("%d", guiBodyNumPoints), guiBodyNumPoints, 3, 10);
	//	int pickedColor = GuiColorPicker((Rectangle) { 20, 340, 280, 20 }, "Color", &guiBodyColor);
	//	guiBodyColor = GetColor(pickedColor);


	//	numLights = 0; // reset number of lights each frame

		if (IsMouseButtonDown(0))
		{
			int numBodies = 80;
			for (int i = 0; i < numBodies; i++)
			{
				ncBody* body = CreateBody();
				if (body != NULL)
				{
					// Use GUI values to set body properties
		/*			body->mass = guiBodyMass;
					body->inverseMass = 1.0f / guiBodyMass;
					body->damping = guiBodyDamping;
					body->gravityScale = guiBodyGravityScale;
					body->outerRadius = guiBodyOuterRadius;
					body->innerRadius = guiBodyInnerRadius;
					body->numPoints = guiBodyNumPoints;
					body->color = guiBodyColor;
					body->lifespan = guiBodyLifespan;
					body->alpha = guiBodyAlpha;*/

					// calculate random position on surface of a sphere
					float theta = GetRandomFloatValue(0, 2 * PI);
					float phi = GetRandomFloatValue(0, PI);
					float x = sphereRadius * sinf(phi) * cosf(theta);
					float y = sphereRadius * sinf(phi) * sinf(theta);
					float z = sphereRadius * cosf(phi);

					//body->position = position;
					body->position = Vector2Add(position, (Vector2) { x, y });
					body->mass = GetRandomFloatValue(ncEditorData.MassMinValue, ncEditorData.MassMaxValue);
					//body->mass = GetRandomFloatValue(1, 10);
					body->inverseMass = 1.0f / body->mass;
					body->type = BT_DYNAMIC;
					body->damping = 0;// 0.2f;
					body->gravityScale = 0.0f;

					// Star var
					//body->color = ColorFromHSV(GetRandomValue(0, 60), 0.8f, GetRandomValue(200, 255)); //<- random color
					body->outerRadius = GetRandomValue(3, 8);
					body->innerRadius = body->outerRadius * 0.9f;
					body->numPoints = GetRandomValue(4, 8);
					body->color = GetStarColor(GetRandomValue(3000, 25000));
					body->randomTwinkleOffset = GetRandomFloatValue(0, 2 * PI); //<- random offset for twinkling effect per star

					// caluclate direction from center to body position
					Vector2 direction = Vector2Subtract(body->position, position);
					direction = Vector2Normalize(direction);

					// apply initial force in directon of sphere surface
					ApplyForce(body, Vector2Scale(direction, initialSpeed), FM_VELOCITY);


					ApplyForce(body, (Vector2) { GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
					body->lifespan = LIFESPAN;
					body->alpha = 1.0f; 
					
				}
			}
		}
		//BeginTextureMode(target);
		//ClearBackground(BLACK);

		

		// Update bodies and draw to target texture
		ncBody* body = ncBodies;
		while (body)
		{
			float gravitationStrength = 1.0f;
			//ApplyGravitation(ncBodies, ncEditorData.GravitationValue);

			ncBody* nextBody = body->next;
			UpdateBody(body, dt);
			if (body->lifespan == 0.0f && body->alpha == 0.0f) //<- if body has no lifespan and no alpha
			{
				DestroyBody(body);
			}
			else
			{

				DrawStar(body->position, body->outerRadius, body->innerRadius, body->numPoints, body->color, body->randomTwinkleOffset, scaleFactor);
				// check whether the body should be considered a lightsource
				if ((body->color.r + body->color.g + body->color.b) / 3 > BRIGHTNESS_THRESHOLD && numLights < MAX_LIGHTS)
				{
					lightPositions[numLights++] = body->position;
				}
			}
			body = nextBody;
		}
		//EndTextureMode();

		// Update light positions for lense flare
		SetShaderValue(lensFlareShader, GetShaderLocation(lensFlareShader, "lightPositions"), lightPositions, SHADER_UNIFORM_VEC2, numLights);
		SetShaderValue(lensFlareShader, GetShaderLocation(lensFlareShader, "numLights"), &numLights, SHADER_UNIFORM_INT);

		//----------------------------------------------------------------------------------
		// Draw to tracer texture
		BeginTextureMode(tracerTexture);
		//ClearBackground(BLACK, decay);
		BeginBlendMode(BLEND_ALPHA); //<- blend mode to alpha blend the tracers together over time (BLEND_ALPHA) 
		DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 1.0f - decay)); 
		//DrawTextureRec(tracerTexture.texture, (Rectangle) { 0, 0, (float)tracerTexture.texture.width, -(float)tracerTexture.texture.height }, (Vector2) { 0, 0 }, WHITE); //<- draw tracer texture



		body = ncBodies;
		while (body)
		{
			float tracerAlpha = body->alpha;
			Color colorWithAlpha = Fade(body->color, tracerAlpha + 10000.0f); //<- fade the color of the body based on its alpha (WHICH ISN'T FUCKING WORKING!!) 
			//DrawCircle((int)body->position.x, (int)body->position.y, body->mass, colorWithAlpha);

			Color dimmedColor = ApplyDimming(body->color, 5000.1f);
			DrawStar(body->position, body->outerRadius, body->innerRadius, body->numPoints, colorWithAlpha, body->randomTwinkleOffset, scaleFactor);
			


			body = body->next; //<- move to the next body
		}
		EndBlendMode();
		EndTextureMode();

		//----------------------------------------------------------------------------------

		// Draw bodies to target texture
		BeginTextureMode(target);
		ClearBackground(BLACK);
		BeginBlendMode(BLEND_ALPHA);

		body = ncBodies;
		while (body)
		{
			//DrawCircle((int)body->position.x, (int)body->position.y, body->mass, body->color);
			//Color multColor = ApplyDimming(body->color, 2.0f);
			DrawStar(body->position, body->outerRadius, body->innerRadius, body->numPoints, body->color, body->randomTwinkleOffset, scaleFactor);

			body = body->next;
		}
		EndBlendMode();
		EndTextureMode();

		//-------------------------------------------------------------------------------------
		//Apply bloom shader on target texture
		BeginDrawing();
		ClearBackground(BLACK);

		
		BeginShaderMode(bloomShader);
		//SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "size"), &(Vector2){ 1280, 720 }, SHADER_UNIFORM_VEC2);
		//SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "samples"), &(float){ 10.0f }, SHADER_UNIFORM_FLOAT);
		//SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "quality"), &(float){ 4.0f }, SHADER_UNIFORM_FLOAT);
		DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Vector2) { 0, 0 }, WHITE); //<- draw target texture
		EndShaderMode();

		// Apply lens flare shader
		BeginShaderMode(lensFlareShader);
		DrawTextureRec(target.texture, (Rectangle) { 0, 0, screenWidth, -screenHeight }, (Vector2) { 0, 0 }, WHITE);
		EndShaderMode();

		// Overlay tracers
		DrawTextureRec(tracerTexture.texture, (Rectangle) { 0, 0, (float)tracerTexture.texture.width, -(float)tracerTexture.texture.height }, (Vector2) { 0, 0 }, WHITE); //<- draw tracer texture


		DrawEditor();
		UpdateEditor(position);

		// HUD
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: (%.4f)", dt), 10, 30, 20, LIME);

		EndDrawing();

		decay *= DECAY_RATE;
		
	}
	UnloadShader(bloomShader);
	UnloadRenderTexture(target);
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





