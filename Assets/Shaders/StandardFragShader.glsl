#version 330

#define ENABLE_NORMAL_MAPPING 1
#define MAX_LIGHT_TYPE 12

struct Material {
	vec3		ambiant;
    vec3		diffuse;
    vec3		specular;
    float		shininess;
	sampler2D	diffuseMap1;
	sampler2D	diffuseMap2;
	sampler2D	specularMap1;
	sampler2D	normalMap;
	sampler2D	glowMap;
	bool		hasNormalMap;
	vec2		tiling;
}; 

struct PointLight {
	vec3	position;
	vec3	intensity;
	float	constant;
    float	linear;
    float	quadratic;
};

struct DirectionalLight {
	vec3	direction;
	vec3	intensity;
};

struct SpotLight {
    vec3	position;
	vec3	intensity;
    vec3	direction;
    float	cutOff;
	float	outerCutOff;
};

uniform bool				hasSkybox;
uniform samplerCube			skybox;

uniform Material			material;
uniform vec3				ambientIntensity;
uniform vec3				viewPos;
uniform int					pointLightCout;
uniform int					directionalLightCout;
uniform int					spotLightCout;
uniform DirectionalLight	directionalLights[MAX_LIGHT_TYPE];
uniform PointLight			pointLights[MAX_LIGHT_TYPE];
uniform SpotLight			spotLights[MAX_LIGHT_TYPE];

in VS_OUT {
	vec3 fragPosition;
	vec3 fragNormal;
	vec2 TexCoord;
	mat3 TBN;
} fs_in;

layout( location = 0 ) out vec4 color;
layout (location = 1) out vec4 BrightColor;


vec3 calcDiffuse(vec3 normal, vec3 lightDirection, vec3 lightIntensity, vec3 diffuseColor)
{
	float diff = max(dot(normal, lightDirection), 0.0);
	return (lightIntensity * diff * diffuseColor);
}

vec3 calcSpecular(vec3 viewDirection, vec3 normal, vec3 lightDirection, vec3 lightIntensity, vec3 specularColor)
{
	vec3 reflectDir = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
	return (lightIntensity * spec * specularColor);
} 



vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
	vec3 lightDirection = normalize(light.position - fs_in.fragPosition);

	vec3 diffuse =	calcDiffuse(normal, lightDirection, light.intensity, diffuseColor);
	vec3 specular = calcSpecular(viewDir, normal, lightDirection, light.intensity, specularColor);
	
	// Attenuation over distance from light source
	float distance = length(light.position - fs_in.fragPosition);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	diffuse  *= attenuation;
	specular *= attenuation;
	
	return (diffuse + specular); 
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor)
{
	// =======> WRONG somewhere, needs to be fixed <=======
	
	vec3 lightDirection = normalize(light.position - fs_in.fragPosition); //direction form light to fragment
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  
	   
	vec3 diffuse =	calcDiffuse(normal, lightDirection, light.intensity, diffuseColor);
	vec3 specular = calcSpecular(viewDir, normal, lightDirection, light.intensity, specularColor);

	diffuse  *= intensity;
	specular *= intensity;

	return (diffuse + specular); 
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularColor) {
	vec3 lightDirection = normalize(-light.direction);

	vec3 diffuse =	calcDiffuse(normal, lightDirection, light.intensity, diffuseColor);
	vec3 specular = calcSpecular(viewDir, normal, lightDirection, light.intensity, specularColor);

	return (diffuse + specular); 
}


vec4 processBrightness(vec3 fragColor, vec4 glowColor)
{
	float tmp = dot(glowColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (tmp >= 0.5)
		return vec4((glowColor.rgb) * 2 + fragColor, 1.0);
	// check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(fragColor, vec3(0.2126, 0.7152, 0.0722));
    if (brightness >= 1.0)
       return vec4(fragColor, 1.0);
	else
		return vec4(0, 0, 0, 1.0);
}

void main() {
	
	// Get color diffuse & specular maps
	vec4 mainTexColor = texture(material.diffuseMap1, fs_in.TexCoord * material.tiling);
	vec4 secTexColor = texture(material.diffuseMap2, fs_in.TexCoord * material.tiling);

	vec3 diffuseColor = mix(mainTexColor, secTexColor, secTexColor.a).rgb * material.diffuse;
	vec3 specularColor = texture(material.specularMap1, fs_in.TexCoord * material.tiling).rgb * material.specular;


	//TODO : use uniform bool to detect if normal mapping is available

	vec3 normal = fs_in.fragNormal; // default normal
	if (material.hasNormalMap) // if enabled, get normal from normal map
	{ 
		normal = texture(material.normalMap, fs_in.TexCoord * material.tiling).rgb;
		normal = normalize(normal * 2.0 - 1.0);
		normal = normalize(fs_in.TBN * normal); // transform normal from tangent space to world space
	}


	vec3 viewDir = normalize(viewPos - fs_in.fragPosition);

	/*if (hasSkybox)
	{
		vec3 InvViewDir = normalize(fs_in.fragPosition - viewPos);
		vec3 R = reflect(InvViewDir, normalize(normal));
		color = texture(skybox, R);
	}*/

	//calc ambiant lighting
	vec3 result = ambientIntensity * diffuseColor;

	// Directional lights
	for(int i = 0; i < directionalLightCout; i++)
		result += CalcDirectionalLight(directionalLights[i], normal, viewDir, diffuseColor, specularColor);
	
	// Point lights
	for(int i = 0; i < pointLightCout; i++)
		result += CalcPointLight(pointLights[i], normal, viewDir, diffuseColor, specularColor);

	// Spot lights
	for(int i = 0; i < spotLightCout; i++)
		result += CalcSpotLight(spotLights[i], normal, viewDir, diffuseColor, specularColor);

	color = vec4(result, 1.0);
	vec4 glowColor = texture(material.glowMap, fs_in.TexCoord * material.tiling);
	BrightColor = processBrightness(color.rgb, glowColor);
	color += glowColor;
}
