#include "../Core/GameObject.h"
#include "../Core/Global.h"

#include "RenderingEngine.h"
#include "SimpleShader.h"
DimEngine::Rendering::RenderingEngine* DimEngine::Rendering::RenderingEngine::singleton = nullptr;

DimEngine::Rendering::RenderingEngine* DimEngine::Rendering::RenderingEngine::GetSingleton()
{
	if (!singleton)
		Initialize();

	return singleton;
}

void DimEngine::Rendering::RenderingEngine::Initialize(i32 maxNumMaterials, i32 maxNumMeshes, i32 defaultNumRenderables, i32 defaultNumViews)
{
	singleton = new RenderingEngine(maxNumMeshes, maxNumMeshes, defaultNumRenderables, defaultNumViews);
}

void DimEngine::Rendering::RenderingEngine::Stop()
{
	delete singleton;
}

DimEngine::Rendering::RenderingEngine::RenderingEngine(i32 maxNumMaterials, i32 maxNumMeshes, i32 defaultNumRenderables, i32 defaultNumCameraProxies) : materialAllocator(maxNumMeshes), meshAllocator(maxNumMeshes), renderableAllocator(defaultNumRenderables), viewerAllocator(defaultNumCameraProxies)
{
	rendererList = nullptr;
	cameraList = nullptr;
	lightList = nullptr;
	shadow = nullptr;

	XMMATRIX shadowView = XMMatrixTranspose(XMMatrixLookToLH(XMVectorSet(0, 10, 0, 0), XMVectorSet(0, 1, -1, 0), XMVectorSet(0, 0, 1, 0)));
	XMMATRIX shadowProjection = XMMatrixTranspose(XMMatrixOrthographicLH(100, 100, 0.1f, 100));

	XMStoreFloat4x4(&shadowViewProjectionMat, XMMatrixMultiply(shadowProjection, shadowView));


	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	//depthStencilDesc.DepthEnable = true;
	//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
}

DimEngine::Rendering::RenderingEngine::~RenderingEngine()
{
	//delete vsZPrepass;
	singleton = nullptr;
}

void DimEngine::Rendering::RenderingEngine::AddRenderer(Renderer* renderer)
{
	renderer->next = rendererList;
	renderer->previous = nullptr;

	if (rendererList)
		rendererList->previous = renderer;
	
	rendererList = renderer;
}

void DimEngine::Rendering::RenderingEngine::AddCamera(Camera* camera)
{
	camera->next = cameraList;
	camera->previous = nullptr;

	if (cameraList)
		cameraList->previous = camera;
	
	cameraList = camera;
}

void DimEngine::Rendering::RenderingEngine::AddLight(Light* light)
{
	light->next = lightList;
	light->previous = nullptr;

	if (lightList)
		lightList->previous = light;
	
	lightList = light;
}

void DimEngine::Rendering::RenderingEngine::RemoveRenderer(Renderer* renderer)
{
	if (!rendererList)
		return;

	Renderer* next = renderer->next;
	Renderer* previous = renderer->previous;

	if (previous)
		previous->next = next;
	else
		rendererList = next;

	if (next)
		next->previous = previous;
}

void DimEngine::Rendering::RenderingEngine::RemoveCamera(Camera * camera)
{
	if (!rendererList)
		return;

	Camera* next = camera->next;
	Camera* previous = camera->previous;

	if (previous)
		previous->next = next;
	else
		cameraList = next;

	if (next)
		next->previous = previous;
}

void DimEngine::Rendering::RenderingEngine::RemoveLight(Light* light)
{
	if (!rendererList)
		return;

	Light* next = light->next;
	Light* previous = light->previous;

	if (previous)
		previous->next = next;
	else
		lightList = next;

	if (next)
		next->previous = previous;	
}

void DimEngine::Rendering::RenderingEngine::DestroyRenderable(i32 id)
{
	// renderableAllocator.Free(id);
}

void DimEngine::Rendering::RenderingEngine::DestroyViewer(i32 id)
{
	viewerAllocator.Free(id);
}

void DimEngine::Rendering::RenderingEngine::UpdateRenderables()
{
	renderableAllocator.Flush();

	for (Renderer* renderer = rendererList; renderer; renderer = renderer->next)
	{
		if (renderer->IsActive())
		{
			Renderable& renderable = renderableAllocator[renderableAllocator.Allocate()];

			renderable.worldMatrix = XMMatrixTranspose(renderer->GetGameObject()->GetWorldMatrix());
			renderable.material = renderer->material;
			renderable.mesh = renderer->mesh;
		}
	}
}

