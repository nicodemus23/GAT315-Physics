#version 330

in vec2 fragTexCoord;

uniform sampler2D originalTexture;
uniform sampler2D blurredTexture;
uniform sampler2D tracerTexture;
uniform float intensity;

out vec4 fragColor;

void main()
{
    vec4 scene = texture(originalTexture, fragTexCoord);
    vec4 bloom = texture(blurredTexture, fragTexCoord) * intensity;
    vec4 tracer = texture(tracerTexture, fragTexCoord);
    float tracerAlpha = tracer.a;
    vec4 combinedColor = scene + bloom;
    fragColor = combinedColor + tracer * tracerAlpha;
}


//uniform sampler2D originalTexture;
//uniform sampler2D blurredTexture;
//uniform sampler2D tracerTexture;
//uniform float intensity;
//
//in vec2 fragTexCoord;
//out vec4 finalColor;
//
//void main()
//{
//    vec4 originalColor = texture(originalTexture, fragTexCoord);
//    vec4 blurredColor = texture(blurredTexture, fragTexCoord);
//    vec4 tracerColor = texture(tracerTexture, fragTexCoord);
//
//    vec4 combinedColor = originalColor + blurredColor * intensity;
//    finalColor = mix(combinedColor, tracerColor, tracerColor.a);
//}

//The shader is pretty simple.It takes the original texture, the blurred texture, and the tracer texture as input.It then combines the original and blurred textures and mixes in the tracer texture.The intensity of the blur is controlled by the intensity uniform.
//The shader is then used in the  PostProcessingStack  class.
// Path: src/PostProcessingStack.cpp