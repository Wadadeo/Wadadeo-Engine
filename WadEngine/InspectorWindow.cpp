#include "InspectorWindow.h"



InspectorWindow::InspectorWindow()
{
}


InspectorWindow::~InspectorWindow()
{
}

void InspectorWindow::init(AssetCatalogue * assetCatalogue, Scene *scene)
{
	assets = assetCatalogue;
	this->scene = scene;
	this->data = nullptr;
}

void InspectorWindow::draw()
{
	ImGui::SetNextWindowSize(ImVec2(400, 300));
	if (!ImGui::Begin("Inspector"))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}


	switch (displayState)
	{
		case SHOW_GAMEOBJECT:
			drawGameObject((GameObject*)data);
			break;
		case SHOW_MATERIAL:
			drawMaterial((Material*)data);
			break;
	default:
		break;
	}

	ImGui::End();
}

void InspectorWindow::display(eInspectorState state, void * data)
{
	this->displayState = state;
	this->data = data;
}

void InspectorWindow::drawGameObject(GameObject* selected)
{
	if (!selected) return;

	//gameobject Header
	ImGui::Text("GameObject : ");
	ImGui::SameLine();
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.0f, 0.6f, 0.6f));
	ImGui::Button(selected->name.c_str());
	ImGui::PopStyleColor(1);
	ImGui::PopID();

	ImGui::SameLine(300);
	if (ImGui::Button("delete"))
	{
		scene->removeFromScene(selected);
		displayState = BLANK;
		return;
	}

	
	//transform component
	displayTransform(selected->transform);

	//mesh component
	MeshComponent *meshCpt = selected->getComponent<MeshComponent>();
	if (meshCpt) displayMesh(meshCpt);

	//renderer component
	RendererComponent *renderer = selected->getComponent<RendererComponent>();
	if (renderer) displayRenderer(renderer);

	LightComponent *lightCpt = selected->getComponent<LightComponent>();
	if (lightCpt) displayLight(lightCpt);
}

void InspectorWindow::displayTransform(TransformComponent * transform)
{
	ImGui::Separator();
	ImGui::Text("Transform");

	//position
	vec3 position = transform->position();
	float pos[3] = { position.x, position.y , position.z };
	if (ImGui::InputFloat3("Position", &pos[0]))
	{
		transform->Translate(vec3(pos[0], pos[1], pos[2]));
	}

	//rotation
	vec3 rotation = transform->rotation();
	float rot[3] = { rotation.x, rotation.y , rotation.z };
	if (ImGui::InputFloat3("Rotation", &rot[0]))
	{
		transform->Rotate(vec3(rot[0], rot[1], rot[2]));
	}

	//scale
	vec3 scale = transform->scale();
	float scl[3] = { scale.x, scale.y , scale.z };
	if (ImGui::InputFloat3("Scale", &scl[0]))
	{
		transform->Scale(vec3(scl[0], scl[1], scl[2]));
	}

}

void InspectorWindow::displayRenderer(RendererComponent * renderer)
{
	ImGui::Separator();
	ImGui::Text("Renderer");

	Material* mat = renderer->material();
	if (ImGui::Button("Material..."))
		ImGui::OpenPopup("material");
	ImGui::SameLine();
	ImGui::Text(mat->getName().c_str());
	if (ImGui::BeginPopup("material"))
	{
		for (auto elt : assets->materials())
			if (ImGui::Selectable(elt->getName().c_str()))
				renderer->setMaterial(elt);
		ImGui::EndPopup();
	}

	ImGui::Text("Shader : ");
	ImGui::SameLine();
	ImGui::Text(mat->getShader()->name().c_str());

}

void InspectorWindow::displayMesh(MeshComponent * meshCpt)
{
	ImGui::Separator();
	ImGui::Text("Mesh Data");

	Mesh* mesh = meshCpt->mesh();
	if (!mesh) return;

	ImGui::Text("Name :");
	ImGui::SameLine();
	ImGui::Text(mesh->name.c_str());
	ImGui::Text("%d vertices", mesh->vertices().size());
	ImGui::Text("%d indices", mesh->indices().size());

}

