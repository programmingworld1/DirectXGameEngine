#pragma once
///////////////////////////////
// Filename: systemclass.h
///////////////////////////////

// Als het niet gedefined is, dan definen we het
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_


// Here we define WIN32_LEAN_AND_MEAN. 
// We do this to speed up the build process, 
// it reduces the size of the Win32 header files 
// by excluding some of the less used APIs. 

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
/* Do this to speed up the build proces, it reduces the size of the Win32 header files by excluding
some of the less used APIs. You can include those header files manually if you need 1 of those or just
delete this line if you need all of them*/
#define WIN32_LEAN_AND_MEAN

///////////////////////////////
// INCLUDES //
///////////////////////////////

// Includes
//#include<windows.h> /* Windows.h is included so that we can call the functions to create/destroy windows and be able to use the other useful win32 functions. */

// My own class includes
#include "Input.h" /* For handeling user input */
#include "Graphics.h" /* for handeling the directX graphics code*/

///////////////////////////////
// Class name: SystemClass
///////////////////////////////

/*The definition of the class is fairly simple. We see the Initialize, Shutdown, and Run function that
was called in WinMain defined here. There are also some private functions that will be called inside those functions.
We have also put a MessageHandler function in the class to handle the windows system messages that will get sent to
the application while it is running. And finally we have some private variables m_Input and m_Graphics which will be
pointers to the two objects that will handle graphics and input. */
class System {
public: 
	System();
	System(const System&);
	~System();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
    void InitializeWindows(int&, int&);
	void ShutdownWindows();


private:
	LPCWSTR m_applicationName;

	// the only thing a HWND and a HINSTANCE have in common is that both are handles
	HINSTANCE m_hinstance; // is a handle to the instance of the program you are writing 

	/*HWND is a handle to a window. As we went over before, handles are integers which 
	identify an object. In this case, we're identifying a window. Simple. Because we have 
	no window made, we put NULL, and Windows knows not to have this message come from 
	any window, but the desktop.*/
	HWND m_hwnd; 

	Input* m_Input;
	Graphics* m_Graphics;
};

///////////////////////////////
// FUNCTION PROPERTYPES //
///////////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

///////////////////////////////
// GLOBALS //
///////////////////////////////
static System* ApplicationHandle = 0;

#endif

/*The WndProc function and ApplicationHandle pointer are also included in this class 
file so we can re-direct the windows system messaging into our MessageHandler function 
inside the system class. */