#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform float screenWidth;
uniform float screenHeight;
uniform float threshold;
uniform float intensity;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec4 brightColor = vec4(0.0);

    // Extract bright areas
    float brightness = dot(texelColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > threshold)
    {
        brightColor = texelColor;
    }

    // Apply Gaussian blur
    vec2 texelSize = 1.0 / vec2(screenWidth, screenHeight);
    vec4 bloomColor = vec4(0.0);
    float weightSum = 0.0;
    for (int i = -4; i <= 4; i++)
    {
        for (int j = -4; j <= 4; j++)
        {
            float weight = exp(-float(i * i + j * j) / 8.0);  // Gaussian function
            bloomColor += texture(texture0, fragTexCoord + vec2(i, j) * texelSize) * weight;
            weightSum += weight;
        }
    }
    bloomColor /= weightSum;

    // Combine original, bright, and blurred colors
    finalColor = texelColor + brightColor * intensity + bloomColor * intensity * 0.5;
}
