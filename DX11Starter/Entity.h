#pragma once
#include "DXCore.h"
#include <DirectXMath.h>
#include "Mesh.h"
//TODO ? should there be an option to force sets to calculate immediately
class Entity
{
public:
	Entity(Mesh* mesh);
	~Entity();

	// Getters and Setters
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT3 GetRotation();

	Entity* SetPosition(DirectX::XMFLOAT3 position);
	Entity* SetScale(DirectX::XMFLOAT3 scale);
	Entity* SetRotation(DirectX::XMFLOAT3 rotation);
	Entity* SetPosition(float x, float y, float z);
	Entity* SetScale(float x, float y, float z);
	Entity* SetRotation(float x, float y, float z);
	//TODO: ? component-wise setting of rotation?

	Entity* Move(DirectX::XMFLOAT3 movement);
	Entity* Move(float x, float y, float z);
	Entity* MoveForward(float distance);

	Mesh* mesh = nullptr;
	DirectX::XMFLOAT4X4 GetWorldMatrix();
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	//TODO Quaternions
	DirectX::XMFLOAT3 rotation;
	
	bool outdatedMatrix;
	DirectX::XMFLOAT4X4 worldMatrix;
};