#version 330 core

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 blurDirection; // Use (1, 0) for horizontal and (0, 1) for vertical

void main()
{
    float kernel[5] = float[](0.227027, 0.316216, 0.070270, 0.009243, 0.000000);
    vec4 color = texture(texture0, fragTexCoord) * kernel[0];

    for (int i = 1; i < 5; ++i)
    {
        color += texture(texture0, fragTexCoord + blurDirection * float(i) / 300.0) * kernel[i];
        color += texture(texture0, fragTexCoord - blurDirection * float(i) / 300.0) * kernel[i];
    }

    finalColor = color;
}
