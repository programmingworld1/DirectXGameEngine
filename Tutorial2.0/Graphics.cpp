/////////////////////////////////////
// Filename: graphicsclass.cpp
/////////////////////////////////////
#include "Graphics.h"

Graphics::Graphics()
{
	// So the very first change is in the class constructor. Here we initialize the pointer to null for safety reasons as we do with all class pointers. 

	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
}

Graphics::Graphics(const Graphics& other)
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	/* The second change is in the Initialize function inside the GraphicsClass.
	Here we create the D3DClass object and then call the D3DClass Initialize function.
	We send this function the screen width, screen height,
	handle to the window, and the four global variables from the Graphicsclass.h file.
	The D3DClass will use all these variables to setup the Direct3D system.
	We'll go into more detail about that once we look at the d3dclass.cpp file. */

	bool result;

	// Create the direct3d objec 
	m_Direct3D = new D3d;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object. 
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR); // - added
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(-0.0f, 0.0f, -5.0f);

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../data/cube.txt", "../data/stone01.tga");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//The new light shader object is created and initialized here.

	// Create the light shader object.
	m_LightShader = new LightShader;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}
	//The new light object is created here.

	// Create the light object.
	m_Light = new Light;
	if (!m_Light)
	{
		return false;
	}
	//The color of the light is set to purple and the light direction is set to point down the positive Z axis.

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void Graphics::Shutdown()
{

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	/*The next change is in the Shutdown function in the GraphicsClass.
	Shut down of all graphics objects occur here so we have placed the D3DClass
	shutdown in this function. Note that I check to see if the pointer was initialized
	or not. If it wasn't we can assume it was never set up and not try to shut it down.
	That is why it is important to set all the pointers to null in the class constructor.
	If it does find the pointer has been initialized then it will attempt to shut down
	the D3DClass and then clean up the pointer afterwards. */

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}

bool Graphics::Frame()
{
	/*The Frame function has been updated so that it now calls the Render
	function each frame. */

	bool result;

	// We add a new static variable to hold an updated rotation value each frame that will be passed into the Render function.

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}
	return true;
}

bool Graphics::Render(float rotation)
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Here we rotate the world matrix by the rotation value so that when we render the triangle using this updated world matrix it will spin the triangle by the rotation amount.

	worldMatrix = XMMatrixRotationY(rotation);
	// Rotate the world matrix by the rotation value so that the triangle will spin.
	//XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// The light shader is called here to render the triangle.The new light object is used to send the diffuse light color and light direction into the Render function so that the shader has access to those values.

	// Render the model using the light shader.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}



