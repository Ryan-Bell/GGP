#include "Camera.h"

Camera::Camera(float width, float height)
{
	//Default Values
	//TODO allow these to be passed in?
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0, 0, -5, 0);
	DirectX::XMVECTOR dir = DirectX::XMVectorSet(0, 0, 1, 0);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);

	CreateView(pos, dir, up);
	CreateProjection(width, height);
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Camera::CreateProjection(float width, float height)
{
	// Update our projection matrix since the window size changed
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void Camera::CreateView(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir, DirectX::XMFLOAT3 up)
{
	CreateView(DirectX::XMLoadFloat3(&pos), DirectX::XMLoadFloat3(&dir), DirectX::XMLoadFloat3(&up));
}

void Camera::CreateView(DirectX::XMVECTOR pos, DirectX::XMVECTOR dir, DirectX::XMVECTOR up) {
	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space

	DirectX::XMMATRIX V = DirectX::XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!
}

Camera::~Camera()
{
}

Camera * Camera::Update()
{
	if (outdatedMatrix) {
		//TODO math
	}
	return this;
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
