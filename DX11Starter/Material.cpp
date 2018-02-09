#include "Material.h"

Material::Material(SimpleVertexShader * vertexShader, SimplePixelShader * pixelShader)
{
	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}
