// The ColorShaderClass is what we will use to invoke(aanroepen) our HLSL shaders for drawing the 3D models that are on the GPU.

////////////////////////////////////////////////////////////////////////////////
// Filename: colorshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_


//////////////
// INCLUDES // https://www.3dgep.com/introduction-to-directx-11/
//////////////
#include <d3d11.h>
#include <d3dcompiler.h> // The d3dcompiler header file is required for loading and compiling HLSL shaders. 
#include <directxmath.h> // The DirectXMath header file includes math primitives like vectors, matrices and quaternions as well as the functions to operate on those primitives.
#include <fstream>
using namespace DirectX;
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: ColorShaderClass
////////////////////////////////////////////////////////////////////////////////
class ColorShaderClass
{
private:
	/*Here is the definition of the cBuffer type that will be used with the vertex shader.
	This typedef must be exactly the same as the one in the vertex shader as the model data 
	needs to match the typedefs in the shader for proper rendering.*/
	struct MatrixBufferType
	{
		/*The World Matrix translates the position of your vertices from model space to World space. That means it applies its position in the world and its rotation.

		The View Matrix translates those vertices from world space to camera space. This means the new vertices are in relation to you camera.

		The Projection Matrix finally translates those vertices from world space to projection Space. That means it finally calculates where you vertices are actually displayed on you Display/Monitor.*/
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

	/*The functions here handle initializing and shutdown of the shader. 
	The render function sets the shader parameters and then draws the prepared model vertices using the shader.*/
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader * m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif