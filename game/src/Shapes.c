//#include "shapes.h"
//#include "render.h"
//
//
//
//
//
//void DrawStar(Vector2 position, float outerRadius, float innerRadius, int numPoints, Color baseColor, float randomTwinkleOffset, float scaleFactor) {
//	const float twicePi = 2.0f * PI; // Pre-calculate the value of 2 * PI for efficiency
//	Vector2 vertices[1000]; // Ensure this array is large enough for the number of points
//
//	//float scaledOuterRadius = outerRadius / scaleFactor;
//	//float scaledInnerRadius = innerRadius / scaleFactor;
//
//	Vector2 center = ConvertWorldToScreen(position); // Convert world position to screen position
//
//	float scaledOuterRadius = ConvertWorldToPixel(outerRadius) / scaleFactor;
//	float scaledInnerRadius = ConvertWorldToPixel(innerRadius) / scaleFactor;
//
//
//	// Twinkle variables
//	float time = GetTime();
//	float twinkleSpeed = 5.0f;
//	float twinkleFactor = (sinf(time * twinkleSpeed + randomTwinkleOffset) + 1.0f) / 2.0f; // Calculate twinkle factor
//
//	// Adjust the base color brightness and alpha based on the twinkle factor
//	Color twinkledColor = baseColor;
//	twinkledColor.r = (unsigned char)(twinkledColor.r * twinkleFactor);
//	twinkledColor.g = (unsigned char)(twinkledColor.g * twinkleFactor);
//	twinkledColor.b = (unsigned char)(twinkledColor.b * twinkleFactor);
//	twinkledColor.a = (unsigned char)(255 * (0.0f + twinkleFactor * 0.5f)); // Adjust alpha based on twinkle factor
//
//	// Define the vertices for the TriangleFan (star shape)
//	for (int i = 0; i < numPoints * 3; i++) {
//		//float radius = (i % 2 == 0) ? outerRadius : innerRadius;
//		float angle = i * twicePi / (numPoints * 2) + randomTwinkleOffset;
//		float radius;
//		if (i % 2 == 0) {
//			radius = scaledOuterRadius; // Outer point
//		}
//		else { 
//			radius = scaledInnerRadius; // Inner point
//			float pointFactor = 35.0f; 
//			radius = Lerp(scaledInnerRadius, scaledOuterRadius, pointFactor);
//		}
//		vertices[i].x = center.x + radius * sinf(angle);
//		vertices[i].y = center.y + radius * cosf(angle);
//	}
//
//	// Draw filled star with twinkled color
//	DrawTriangleFan(vertices, numPoints * 2, twinkledColor); 
//
//	// Apply a glowing effect at the center
//	float glowRadius = scaledInnerRadius * 35.8f;
//	Color glowColor = twinkledColor;
//	glowColor.a = (unsigned char)(twinkledColor.a * 0.5f); // Make the center glow slightly less opaque for a hot effect
//	DrawCircleGradient(center.x, center.y, glowRadius, glowColor, BLANK); // BLANK for transparent outside
//}
//
//Color GetStarColor(int temperature)
//{
//	Color color = WHITE;
//
//	if (temperature > 10000) color = BLUE;
//	else if (temperature > 7500) color = SKYBLUE;
//	else if (temperature > 6000) color = WHITE;
//	else if (temperature > 5000) color = YELLOW;
//	else if (temperature > 3500) color = ORANGE;
//	else color = RED;
//
//	//if (temperature > 10000) color = (Color){ 0, 0, 255, 255 };   // BLUE
//	//else if (temperature > 7500) color = (Color){ 135, 206, 235, 255 }; // SKYBLUE
//	//else if (temperature > 6000) color = WHITE;
//	//else if (temperature > 5000) color = (Color){ 255, 255, 0, 255 };   // YELLOW
//	//else if (temperature > 3500) color = (Color){ 255, 165, 0, 255 };   // ORANGE
//	//else color = (Color){ 255, 0, 0, 255 };   // RED
//
//	// Adjust saturation and brightness
//	float saturation = 0.4f;
//	float brightness = 2.0f;
//
//	Vector3 hsv = ColorToHSV(color);
//	hsv.y *= saturation;
//	hsv.z *= brightness;
//
//	return ColorFromHSV(hsv.x, hsv.y, hsv.z);
//}
//
//// Function to apply a dimming factor to a color
//Color ApplyDimming(Color color, float factor) {
//	return (Color) {
//		(unsigned char)(color.r * factor),
//			(unsigned char)(color.g * factor),
//			(unsigned char)(color.b * factor),
//			color.a  // Keep alpha unchanged or adjust as necessary
//	};
//}
//
//void DrawTradStar(Vector2 center, float outerRadius, float innerRadius, int numPoints, Color color, float randomTwinkleOffset) {
//	float twicePi = 2.0f * PI;
//	Vector2 vertices[100]; 
//
//	for (int i = 0; i < numPoints * 2; i++) {
//		float radius = (i % 2 == 0) ? outerRadius : innerRadius;
//		float angle = i * twicePi / (numPoints * 2) + randomTwinkleOffset;
//
//		vertices[i].x = center.x + radius * sinf(angle);
//		vertices[i].y = center.y + radius * cosf(angle);
//	}
//
//	for (int i = 0; i < numPoints * 2; i++) {
//		DrawLine(vertices[i].x, vertices[i].y, vertices[(i + 1) % (numPoints * 2)].x, vertices[(i + 1) % (numPoints * 2)].y, color);
//	}
//}
//
//
//
//void DrawWeirdStar(Vector2 position, float outerRadius, float innerRadius, int numPoints, Color color, float randomTwinkleOffset)
//{
//	float twinkleSpeed = 3.0f;
//	float twinkleAmplitude = 0.4f;
//	float timeOffset = ((float)GetTime() + randomTwinkleOffset) * twinkleSpeed;
//
//	// Draw the star shape
//	Vector2 points[16]; // Maximum number of points for a star
//	int count = 0;
//	for (int i = 0; i < numPoints; i++)
//	{
//		float angle = ((float)i / numPoints) * PI * 2;
//		points[count++] = (Vector2){ position.x + cosf(angle) * outerRadius, position.y + sinf(angle) * outerRadius }; // Outer point 
//		angle += PI / numPoints; // Move to the inner point
//		points[count++] = (Vector2){ position.x + cosf(angle) * innerRadius, position.y + sinf(angle) * innerRadius }; // Inner point
//
//	}
//	DrawTriangleFan(points, numPoints, color);
//
//	// Apply twinkling effect to the center
//	float twinkleFactor = (sinf(timeOffset) + 1.0f);
//	twinkleFactor = Lerp(1.0f, twinkleAmplitude, twinkleFactor);
//	Color brightColor = ColorAlpha(BLACK, twinkleFactor);
//	DrawCircleGradient(position.x, position.y, innerRadius * 1.8f, brightColor, ColorAlpha(color, 0.0f)); // Draw a gradient circle at the center of the star
//}
//
//
//
//void DrawStarRound(Vector2 position, float outerRadius, float innerRadius, int numPoints, Color color, float randomTwinkleOffset)
//{
//	float twinkleSpeed = 5.0f;
//	float twinkleAmplitude = 0.5f;
//	float timeOffset = ((float)GetTime() + randomTwinkleOffset) * twinkleSpeed;
//
//	// Draw the star shape
//	Vector2 points[8]; // Maximum number of points for a star
//	for (int i = 0; i < numPoints; i++)
//	{
//		float angle = (float)i / numPoints * PI * 2;
//		float radius = (i % 2 == 0) ? outerRadius : innerRadius;
//		points[i] = (Vector2){ position.x + cosf(angle) * radius, position.y + sinf(angle) * radius };
//	}
//	DrawTriangleFan(points, numPoints, color);
//
//	// Apply twinkling effect to the center
//	float twinkleFactor = (sinf(timeOffset) + 2.0f) * 0.5f; // Calculate twinkling factor based on time offset and sine wave function (0.0 to 1.0)
//	twinkleFactor = Lerp(1.0f, twinkleAmplitude, twinkleFactor);
//	Color brightColor = ColorAlpha(WHITE, twinkleFactor);
//	DrawCircleGradient(position.x, position.y, innerRadius * 0.8f, brightColor, ColorAlpha(color, 0.0f));
//}
//
//void ShapeWTF(Vector2 position, float outerRadius, float innerRadius, int numPoints, Color color)
//{
//	float twinkleSpeed = 2.0f;
//	float twinkleAmplitude = 0.5f;
//	float timeOffset = (float)GetTime() * twinkleSpeed;
//
//	for (int i = 0; i < numPoints * 2; i++)
//	{
//		float angle = (float)i / (numPoints * 2) * PI * 2; // Calculate the angle of the point
//		float radius = (i % 2) == 0 ? outerRadius : innerRadius; // Calculate the radius of the point
//		Vector2 p1 = { position.x + cosf(angle) * radius, position.y + sinf(angle) * radius }; // Calculate the position of the point
//		angle += PI / numPoints; // Move the angle to the next point
//		radius = (i % 2) == 0 ? outerRadius : innerRadius; // Calculate the radius of the next point
//		Vector2 p2 = { position.x + cosf(angle) * radius, position.y + sinf(angle) * radius }; // Calculate the position of the next point
//		DrawLineEx(p1, p2, 2, color); // Draw a line between the two points
//
//		// applying twinkle effect
//		float twinkleFactor = (sinf(timeOffset + angle) + 1.0f) * 0.5f;
//		twinkleFactor = Lerp(1.0f, twinkleAmplitude, twinkleFactor);
//
//		// adjust brightness based on twinkle factor
//		Color brightColor = ColorAlpha(color, color.a * twinkleFactor);
//
//		// draw star point with gradient
//		Vector2 gradientStart = Vector2Lerp(position, p1, 0.2f);
//		Vector2 gradientEnd = Vector2Lerp(position, p1, 0.8f);
//		DrawLineBezier(gradientStart, gradientEnd, 2.0f, brightColor, color);
//	}
//}