void InspectorWindow::displayLight(LightComponent * lightCpt)
{
	ImGui::Separator();
	ImGui::Text("LIGHT");

	vec3 color = lightCpt->getColor();
	if (ImGui::ColorEdit3("Color", glm::value_ptr(color)))
		lightCpt->setColor(color);
	float intensity = lightCpt->getIntensity();
	if (ImGui::InputFloat("Intensity", &intensity))
		lightCpt->setIntensity(intensity);


	switch (lightCpt->lightType())
	{
	case DIRECTIONAL_LIGHT:

		ImGui::Text("Type :");
		ImGui::SameLine();
		ImGui::Text("Directional");
		break;
	case POINT_LIGHT:

		ImGui::Text("Type :");
		ImGui::SameLine();
		ImGui::Text("Point");
		break;
	case SPOT_LIGHT:

		ImGui::Text("Type :");
		ImGui::SameLine();
		ImGui::Text("Spot");
		break;
	}
}

void InspectorWindow::drawMaterial(Material * material)
{
	//header
	ImGui::Text("Material : ");
	ImGui::SameLine();
	ImGui::Text(material->getName().c_str());
	ImGui::Separator();

	//shader used
	ImGui::Text("Shader :"); ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, BETWEEN_PROPERTIES_SPACING));
	ImGui::Text(material->getShader()->name().c_str());
	ImGui::PopStyleVar();

	//Diffuse (main) properties
	diffuseProp(material);

	//Normal properties
	normalProp(material);
	
	//Specular properties
	vec3 color;
	color = material->getAmbiant();
	if (ImGui::ColorEdit3("Ambiant", glm::value_ptr(color)))
		material->setAmbiant(color);

	color = material->getSpecular();
	if (ImGui::ColorEdit3("Specular", glm::value_ptr(color)))
		material->setSpecular(color);
	
	float shininess = material->getShininess();
	if (ImGui::InputFloat("Shininess", &shininess))
		material->setShinisess(shininess);
}

void InspectorWindow::diffuseProp(Material * material)
{
	//diffuseMap
	GLuint diffuseTextureID = material->mainDiffuseTexture();

	if (ImGui::Button("Diffuse (main)"))
		ImGui::OpenPopup("mainTex");
	ImGui::SameLine();

	if (ImGui::BeginPopup("mainTex"))
	{
		if (ImGui::Selectable("<None>"))
			material->setMainTexture(AssetCatalogue::whiteTexture);
		for (auto elt : assets->textures())
			if (ImGui::Selectable(elt->name.c_str()))
				material->setMainTexture(elt->id);
		ImGui::EndPopup();
	}
	if (diffuseTextureID == 0 || diffuseTextureID == AssetCatalogue::whiteTexture)
		ImGui::Text("<None>");
	else
		showTexturePreview(diffuseTextureID);

	//Tint
	vec3 diffuseTint = material->getDiffuse();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, BETWEEN_PROPERTIES_SPACING));
	if (ImGui::ColorEdit3("Diffuse Tint", glm::value_ptr(diffuseTint)))
		material->setDiffuse(diffuseTint);
	ImGui::PopStyleVar();
}

void InspectorWindow::normalProp(Material * material)
{
	GLuint diffuseTextureID = material->normalMap();
	

	if (ImGui::Button("Normal"))
		ImGui::OpenPopup("normalMap");
	ImGui::SameLine();

	if (ImGui::BeginPopup("normalMap"))
	{
		if (ImGui::Selectable("<None>"))
			material->setNormalTexture((GLuint)0);
		for (auto elt : assets->textures())
			if (ImGui::Selectable(elt->name.c_str()))
				material->setNormalTexture(elt->id);
		ImGui::EndPopup();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, BETWEEN_PROPERTIES_SPACING));
	if (diffuseTextureID == 0 || diffuseTextureID == AssetCatalogue::whiteTexture)
		ImGui::Text("<None>");
	else
		showTexturePreview(diffuseTextureID);
	ImGui::PopStyleVar();
}

void showTexturePreview(GLuint textureID)
{
	ImGui::Image((GLuint*)textureID, ImVec2(20, 20));
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Image((GLuint*)textureID, ImVec2(128, 128));
		ImGui::EndTooltip();
	}
}
