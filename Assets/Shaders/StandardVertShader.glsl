#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec3 vertexBitTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 NormalMatrix; 

out VS_OUT {
	vec3 fragPosition;
	vec3 fragNormal;
	vec2 TexCoord;
	mat3 TBN;
} vs_out;

void main(){
	gl_Position = projection * view * model * vec4(vertexPosition, 1);
	vs_out.fragPosition = vec3(model * vec4(vertexPosition, 1));
	vs_out.TexCoord = vertexUV;
	
	vec3 T = normalize(vec3(model * vec4(vertexTangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(vertexBitTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(vertexNormal,    0.0)));
	vs_out.TBN  = mat3(T, B, N);

	vs_out.fragNormal = NormalMatrix * vertexNormal;
}