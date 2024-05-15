#pragma once
#include "raylib.h"

typedef struct ncEditorData
{
	bool EditorBoxActive;
	float MassMinValue;
	float MassMaxValue;
	float GravitationValue;
	bool BodyTypeEditMode;
	int BodyTypeActive;
	float GravityScaleValue;
	int BodyType;
	float BodyGravityScale;
	float BodyDamping;
	float DampingValue; // not sure if this is needed (was in Maple's code)
	Vector2 anchor01;

} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
//void UpdateEditor(Vector2 position);
void UpdateEditor(Vector2 mousePosition);
void DrawEditor(Vector2 position);

struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position); 
void DrawLineBodyToPosition(struct ncBody* body, Vector2 position);