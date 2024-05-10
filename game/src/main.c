#include "raylib.h"
#include "body.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"
#include "raymath.h"
#include "force.h"
#include "editor.h"
#include "shapes.h"

#include <stdlib.h>
#include <assert.h>

#define TRACERLIFESPAN 1.0f
#define DECAY_RATE 10.0f
#define MAX_LIGHTS 100
#define BRIGHTNESS_THRESHOLD 10000.0f //<- brightness threshold for considering a body a light source


int main(void)
{
	// Initialization of first Raylib window
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	Vector2 size = { screenWidth, screenHeight };

	// initialize world
	Vector2 ncGravity = (Vector2){ 0, -9.8f }; //<- gravity vector (0, -9.8f) set to 0, 0 temporarily for exercise

	// create render texture to store rendered scene before applying post-processing effects
	RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
	//RenderTexture2D brightTexture = LoadRenderTexture(screenWidth, screenHeight);
	//RenderTexture2D blurTexture = LoadRenderTexture(screenWidth, screenHeight);
	RenderTexture2D tracerTexture = LoadRenderTexture(screenWidth, screenHeight); //<- tracer texture to keep track of previous frames
	RenderTexture2D bloomTexture = LoadRenderTexture(screenWidth, screenHeight);



	// Load post-processing and bloom shaders
	/*Shader brightPassShader = LoadShader(0, "resources/shaders/PPS_bright.fs");
	Shader blurPassShader = LoadShader(0, "resources/shaders/PPS_blur.fs");
	Shader combinePassShader = LoadShader(0, "resources/shaders/PPS_combine.fs");*/
	Shader bloomShader = LoadShader("resources/shaders/base.vs", "resources/shaders/bloom.fs");
	Shader lensFlareShader = LoadShader("resources/shaders/base.vs", "resources/shaders/lens_flare.fs");

	// set bloom shader uniforms
	SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "size"), &size, SHADER_UNIFORM_VEC2);
	float samples = 100.0f;
	float quality = 50.5f;
	SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "samples"), &samples, SHADER_UNIFORM_FLOAT);
	SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "quality"), &quality, SHADER_UNIFORM_FLOAT);

	// set lens flare shader uniforms
	SetShaderValue(lensFlareShader, GetShaderLocation(lensFlareShader, "resolution"), &size, SHADER_UNIFORM_VEC2);
	Vector2 lightPositions[MAX_LIGHTS];
	int numLights = 0;

	float decay = DECAY_RATE;
	bool firstFrame = true;

	float sphereRadius = 3.0f;
	float initialSpeed = 200.0f;

	float scaleFactor = 5.0f;

	// for scaling stars on draw call:
	//float scaleFactor = 5.0;
	//float scaledOuterRadius = outerRadius / scaleFactor;
	//float scaledInnerRadius = innerRadius / scaleFactor;

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		float fps = (float)GetFPS();
		Vector2 position = GetMousePosition();

		numLights = 0; // reset number of lights each frame

		if (IsMouseButtonDown(0))
		{
			int numBodies = 50;
			for (int i = 0; i < numBodies; i++)
			{
				ncBody* body = CreateBody();
				if (body != NULL)
				{
					// calculate random position on surface of a sphere
					float theta = GetRandomFloatValue(0, 2 * PI);
					float phi = GetRandomFloatValue(0, PI);
					float x = sphereRadius * sinf(phi) * cosf(theta);
					float y = sphereRadius * sinf(phi) * sinf(theta);
					float z = sphereRadius * cosf(phi);

					//body->position = position;
					body->position = Vector2Add(position, (Vector2) { x, y });
					//body->mass = GetRandomFloatValue(ncEditorData.MassMinValue, ncEditorData.MassMaxValue);
					body->mass = GetRandomFloatValue(1, 10);
					body->inverseMass = 1.0f / body->mass;
					body->type = BT_DYNAMIC;
					body->damping = 0.0f;
					body->gravityScale = 1.0f;

					// Star var
					//body->color = ColorFromHSV(GetRandomValue(0, 60), 0.8f, GetRandomValue(200, 255)); //<- random color
					body->outerRadius = GetRandomValue(3, 10);
					body->innerRadius = body->outerRadius * 0.9f;
					body->numPoints = GetRandomValue(3, 5);
					body->color = GetStarColor(GetRandomValue(3000, 25000));
					body->randomTwinkleOffset = GetRandomFloatValue(0, 2 * PI); //<- random offset for twinkling effect per star
				

					// star colors
					//int colorIndex = GetRandomValue(0, 4);
					//switch (colorIndex)
					//{
					//	case 0: body->color = BLUE; break;
					//	case 1: body->color = (Color){ 173, 216, 230, 255 }; break; // Light blue
					//	case 2: body->color = YELLOW; break;
					//	case 3: body->color = ORANGE; break;
					//	case 4: body->color = RED; break;

					//}

					// caluclate direction from center to body position
					Vector2 direction = Vector2Subtract(body->position, position);
					direction = Vector2Normalize(direction);

					// apply initial force in directon of sphere surface
					ApplyForce(body, Vector2Scale(direction, initialSpeed), FM_VELOCITY);


					ApplyForce(body, (Vector2) { GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
					body->lifespan = LIFESPAN;
					body->alpha = 1.0f; 
					//float hue = GetRandomFloatValue(0, 100);
					//float saturation = 0.5f;
					//float brightness = 3000.0f;
					//body->color = ColorFromHSV(hue, saturation, brightness); // <- random color based on hue, saturation, and brightness values 

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
			//ApplyGravitation(ncBodies, gravitationStrength);

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
		EndTextureMode();

	/*	while (body)
		{
			ncBody* nextBody = body->next;
			UpdateBody(body, dt);
			if (body->lifespan == 0.0f && body->alpha == 0.0f)
			{
				DestroyBody(body);
			}

			body = nextBody;
		}*/

		// Update light positions for lense flare
		SetShaderValue(lensFlareShader, GetShaderLocation(lensFlareShader, "lightPositions"), lightPositions, SHADER_UNIFORM_VEC2, numLights);
		SetShaderValue(lensFlareShader, GetShaderLocation(lensFlareShader, "numLights"), &numLights, SHADER_UNIFORM_INT);

		//----------------------------------------------------------------------------------
		// Draw to tracer texture
		BeginTextureMode(tracerTexture);
		BeginBlendMode(BLEND_ALPHA); //<- blend mode to alpha blend the tracers together over time (BLEND_ALPHA) 
		DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 1.0f - decay)); 


		body = ncBodies;
		while (body)
		{
			float tracerAlpha = body->alpha;
			Color colorWithAlpha = Fade(body->color, tracerAlpha * 100.0f); //<- fade the color of the body based on its alpha (WHICH ISN'T FUCKING WORKING!!) 
			//DrawCircle((int)body->position.x, (int)body->position.y, body->mass, colorWithAlpha);

			Color dimmedColor = ApplyDimming(body->color, 100.f);
			DrawStar(body->position, body->outerRadius, body->innerRadius, body->numPoints, dimmedColor, body->randomTwinkleOffset, scaleFactor);


			body = body->next; //<- move to the next body
		}
		EndBlendMode();
		EndTextureMode();

		//----------------------------------------------------------------------------------

		// Draw bodies to target texture
		BeginTextureMode(target);
		ClearBackground(BLACK);
		BeginBlendMode(BLEND_ADDITIVE);



	

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
		SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "size"), &(Vector2){ 1280, 720 }, SHADER_UNIFORM_VEC2);
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


		//DrawEditor();

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





