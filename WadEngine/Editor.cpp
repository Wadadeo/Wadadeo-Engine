#include "Editor.h"
#include <iostream>

Editor::Editor()
{
}


Editor::~Editor()
{
	ImGui_ImplGlfwGL3_Shutdown();
}

bool Editor::init(GLFWwindow* window, bool *shouldExit, AssetCatalogue *assetCatalogue, Scene *scene)
{

	//selected = nullptr;
	displayWindows = true;
	showImportWindow = false;
	this->shouldExit = shouldExit;

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, false);


	_assetCatalogue = assetCatalogue;

	// Load Fonts
	// (see extra_fonts/README.txt for more details)
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	// Merge glyphs from multiple fonts into one (e.g. combine default font with another with Chinese glyphs, or add icons)
	//ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	//ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/fontawesome-webfont.ttf", 18.0f, &icons_config, icons_ranges);
	
	_inspector.init(assetCatalogue, scene);
	
	return true;
}

void Editor::update(AssetCatalogue* assets, GraphicsSystem *graphics, Scene *scene)
{
	ImGui_ImplGlfwGL3_NewFrame();

	displayMainMenu(scene);
	
	if (!displayWindows) return;

	//Debug Window
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	hierarchyPanel(scene);
	cameraPanel(graphics);
	bloomPanel(graphics->bloomEffect());
	importPanel(assets, scene);
	assetPanel(assets);


	//Inspector Window
	_inspector.draw();


}

