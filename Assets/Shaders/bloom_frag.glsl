#version 330 core

in vec2 TexCoords;
layout (location = 0) out vec4 color;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;

void main()
{
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
	
	hdrColor += bloomColor; // additive blending

    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);     // tone mapping

    color = vec4(result, 1.0);
}  