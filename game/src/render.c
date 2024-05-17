#include "render.h" // Include the render header file
#include "raymath.h" // Include the raymath header file

// Define screen size and zoom level
Vector2 ncScreenSize = { 1280, 720 };
float ncScreenZoom = 1.0f;
float ncViewSize = 25.0f;

// Function to convert screen coordinates to world coordinates
Vector2 ConvertScreenToWorld(Vector2 screen)
{
    // Calculate aspect ratio
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate world extents
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Define lower and upper bounds of the world
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Convert screen coordinates to world coordinates
    float nx = screen.x / ncScreenSize.x;
    float ny = (ncScreenSize.y - screen.y) / ncScreenSize.y;

    Vector2 world;
    world.x = ((1 - nx) * lower.x) + (nx * upper.x);
    world.y = ((1 - ny) * lower.y) + (ny * upper.y);

    return world;
}

// Function to convert world coordinates to screen coordinates
Vector2 ConvertWorldToScreen(Vector2 world)
{
    // Calculate aspect ratio
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate world extents
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Define lower and upper bounds of the world
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Convert world coordinates to screen coordinates
    float nx = (world.x - lower.x) / (upper.x - lower.x);
    float ny = (world.y - lower.y) / (upper.y - lower.y);

    Vector2 screen;
    screen.x = nx * ncScreenSize.x;
    screen.y = (1.0f - ny) * ncScreenSize.y;

    return screen;
}

// Function to convert world coordinates to pixels
float ConvertWorldToPixel(float world)
{
    // Calculate aspect ratio
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate world extents
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom);

    // Convert world coordinate to pixel
    float nx = world / extents.x;
    float pixel = nx * ncScreenSize.x;

    return pixel;
}