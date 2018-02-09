#pragma once
#include "DXCore.h"
#include <DirectXMath.h>

class Camera {
public:
	// Contruct/Destruct
	Camera(int width, int height);
	Camera(int width, int height, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	Camera(int width, int height, DirectX::XMVECTOR pos, DirectX::XMVECTOR dir);
	~Camera();
	void init(int width, int height, DirectX::XMVECTOR pos, DirectX::XMVECTOR dir);

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
	Camera* OnMouseMove(int prex, int prey, int x, int y);
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	float cameraSpeed;
	float cameraRotationSpeed;

	void CreateProjection(int width, int height);
private:
	// Matrix Creations
	
	void CreateView(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	void CreateView(DirectX::XMVECTOR pos, DirectX::XMVECTOR dir);

	// Internal Data
	DirectX::XMFLOAT3 cameraPosition;
	DirectX::XMFLOAT3 cameraDirection;
	float xRotation;
	float yRotation;
	int width;
	int height;
	bool outdatedMatrix;

	// The matrices to go from model space to screen space
	//DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
}; 