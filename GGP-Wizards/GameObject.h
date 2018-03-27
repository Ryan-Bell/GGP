#pragma once
#include "DXCore.h"
#include "Object.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"
//TODO ? should there be an option to force sets to calculate immediately
class GameObject : public Object
{
public:
	GameObject(Mesh* mesh, Material* material);
	~GameObject();

	// Getters and Setters
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT3 GetRotation();

	// returning GameObject* (this) allows chaining methods ( obj->do1()->do2() )
	GameObject* SetPosition(DirectX::XMFLOAT3 position);
	GameObject* SetScale(DirectX::XMFLOAT3 scale);
	GameObject* SetRotation(DirectX::XMFLOAT3 rotation);
	GameObject* SetPosition(float x, float y, float z);
	GameObject* SetScale(float x, float y, float z);
	GameObject* SetRotation(float x, float y, float z);

	GameObject* Move(DirectX::XMFLOAT3 movement);
	GameObject* Move(float x, float y, float z);
	GameObject* MoveForward(float distance);

	GameObject* PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

	Mesh* mesh = nullptr;
	Material* material = nullptr;
	DirectX::XMFLOAT4X4 GetWorldMatrix();
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	//TODO Quaternions?
	DirectX::XMFLOAT3 rotation;
	
	bool outdatedMatrix;
	DirectX::XMFLOAT4X4 worldMatrix;
	void CalculateWorldMatrix();
};