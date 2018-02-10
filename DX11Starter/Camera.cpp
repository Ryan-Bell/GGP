#include "Camera.h"

#pragma region Construct/Destruct
Camera::Camera(int width, int height)
{
	//Default values for camera positioning
	DirectX::XMFLOAT3 position  = DirectX::XMFLOAT3(0, 0, -5);
	DirectX::XMFLOAT3 direction = DirectX::XMFLOAT3(0, 0, +1);

	init(width, height, position, direction);
}

Camera::Camera(int width, int height, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction)
{
	init(width, height, position, direction);
}
void Camera::init(int width, int height, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction) {
	rotationx = 0;
	rotationy = 0;
	this->position = position;
	this->direction = direction;

	CreateView(position, direction);
	CreateProjection(width, height);
	cameraSpeed = 3;
	cameraRotationSpeed = 0.002f;
}
Camera::~Camera() {}
#pragma endregion

#pragma region MatrixCreations
// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Camera::CreateProjection(int width, int height)
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
	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space

	DirectX::XMMATRIX V = DirectX::XMMatrixLookToLH(
		DirectX::XMLoadFloat3(&pos),					// The position of the "camera"
		DirectX::XMLoadFloat3(&dir),					// Direction the camera is looking
		DirectX::XMVectorSet(0, 1, 0, 0));				// "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!
}
#pragma endregion

#pragma region Setters
Camera * Camera::SetPosition(DirectX::XMFLOAT3 position)
{
	outdatedMatrix = true;
	this->position = position;
	return this;
}

Camera * Camera::SetDirection(DirectX::XMFLOAT3 direction)
{
	outdatedMatrix = true;
	this->direction = direction;
	return this;
}
#pragma endregion

#pragma region Getters
DirectX::XMFLOAT3 Camera::GetPosition()
{
	return position;
}
DirectX::XMFLOAT3 Camera::GetDirection()
{
	return direction;
}
DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
#pragma endregion

Camera * Camera::Update(float deltaTime, float totalTime)
{
	DirectX::XMVECTOR rot = DirectX::XMQuaternionRotationRollPitchYaw(rotationy, rotationx, 0);

	DirectX::XMVECTOR dir = DirectX::XMVectorSet(0, 0, 1, 0);
	
	DirectX::XMVECTOR cameraDirectionVector = DirectX::XMVector3Rotate(dir, rot);
	DirectX::XMStoreFloat3(&direction, cameraDirectionVector);
	//DirectX::XMVECTOR cameraDirectionVector = DirectX::XMLoadFloat3(&cameraDirection);
	cameraDirectionVector = DirectX::XMVector4Normalize(cameraDirectionVector);
	DirectX::XMVECTOR cameraDirectionLeftVector = DirectX::XMVector3Cross(cameraDirectionVector, DirectX::XMVectorSet(0, 1, 0, 0));

	float scale = deltaTime * cameraSpeed;
	cameraDirectionVector = DirectX::XMVectorScale(cameraDirectionVector, scale);
	cameraDirectionLeftVector = DirectX::XMVectorScale(cameraDirectionLeftVector, scale);

	DirectX::XMVECTOR cameraPositionVector = DirectX::XMLoadFloat3(&position);

	//Movement forward and back along direction vector
	if (GetAsyncKeyState('W') & 0x8000) {
		DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(cameraPositionVector, cameraDirectionVector));
		outdatedMatrix = true;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		DirectX::XMStoreFloat3(&position, DirectX::XMVectorSubtract(cameraPositionVector, cameraDirectionVector));
		outdatedMatrix = true;
	}

	//Movement left and right based on direction vector
	if (GetAsyncKeyState('A') & 0x8000) {
		DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(cameraPositionVector, cameraDirectionLeftVector));
		outdatedMatrix = true;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		DirectX::XMStoreFloat3(&position, DirectX::XMVectorSubtract(cameraPositionVector, cameraDirectionLeftVector));
		outdatedMatrix = true;
	}
	//Movement up and down in world coords
	if (GetAsyncKeyState('Q') & 0x8000) {
		position.y -= deltaTime * cameraSpeed;
		outdatedMatrix = true;
	}
	if (GetAsyncKeyState('E') & 0x8000) {
		position.y += deltaTime * cameraSpeed;
		outdatedMatrix = true;
	}
	
	if (outdatedMatrix) {
		CreateView(this->position, this->direction);
	}
	return this;
}

Camera* Camera::OnMouseMove(int prex, int prey, int x, int y) {
	float deltax = (float)x - prex;
	float deltay = (float)y - prey;
	deltax *= cameraRotationSpeed;
	deltay *= cameraRotationSpeed;

	rotationx += deltax;
	rotationy += deltay;

	const int BOUNDS = 70;
	rotationx = min(max(rotationx, -BOUNDS), BOUNDS);
	rotationy = min(max(rotationy, -BOUNDS), BOUNDS);

	return this;
}