void DimEngine::Rendering::RenderingEngine::UpdateViewers()
{
	f32 screenRatio = DimEngine::Global::GetScreenRatio();

	for (Camera* camera = cameraList; camera; camera = camera->next)
	{
		if (camera->IsActive())
		{
			if (camera->viewer == null_index)
				camera->viewer = viewerAllocator.Allocate();

			Viewer& viewer = viewerAllocator[camera->viewer];

			GameObject* gameObject = camera->GetGameObject();

			XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(gameObject->GetRotation());

			viewer.position = gameObject->GetPosition();
			viewer.viewMatrix = XMMatrixTranspose(XMMatrixLookToLH(viewer.position, XMVector3Transform({ 0, 0, 1 }, rotationMatrix), { 0, 1, 0 }));
			viewer.projectionMatrix = XMMatrixTranspose(XMMatrixPerspectiveFovLH(camera->fov, camera->ratio == 0 ? screenRatio : camera->ratio, camera->nearZ, camera->farZ));
		}
		else
			RenderingEngine::GetSingleton()->DestroyViewer(camera->viewer);
	}
}

void DimEngine::Rendering::RenderingEngine::UpdateLightSources()
{
	lightSourceAllocator.Flush();

	for (Light* light = lightList; light; light = light->next)
	{
		if (light->IsActive())
		{
			LightSource& lightSource = lightSourceAllocator[lightSourceAllocator.Allocate()];
			lightSource.ambientColor = light->ambientColor;
			lightSource.diffuseColor = light->diffuseColor;
			lightSource.type = light->type;
			lightSource.range = 0;

			XMStoreFloat3(&lightSource.position, light->GetPosition());
			XMStoreFloat3(&lightSource.direction, static_cast<DirectionalLight*>(light)->GetDirection());
		}
	}
}

void DimEngine::Rendering::RenderingEngine::SortRenderables()
{
	renderableAllocator.Sort([](Renderable a, Renderable b)
	{
		uptr ma = reinterpret_cast<uptr>(a.material);
		uptr mb = reinterpret_cast<uptr>(b.material);

		return ma < mb;
	});
}

void DimEngine::Rendering::RenderingEngine::UpdateGlobalData(float screenWidth, float screenHeight)
{

}

void DimEngine::Rendering::RenderingEngine::PerformZPrepass(SimpleVertexShader* shader, ID3D11DeviceContext* context)
{
	context->PSSetShader(nullptr, nullptr, 0);

	Viewer& viewer = viewerAllocator[cameraList->viewer];

	XMMATRIX viewProjectionMatrix = XMMatrixMultiply(viewer.projectionMatrix, viewer.viewMatrix);

	i32 j = 0;

	while (j < renderableAllocator.GetNumAllocated())
	{
		Renderable& renderable = renderableAllocator[j];

		shader->SetMatrix4x4("viewProjection", viewProjectionMatrix);
		shader->SetMatrix4x4("world", renderable.worldMatrix);

		shader->CopyAllBufferData();
		shader->SetShader();

		Mesh* mesh = renderable.mesh;

		ID3D11Buffer* vertexBuffer = mesh->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = mesh->GetIndexBuffer();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		UINT indexCount = 0;

		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->DrawIndexed(mesh->GetIndexCount(), 0, 0);

		indexCount += mesh->GetIndexCount();

		++j;
	}
}

void DimEngine::Rendering::RenderingEngine::DrawForward(ID3D11DeviceContext* context)
{
	for (Camera* camera = cameraList; camera; camera = camera->next)
		if (!camera->renderTexture)
			DrawForward(context, camera);
}

void DimEngine::Rendering::RenderingEngine::DrawForward(ID3D11DeviceContext* context, Camera* camera)
{
	Viewer& viewer = viewerAllocator[camera->viewer];

	XMMATRIX viewMatrix = viewer.viewMatrix;
	XMMATRIX projectionMatrix = viewer.projectionMatrix;
	XMMATRIX viewProjectionMatrix = XMMatrixMultiply(projectionMatrix, viewMatrix);
	XMVECTOR cameraPosition = viewer.position;

	i32 j = 0;
	i32 J = renderableAllocator.GetNumAllocated();

	while (j < J)
	{
		Material* material = renderableAllocator[j].material;
		SimpleVertexShader* vertexShader = material->GetVertexShader();
		SimplePixelShader* pixelShader = material->GetPixelShader();
		//std::unordered_map<std::string, std::pair<const void*, unsigned int>> pixelShaderData = material->GetPixelShaderData();

		pixelShader->SetShader();

		pixelShader->SetData("light", lightSourceAllocator.GetMemoryAddress(), sizeof(LightSource));
		pixelShader->SetFloat4("cameraPosition", cameraPosition);

		 //Set up all data on pixel shader
		/*for (auto it = pixelShaderData.begin(); it != pixelShaderData.end(); ++it) {
			pixelShader->SetData(it->first, it->second.first, it->second.second);
		}*/

		if (material->getTexture())
		{
			pixelShader->SetShaderResourceView("TexAlbedo", material->getTexture());
			pixelShader->SetSamplerState("Sampler", material->getSampler());
			
		}

		if (material->HasNormalMap()) {
			pixelShader->SetShaderResourceView("normalMap", material->getNormalMap());
		}

		if (material->HasMetalnessMap()) {
			pixelShader->SetShaderResourceView("metalnessMap", material->getMetalnessMap());
		}

		if (material->HasRoughnessMap()) {
			pixelShader->SetShaderResourceView("roughnessMap", material->getRoughnessMap());
		}

		pixelShader->SetInt("hasNormalMap", material->HasNormalMap());

		bool isOK = pixelShader->SetShaderResourceView("ShadowMap", shadow->getShadowSRV());
		//if (!isOK) printf("shadowMap not imported.\n");
		isOK = pixelShader->SetSamplerState("shadowSampler", shadow->getSampler());
		//if (!isOK) printf("Shadow sampler not loaded.\n");

		pixelShader->CopyAllBufferData();
		
		u32 stride = sizeof(Vertex);
		u32 offset = 0;
		u32 indexCount = 0;

		do
		{
			Renderable& renderable = renderableAllocator[j];

			//std::unordered_map<std::string, std::pair<const void*, unsigned int>> vertexShaderData = material->GetVertexShaderData();
			vertexShader->SetShader();

			vertexShader->SetMatrix4x4("view", viewMatrix);
			vertexShader->SetMatrix4x4("projection", projectionMatrix);
			vertexShader->SetMatrix4x4("viewProjection", viewProjectionMatrix);
			vertexShader->SetMatrix4x4("world", renderable.worldMatrix);

			// Set up all data on vertex shader
			/*for (auto it = vertexShaderData.begin(); it != vertexShaderData.end(); ++it) {
				vertexShader->SetData(it->first, it->second.first, it->second.second);
			}*/

			vertexShader->CopyAllBufferData();

			Mesh* mesh = renderable.mesh;

			ID3D11Buffer* vertexBuffer = mesh->GetVertexBuffer();
			ID3D11Buffer* indexBuffer = mesh->GetIndexBuffer();

			context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			context->DrawIndexed(mesh->GetIndexCount(), 0, 0);

			indexCount += mesh->GetIndexCount();

			++j;
		} while (j < J && renderableAllocator[j].material == material);
	}
}

