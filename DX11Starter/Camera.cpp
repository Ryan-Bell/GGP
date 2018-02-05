#include "Camera.h"

#pragma region Construct/Destruct
Camera::Camera(float width, float height)
{
	//Default values for camera positioning
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0, 0, -5, 0);
	DirectX::XMVECTOR dir = DirectX::XMVectorSet(0, 0, 1, 0);

	init(width, height, pos, dir);
}

Camera::Camera(float width, float height, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir)
{
	init(width, height, DirectX::XMLoadFloat3(&pos), DirectX::XMLoadFloat3(&dir));
}

Camera::Camera(float width, float height, DirectX::XMVECTOR pos, DirectX::XMVECTOR dir)
{
	init(width, height, pos, dir);
}
void Camera::init(float width, float height, DirectX::XMVECTOR pos, DirectX::XMVECTOR dir) {
	up = DirectX::XMVectorSet(0, 1, 0, 0);
	CreateView(pos, dir);
	CreateProjection(width, height);
}
Camera::~Camera()
{
}
#pragma endregion

#pragma region MatrixCreations
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

void Camera::CreateView(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir)
{
	CreateView(DirectX::XMLoadFloat3(&pos), DirectX::XMLoadFloat3(&dir));
}

void Camera::CreateView(DirectX::XMVECTOR pos, DirectX::XMVECTOR dir) {
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
#pragma endregion

#pragma region Setters
Camera * Camera::SetPosition(DirectX::XMFLOAT3 pos)
{
	outdatedMatrix = true;
	cameraPosition = pos;
	return this;
}

Camera * Camera::SetDirection(DirectX::XMFLOAT3 dir)
{
	outdatedMatrix = true;
	cameraDirection = dir;
	return this;
}

Camera * Camera::SetXRotation(float xRotation)
{
	outdatedMatrix = true;
	this->xRotation = xRotation;
	return this;
}

Camera * Camera::SetYRotation(float yRotation)
{
	outdatedMatrix = true;
	this->yRotation = yRotation;
	return this;
}
#pragma endregion

#pragma region Getters
DirectX::XMFLOAT3 Camera::GetPosition()
{
	return cameraPosition;
}
DirectX::XMFLOAT3 Camera::GetDirection()
{
	return cameraDirection;
}
float Camera::GetXRotation()
{
	return xRotation;
}
float Camera::GetYRotation()
{
	return yRotation;
}
#pragma endregion

Camera * Camera::Update()
{
	if (outdatedMatrix) {
		CreateView(cameraPosition, cameraDirection);
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
