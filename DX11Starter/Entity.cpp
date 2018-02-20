#include "Entity.h"

using namespace DirectX;

Entity::Entity(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	this->material = material;
	position = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	rotation = XMFLOAT3(0, 0, 0);
	CalculateWorldMatrix();
}

Entity::~Entity() {}

XMFLOAT3 Entity::GetPosition()
{
	return position;
}

XMFLOAT3 Entity::GetScale()
{
	return scale;
}

XMFLOAT3 Entity::GetRotation()
{
	return rotation;
}

Entity * Entity::SetPosition(XMFLOAT3 position)
{
	outdatedMatrix = true;
	this->position = position;
	return this;
}

Entity * Entity::SetScale(XMFLOAT3 scale)
{
	outdatedMatrix = true;
	this->scale = scale;
	return this;
}

Entity * Entity::SetRotation(XMFLOAT3 rotation)
{
	outdatedMatrix = true;
	this->rotation = rotation;
	return this;
}

Entity * Entity::SetPosition(float x, float y, float z)
{
	return SetPosition(XMFLOAT3(x, y, z));
}

Entity * Entity::SetScale(float x, float y, float z)
{
	return SetScale(XMFLOAT3(x, y, z));
}

Entity * Entity::SetRotation(float x, float y, float z)
{
	return SetRotation(XMFLOAT3(x, y, z));
}

Entity * Entity::Move(XMFLOAT3 movement)
{
	//TODO should this also be delayed until world mat calc?
	XMStoreFloat3(&position, XMVectorAdd(
		XMLoadFloat3(&position), 
		XMLoadFloat3(&movement)
	));
	return this;
}

Entity * Entity::Move(float x, float y, float z)
{
	return Move(XMFLOAT3(x, y, z));
}

Entity * Entity::MoveForward(float distance)
{
	//TODO
	return this;
}

Entity * Entity::PrepareMaterial(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{


	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	SimpleVertexShader* vertexShader = material->GetVertexShader();
	SimplePixelShader* pixelShader = material->GetPixelShader();
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

XMFLOAT4X4 Entity::GetWorldMatrix()
{
	if (outdatedMatrix) {
		CalculateWorldMatrix();
		outdatedMatrix = false;
	}
	return worldMatrix;
}

void Entity::CalculateWorldMatrix()
{
	XMMATRIX tr = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX ro = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX sc = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(sc * ro * tr));
}