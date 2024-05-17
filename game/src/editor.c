#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"
#include "body.h"
#include "render.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
Rectangle editorRect;

Vector2 anchor01 = { 832, 64 };
Texture2D cursorTexture;

bool EditorBoxActive = true;
float MassMinValue = 0.0f;
float MassMaxValue = 0.0f;
float Slider003Value = 0.0f;

float cursorScale = 0.2f;


jgEditorData_t jgEditorDataI;
GuiLayoutNameState state;

void InitEditor()
{
	GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");
	Image image = LoadImage("resources/greetings.png");
	cursorTexture = LoadTextureFromImage(image);
	UnloadImage(image);

	HideCursor();

	jgEditorDataI.Slider003Value = 2;
	jgEditorDataI.MassMinValue = 0.1f;
	jgEditorDataI.MassMaxValue = 10.0f;

	state.anchor01 = (Vector2){ 950, 56 };
	state.anchor02 = (Vector2){ 950, 56 };

	state.MassMinValue = 1.0f;
	state.MassMaxValue = 10.0f;
	state.GravityScaleValue = 1.0f;
	state.BodyTypeEditMode = false;
	state.BodyTypeActive = 0;
	state.Damping = 0;
}

void UpdateEditor(Vector2 position)
{
	//
}

void DrawEditor(Vector2 mousePosition)
{
	if (state.BodyTypeEditMode) GuiLock();
	if (EditorBoxActive)
	{
		//EditorBoxActive = !GuiWindowBox((Rectangle) { anchor01.x + 0, anchor01.y + 0, 248, 464 }, "Editor");
		//GuiSlider((Rectangle) { anchor01.x + 80, anchor01.y + 64, 120, 16 }, "MassMin", NULL, & jgEditorDataI.MassMinValue, 0, 100);
		//GuiSlider((Rectangle) { anchor01.x + 80, anchor01.y + 88, 120, 16 }, "MassMax", NULL, & jgEditorDataI.MassMaxValue, 0, 100);
		//GuiSlider((Rectangle) { anchor01.x + 80, anchor01.y + 112, 120, 16 }, "Gravitation", NULL, & jgEditorDataI.Slider003Value, 0, 100);




		state.WindowBox001Active = !GuiWindowBox((Rectangle) { state.anchor02.x + 0, state.anchor02.y + 0, 256, 496 }, "Das GUI");
		GuiSlider((Rectangle) { state.anchor02.x + 112, state.anchor02.y + 40, 120, 16 }, "MassMin", NULL, & state.MassMinValue, 0, 100);
		GuiSlider((Rectangle) { state.anchor02.x + 112, state.anchor02.y + 72, 120, 16 }, "MassMax", NULL, & state.MassMaxValue, 0, 100);
		GuiSlider((Rectangle) { state.anchor02.x + 112, state.anchor02.y + 104, 120, 16 }, "GravityScale", NULL, & state.GravityScaleValue, 0, 10);
		GuiSlider((Rectangle) { state.anchor02.x + 112, state.anchor02.y + 204, 120, 16 }, "Damping", NULL, & state.Damping, 0, 10);
		if (GuiDropdownBox((Rectangle) { state.anchor02.x + 112, state.anchor02.y + 152, 120, 24 }, "Static; Kinematic; Dynamic", & state.BodyTypeActive, state.BodyTypeEditMode)) state.BodyTypeEditMode = !state.BodyTypeEditMode;



	}
	DrawTextureEx(cursorTexture, (Vector2) {mousePosition.x - cursorTexture.width / 2 * cursorScale, (int)mousePosition.y - cursorTexture.height / 2 * cursorScale
	}, 0.0f, cursorScale, WHITE);	GuiUnlock();
}

Body* GetBodyIntersect(Body* bodies, Vector2 position)
{
	for (Body* body = bodies; body; body = body->next)
	{
		Vector2 screen = ConvertWorldToScreen(body->Position);
		if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass)))
		{
			return body;
		}
	}

	return NULL;
}

void DrawLineBodyToPosition(Body* body, Vector2 position)
{
	Vector2 screen = ConvertWorldToScreen(body->Position);
	DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}