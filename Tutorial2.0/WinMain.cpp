////////////////////
// Filename: main.cpp
///////////////////
#include "System.h"

/*The frame work will begin with four items. It will have a WinMain function to handle the entry point of the application.
It will also have a system class that encapsulates the entire application that will be called from within the WinMain function.
Inside the system class we will have a input class for handling user input and a graphics class for handling the DirectX graphics code.
Here is a diagram of the framework setup: */


/*hInstance: This first parameter is an abbreviation for a "handle to an instance". 
A handle is a 32-bit integer which identifies something, like an object. An instance 
is a copy of an application. Due to multitasking and the ability to run multiple copies 
of a program, Windows needs a way to keep track of which programs are which. It does 
this by giving each instance of any application a "handle to an instance", or an integer
which identifies each application from the others. When your program starts, Windows 
picks a number and puts it into this parameter.*/

/*hPrevInstance: is used if your program had any previous instance. Here we have another relic of the past. This second parameter stands for 
a handle to the previous instance. What this means is, if there are multiple copies of an 
application open, hPrevInstance will contain the handle of the last instance to be created. 
Theoretically.
Way back before 32-bit processors came around and before Windows 95, multiple copies of the 
same application were forced to share the same memory space. This complicated things greatly,
and it was of interest to many programmers to keep their applications from running multiple
copies of themselves. In order to avoid this, hPrevInstance would give the handle to the 
previous instance (if any) and return NULL if this was the only one, allowing a programmer 
to detect any running copies.
However, now that we have 32-bit applications, Windows now provides each application 
with its own memory space, and hPrevInstance is now obsolete. It only exists for backward 
compatibility and is equal to NULL in every application, despite the existence of other 
copies.
There is a method of detecting previous instances of an application*/


/*lpCmdLine: This parameter is a pointer to a string that contains the command line 
calling existence to the application. Example: If you had an application called "MyApp.exe" 
and ran it from the Run command prompt in the Start Menu, you could run it like this 
"MyApp.exe" or "MyApp.exe RunA" or "MyApp.exe RunB". In either case, lpCmdLine would 
store the entirety of whatever was entered, allowing the program to check for special 
parameters.
This is useful in game programming for running special modes such as Safe Mode, 
Windowed Mode, Software-Rendering Mode, Cheat-Code Mode or whatever you want.*/

/*iCmdShow : controls how the window you are building will displayed soo This parameter indicates how the window is to appear when created. 
For instance, this could call for the window being minimized, maximized, or normal, 
or even open a window with it running in the background. You do not need to use these, 
but they are available if you so desire. Usually you will not use them in game programming, 
but they can be useful on occasion. Below are some common values that can come from this
parameter:
SW_SHOWNORMAL	Activates and displays a window. If the window is minimized or maximized, the system restores it to its original size and position. An application should specify this flag when displaying the window for the first time.
SW_SHOW	Activates the window and displays it in its current size and position.
SW_HIDE	Hides the window and activates another window.
SW_MAXIMIZE	Maximizes the specified window.
SW_MINIMIZE	Minimizes the specified window and activates the next top-level window in the Z order.
SW_RESTORE	Activates and displays the window. If the window is minimized or maximized, the system restores it to its original size and position. An application should specify this flag when restoring a minimized window.
SW_SHOWMAXIMIZED	Activates the window and displays it as a maximized window.
SW_SHOWMINIMIZED	Activates the window and displays it as a minimized window.
SW_SHOWMINNOACTIVE	Displays the window as a minimized window. This value is similar to SW_SHOWMINIMIZED, except the window is not activated.
SW_SHOWNA	Displays the window in its current size and position. This value is similar to SW_SHOW, except the window is not activated.
SW_SHOWNOACTIVATE	Displays a window in its most recent size and position. This value is similar to SW_SHOWNORMAL, except the window is not actived.*/


/*The C and C++ standards require any program (for a “hosted” C or C++ implementation) to have a function called main, which serves as the program's startup function.*/
/*takes I think always 4 parameters*/
/*Before we get into the parameters, let's look at something just before them. Between the 
int and the WinMain is the word WINAPI. WINAPI is a method of passing parameters which comes
from the days of Fortran and Pascal. In short, what it does is reverse the order the 
parameters are passed. They normally pass from right to left, but with WINAPI, they are 
passed from left to right. The details on why this is are not really important. Windows 
simply requires it to be this way.*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
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


/*
// LPCTSTR lptext (first one): This is a pointer to a 16-bit string that contains the text of the message box. Nothing more to it.
// LPCTSTR lpcaption (second) : This is a pointer to a 16-bit string that contains the text of the title bar, or caption, for the message box. Simple.
// UINT : This parameter determines what the style of the message box is. Following is a list of commonly used values you can put into this parameter (combined with a logical OR operator for multiple values). To indicate the buttons displayed in the message box, specify one of the following values:

// To indicate the buttons displayed in the message box, specify one of the following values:
MB_CANCELTRYCONTINUE	The message box contains three push buttons: Cancel, Try Again, Continue.
MB_OK	The message box contains one push button: OK. This is the default.
MB_OKCANCEL	The message box contains two push buttons: OK and Cancel.
MB_RETRYCANCEL	The message box contains two push buttons: Retry and Cancel.
MB_YESNO	The message box contains two push buttons: Yes and No.
MB_YESNOCANCEL	The message box contains three push buttons: Yes, No, and Cancel.

// To display an icon in the message box, specify one of the following values:
MB_ICONINFORMATION	An icon consisting of a lowercase letter i in a circle appears in the message box.
MB_ICONEXCLAMATION	An exclamation-point icon appears in the message box.
MB_ICONERROR	A stop-sign icon appears in the message box.

// In game programming, you normally won't need return values, because error messages (which is what you will use this for 99 percent of the time) only have one direction to go in: quit. However, it is still useful to know. Following is a list of return values, letting you know which button was pressed:
IDCANCEL	The 'Cancel' button was selected.
IDTRYAGAIN	The 'Try Again' button was selected.
IDCONTINUE	The 'Continue' button was selected.
IDNO	The 'No' button was selected.
IDYES	The 'Yes' button was selected.
IDOK	The 'OK' button was selected.


int MessageBox(HWND hWnd,
LPCTSTR lptext,
LPCTSTR lpcaption,
UINT utype);


MessageBox(NULL,
L"Hello World!",
L"Just another Hello World program!",
MB_ICONEXCLAMATION | MB_OK);

*/