#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// Custom vertex definition
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};