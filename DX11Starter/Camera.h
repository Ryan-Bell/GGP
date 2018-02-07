#pragma once
#include "DXCore.h"
#include <DirectXMath.h>

class Camera {
public:
	// Contruct/Destruct
	Camera(float width, float height);
	Camera(float width, float height, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	Camera(float width, float height, DirectX::XMVECTOR pos, DirectX::XMVECTOR dir);
	~Camera();
	void init(float width, float height, DirectX::XMVECTOR pos, DirectX::XMVECTOR dir);

	// Getters/Setters
	Camera* SetPosition(DirectX::XMFLOAT3 pos);
	Camera* SetDirection(DirectX::XMFLOAT3 dir);
	Camera* SetXRotation(float xRotation);
	Camera* SetYRotation(float yRotation);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetDirection();
	float GetXRotation();
	float GetYRotation();

	Camera* Update(float deltaTime, float totalTime);
	Camera* OnMouseMove(float prex, float prey, float x, float y);
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	float cameraSpeed;
	float cameraRotationSpeed;

	void CreateProjection(float width, float height);
private:
	// Matrix Creations
	
	void CreateView(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	void CreateView(DirectX::XMVECTOR pos, DirectX::XMVECTOR dir);

	// Internal Data
	DirectX::XMFLOAT3 cameraPosition;
	DirectX::XMFLOAT3 cameraDirection;
	float xRotation;
	float yRotation;
	float width;
	float height;
	bool outdatedMatrix;

	// The matrices to go from model space to screen space
	//DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
}; 