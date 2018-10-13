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

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	/*The functions here handle initializing and shutdown of the model's vertex and index buffers. 
	The Render function puts the model geometry on the video card to prepare it for drawing by the color shader.*/
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
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

	/*The private variables in the ModelClass are the vertex and index buffer as well as two integers to keep track of the size of each buffer. 
	Note that all DirectX 11 buffers generally use the generic ID3D11Buffer type and are more clearly identified by a buffer description when they are first created.*/
private:
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	// The m_Texture variable is used for loading, releasing, and accessing the texture resource for this model.
	Texture* m_Texture;
};

#endif