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
	DirectX::XMStoreFloat3(&cameraDirection, dir);
	DirectX::XMStoreFloat3(&cameraPosition, pos);

	CreateView(pos, dir);
	CreateProjection(width, height);
	cameraSpeed = 3;
	cameraRotationSpeed = 0.002f;
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
	this->width = width;
	this->height = height;
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
		DirectX::XMVectorSet(0, 1, 0, 0));     // "Up" direction in 3D space (prevents roll)
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

Camera * Camera::Update(float deltaTime, float totalTime)
{
	DirectX::XMVECTOR cameraDirectionVector = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMQuaternionRotationRollPitchYaw(-cameraDirection.y, cameraDirection.x, 0));

	//DirectX::XMVECTOR cameraDirectionVector = DirectX::XMLoadFloat3(&cameraDirection);
	cameraDirectionVector = DirectX::XMVector4Normalize(cameraDirectionVector);
	DirectX::XMVECTOR cameraDirectionLeftVector = DirectX::XMVector3Cross(cameraDirectionVector, DirectX::XMVectorSet(0, 1, 0, 0));

	float scale = deltaTime * cameraSpeed;
	cameraDirectionVector = DirectX::XMVectorScale(cameraDirectionVector, scale);
	cameraDirectionLeftVector = DirectX::XMVectorScale(cameraDirectionLeftVector, scale);

	DirectX::XMVECTOR cameraPositionVector = DirectX::XMLoadFloat3(&cameraPosition);

	//Movement forward and back along direction vector
	if (GetAsyncKeyState('W') & 0x8000) {
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorAdd(cameraPositionVector, cameraDirectionVector));
		outdatedMatrix = true;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorSubtract(cameraPositionVector, cameraDirectionVector));
		outdatedMatrix = true;
	}

	//Movement left and right based on direction vector
	if (GetAsyncKeyState('A') & 0x8000) {
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorAdd(cameraPositionVector, cameraDirectionLeftVector));
		outdatedMatrix = true;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		DirectX::XMStoreFloat3(&cameraPosition, DirectX::XMVectorSubtract(cameraPositionVector, cameraDirectionLeftVector));
		outdatedMatrix = true;
	}
	//Movement up and down in world coords
	if (GetAsyncKeyState('Q') & 0x8000) {
		cameraPosition.y -= deltaTime * cameraSpeed;
		outdatedMatrix = true;
	}
	if (GetAsyncKeyState('E') & 0x8000) {
		cameraPosition.y += deltaTime * cameraSpeed;
		outdatedMatrix = true;
	}
	
	if (outdatedMatrix) {
		CreateView(cameraPosition, cameraDirection);
	}
	return this;
}

Camera* Camera::OnMouseMove(float prex, float prey, float x, float y) {
	float screenCenterX = width / 2;
	float screenCenterY = height / 2;
	
	float deltax = x - prex;
	float deltay = y - prey;
	deltax *= cameraRotationSpeed;
	deltay *= cameraRotationSpeed;
	cameraDirection.x += deltax;
	cameraDirection.y -= deltay;
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
