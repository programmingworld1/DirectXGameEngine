////////////////////
// Filename: main.cpp
///////////////////
#include "System.h"

/*The frame work will begin with four items. It will have a WinMain function to handle the entry point of the application.
It will also have a system class that encapsulates the entire application that will be called from within the WinMain function.
Inside the system class we will have a input class for handling user input and a graphics class for handling the DirectX graphics code.
Here is a diagram of the framework setup: */

/*
hInstance : is a handle to the instance of the program you are writing
hPrevInstance : is used if your program had any previous instance.
if not, this argument can be ignored, which will always be the case
pScmdline : a string that represents all items used on the command line to compile the application
iCmdshow : controls how the window you are building will displayed
*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// blaa
    System* system;
	bool result;

	// Create the system object.
	system = new System;
	if (!system) {
		return 0;
	}

	// Initialize and run the system object.
	result = system->Initialize();
	if (result) {
		system->Run();
	}

	// Shutdown and release the system object.
	system->Shutdown();

	//The delete operator destroys an object that has been created by new when passed a pointer to the object
	delete system;
	system = 0;

	return 0;
}

/* As you can see we kept the WinMain function fairly simple. We create the system class and then initialize it.
If it initializes with no problems then we call the system class Run function.
The Run function will run its own loop and do all the application code until it completes.
After the Run function finishes we then shut down the system object and do the clean up of the system object.
So we have kept it very simple and encapsulated the entire application inside the system class.
Now lets take a look at the system class header file. */