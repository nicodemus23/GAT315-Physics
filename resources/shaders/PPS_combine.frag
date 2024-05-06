#version 330 core

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D originalTexture;
uniform sampler2D blurredTexture;
uniform float intensity;

void main()
{
    vec4 original = texture(originalTexture, fragTexCoord);
    vec4 bloom = texture(blurredTexture, fragTexCoord) * intensity;
    finalColor = original + bloom;
}
