#pragma once
#include "DXCore.h"
#include <DirectXMath.h>

class Camera {
public:
	// Contruct/Destruct
	Camera(int width, int height);
	Camera(int width, int height, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction);
	~Camera();
	void init(int width, int height, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction);

	// Getters/Setters
	Camera* SetPosition(DirectX::XMFLOAT3 position);
	Camera* SetDirection(DirectX::XMFLOAT3 direction);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetDirection();
	
	Camera* Update(float deltaTime, float totalTime);
	Camera* OnMouseMove(int prex, int prey, int x, int y);
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	float cameraSpeed;
	float cameraRotationSpeed;
	float rotationx;
	float rotationy;

	void CreateProjection(int width, int height);
private:
	void CreateView(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);

	// Internal Data
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
	bool outdatedMatrix;

	// The matrices to go from model space to screen space
	//DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
}; 