#pragma once
#include "raylib.h"

extern bool ncEditorActive;
extern bool ncEditorIntersect;

typedef struct ncEditorData {
    float MassMinValue;
    float MassMaxValue;
    float Slider003Value;
} ncEditorData_t;

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

extern ncEditorData_t jgEditorDataI;
extern GuiLayoutNameState state;

void InitEditor();
void UpdateEditor(Vector2 mouseposition);
void DrawEditor(Vector2 mouseposition);

struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position);
void DrawLineBodyToposition(struct ncBody* body, Vector2 position);