#version 330

in vec2 fragTexCoord;
uniform sampler2D texture0;
uniform float threshold;
out vec4 fragColor;

void main()
{
    vec4 color = texture(texture0, fragTexCoord);
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722)); // Calculate brightness
    fragColor = color * max(brightness - threshold, 0.0);
}
//The shader is pretty simple.It takes a texture as input and calculates the brightness of each pixel in the texture.If the brightness is greater than the threshold, the pixel is kept; otherwise, it is discarded.