#pragma once
#include "raylib.h"

typedef struct ncEditorData
{
	float MassMinValue;
	float MassMaxValue;
	float GravitationValue;
	int BodyType;
	float BodyGravityScale;
	float BodyDamping;

} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor();