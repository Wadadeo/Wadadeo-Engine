#version 330 core

in vec2 TexCoords;
layout (location = 0) out vec4 color;

uniform sampler2D screenTexture;

void main()
{ 
    color = texture(screenTexture, TexCoords);

	// GREY FILTER

	//human eye in more sensible to green then red and least blue
	float greyTint = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b; 
    color = vec4(greyTint, greyTint, greyTint, 1.0);
}