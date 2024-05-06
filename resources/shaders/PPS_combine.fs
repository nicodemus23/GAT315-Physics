#version 330

in vec2 fragTexCoord;
uniform sampler2D originalTexture;
uniform sampler2D blurredTexture;
uniform float intensity;
out vec4 fragColor;

void main()
{
    vec4 scene = texture(originalTexture, fragTexCoord);
    vec4 bloom = texture(blurredTexture, fragTexCoord) * intensity;
    fragColor = scene + bloom; // Combine scene and bloom
}
