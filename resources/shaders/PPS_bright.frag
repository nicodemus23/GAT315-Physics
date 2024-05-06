#version 330 core

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float threshold;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    float brightness = dot(texelColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    finalColor = brightness > threshold ? texelColor : vec4(0.0);
}
