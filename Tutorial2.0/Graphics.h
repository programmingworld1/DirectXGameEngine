#pragma once
/*The graphics class is the other object that is created by the system class.
All the graphics functionality in this application will be encapsulated in this class.
I will also use the header in this file for all the graphics related global settings
that we may want to change such as full screen or windowed mode. Currently this class
will be empty but in future tutorials will contain all the graphics objects. */

////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////

// Als het niet gedefined is, dan definen we het
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

///////////
// INCLUDES //
//////////
// Here is the first change. We have taken out the include for windows.h and instead included the new d3dclass.h. 
// #include <windows.h>
#include "D3d.h"
#include "cameraclass.h"
#include "Modelclass.h"
#include "Textureshader.h"

//////////
// GLOBALS //
//////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

//////////////////////////////////
// Class name: GrapchisClass
//////////////////////////////////

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	// And the second change is the new private pointer to the D3DClass which we have called m_Direct3D. In case you were wondering I use the prefix m_ on all class variables. That way when I'm coding I can remember quickly which variables are members of the class and which are not. 
	D3d * m_Direct3D; // - added
	CameraClass* m_Camera;
	ModelClass* m_Model;
	TextureShaderClass* m_TextureShader;

};

#endif