void DimEngine::Rendering::RenderingEngine::setShadow(ShadowMap * _shadow)
{
	shadow = _shadow;
}



bool DimEngine::Rendering::RenderingEngine::RenderShadowMap(ID3D11DeviceContext* deviceContext)
{
	deviceContext->OMSetRenderTargets(0, nullptr, shadow->getShadowDSV());
	deviceContext->ClearDepthStencilView(shadow->getShadowDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	deviceContext->RSSetState(shadow->getRasterizerState());

	SimpleVertexShader* shader = shadow->getShadowShader();
	//SimpleVertexShader* shader = shadow->getShadowShader();

	shader->SetShader();

	deviceContext->PSSetShader(0, 0, 0);

	i32 j = 0;

	while (j < renderableAllocator.GetNumAllocated())
	{
		Renderable& renderable = renderableAllocator[j];

		bool isOK = shader->SetMatrix4x4("viewProjection", shadowViewProjectionMat);
		//bool isOK = shader->SetMatrix4x4("shadowView", shadowViewProjectionMat);
		//if (!isOK) printf("shadow view matrix error\n");

		//isOK = shader->SetMatrix4x4("shadowProjection", shdaowProjectionMat);
		//if (!isOK) printf("shadow projection matrix error");

		isOK = shader->SetMatrix4x4("world", renderable.worldMatrix);
		//if (!isOK) printf("world matrix in shadow map error\n");

		shader->CopyAllBufferData();

		Mesh* mesh = renderable.mesh;

		ID3D11Buffer* vertexBuffer = mesh->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = mesh->GetIndexBuffer();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		UINT indexCount = 0;

		deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);

		indexCount += mesh->GetIndexCount();

		++j;
	}

	return true;
}

void DimEngine::Rendering::RenderingEngine::RenderCubeMap(ID3D11DeviceContext* deviceContext, CubeMap* cubeMap)
{
	ID3D11Buffer* cubeVB = cubeMap->getVB();
	ID3D11Buffer* cubeIB = cubeMap->getIB();

	u32 stride = sizeof(Vertex);
	u32 offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &cubeVB, &stride, &offset);
	deviceContext->IASetIndexBuffer(cubeIB, DXGI_FORMAT_R32_UINT, 0);

	Viewer& viewer = viewerAllocator[cameraList->viewer];

	XMMATRIX viewMatrix = viewer.viewMatrix;
	XMMATRIX projectionMatrix = viewer.projectionMatrix;

	SimpleVertexShader* cubeVS = cubeMap->getVS();
	SimplePixelShader* cubePS = cubeMap->getPS();

	cubeVS->SetMatrix4x4("view", viewMatrix);
	cubeVS->SetMatrix4x4("projection", projectionMatrix);
	cubeVS->CopyAllBufferData();
	cubeVS->SetShader();

	cubePS->SetShaderResourceView("cubeTexture", cubeMap->getSRV());
	cubePS->SetSamplerState("basicSampler", cubeMap->getSampler());
	cubePS->SetShader();

	deviceContext->RSSetState(cubeMap->getRS());
	deviceContext->OMSetDepthStencilState(cubeMap->getDSS(), 0);

	deviceContext->DrawIndexed(cubeMap->getIndexCount(), 0, 0);
}
