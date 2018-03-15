#pragma once
/////////////////////////////
// Filename : D3DClass
/////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

/*First thing in the header is to specify the libraries to link when using this object
module. The first library contains all the Direct3D functionality for setting up and
drawing 3D graphics in DirectX 11. The second library contains tools to interface with
the hardware on the computer to obtain information about the refresh rate of the monitor,
the video card being used, and so forth. The third library contains functionality for
compiling shaders which we will cover in the next tutorial. */

//////////
// LINKING //
/////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

/*The next thing we do is include the headers for those libraries that we are linking to
this object module as well as headers for DirectX type definitions and math functionality. */

////////////
// INCLUDES //
///////////
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

/*The class definition for the D3DClass is kept as simple as possible here.
It has the regular constructor, copy constructor, and destructor.
Then more importantly it has the Initialize and Shutdown function.
This will be what we are mainly focused on in this tutorial.
Other than that I have a couple helper functions which aren't
important to this tutorial and a number of private member variables that will
be looked at when we examine the d3dclass.cpp file. For now just realize the
Initialize and Shutdown functions are what concerns us. */

////////////////////
// Class name: D3DClass
///////////////////
class D3d
{
public:
	D3d();
	D3d(const D3d&);
	~D3d();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
};

#endif

/*For those familiar with Direct3D already you may notice I don't have a view matrix
variable in this class. The reason being is that I will be putting it in a camera
class that we will be looking at in future tutorials. */

