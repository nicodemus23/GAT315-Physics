#pragma once
#include "raylib.h"

typedef struct ncEditorData
{
	float MassMinValue;
	float MassMaxValue;
	float GravitationValue;

} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor();