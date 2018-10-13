////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;
// The fstream library is now included to handle reading from the model text file.
#include <fstream>
using namespace std;
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "texture.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:

	// The VertexType has replaced the color component with a texture coordinate component.The texture coordinates are now replacing the green color that was used in the previous tutorial.
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	//The next change is the addition of a new structure to represent the model format.It is called ModelType.It contains position, texture, and normal vectors the same as our file format does.

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	// The Initialize function will now take as input the character string file name of the model to be loaded.

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	// The ModelClass now has a GetTexture function so it can pass its own texture resource to shaders that will draw this model.
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	// ModelClass also now has both a private LoadTexture and ReleaseTexture for loading and releasing the texture that will be used to render this model.
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	// We also have two new functions to handle loading and unloading the model data from the text file.

	bool LoadModel(char*);
	void ReleaseModel();

	/*The private variables in the ModelClass are the vertex and index buffer as well as two integers to keep track of the size of each buffer.
	Note that all DirectX 11 buffers generally use the generic ID3D11Buffer type and are more clearly identified by a buffer description when they are first created.*/
private:
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	// The m_Texture variable is used for loading, releasing, and accessing the texture resource for this model.
	Texture* m_Texture;

	/*The final change is a new private variable called m_model which is going to be an array of the new private structure ModelType.
	This variable will be used to read in and hold the model data before it is placed in the vertex buffer.*/
	ModelType* m_model;
};

#endif