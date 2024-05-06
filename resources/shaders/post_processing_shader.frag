#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform float screenWidth;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    finalColor = vec4(1.0 - texelColor.rgb, texelColor.a);
}