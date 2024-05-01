#include "raylib.h"
#include "body.h"
#include "mathf.h"
#include "raymath.h"
#include "world.h"
#include <stdlib.h>
#include <assert.h>

int main(void)
{
    // Initialization of first Raylib window
    InitWindow(1280, 720, "Physics Engine");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // update
        float dt = GetFrameTime();
        float fps = (float)GetFPS();
        Vector2 position = GetMousePosition();

        if (IsMouseButtonDown(0))
        {
            Body* newBody = CreateBody();
            newBody->position = position;
            newBody->velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        // render
        BeginDrawing();
        ClearBackground(BLACK);

        // stats
        DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000 / fps), 10, 10, 20, LIME);
        DrawText(TextFormat("FRAME: (%.4f)", dt), 10, 30, 20, LIME);
        DrawCircle((int)position.x, (int)position.y, 20, RED);

        // update / draw bodies
        Body* body = bodies;
        while (body)
        {
            // update body position
            body->position.x += body->velocity.x;
            body->position.y += body->velocity.y;

            // draw body
            DrawCircle((int)body->position.x, (int)body->position.y, 10, BLUE);

            body = body->next;
        }

        EndDrawing();
    }

    // Clean up bodies
    Body* body = bodies;
    while (body)
    {
        Body* nextBody = body->next;
        DestroyBody(body);
        body = nextBody;
    }

    CloseWindow();

    return 0;
}