#pragma once

#include <d3d11.h>
#include "Vertex.h"

class Mesh {
public:
	Mesh(Vertex* vertices, int vertex_count, unsigned int *indices, int index_count, ID3D11Device* device);
	Mesh(char* filename, ID3D11Device* device);
	~Mesh();
	Mesh* init(Vertex* vertices, int vertex_count, unsigned int *indices, int index_count, ID3D11Device* device);
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer** GetVertexBufferAddress();
	ID3D11Buffer* GetIndexBuffer();
	unsigned int GetIndexCount();
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int index_count;
};