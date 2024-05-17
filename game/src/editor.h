#pragma once
#include "raylib.h"

extern bool ncEditorActive;
extern bool ncEditorIntersect;

typedef struct jgEditorData {
    float MassMinValue;
    float MassMaxValue;
    float Slider003Value;
} jgEditorData_t;

typedef struct {
    Vector2 anchor01;
    Vector2 anchor02;

    bool WindowBox001Active;
    float MassMinValue;
    float MassMaxValue;
    float GravityScaleValue;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    float Damping;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiLayoutNameState;

extern jgEditorData_t jgEditorDataI;
extern GuiLayoutNameState state;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor(Vector2 mousePosition);

struct Body* GetBodyIntersect(struct Body* bodies, Vector2 position);
void DrawLineBodyToPosition(struct Body* body, Vector2 position);