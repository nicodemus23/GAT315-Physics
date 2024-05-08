#include "render.h"
#include "raymath.h"

Vector2 ncScreenSize = { 1280, 720 };
float ncScreenZoom = 1.0f;
float ncViewSize = 25.0f;

Vector2 ConvertScreenToWorld(Vector2 screen)
{
	float ratio = ncScreenSize.x / ncScreenSize.y;
	Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
	extents = Vector2Scale(extents, ncScreenZoom);

	Vector2 lower = Vector2Negate(extents);
	Vector2 upper = extents;

	float nx = screen.x / ncScreenSize.x;
	float ny = (ncScreenSize.y - screen.y) / ncScreenSize.y;

	Vector2 world;
	world.x = ((1 - nx) * lower.x) + (nx * upper.x);
	world.y = ((1 - ny) * lower.y) + (ny * upper.y);

	return world;
}

Vector2 ConvertWorldToScreen(Vector2 world)
{
	float ratio = ncScreenSize.x / ncScreenSize.y;
	Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
	extents = Vector2Scale(extents, ncScreenZoom);

	Vector2 lower = Vector2Negate(extents);
	Vector2 upper = extents;

	float nx = (world.x - lower.x) / (upper.x - lower.x);
	float ny = (world.y - lower.y) / (upper.y - lower.y);

	Vector2 screen;
	screen.x = nx * ncScreenSize.x;
	screen.y = (1.0f - ny) * ncScreenSize.y;

	return screen;
}

float ConvertWorldToPixel(float world)
{
	float ratio = ncScreenSize.x / ncScreenSize.y;
	Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
	extents = Vector2Scale(extents, ncScreenZoom);

	float nx = world / extents.x;
	float pixel = nx * ncScreenSize.x;

	return pixel;
}
