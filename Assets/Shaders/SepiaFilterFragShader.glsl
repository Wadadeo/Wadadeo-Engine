#version 330 core

in vec2 TexCoords;
layout (location = 0) out vec4 color;

uniform sampler2D screenTexture;

void main()
{ 
    color = texture(screenTexture, TexCoords);

	//SEPIA
	vec4 sepia = color;
    sepia.r = (color.r * 0.393) + (color.g * 0.769) + (color.b * 0.189);
    sepia.g = (color.r * 0.349) + (color.g * 0.686) + (color.b * 0.168);    
    sepia.b = (color.r * 0.272) + (color.g * 0.534) + (color.b * 0.131);
	color = sepia;
	
}