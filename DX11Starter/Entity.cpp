#include "Entity.h"

Entity::Entity(Mesh* mesh)
{
	this->mesh = mesh;
	position = DirectX::XMFLOAT3(0, 0, 0);
	scale = DirectX::XMFLOAT3(1, 1, 1);
	rotation = DirectX::XMFLOAT3(0, 0, 0);
	CalculateWorldMatrix();
}

Entity::~Entity() {}

DirectX::XMFLOAT3 Entity::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Entity::GetScale()
{
	return scale;
}

DirectX::XMFLOAT3 Entity::GetRotation()
{
	return rotation;
}

Entity * Entity::SetPosition(DirectX::XMFLOAT3 position)
{
	outdatedMatrix = true;
	this->position = position;
	return this;
}

Entity * Entity::SetScale(DirectX::XMFLOAT3 scale)
{
	outdatedMatrix = true;
	this->scale = scale;
	return this;
}

Entity * Entity::SetRotation(DirectX::XMFLOAT3 rotation)
{
	outdatedMatrix = true;
	this->rotation = rotation;
	return this;
}

Entity * Entity::SetPosition(float x, float y, float z)
{
	return SetPosition(DirectX::XMFLOAT3(x, y, z));
}

Entity * Entity::SetScale(float x, float y, float z)
{
	return SetScale(DirectX::XMFLOAT3(x, y, z));
}

Entity * Entity::SetRotation(float x, float y, float z)
{
	return SetRotation(DirectX::XMFLOAT3(x, y, z));
}

Entity * Entity::Move(DirectX::XMFLOAT3 movement)
{
	//TODO should this also be delayed until world mat calc?
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(
		DirectX::XMLoadFloat3(&position), 
		DirectX::XMLoadFloat3(&movement)
	));
	return this;
}

Entity * Entity::Move(float x, float y, float z)
{
	return Move(DirectX::XMFLOAT3(x, y, z));
}

Entity * Entity::MoveForward(float distance)
{
	//TODO
	return this;
}

DirectX::XMFLOAT4X4 Entity::GetWorldMatrix()
{
	if (outdatedMatrix) {
		CalculateWorldMatrix();
		outdatedMatrix = false;
	}
	return worldMatrix;
}

void Entity::CalculateWorldMatrix()
{
	DirectX::XMMATRIX tr = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX ro = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	DirectX::XMMATRIX sc = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(sc * ro * tr));
}