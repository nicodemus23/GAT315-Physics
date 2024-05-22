// lens_flare.fs
#version 330 core

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 resolution;
uniform vec2 lightpositions[10]; // Array of light positions
uniform int numLights; // Number of active lights

const int NUM_SAMPLES = 24;
const float DISPERSION = 1.0f;
const float INTENSITY = 3.5;

void main()
{
    vec2 texCoord = fragTexCoord;
    vec4 color = texture(texture0, texCoord);

    for (int light = 0; light < numLights; ++light)
    {
        vec2 delta = (texCoord - lightpositions[light]) * DISPERSION / resolution;
        for (int i = 1; i <= NUM_SAMPLES; ++i)
        {
            color += texture(texture0, texCoord + delta * float(i));
        }
    }

    color /= float(NUM_SAMPLES * numLights + 1);
    color *= INTENSITY;

    finalColor = vec4(color.rgb, 1.0);
}
