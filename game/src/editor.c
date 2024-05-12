#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
Rectangle editorRect;
ncEditorData_t ncEditorData;


Vector2 anchor01 = { 950, 64 };

bool EditorBoxActive = true;
float MassMinValue = 0.0f;
float MassMaxValue = 0.0f;
float GravitationValue = 0.0f;

void InitEditor()
{
	GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

	ncEditorData.GravitationValue = 2.0f;
	ncEditorData.MassMinValue = 0.1f;
	ncEditorData.MassMaxValue = 1.0f;

	editorRect = (Rectangle){ anchor01.x + 0, anchor01.y + 0, 296, 576 };
}

void UpdateEditor(Vector2 position)
{
	//toggle show/hide editor box with key press
	if (IsKeyPressed(KEY_TAB)) EditorBoxActive = !EditorBoxActive;

	// check if cursor position is intersecting the editor box
	ncEditorIntersect = EditorBoxActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor()
{
	if (EditorBoxActive)
	{
		EditorBoxActive = !GuiWindowBox((Rectangle) { anchor01.x + 0, anchor01.y + 0, 296, 576 }, "Editor");
		GuiSliderBar((Rectangle) { anchor01.x + 104, anchor01.y + 64, 120, 16 }, "Mass Min", NULL, & ncEditorData.MassMinValue, 0, 10);
		GuiSliderBar((Rectangle) { anchor01.x + 104, anchor01.y + 112, 120, 16 }, "Mass Max", NULL, & ncEditorData.MassMaxValue, 0, 10);
		GuiSliderBar((Rectangle) { anchor01.x + 104, anchor01.y + 160, 120, 16 }, "Gravitation", NULL, & ncEditorData.GravitationValue, 0, 100);
	}
}
