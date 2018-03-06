#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"GGP Wizards",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
	delete material;
	delete camera;
	
	delete basicGeometry.cone;
	delete basicGeometry.cube;
	delete basicGeometry.cylinder;
	delete basicGeometry.helix;
	delete basicGeometry.sphere;
	delete basicGeometry.torus;

	for (int i = 0; i < 6; i++) {
		delete entities[i];
	}
	free(entities);
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	LoadShaders();
	CreateBasicGeometry();
	LoadTextures();

	camera = new Camera(width, height);

	// Initialize the directional light
	directionalLight.ambientColor	= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	directionalLight.diffuseColor	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	directionalLight.direction		= XMFLOAT3(1.f, -1.f, 1.f);

	directionalLight2.ambientColor	= XMFLOAT4(0.f, 0.f, 0.f, 1.0f);
	directionalLight2.diffuseColor	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	directionalLight2.direction		= XMFLOAT3(1.f, 0.f, 0.f);


	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}

// --------------------------------------------------------
// Loads the objs as meshes and stores pointers in a struct
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	//TODO: autoload all assets in folder and store as filename
	basicGeometry.cone		= new Mesh("../../Assets/Models/cone.obj",		device);
	basicGeometry.cube		= new Mesh("../../Assets/Models/cube.obj",		device);
	basicGeometry.cylinder	= new Mesh("../../Assets/Models/cylinder.obj",	device);
	basicGeometry.helix		= new Mesh("../../Assets/Models/helix.obj",		device);
	basicGeometry.sphere	= new Mesh("../../Assets/Models/sphere.obj",	device);
	basicGeometry.torus		= new Mesh("../../Assets/Models/torus.obj",		device);

	const int ENTITY_COUNT = 6;
	entities = (Entity**)malloc(sizeof(Entity*) * ENTITY_COUNT);
	
	ID3D11ShaderResourceView* shaderResourceView;
	CreateWICTextureFromFile(device, context, L"../../Assets/Textures/grid.png", 0, &shaderResourceView);

	ID3D11SamplerState* samplerState;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);

	material = new Material(vertexShader, pixelShader, shaderResourceView, samplerState);

	Mesh** basicGeoSubPointer = (Mesh**)&basicGeometry;
	for (int i = 0; i < ENTITY_COUNT; i++) {
		entities[i] = new Entity(*basicGeoSubPointer++, material);
		entities[i]->SetPosition(-2.5f + i, 0, 0)->SetScale(0.5f, 0.5f, 0.5f); 
	}
}

void Game::LoadTextures() {
	//TODO
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	camera->CreateProjection(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	camera->Update(deltaTime, totalTime);

	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//send light data to pixel shader
	pixelShader->SetData("directionalLight", &directionalLight, sizeof(DirectionalLight));
	pixelShader->SetData("directionalLight2", &directionalLight2, sizeof(DirectionalLight));
	pixelShader->CopyAllBufferData();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	for (int i = 5; i >= 0; i--) {
		entities[i]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());
		context->IASetVertexBuffers(0, 1, entities[i]->mesh->GetVertexBufferAddress(), &stride, &offset);
		context->IASetIndexBuffer(entities[i]->mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			entities[i]->mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices

	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	RECT clientRect;
	SetRect(&clientRect, 0, 0, width, height);

	RECT desktopRect;
	GetClientRect(GetDesktopWindow(), &desktopRect);
	int centeredX = (desktopRect.right / 2) - (clientRect.right / 2);
	int centeredY = (desktopRect.bottom / 2) - (clientRect.bottom / 2);
	
	POINT point;
	GetCursorPos(&point);

	camera->OnMouseMove(desktopRect.right / 2, desktopRect.bottom / 2, point.x, point.y);

	SetCursorPos(desktopRect.right / 2, desktopRect.bottom / 2);
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion