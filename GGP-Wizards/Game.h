#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "GameObject.h"
#include "Camera.h"
#include "Lights.h"
#include "WICTextureLoader.h"

struct BasicGeometry {
	Mesh* cone;
	Mesh* cube;
	Mesh* cylinder;
	Mesh* helix;
	Mesh* sphere;
	Mesh* torus;
};

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders();
	void CreateBasicGeometry();
	void LoadTextures();

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	BasicGeometry basicGeometry;
	Material* material;
	GameObject** gameObjects;
	Camera* camera;

	DirectionalLight directionalLight;
	DirectionalLight directionalLight2;
};

