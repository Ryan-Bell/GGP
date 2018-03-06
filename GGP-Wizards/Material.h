#pragma once
#include "SimpleShader.h"

class Material {
public:
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* shaderResourceView, ID3D11SamplerState* samplerState);

	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
	ID3D11ShaderResourceView* shaderResourceView;
	ID3D11SamplerState* samplerState;

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
};