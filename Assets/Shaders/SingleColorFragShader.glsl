#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 TexCoord;

uniform vec3 color;

layout( location = 0 ) out vec4 outColor;
layout (location = 1) out vec4 BrightColor;

void main() {
    outColor = vec4(color, 1.0);
	
	float brightness = dot(outColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness >= 1.0)
        BrightColor = vec4(outColor.rgb, 1.0);
	else
		BrightColor = vec4(0,0,0,1.0);
}

