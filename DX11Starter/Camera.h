#pragma once
#include "DXCore.h"
#include <DirectXMath.h>

class Camera {
public:
	Camera(float width, float height);
	~Camera();
	void SetSize(float width, float height);
	Camera* Update();
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
private:
	DirectX::XMFLOAT3 cameraPosition;
	DirectX::XMFLOAT3 cameraDirection;
	float xRotation;
	float yRotation;

	// The matrices to go from model space to screen space
	//DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
};
