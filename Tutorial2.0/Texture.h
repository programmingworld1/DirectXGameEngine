// The TextureClass encapsulates the loading, unloading, and accessing of a single texture resource.For each texture needed an object of this class must be instantiated.

////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class Texture
{
private:

	//We define the targa file header structure here to make reading in the data easier.
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:

	// Here we have our targa reading function. If you wanted to support more formats you would add reading functions here.
	bool LoadTarga(char*, int&, int&);

private:
	
	/* This class has three member variables.The first one holds the raw targa data read straight in from the file.
	The second variable called m_texture will hold the structured texture data that DirectX will use for 
	rendering.And the third variable is the resource view that the shader uses to access the texture data when drawing.*/

	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
};

#endif