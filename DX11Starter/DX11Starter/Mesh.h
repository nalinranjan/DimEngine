#pragma once

#include <d3d11.h>
#include "Vertex.h"

class Mesh {

public:

	// Constructor/Destructor
	Mesh(Vertex* _vertices, int _vertexArrayLen, UINT* _indices, int _indexArrayLen, ID3D11Device* _device);
	Mesh(char* _objFilepath, ID3D11Device* _device);
	~Mesh();

	// Getters for vertex and index info
	ID3D11Buffer* GetVertexBuffer() { return vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer() { return indexBuffer; }
	int GetIndexCount() { return indexBufferLen; }

private:

	// Vertex and Index buffers for storing mesh data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int indexBufferLen;

	void CreateBuffers(Vertex* _vertices, int _vertexArrayLen, UINT* _indices, int _indexArrayLen, ID3D11Device* _device);

};