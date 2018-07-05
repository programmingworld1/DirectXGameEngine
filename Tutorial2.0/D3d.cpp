//////////////////////////////
// Filename: d3dclass.cpp
//////////////////////////////
#include "D3d.h"

/*So like most classes we begin with initializing all the member pointers to null
in the class constructor. All pointers from the header file have all been accounted
for here. */

D3d::D3d()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}

D3d::D3d(const D3d& other)
{

}

D3d::~D3d()
{

}

/*The Initialize function is what does the entire setup of Direct3D for DirectX 11.
I have placed all the code necessary in here as well as some extra stuff that will
facilitate future tutorials. I could have simplified it and taken out some items but
it is probably better to get all of this covered in a single tutorial dedicated to it.

The screenWidth and screenHeight variables that are given to this function are the width
and height of the window we created in the SystemClass. Direct3D will use these to
initialize and use the same window dimensions. The hwnd variable is a handle to the window.
Direct3D will need this handle to access the window previously created.
The fullscreen variable is whether we are running in windowed mode or fullscreen.
Direct3D needs this as well for creating the window with the correct settings.
The screenDepth and screenNear variables are the depth settings for our 3D environment
that will be rendered in the window. The vsync variable indicates if we want Direct3D to
render according to the users monitor refresh rate or to just go as fast as possible. */
// https://msdn.microsoft.com/en-us/library/windows/desktop/bb205075(v=vs.85).aspx
bool D3d::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result; // A 32-bit value that is used to describe an error or warning.
	IDXGIFactory* factory; // CreateSoftwareAdapter, CreateSwapChain, EnumAdapters, GetWindowAssociation, MakeWindowAssociation
	IDXGIAdapter* adapter; /*This could simply be described as the virtutal representation of the video card (assuming the video card is separate, and not built into the motherboard).*/
	IDXGIOutput* adapterOutput; // An IDXGIOutput interface represents an adapter output (such as a monitor). 
	unsigned int numModes, i, numerator, denominator; // unsigned int is only + not in the - starting from 0
	unsigned long long stringLength; 
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// Storr the csync settings.
	m_vsync_enabled = vsync;

	/*Before we can initialize Direct3D we have to get the refresh rate from the video
	card/monitor. Each computer may be slightly different so we will need to query for
	that information. We query for the numerator and denominator values and then pass
	them to DirectX during the setup and it will calculate the proper refresh rate.
	If we don't do this and just set the refresh rate to a default value which may not
	exist on all computers then DirectX will respond by performing a blit instead
	of a buffer flip which will degrade performance and give us annoying errors in the
	debug output. */

	/*DXGI (DirectX Graphics Infrastructure) is a separate API from Direct3D that handles graphics related things like the swap chain, 
	enumerating graphics hardware, and switching between windowed and full-screen mode. The idea for keeping it separate from Direct3D 
	is that other graphics APIs (such as Direct2D) also require swap chains, enumerating graphics hardware, and switching between windowed 
	and full-screen mode. In this way, multiple graphics APIs can use the DXGI API*/

	// Create a directx graphics interface factory.
	/*The DirectX Graphics Infrastructure is a component that lies at the base of all the most recent versions of Direct3D.
	Its job is to handle fundamental tasks such as displaying images on the screen and finding out what resolutions the monitor and video card can handle.
	DXGI is not actually a part of Direct3D. It underlies it and other graphics components, and it acts as an interface between Direct3D and the hardware.
	Use a DXGI factory to generate objects that enumerate adapters, create swap chains, and associate a window with the alt+enter key sequence for
	toggling to and from the fullscreen display mode.If the CreateDXGIFactory function succeeds, the reference count on the IDXGIFactory interface is incremented.
	To avoid a memory leak, when you finish using the interface, call the IDXGIFactory::Release method to release the interface.*/
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	/*An adapter is an abstraction of the hardware and the software capability of your computer.
	There are generally many adapters on your machine. Some devices are implemented in hardware (like your video card)
	and some are implemented in software (like the Direct3D reference rasterizer). Adapters implement functionality used by a graphic application.
	When enumerating these pieces of hardware, DXGI creates an IDXGIOutput1 interface for each output (or monitor) and an IDXGIAdapter2 interface for
	each video card (even if it is a video card built into a motherboard). Enumeration is done by using an IDXGIFactory interface call, IDXGIFactory::EnumAdapters,
	to return a set of IDXGIAdapter interfaces that represent the video hardware.*/

	// When you create a factory, the factory enumerates the set of adapters that are available in the system. 
	// Use the factory to create an adapter for the primary graphics interface(video card), Enumerates the adapters (video cards).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fix the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL); // pak eerst aantal modus
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes]; // maak nu een lijst aan met de aantal modus
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures. 
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList); // full dat lijst met de echte modus
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i<numModes; i++) // loop nu door alle modus heen en als het overeenkomt pak je die twee waardes
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	/*We now have the numerator and denominator for the refresh rate.
	The last thing we will retrieve using the adapter is the name of the
	video card and the amount of video memory. */

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	/*Now that we have stored the numerator and denominator for the refresh rate
	and the video card information we can release the structures and interfaces
	used to get that information. */

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	/*Now that we have the refresh rate from the system we can start the DirectX
	initialization. The first thing we'll do is fill out the description of the swap chain.
	The swap chain is the front and back buffer to which the graphics will be drawn.
	Generally you use a single back buffer, do all your drawing to it, and then swap
	it to the front buffer which then displays on the user's screen. That is why it is
	called a swap chain. */

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer. The number of back buffers to use in the swap chain; we usually only use one back buffer for double buffering, although you could use two for triple buffering. 
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer, resolutie of de backbuffer, in pixels, het zelde afmeting als de window
	// als je allebei op 0 doet pakt die de window size maar beter is om het toch te setten
	// BufferDesc: This structure describes the properties of the back buffer we want to create. The main properties we are concerned with are the width and height, and pixel format;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	/*The next part of the description of the swap chain is the refresh rate.
	The refresh rate is how many times a second it draws the back buffer to the
	front buffer. If vsync is set to true in our graphicsclass.h header then this
	will lock the refresh rate to the system settings (for example 60hz).
	That means it will only draw the screen 60 times a second (or higher if the system
	refresh rate is more than 60). However if we set vsync to false then it will draw
	the screen as many times a second as it can, however this can cause some visual
	artifacts. */

	// Set the refresh rate of the back buffer.
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		// 0 over anything betekent geen vsync en.. updaten van scherm zo snel mogelijk als je het zo doet
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer. Specify DXGI_USAGE_RENDER_TARGET_OUTPUT because we are going to be rendering to the back buffer (i.e., use it as a render target)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// A handle to the window we are rendering into. 
	swapChainDesc.OutputWindow = hwnd;

	/*Multisampling or (MSAA) is a form of Antialiasing which is used to remove the jagged lines from the images we see on the screen.
	You might have noticed this jaggedness or aliasing, in 3D games and graphics ? If we enable AA, then the image becomes smoother,
	because more pixel samples are used to lend these jagged lines. But this reduces the overall performance/fps when high values like 4X, 8X etc. are selected.
	Though MSAA does not have a very high impact on FPS, as compared to Supersampling and Gamma Correct Antialiasing
	If you've ever played a video game on your PC, you've probably seen a setting called "anti-aliasing", which smooths out jagged graphics. But there are different types of anti-aliasing, and some are better than others.*/

	/*SSAA (also known as FSAA): Super sampling anti-aliasing was the first type of anti-aliasing available. It's useful on photorealistic images, but isn't very common in games anymore, because it uses so much processing power.
	MSAA: Multisample anti-aliasing is one of the more common types of anti-aliasing available in modern games. It only smooths out the edges of polygons, not anything else—which cuts down on processing power compared to SSAA, but doesn't solve pixelated textures. (MSAA still uses quite a bit of power, though.)
	CSAA and EQAA: These types of anti-aliasing (used by newer NVIDIA and AMD cards, respectively) are similar to MSAA, but at a fraction of the performance cost.
	FXAA: Fast approximate anti-aliasing, which we've mentioned before, has a very small performance cost, and smooths out edges in all parts of the image. However, it usually makes the image look blurry, which means it isn't ideal if you want crisp graphics.
	TXAA: Temporal anti-aliasing only works on certain newer graphics cards, but combines lots of different techniques to smooth out edges. It's better than FXAA, but still has some blurriness to it, and uses a bit more processing power.*/
	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1; // This member is used to tell Direct3D how to perform multisample anti-aliased (MSAA) rendering. If you are a next-gen game enthusiast, you have probably heard of this. Basically, anti-aliasing renders the edges of shapes smoothly by blending each pixel slightly with the surounding pixels.
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting. Specify DXGI_SWAP_EFFECT_DISCARD in order to let the display driver select the most efficient presentation method. 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.  If this flag is not specified, then when the application is switching to full-screen mode, it will use the current desktop display mode. 
	swapChainDesc.Flags = 0;

	/*After setting up the swap chain description we also need to setup one more variable
	called the feature level. This variable tells DirectX what version we plan to use.
	Here we set the feature level to 11.0 which is DirectX 11. You can set this to 10 or 9
	to use a lower level version of DirectX if you plan on supporting multiple versions or
	running on lower end hardware. */

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	/*Now that the swap chain description and feature level have been filled out
	we can create the swap chain, the Direct3D device, and the Direct3D device context.
	The Direct3D device and Direct3D device context are very important, they are the
	interface to all of the Direct3D functions. We will use the device and device context
	for almost everything from this point forward. */

	/*Those of you reading this who are familiar with the previous versions of
	DirectX will recognize the Direct3D device but will be unfamiliar with the new
	Direct3D device context. Basically they took the functionality of the Direct3D
	device and split it up into two different devices so you need to use both now. */

	/*Note that if the user does not have a DirectX 11 video card this
	function call will fail to create the device and device context. Also
	if you are testing DirectX 11 functionality yourself and don't have a DirectX 11
	video card then you can replace D3D_DRIVER_TYPE_HARDWARE with D3D_DRIVER_TYPE_REFERENCE
	and DirectX will use your CPU to draw instead of the video card hardware.
	Note that this runs 1/1000 the speed but it is good for people who don't have DirectX
	11 video cards yet on all their machines. */

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(
		NULL, /* Specifies the display adapter we want the create device to represent. Specifying null for this parameter uses the primary display adapter. */
		D3D_DRIVER_TYPE_HARDWARE, // The D3D_DRIVER_TYPE, which represents the driver type to create. In general, you will always specify D3D_DRIVER_TYPE_HARDWARE for this parameter to use 3D hardware acceleration for rendering.  but there are other options
		NULL, /* This is used for supplying a software driver. We always specify null because we are using hardware for rendering. */
		0, 
		&featureLevel, 1, /* Specifying null for this parameter indicates to choose the greatest feature level supported. */
		D3D11_SDK_VERSION, 
		&swapChainDesc, // A pointer to a swap chain description (see DXGI_SWAP_CHAIN_DESC) that contains initialization parameters for the swap chain.
		&m_swapChain, //Returns the address of a pointer to the IDXGISwapChain object that represents the swap chain used for rendering.
		&m_device, // Returns the address of a pointer to an ID3D11Device object that represents the device created. If this parameter is NULL, no ID3D11Device will be returned'.
		NULL, 
		&m_deviceContext); // Returns the address of a pointer to an ID3D11DeviceContext object that represents the device context. If this parameter is NULL, no ID3D11DeviceContext will be returned.

	if (FAILED(result))
	{
		return false;
	}

	/*Sometimes this call to create the device will fail if the primary video card
	is not compatible with DirectX 11. Some machines may have the primary card as a
	DirectX 10 video card and the secondary card as a DirectX 11 video card. Also
	some hybrid graphics cards work that way with the primary being the low power
	Intel card and the secondary being the high power Nvidia card. To get around this
	you will need to not use the default device and instead enumerate all the video cards
	in the machine and have the user choose which one to use and then specify that card
	when creating the device. */


	/*Let's start with where to render to. Now logically you would say, "the backbuffer, duh!" and be done. 
	However, Direct3D doesn't actually know that at this point. It is possible that you do not want to render 
	to the back buffer right away. For example many games render to the surface of a model, then render that 
	model to the back buffer. This technique can produce a variety of effects.*/

	/*Now that we have a swap chain we need to get a pointer to the back buffer
	and then attach it to the swap chain. We'll use the CreateRenderTargetView
	function to attach the back buffer to our swap chain. */

	/*In 3D rendering, a texture is another name for an image. An ID3D11Texture2D is an object that stores a flat image. 
	Like any COM object, we first define the pointer, and later a function creates the object for us.*/
	/*GetBuffer() is a function finds the back buffer on the swap chain and creates an interface directly pointing to it.
	The first parameter is the number of the back buffer we want to obtain. 
	We are only using one back buffer on this swap chain, and it is back buffer #0. Therefore the first parameter will be 0.
	The second parameter is not that new to us. __uuidof() tells GetBuffer() what type of COM interface we want to obtain.
	The third parameter is the address of our interface ComPtr. GetBuffer() will store our interface here.*/
	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	/*When rendering in Direct3D, DirectX must know where exactly to render to. 
	The render target view is a COM object that maintains a location in video memory to render into. In most cases this will be the back buffer. */
	/*So, if I understand this correctly, getBuffer returns the pointer to actual buffer. 
	CreateRenderTargetView makes this buffer a render target (aka Framebuffer) which is then bound
	to in the rendering pipeline. PS: RenderTargetView doesn't create another buffer.*/
	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	/*We will also need to set up a depth buffer description.
	We'll use this to create a depth buffer so that our polygons can be rendered
	properly in 3D space. At the same time we will attach a stencil buffer to our
	depth buffer. The stencil buffer can be used to achieve effects such as motion
	blur, volumetric shadows, and other things. */
	/*In order for Direct3D to determine which pixels of an object are in front of another, it uses a technique called depth buffering or z-buffering.*/

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth; // The width of the texture in texels
	depthBufferDesc.Height = screenHeight; // The height of the texture in texels
	depthBufferDesc.MipLevels = 1; // The number of mipmap levels. For creating the depth/stencil buffer, our texture only needs one mipmap level
	depthBufferDesc.ArraySize = 1; // The number of textures in a texture array. For the depth/stencil buffer, we only need one texture
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // A member of the DXGI_FORMAT enumerated type specifying the format of the texels
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT; // A member of the D3D11_USAGE enumerated type specifying how the texture will be used
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // One or more flags ORed together specifying where the resource will be bound to the pipeline. For a depth/stencil buffer, this needs to be D3D11_BIND_DEPTH_STENCIL. 
	depthBufferDesc.CPUAccessFlags = 0; // Specifies how the CPU will access the resource. For the depth/stencil buffer, only the GPU writes and reads to the depth/buffer; therefore, we can specify zero for this value, as the CPU will not be reading or writing to the depth/stencil buffer. 
	depthBufferDesc.MiscFlags = 0; // Optional flags, which do not apply to the depth/stencil buffer, so are set to zero.

	/*Now we create the depth/stencil buffer using that description.
	You will notice we use the CreateTexture2D function to make the buffers,
	hence the buffer is just a 2D texture. The reason for this is that once your
	polygons are sorted and then rasterized they just end up being colored pixels
	in this 2D buffer. Then this 2D buffer is drawn to the screen. */

	/* the depth buffer is just a 2D texture that stores the depth information (and stencil information if using stenciling)*/
	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	/*Now we need to setup the depth stencil description.
	This allows us to control what type of depth test Direct3D will do for each pixel. */
	/*we can use the stencil buffer to block rendering to certain areas of the back buffer. The decision to block a particular pixel from being written is decided by the stencil tes*/
	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	/*With the description filled out we can now create a depth stencil state. */
	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	/*With the created depth stencil state we can now set it so that it takes effect. Notice we use the device context to set it. */
	/*we bind a depth/stencil state block to the output merger stage of the pipeline*/
	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	/*The next thing we need to create is the description of the view of the depth
	stencil buffer. We do this so that Direct3D knows to use the depth buffer as
	a depth stencil texture. After filling out the description we then call the function
	CreateDepthStencilView to create it. */

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	/*With that created we can now call OMSetRenderTargets. This will bind the render
	target view and the depth stencil buffer to the output render pipeline.
	This way the graphics that the pipeline renders will get drawn to our back
	buffer that we previously created. With the graphics written to the back buffer
	we can then swap it to the front and display our graphics on the user's screen. */
	/*The first parameter is the number of render targets we are binding; we bind only one here, but more can be bound to render 
	simultaneously to several render targets (an advanced technique). The second parameter is a pointer to the first element in an 
	array of render target view pointers to bind to the pipeline. The third parameter is a pointer to the depth/stencil view to bind to the pipeline.*/
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	/*Now that the render targets are setup we can continue on to some extra functions
	that will give us more control over our scenes for future tutorials. First thing
	is we'll create is a rasterizer state. This will give us control over how polygons
	are rendered. We can do things like make our scenes render in wireframe mode or have
	DirectX draw both the front and back faces of polygons. By default DirectX already
	has a rasterizer state set up and working the exact same as the one below but you
	have no control to change it unless you set up one yourself. */
	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	/*The viewport also needs to be setup so that Direct3D can map clip space
	coordinates to the render target space. Set this to be the entire size of the window. */

	// Setup the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);

	/*Now we will create the projection matrix. The projection matrix is used to
	translate the 3D scene into the 2D viewport space that we previously created.
	We will need to keep a copy of this matrix so that we can pass it to our shaders
	that will be used to render our scenes. */

	// Setup the projection matrix.
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);

	/*We will also create another matrix called the world matrix.
	This matrix is used to convert the vertices of our objects into vertices in
	the 3D scene. This matrix will also be used to rotate, translate, and scale
	our objects in 3D space. From the start we will just initialize the matrix to
	the identity matrix and keep a copy of it in this object. The copy will be needed
	to be passed to the shaders for rendering also. */

	// Initialize the world matrix to the identity matrix. (Returns the identity matrix.)
	m_worldMatrix = XMMatrixIdentity();

	/*This is where you would generally create a view matrix.
	The view matrix is used to calculate the position of where we are
	looking at the scene from. You can think of it as a camera and you
	only view the scene through this camera. Because of its purpose I am
	going to create it in a camera class in later tutorials since logically
	it fits better there and just skip it for now. */

	/*And the final thing we will setup in the Initialize function is an orthographic
	projection matrix. This matrix is used for rendering 2D elements like user
	interfaces on the screen allowing us to skip the 3D rendering. You will see
	this used in later tutorials when we look at rendering 2D graphics and fonts
	to the screen. */

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

