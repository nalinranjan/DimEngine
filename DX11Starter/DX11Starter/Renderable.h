#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

class Renderable {

public:

	Renderable(Mesh* _mesh, Material* _material);
	Renderable();
	~Renderable();

	void PrepareMaterial(DirectX::XMFLOAT4X4 _view, DirectX::XMFLOAT4X4 _proj);
	Transform* GetTransform();
	Mesh* GetMesh();

private:

	Transform* transform;
	Mesh* mesh;
	Material* material;

};