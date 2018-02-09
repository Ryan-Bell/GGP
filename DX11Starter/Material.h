#pragma once
#include "SimpleShader.h"

class Material {
public:
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader);

	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
};