/*The Shutdown function will release and clean up all the pointers used in the Initialize
function, its pretty straight forward. However before doing that I put in a call to force
the swap chain to go into windowed mode first before releasing any pointers. If this is
not done and you try to release the swap chain in full screen mode it will throw some
exceptions. So to avoid that happening we just always force windowed mode before shutting
down Direct3D. */

void D3d::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	return;
}

/*In the D3DClass I have a couple helper functions.
The first two are BeginScene and EndScene. BeginScene will be called whenever we
are going to draw a new 3D scene at the beginning of each frame. All it does is
initializes the buffers so they are blank and ready to be drawn to. The other function
is Endscene, it tells the swap chain to display our 3D scene once all the drawing has
completed at the end of each frame. */

void D3d::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3d::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}

	return;
}

/*These next functions simply get pointers to the Direct3D device and the Direct3D
device context. These helper functions will be called by the framework often. */

ID3D11Device* D3d::GetDevice()
{
	return m_device;
}


ID3D11DeviceContext* D3d::GetDeviceContext()
{
	return m_deviceContext;
}

/*The next three helper functions give copies of the projection, world, and
orthographic matrices to calling functions. Most shaders will need these matrices
for rendering so there needed to be an easy way for outside objects to get a copy
of them. We won't call these functions in this tutorial but I'm just explaining why
they are in the code. */

void D3d::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void D3d::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}


void D3d::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

/*The last helper function returns by reference the name of the video card and
the amount of video memory. Knowing the video card name can help in debugging on
different configurations. */

void D3d::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

/*So now we are finally able to initialize and shut down Direct3D.
Compiling and running the code will produce the same window as the
last tutorial but Direct3D is initialized now and it clears the window to a grey color.
Compiling and running the code will also show if your compiler is set up properly and if
it can see the headers and libraries files from the Windows SDK. */