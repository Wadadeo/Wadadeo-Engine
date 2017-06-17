#include "LightSystem.h"
#include "LightComponent.h"

LightSystem* LightComponent::system = nullptr;

LightSystem::LightSystem()
{
	_ambiantBaseColor = vec3(1, 1, 1);
	_ambiantIntensity = 0.1f; //must be between 0 and 1
	LightComponent::system = this;
}

LightSystem::LightSystem(const LightSystem & other)
{
	_lights = other._lights;
	_ambiantBaseColor = other._ambiantBaseColor;
	_ambiantIntensity = other._ambiantIntensity;
	_debugLightShader = other._debugLightShader;
	_drawDebugLight = other._drawDebugLight;
}


LightSystem::~LightSystem()
{
	_lights.clear();
}


void LightSystem::init(Shader * debugLightShader)
{
	_debugLightShader = debugLightShader;
	_drawDebugLight = true;
}

void LightSystem::update(const Camera& camera)
{
	if (!_drawDebugLight) return;

	for (auto light : _lights)
		light->drawDebugMesh(camera, _debugLightShader);
}

void LightSystem::addLight(LightComponent * light)
{
	_lights.push_back(light);
}

void LightSystem::remove(LightComponent * light)
{
	for (auto it = _lights.begin(); it != _lights.end(); ++it)
	{
		if (*it == light)
		{
			if (_lights.size() >= 2)
			{
				std::swap(*it, _lights.back());
				_lights.pop_back();
			}
			else
			{
				_lights.clear();
			}
			break;
		}
	}
}

void LightSystem::loadLightsInShader(Shader * shader) const
{
	LightIndex index;
	for (auto light : _lights) 
	{
		light->sendLightToShader(shader, &index);
	}
	shader->uniform("pointLightCout", index.pointIndex);
	shader->uniform("directionalLightCout", index.directionalIndex);
	shader->uniform("spotLightCout", index.spotIndex);

	shader->uniform("ambientIntensity", ambiantColor());

}

void LightSystem::setAmbiantColor(const vec3 &color) { _ambiantBaseColor = color; }
void LightSystem::setAmbiantIntensity(const float & intensity) { _ambiantIntensity = intensity; }


const vector<LightComponent*>& LightSystem::getLights() const { return _lights; }
const vec3& LightSystem::ambiantBaseColor() const { return _ambiantBaseColor; }
const float& LightSystem::ambiantIntensity() const { return _ambiantIntensity; }

vec3 LightSystem::ambiantColor() const { return _ambiantBaseColor * _ambiantIntensity; }
