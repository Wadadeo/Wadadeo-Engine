#include "GameObject.h"


GameObject::GameObject(const std::string &name) : 
	name(name)
{
	transform = new TransformComponent(this);
	_components.push_back(transform);

}

GameObject::GameObject(Model * model)
{
	name = model->name;
	transform = new TransformComponent(this);
	_components.push_back(transform);
	
	if (model->mesh) {

		addComponent<MeshComponent>()->mesh(model->mesh);
		addComponent<RendererComponent>()->setMaterial(model->mesh->material());
	}

	for (auto it = model->childs.begin(); it != model->childs.end(); it++)
	{
		GameObject *child = new GameObject(*it);
		child->transform->setParent(transform);
	}
}


GameObject::~GameObject()
{	
	cout << "Delete Game Object : " << name << endl;
	
	for (auto component : _components)
		delete component;
}


void GameObject::send(int message)
{
	for (auto it = _components.begin(); it != _components.end(); it++) {
		(*it)->receive(message);
	}
}

void GameObject::setRenderer(RendererComponent * renderer)
{
	_renderer = renderer;
}

void GameObject::draw()
{
	//Shader *outlineShader = ShaderCatalogue::getInstance().getShader("singleColor");
	std::vector<TransformComponent*> childs = transform->getChilds();


	//if (!selected) {
		//glStencilMask(0x00); // Make sure we don't update the stencil buffer while drawing the floor
//		if (_renderer) _renderer->draw();
	//	for (size_t i = 0; i < childs.size(); i++)
		//	childs[i]->gameObject->draw();
	//}
	/*else {
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		if (_renderer) _renderer->draw();
		for (auto i = 0; i < childs.size(); i++)
			childs[i]->gameObject->getComponent<RendererComponent>()->draw();

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		
		vec3 scale = transform->scale();
		transform->Scale(scale + 0.1f);
		if (_renderer) _renderer->draw(outlineShader);
		for (auto i = 0; i < childs.size(); i++)
			childs[i]->gameObject->getComponent<RendererComponent>()->draw(outlineShader);
		transform->Scale(scale);

		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
	}*/
}
