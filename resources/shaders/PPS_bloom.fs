#version 330

//in vec2 fragTexCoord;
//in vec4 fragColor;
//
//out vec4 finalColor;
//
//uniform sampler2D texture0;
//uniform float screenWidth;
//uniform float screenHeight;
//uniform float threshold;
//uniform float intensity;
//
//void main()
//{
//    vec4 texelColor = texture(texture0, fragTexCoord);
//    vec4 brightColor = vec4(0.0);
//
//    // Extract bright areas
//    float brightness = dot(texelColor.rgb, vec3(0.2126, 0.7152, 0.0722));
//    if (brightness > threshold)
//    {
//        brightColor = texelColor;
//    }
//
//    // Apply Gaussian blur
//    vec2 texelSize = 1.0 / vec2(screenWidth, screenHeight);
//    vec4 bloomColor = vec4(0.0);
//    float weightSum = 0.0;
//    for (int i = -4; i <= 4; i++)
//    {
//        for (int j = -4; j <= 4; j++)
//        {
//            float weight = exp(-float(i * i + j * j) / 8.0);  // Gaussian function
//            bloomColor += texture(texture0, fragTexCoord + vec2(i, j) * texelSize) * weight;
//            weightSum += weight;
//        }
//    }
//    bloomColor /= weightSum;
//
//    // Combine original, bright, and blurred colors
//    finalColor = texelColor + brightColor * intensity + bloomColor * intensity * 0.5;
//}

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

const vec2 size = vec2(1280, 720);   // Framebuffer size
const float samples = 4.0;          // Pixels per axis; higher = bigger glow, worse performance
const float quality = 0.2;          // Defines size factor: Lower = smaller glow, better quality

void main()
{
    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1) / size * quality;

    // Texel color fetching from texture sampler
    vec4 source = texture(texture0, fragTexCoord);

    const int range = 2;            // should be = (samples - 1)/2;

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            sum += texture(texture0, fragTexCoord + vec2(x, y) * sizeFactor);
        }
    }

    // Calculate final fragment color
    finalColor = ((sum / (samples * samples)) + source) * colDiffuse;
}
