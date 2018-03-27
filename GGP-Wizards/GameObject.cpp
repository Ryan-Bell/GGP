#include "GameObject.h"

using namespace DirectX;

GameObject::GameObject(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	this->material = material;
	position = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	rotation = XMFLOAT3(0, 0, 0);
	CalculateWorldMatrix();
}

GameObject::~GameObject() {}

XMFLOAT3 GameObject::GetPosition()
{
	return position;
}

XMFLOAT3 GameObject::GetScale()
{
	return scale;
}

XMFLOAT3 GameObject::GetRotation()
{
	return rotation;
}

GameObject * GameObject::SetPosition(XMFLOAT3 position)
{
	outdatedMatrix = true;
	this->position = position;
	return this;
}

GameObject * GameObject::SetScale(XMFLOAT3 scale)
{
	outdatedMatrix = true;
	this->scale = scale;
	return this;
}

GameObject * GameObject::SetRotation(XMFLOAT3 rotation)
{
	outdatedMatrix = true;
	this->rotation = rotation;
	return this;
}

GameObject * GameObject::SetPosition(float x, float y, float z)
{
	return SetPosition(XMFLOAT3(x, y, z));
}

GameObject * GameObject::SetScale(float x, float y, float z)
{
	return SetScale(XMFLOAT3(x, y, z));
}

GameObject * GameObject::SetRotation(float x, float y, float z)
{
	return SetRotation(XMFLOAT3(x, y, z));
}

GameObject * GameObject::Move(XMFLOAT3 movement)
{
	//TODO should this also be delayed until world mat calc?
	XMStoreFloat3(&position, XMVectorAdd(
		XMLoadFloat3(&position), 
		XMLoadFloat3(&movement)
	));
	return this;
}

GameObject * GameObject::Move(float x, float y, float z)
{
	return Move(XMFLOAT3(x, y, z));
}

GameObject * GameObject::MoveForward(float distance)
{
	//TODO
	return this;
}

GameObject * GameObject::PrepareMaterial(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	SimpleVertexShader* vertexShader = material->GetVertexShader();
	SimplePixelShader*  pixelShader  = material->GetPixelShader();
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	vertexShader->SetMatrix4x4("world", GetWorldMatrix());
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();

	pixelShader->SetSamplerState("basicSampler", material->samplerState);
	pixelShader->SetShaderResourceView("diffuseTexture", material->shaderResourceView);

	//TODO is this needed for the ss and srv?
	pixelShader->CopyAllBufferData();

	return this;
}

XMFLOAT4X4 GameObject::GetWorldMatrix()
{
	if (outdatedMatrix) {
		CalculateWorldMatrix();
		outdatedMatrix = false;
	}
	return worldMatrix;
}

void GameObject::CalculateWorldMatrix()
{
	XMMATRIX tr = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX ro = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX sc = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(sc * ro * tr));
}