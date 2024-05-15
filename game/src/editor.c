#include "editor.h"
#include "body.h"
#include "render.h"

#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
Rectangle editorRect;
ncEditorData_t ncEditorData;

Vector2 anchor01 = { 950, 64 };
Texture2D cursorTexture;

bool EditorBoxActive = true;
float MassMinValue = 0.1f;
float MassMaxValue = 1.0f;
float GravitationValue = 0.0f;
float cursorScale = 0.5f;

void InitEditor()
{
	GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

	Image image = LoadImage("resources/greetings.png");

	cursorTexture = LoadTextureFromImage(image);
	UnloadImage(image);
	HideCursor();

	ncEditorData.anchor01 = (Vector2) { 950, 64 };
	ncEditorData.EditorBoxActive = true;
	ncEditorData.MassMinValue = 0.1f;
	ncEditorData.MassMaxValue = 1.0f;
	//cEditorData.GravitationValue = 2.0f;
	//ncEditorData.BodyTypeEditMode = false;
	//ncEditorData.BodyTypeActive = 0;
	//ncEditorData.GravityScaleValue = 0.0f;
	ncEditorData.BodyType = 0;
	ncEditorData.BodyGravityScale = 1.0f;
	ncEditorData.BodyDamping = 0.2f;
	//ncEditorData.DampingValue = 0.2f;

	editorRect = (Rectangle){ anchor01.x + 0, anchor01.y + 0, 296, 576 };
}

void UpdateEditor(Vector2 position)
{
	//toggle show/hide editor box with key press
	if (IsKeyPressed(KEY_TAB)) EditorBoxActive = !EditorBoxActive;

	// check if cursor position is intersecting the editor box
	ncEditorIntersect = EditorBoxActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor(Vector2 position)
{
	if (ncEditorData.BodyTypeEditMode) GuiLock();

	if (ncEditorData.EditorBoxActive)
	{
		ncEditorData.EditorBoxActive = !GuiWindowBox((Rectangle) { anchor01.x + 0, anchor01.y + 0, 296, 576 }, "Editor");
		GuiSliderBar((Rectangle) { anchor01.x + 104, anchor01.y + 64, 120, 16 }, "Mass Min", NULL, & ncEditorData.MassMinValue, 0, 10);
		GuiSliderBar((Rectangle) { anchor01.x + 104, anchor01.y + 112, 120, 16 }, "Mass Max", NULL, & ncEditorData.MassMaxValue, 0, 10);
		GuiSliderBar((Rectangle) { anchor01.x + 104, anchor01.y + 160, 120, 16 }, "Gravitation", NULL, & ncEditorData.GravitationValue, 0, 100);

	
		// Body Type Dropdown
		const char* bodyTypes[] = { "Dynamic", "Static", "Kinematic" };
		ncEditorData.BodyType = GuiComboBox((Rectangle) { anchor01.x + 104, anchor01.y + 208, 120, 20 }, "Body Type", bodyTypes, 3, ncEditorData.BodyType);

		// Body Gravity Scale Slider
		GuiSliderBar((Rectangle) { anchor01.x + 104, anchor01.y + 256, 120, 16 }, "Body Gravity Scale", NULL, & ncEditorData.BodyGravityScale, 0, 5);

		// Body Damping Slider
		GuiSliderBar((Rectangle) { anchor01.x + 104, anchor01.y + 304, 120, 16 }, "Body Damping", NULL, & ncEditorData.BodyDamping, 0, 1);
		//if (GuiButton((Rectangle) { anchor01.x + 104, anchor01.y + 352, 120, 20 }, GuiIconText(RICON_FILE_SAVE, "Save"))) SaveWorld("world.txt");
	}
	DrawTextureEx(cursorTexture, (Vector2) { position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2 }, 0.0f, cursorScale, WHITE);

	if (ncEditorData.BodyTypeEditMode) GuiUnlock();
	GuiUnlock();
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
	for (ncBody* body = bodies; body; body = body->next)
	{
		Vector2 screen = ConvertWorldToScreen(body->position);
		if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass)))
		{
			return body;
		}
	}

	return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
	Vector2 screen = ConvertWorldToScreen(body->position);
	DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}
