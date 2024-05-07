#include "raylib.h"
#include "body.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"
#include "raymath.h"
#include "force.h"

#include <stdlib.h>
#include <assert.h>

#define TRACERLIFESPAN 1.0f
#define DECAY_RATE 0.5f


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
	//RenderTexture2D brightTexture = LoadRenderTexture(screenWidth, screenHeight);
	//RenderTexture2D blurTexture = LoadRenderTexture(screenWidth, screenHeight);
	RenderTexture2D tracerTexture = LoadRenderTexture(screenWidth, screenHeight); //<- tracer texture to keep track of previous frames
	RenderTexture2D bloomTexture = LoadRenderTexture(screenWidth, screenHeight);



	// Load post-processing and bloom shaders
	/*Shader brightPassShader = LoadShader(0, "resources/shaders/PPS_bright.fs");
	Shader blurPassShader = LoadShader(0, "resources/shaders/PPS_blur.fs");
	Shader combinePassShader = LoadShader(0, "resources/shaders/PPS_combine.fs");*/
	Shader bloomShader = LoadShader("resources/shaders/base.vs", "resources/shaders/bloom.fs");

	// set bloom shader uniforms
	Vector2 size = { screenWidth, screenHeight };
	SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "size"), &size, SHADER_UNIFORM_VEC2);
	float samples = 15.0f;
	float quality = 8.5f;
	SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "samples"), &samples, SHADER_UNIFORM_FLOAT);
	SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "quality"), &quality, SHADER_UNIFORM_FLOAT);

	float decay = 1.0f;
	bool firstFrame = true;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        float fps = (float)GetFPS();
        Vector2 position = GetMousePosition();

        if (IsMouseButtonDown(0))
        {
            int numBodies = 50;
            for (int i = 0; i < numBodies; i++)
            {
                ncBody* body = CreateBody();
                if (body != NULL)
                {
                    body->position = position;
                    body->mass = GetRandomFloatValue(1, 20);
                    body->inverseMass = 1.0f / body->mass;
                    body->type = BT_DYNAMIC;
                    body->damping = 2.5f;
                    body->gravityScale = 20.0f;
                    ApplyForce(body, (Vector2) { GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
                    body->lifespan = LIFESPAN;
                    body->alpha = 1.0f;
                    float hue = GetRandomFloatValue(0, 10);
                    float saturation = 0.5f;
                    float brightness = 1.0f;
                    body->color = ColorFromHSV(hue * 360.0f, saturation, brightness);
                }
            }
        }

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

        // Draw to tracer texture
        BeginTextureMode(tracerTexture);
        BeginBlendMode(BLEND_ALPHA);
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, decay));

        body = ncBodies;
        while (body)
        {
            float tracerAlpha = body->alpha;
            Color colorWithAlpha = Fade(body->color, tracerAlpha);
            DrawCircle((int)body->position.x, (int)body->position.y, body->mass, colorWithAlpha);
            body = body->next;
        }
        EndBlendMode();
        EndTextureMode();

        // Draw bodies to target texture
        BeginTextureMode(target);
        ClearBackground(BLACK);
        body = ncBodies;
        while (body)
        {
            DrawCircle((int)body->position.x, (int)body->position.y, body->mass, body->color);
            body = body->next;
        }
        EndTextureMode();

        // Apply bloom shader on target texture
        BeginDrawing();
        ClearBackground(BLACK);

        BeginShaderMode(bloomShader);
        SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "size"), &(Vector2){ 1280, 720 }, SHADER_UNIFORM_VEC2);
        SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "samples"), &(float){ 5.0f }, SHADER_UNIFORM_FLOAT);
        SetShaderValue(bloomShader, GetShaderLocation(bloomShader, "quality"), &(float){ 2.5f }, SHADER_UNIFORM_FLOAT);
        DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Vector2) { 0, 0 }, WHITE);
        EndShaderMode();

        // Overlay tracers
        DrawTextureRec(tracerTexture.texture, (Rectangle) { 0, 0, (float)tracerTexture.texture.width, -(float)tracerTexture.texture.height }, (Vector2) { 0, 0 }, WHITE);

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





