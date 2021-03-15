#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Vertex.h"
#include <memory>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Mesh
{
private:
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	int indexBufferLength = 0;

	void Init(Vertex* vertices, int vertexCount, unsigned int* indices, int indexCount, ComPtr<ID3D11Device> device);

public:
	ComPtr<ID3D11Buffer> GetVertexBuffer();
	ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();

	//ctor taking mesh data and d3d device
	Mesh(Vertex* vertices, int vertexCount, 
		 unsigned int* indices, int indexCount, 
		 ComPtr<ID3D11Device> device);

	Mesh(const char* filename, ComPtr<ID3D11Device> device);

};

