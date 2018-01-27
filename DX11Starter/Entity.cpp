#include "Entity.h"

Entity::Entity(Mesh* mesh)
{
	this->mesh = mesh;
}

Entity::~Entity()
{
}

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
	outdatedMatrix = true;
	this->position = DirectX::XMFLOAT3(x, y, z);
	return this;
}

Entity * Entity::SetScale(float x, float y, float z)
{
	outdatedMatrix = true;
	this->scale = DirectX::XMFLOAT3(x, y, z);
	return this;
}

Entity * Entity::SetRotation(float x, float y, float z)
{
	outdatedMatrix = true;
	this->rotation = DirectX::XMFLOAT3(x, y, z);
	return this;
}

Entity * Entity::Move(DirectX::XMFLOAT3 movement)
{
	//TODO
	return this;
}

Entity * Entity::Move(float x, float y, float z)
{
	//TODO
	return this;
}

Entity * Entity::MoveForward(float distance)
{
	//TODO
	return this;
}

DirectX::XMFLOAT4X4 Entity::GetWorldMatrix()
{
	if (outdatedMatrix) {
		DirectX::XMMATRIX tr = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		DirectX::XMMATRIX ro = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		DirectX::XMMATRIX sc = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

		XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(sc * ro * tr));
		outdatedMatrix = false;
	}
	return worldMatrix;
}