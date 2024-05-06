#version 330

in vec2 fragTexCoord;
uniform sampler2D texture0;
uniform vec2 blurDirection;
out vec4 fragColor;

const float weights[5] = float[](0.227027, 0.316216, 0.070270, 0.008264, 0.000000);

void main()
{
    vec2 texOffset = blurDirection / textureSize(texture0, 0);
    fragColor = texture(texture0, fragTexCoord) * weights[0];

    for (int i = 1; i < 5; ++i)
    {
        fragColor += texture(texture0, fragTexCoord + texOffset * float(i)) * weights[i];
        fragColor += texture(texture0, fragTexCoord - texOffset * float(i)) * weights[i];
    }
}
