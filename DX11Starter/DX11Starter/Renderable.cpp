#include "Renderable.h"

Renderable::Renderable(Mesh* _mesh, Material* _material) {
	mesh = _mesh;
	material = _material;
	transform = new Transform();
}

Renderable::Renderable() {

}

Renderable::~Renderable() {
	delete transform;
}

void Renderable::PrepareMaterial(DirectX::XMFLOAT4X4 _view, DirectX::XMFLOAT4X4 _proj) {

	SimpleVertexShader* vertexShader = material->GetVertexShader();
	SimplePixelShader* pixelShader = material->GetPixelShader();

	vertexShader->SetMatrix4x4("view", _view);
	vertexShader->SetMatrix4x4("projection", _proj);

	vertexShader->SetMatrix4x4("world", transform->GetWorldMatrix());

	pixelShader->SetSamplerState("basicSampler", material->GetSamplerState());
	pixelShader->SetShaderResourceView("diffuseTexture", material->GetShaderResourceView());

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();
	pixelShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();

}

Transform* Renderable::GetTransform() {
	return transform;
}

Mesh* Renderable::GetMesh() {
	return mesh;
}