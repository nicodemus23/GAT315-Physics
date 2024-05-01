#include "raylib.h"
#include "body.h"
#include "mathf.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 10000

int main(void)
{   // Initialization of first Raylib window
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	Body* bodies = (Body*)malloc(sizeof(Body) * MAX_BODIES); // Allocate memory for bodies
	assert(bodies != NULL); // Check if memory was allocated

	int bodyCount = 0; // Initialize body count to zero


	while (!WindowShouldClose())
	{

		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		if (IsMouseButtonDown(0)) 
		{
			bodies[bodyCount].position = position;
			bodies[bodyCount].velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
			bodyCount++;
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

		// update bodies
		for (int i = 0; i < bodyCount; i++)
		{
			Vector2Add(bodies[i].position, bodies[i].velocity);
			DrawCircle((int)bodies[i].position.x, (int)bodies[i].position.y, 10, BLUE); // Draw circle at body position with radius 10 and color blue
		}	

		EndDrawing();
	}
	CloseWindow();
	free(bodies);
	return 0;
}  
 /*The code above is a simple example of a window that will open and display the text “Congrats! You created your first window!” in the center of the window. 
 The code is divided into three main parts: 
 Initialization of the window:  The first part of the code initializes the window with the  InitWindow()  function. This function takes three arguments: the width and height of the window and the title of the window. The window will be 800 pixels wide, 450 pixels high, and will have the title “raylib [core] example - basic window”. Main loop:  The second part of the code is the main loop. This loop will run as long as the window is open. The  WindowShouldClose()  function checks if the window should close. If the window should close, the loop will exit. Drawing:  The third part of the code is the drawing part. This part of the code is inside the main loop. The  BeginDrawing()  function starts the drawing process, and the  ClearBackground()  function clears the window with the color  RAYWHITE . The  DrawText()  function draws the text “Congrats! You created your first window!” in the center of the window. The  EndDrawing()  function ends the drawing process. 
 The  CloseWindow()  function closes the window when the main loop exits. 
 To compile and run the code, you need to have the Raylib library installed on your system. You can download the library from the  official Raylib website. 
 After downloading the library, you can compile the code with the following command: 
 gcc -o main src/main.c -lraylib
 
 This command will compile the code and create an executable file named  main . You can run the executable file with the following command: 
 ./main
 
 The window will open and display the text “Congrats! You created your first window!” in the center of the window. 
 Conclusion 
 Raylib is a simple and easy-to-use library for creating 2D and 3D games and applications. It provides a wide range of functions for drawing graphics, handling input, and playing sounds. 
 In this article, you learned how to install Raylib on Ubuntu and create a simple window with the Raylib library. You also learned how to compile and run a simple Raylib program. 
 If you are interested in game development, you can explore the */