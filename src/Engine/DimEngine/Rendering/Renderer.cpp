#include "Renderer.h"
#include "RenderingEngine.h"

DimEngine::Renderer::Renderer()
{
	this->mesh = nullptr;
	this->material = nullptr;
	this->renderable = null_index;
}

DimEngine::Renderer::Renderer(Material* material, Mesh* mesh)
{
	this->mesh = mesh;
	this->material = material;
	this->renderable = null_index;

	RenderingEngine::GetSingleton()->AddRenderer(this);
}

DimEngine::Renderer::~Renderer()
{
	if (renderable != null_index)
		RenderingEngine::GetSingleton()->DestroyRenderable(renderable);

	RenderingEngine::GetSingleton()->RemoveRenderer(this);
}
