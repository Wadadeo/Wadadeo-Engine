#version 330 core

in vec2 TexCoords;
layout (location = 0) out vec4 color;

uniform float gamma;
uniform float exposure;
uniform sampler2D screenTexture;

void main()
{ 
	vec3 hdrColor = texture(screenTexture, TexCoords).rgb;

    //vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);     // Exposure tone mapping
  
    color = vec4(hdrColor, 1.0);

}