void Editor::displayMainMenu(Scene *scene)
{
	// Menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{		
			if (ImGui::MenuItem("Display windows", NULL, displayWindows))
				displayWindows = !displayWindows;
			if (ImGui::MenuItem("Quit"))
				*shouldExit = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				GameObject *selected = scene->addGameobjectOfPrimitive(CUBE, "Cube");
				_inspector.display(SHOW_GAMEOBJECT, selected);
			}
			if (ImGui::MenuItem("Sphere"))
			{
				GameObject *selected = scene->addGameobjectOfPrimitive(SPHERE, "Sphere");
				_inspector.display(SHOW_GAMEOBJECT, selected);
			}
			if (ImGui::MenuItem("Torus"))
			{
				GameObject *selected = scene->addGameobjectOfPrimitive(TORUS, "Torus");
				_inspector.display(SHOW_GAMEOBJECT, selected);
			}
			if (ImGui::MenuItem("Plane"))
			{
				GameObject *selected = scene->addGameobjectOfPrimitive(QUAD, "Plane");
				_inspector.display(SHOW_GAMEOBJECT, selected);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Directional Light"))
			{
				GameObject *selected = scene->addLight(DIRECTIONAL_LIGHT, "Directional Light");
				_inspector.display(SHOW_GAMEOBJECT, selected);
			}
			if (ImGui::MenuItem("Point Light"))
			{
				GameObject *selected = scene->addLight(POINT_LIGHT, "Point Light");
				_inspector.display(SHOW_GAMEOBJECT, selected);
			}
			if (ImGui::MenuItem("Spot Light"))
			{
				GameObject *selected = scene->addLight(SPOT_LIGHT, "Spot Light");
				_inspector.display(SHOW_GAMEOBJECT, selected);
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Material")) 
			{
				static int matNumber = 0;
				Material* test = _assetCatalogue->addMaterial(string("Material") + to_string(++matNumber));
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Editor::hierarchyPanel(Scene *scene)
{
	const vector<TransformComponent*>& hierarchy = scene->hierarchy();
	//if (selected == nullptr && hierarchy.size() > 0) onSelection(hierarchy[0]);

	if (ImGui::CollapsingHeader("Hierarchy", NULL, true, true))
	{
		displayTransformNode(hierarchy);
	}
}

void Editor::displayTransformNode(const vector<TransformComponent*>& transforms)
{
	for (auto elt : transforms)
	{
		const vector<TransformComponent*>& childs = elt->getChilds();
		const char* name = elt->gameObject->name.c_str();
		if (childs.size() > 0)
		{
			if (ImGui::TreeNode(name))
			{			
				ImGui::SameLine();
				if (ImGui::SmallButton("i")) onSelection(elt);
				displayTransformNode(childs);
				ImGui::TreePop();
			}
		}
		else 
		{		
			if (ImGui::TreeNode(name))
			{		
				ImGui::SameLine();
				if (ImGui::SmallButton("i")) onSelection(elt);
				ImGui::TreePop();
			}
		}
	}
}

void Editor::onSelection(TransformComponent * tranform)
{
	_inspector.display(SHOW_GAMEOBJECT, tranform->gameObject);
}

void Editor::cameraPanel(GraphicsSystem * graphics)
{
	if (ImGui::CollapsingHeader("Camera Options"))
	{
		static int e = 0;
		ImGui::Text("Background :"); ImGui::SameLine();
		ImGui::RadioButton("Solid Color", &e, 0); ImGui::SameLine();
		ImGui::RadioButton("Skybox", &e, 1);

		SkyboxRenderer &renderer = graphics->skyboxRenderer();

		if (e == 0) {
			renderer.enabled = false;
			ImGui::ColorEdit3("color 1", graphics->getClearColor());
		}
		else {
			renderer.enabled = true;
			//ImGui::Text("Skybox :"); ImGui::SameLine();
			//ImGui::Text(renderer.cubeMap().name().c_str());
		}

		//EFFECT

		if (ImGui::Button("Effect..."))
			ImGui::OpenPopup("screen_effect");
		ImGui::SameLine();
		
		string effectName = graphics->effect()->name();
		string effectNames[4] = {
			"<None>",
			"hdr",
			"greyFilter",
			"sepiaFilter"
		};

		if (ImGui::BeginPopup("screen_effect"))
		{
			for (auto elt : effectNames)
				if (ImGui::Selectable(elt.c_str()))
					graphics->useEffect(elt == "<None>" ? "" : elt);
			ImGui::EndPopup();
		}
	
		if (effectName == "screen")
			effectName = "<None>";
		ImGui::Text(effectName.c_str());
		if (effectName == "hdr")
			ImGui::InputFloat("exposure", &GraphicsSystem::exposure);

	}
}

void Editor::bloomPanel(BloomEffect &bloom)
{
	if (ImGui::CollapsingHeader("Bloom Effect"))
	{
		int e = bloom.debug;

		ImGui::Text("Show :");
		ImGui::RadioButton("Complete", &e, COMPLETE);  ImGui::SameLine();
		ImGui::RadioButton("Scene", &e, SCENE);  ImGui::SameLine();
		ImGui::RadioButton("Bright area", &e, BRIGHT_ONLY);  ImGui::SameLine();
		ImGui::RadioButton("Blurred Bright", &e, BLURRED_BRIGHT);

		bloom.debug = (eBloomDebug)e;
		
		ImGui::InputFloat("exposure", &bloom.exposure);
	}
}

void Editor::importPanel(AssetCatalogue * assets, Scene * scene)
{
	if (ImGui::CollapsingHeader("Import Assets"))
	{
		//import model
		//ImGui::Text("Import Model"); ImGui::SameLine();
		const bool tmp = ImGui::Button("Model");
		if (tmp) importState = IMPORT_MODEL;
		const bool impTextButton = ImGui::Button("Texture");
		if (impTextButton) importState = IMPORT_TEXTURE;

		static ImGuiFs::Dialog dlg;                                                     // one per dialog (and must be static)
		const char* chosenPath = dlg.chooseFileDialog(tmp || impTextButton);             // see other dialog types and the full list of arguments for advanced usage
		if (strlen(chosenPath) > 0) {
			// A path (chosenPath) has been chosen RIGHT NOW. However we can retrieve it later more comfortably using: dlg.getChosenPath()
			cout << "import :" << chosenPath << endl;
			if (importState == IMPORT_MODEL)
			{
				Model* model = assets->addModel(chosenPath);
				if (model) scene->addGameobjectOfModel(model);
			}
			if (importState == IMPORT_TEXTURE)
				assets->addTexture(chosenPath);
		}

	}
}


void Editor::assetPanel(AssetCatalogue* assets)
{
	if (ImGui::CollapsingHeader("Assets"))
	{
		if (ImGui::TreeNode("Materials"))
		{
			for (auto material : assets->materials())
			{
				if (ImGui::Selectable(material->getName().c_str()))
					_inspector.display(SHOW_MATERIAL, material);
			}
			ImGui::TreePop();
		}
	}
}



