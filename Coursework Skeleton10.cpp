//**************************************************************************//
// DirectX10 version!!														//
//																			//
// You are very welcome to use this as a framework for your coursework.		//
//																			//
// This is based on an empty sample, "simpleSample" provided for us by		//
// Microsoft.  It does all the hard work of enumerating the hardware and	//
// giving us these reasonable cool "DxUT buttons" to change things.			//
//																			//
// This version uses the fixed finction pipeline to do the rendering, the   //
// build in lighting etc. within DirectX.  There is an alternative that uses//
// a shader file, which you don't have to use.								//
//																			//
// Look for this style of comments for the bits you must work on.			//
//																			//
// OnD3D10CreateDevice():													//
//					The tiger mesh and the light are created in this one,   //
//					but are never destroyed.  Odd things happen even if you	//
//					change the screen to, say, full	screen.					//
// OnD3D10FrameRender():													//
//					The vast majority of the action takes place in here.	//
// OnFrameMove():	Movement is processed in this one, based on some global //
//					variables that indicate keyboard status.				//
// Keyboard():		Key events are processed in here.  Not proper			//
//					DirectInput, I'm afraid.								//
//																			//
// NOTE that by default, this sample synchronizes the frame rate to the		//
// vSync event.  You can turn that off with the "change device" button.		//
//																			//
// Also NOTE that this uses the Microsoft utitity class CDXUTMesh to load	//
// and render the mesh, but I am not using the Microsoft camara class		//
// CModelViewerCamera.  That class looks very nice, do use it is you want	//
// to, but also understand how the view matrix is created.					//
//																			//
// And, as usual, you should correct at least 10% of the typig abd spekking //
// errirs.	nigel@soc.plymouth.ac.uk.										//
//																			//
// Heavily based on "SimpleSample" Copyright (c) Microsoft Corporation.		//
//**************************************************************************//

//**************************************************************************//
// Note that, unlike the old common framework applications which all extend //
// the CD3DApplication, this is not a class.  It is just a program that		//
// defines several callbacks, like OnFrameRender().							//
//																			//
//**************************************************************************//

#include "DXUT.h"
#include "DXUTgui.h"
#include "DXUTmisc.h"
#include "DXUTCamera.h"
#include "DXUTSettingsDlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"
#include "resource.h"
#include "nig.h"
#include <stdio.h>
#include "TObject3D.h"
#include "TObject2D.h"
#include "TBall.h"
#include <vector>
#include "NigSoundManager.h"
#include "NigSound.h"
#include "TTexture.h"
#include "TMeshProducer.h"


//#define DEBUG_VS   // Uncomment this line to debug D3D9 vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug D3D9 pixel shaders 


//**************************************************************************//
// Convert an old chrtacter (char *) string to a WCHAR * string.  There must//
// be something built into Visual Studio to do this for me, but I can't		//
// find it - Nigel.															//
//**************************************************************************//

void charStrToWideChar(WCHAR *dest, char *source)
{
	int length = strlen(source);
	for (int i = 0; i <= length; i++)
		dest[i] = (WCHAR) source[i];
	
}


//**************************************************************************//
// Global variables originally defined by Microsoft.						//
//**************************************************************************//

ID3D10Device* g_p_d3dDevice;

CModelViewerCamera          g_Camera;               // A model viewing camera
CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg             g_SettingsDlg;          // Device settings dialog
CDXUTTextHelper*            g_pTxtHelper = NULL;
CDXUTDialog                 g_HUD;                  // dialog for standard controls
CDXUTDialog                 g_SampleUI;             // dialog for sample specific controls

// Direct3D 10 resources
ID3DX10Font*                g_pFont10 = NULL;
ID3DX10Sprite*              g_pSprite10 = NULL;
ID3D10InputLayout*          g_pVertexLayout = NULL;


//**************************************************************************//
// Nigel added variables.  All in Hungarian notation, of course.  Don't	//
// feel that you have to use Hungarian notation, though, especially if you //
// aren't Hungarian.														//
//																			//
// There are many global variables here.  I suggest that at some point you	//
// 1: Don't tell Mary that you are using so many globals.					//
// 2: Consider encapsulating some of this stuff into classes of your own.	//
//**************************************************************************//

TMeshProducer	       *g_MeshProducer;

TObject3D			   *g_NewTiger;
TObject3D			   *g_NewSkyBox;
TObject3D			   *g_RWing;
TObject3D			   *g_LWing;

TTexture			   *texture;

std::vector<TObject2D *> tiles;
std::vector<TBall *>     balls;

TEffect				   *g_p_TEffect;

NigSoundManager soundManager;	//You need one of these.
NigSound sound1(&soundManager); //And one of these per sound.
NigSound sound2(&soundManager);
NigSound sound3(&soundManager);

DWORD sound1State = 0;

//**********************************************************************//
// Variables to control the movement of the tiger.						//
//**********************************************************************//

bool		 g_b_LeftArrowDown      = false;	//Status of keyboard.  Thess are set
bool		 g_b_RightArrowDown     = false;	//in the callback KeyboardProc(), and 
bool		 g_b_UpArrowDown	    = false;	//are used in onFrameMove().
bool		 g_b_DownArrowDown	    = false;
bool		 g_b_Space	    = false;

bool		 g_Rotating = false;
bool		 g_Moving	= false;

bool		 g_b_W = false;
bool		 g_b_S = false;



//**********************************************************************//
// XBOX controller macros and structures.								//
//**********************************************************************//

#define MAX_CONTROLLERS 4  // XInput handles up to 4 controllers 
#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

struct CONTROLER_STATE
{
    XINPUT_STATE state;
    bool connected;
};

CONTROLER_STATE g_Controllers[MAX_CONTROLLERS];
WCHAR g_szMessage[4][1024] = {0};
HWND    g_hWnd;
bool    g_bDeadZoneOn = true;




//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           2
#define IDC_CHANGEDEVICE        3
#define IDC_TOGGLEWARP          4


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext );
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );

bool CALLBACK IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext );
HRESULT CALLBACK OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnD3D10ReleasingSwapChain( void* pUserContext );
void CALLBACK OnD3D10DestroyDevice( void* pUserContext );

void InitApp();
void RenderText();
void UpdateControllerState(float frameTime);




//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device (either D3D9 or D3D10) 
    // that is available on the system depending on which D3D callbacks are set below

    // Set DXUT callbacks
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

    DXUTSetCallbackD3D10DeviceAcceptable( IsD3D10DeviceAcceptable );
    DXUTSetCallbackD3D10DeviceCreated( OnD3D10CreateDevice );
    DXUTSetCallbackD3D10SwapChainResized( OnD3D10ResizedSwapChain );
    DXUTSetCallbackD3D10SwapChainReleasing( OnD3D10ReleasingSwapChain );
    DXUTSetCallbackD3D10DeviceDestroyed( OnD3D10DestroyDevice );
    DXUTSetCallbackD3D10FrameRender( OnD3D10FrameRender );

    InitApp();
    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true );
    DXUTCreateWindow( L"Starting point for Coursework" );
    DXUTCreateDevice( true, 640, 480 );
    DXUTMainLoop(); // Enter into the DXUT render loop

    return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    g_SettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_SampleUI.Init( &g_DialogResourceManager );

    g_HUD.SetCallback( OnGUIEvent ); int iY = 10;
    g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
    g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );
    g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22, VK_F3 );
    g_HUD.AddButton( IDC_TOGGLEWARP, L"Toggle WARP (F4)", 35, iY += 24, 125, 22, VK_F4 );

    g_SampleUI.SetCallback( OnGUIEvent ); iY = 10;


	//**********************************************************************//
	// Zero the memory block used by the controller.						//
	//**********************************************************************//
    ZeroMemory( g_Controllers, sizeof( CONTROLER_STATE ) * MAX_CONTROLLERS );

}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
    g_pTxtHelper->Begin();
    g_pTxtHelper->SetInsertionPos( 5, 5 );
    g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    g_pTxtHelper->DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
    g_pTxtHelper->DrawTextLine( DXUTGetDeviceStats() );



	//**********************************************************************//
	// Write controller related stuff.  As usual this is not good from a    //
	// performance point of view as we are calling this code every frame.   //
	//**********************************************************************//

	WCHAR strBuffer[50];
	int numControllers = 0;
	for (int i = 0; i < MAX_CONTROLLERS; i++)
		if (g_Controllers[i].connected) numControllers++;

	swprintf(strBuffer, L"Num Controllers found: %d", numControllers);
	g_pTxtHelper->DrawTextLine(strBuffer);

    g_pTxtHelper->End();
}


//--------------------------------------------------------------------------------------
// Reject any D3D10 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D10 resources that aren't dependent on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext )
{
	g_p_d3dDevice = pd3dDevice;
	g_MeshProducer = new TMeshProducer(pd3dDevice);

    HRESULT hr;

    V_RETURN( D3DX10CreateSprite( pd3dDevice, 500, &g_pSprite10 ) );
    V_RETURN( g_DialogResourceManager.OnD3D10CreateDevice( pd3dDevice ) );
    V_RETURN( g_SettingsDlg.OnD3D10CreateDevice( pd3dDevice ) );
    V_RETURN( D3DX10CreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                                OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                L"Arial", &g_pFont10 ) );
    g_pTxtHelper = new CDXUTTextHelper( NULL, NULL, g_pFont10, g_pSprite10, 15 );


	//**********************************************************************//
	// We use the camera object to store the View and Projection matrices.  //
	// The projection matrix is created in OnD3D10ResizedSwapChain().		//
	// Setup the camera's view parameters here, which creates the view		//
	// matrix.																//
	//**********************************************************************//
    
	D3DXVECTOR3 vecEye( 0.0f, 1.0f, -5.0f );
    D3DXVECTOR3 vecAt ( 0.0f, 0.0f, 0.0f );
    g_Camera.SetViewParams( &vecEye, &vecAt );


	//**********************************************************************//
	// Vertex format for our mesh.  The vertices are not coloured, but 		//
	// there is a texture u and v here.										//
	//**********************************************************************//

	g_p_TEffect = new TEffect(pd3dDevice,L"Coursework Skeleton10.fx");

	texture = new TTexture(pd3dDevice,g_p_TEffect,g_p_TEffect->g_p_TechniqueRenderScene,L"Media\\seafloor.dds");

	for(int x = 0; x < 20; x += 2){
		for(int z = 0; z < 20; z += 2){
			TObject2D *tile = new TObject2D(pd3dDevice,g_p_TEffect,g_p_TEffect->g_p_TechniqueRenderScene,texture);
			tile->position->MoveTo(x,0,z);
			tiles.push_back(tile);
		}
	}

	g_NewTiger = new TObject3D(pd3dDevice,g_p_TEffect,g_p_TEffect->g_p_TechniqueRenderScene,g_MeshProducer->ProduceTiger());
	g_NewTiger->position->MoveTo(0,0.70,0);
	g_NewTiger->position->RotateToDeg(0,0,0);
	g_NewTiger->position->ScaleTo(1,1,1);

	g_LWing = new TObject3D(pd3dDevice, g_p_TEffect, g_p_TEffect->g_p_TechniqueRenderWing, g_MeshProducer->ProduceWing());
	g_RWing = new TObject3D(pd3dDevice, g_p_TEffect, g_p_TEffect->g_p_TechniqueRenderWing, g_MeshProducer->ProduceWing());
	
	g_LWing->position->RotateToDeg(0,180,0);
	g_LWing->position->MoveBy(-0.2, 0.4, -0.5);
	g_RWing->position->MoveBy(0.2, 0.4, -0.5);

	g_NewTiger->children.push_back(g_LWing);
	g_NewTiger->children.push_back(g_RWing);

	g_NewSkyBox = new TObject3D(pd3dDevice,g_p_TEffect,g_p_TEffect->g_p_TechniqueRenderSkyBox, g_MeshProducer->ProduceSkyBox());
	g_NewSkyBox->position->MoveTo(0,0,0);
	g_NewSkyBox->position->RotateToDeg(0,0,0);
	g_NewSkyBox->position->ScaleTo(0.1,0.1,0.1);

	//V( g_MeshTeapot.Create( pd3dDevice, L"Media\\Teapot.sdkmesh", true ) );

	//**********************************************************************//
	// Create the sounds.													//
	//**********************************************************************//

	V(soundManager.init());
	V(sound1.createSoundFromFile(L"Media\\Clank.wav", 1));
	
	sound2.numAudioChannels = 1;		//I'm afraid you have to guess these
	sound2.bitsPerSample = 8;
	sound2.samplingRate = 12000;
	V(sound2.createSoundFromFile(L"Media\\Horse.wav", 2));
	
	sound3.numAudioChannels = 1;		//I'm afraid you have to guess these
	sound3.bitsPerSample = 8;
	sound3.samplingRate = 24000;

	V(sound3.createSoundFromFile(L"Media\\Bark.wav", 1));

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D10 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnD3D10ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
    V_RETURN( g_SettingsDlg.OnD3D10ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );



 	//**********************************************************************//
	// Create the projection matrix.	Again, the projection matrix is		//
	// contained within the camera.											//
	//**********************************************************************//
    float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );


    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
    g_Camera.SetButtonMasks( MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON );

    g_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
    g_HUD.SetSize( 170, 170 );
    g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 300 );
    g_SampleUI.SetSize( 170, 300 );

    return S_OK;
}


//**************************************************************************//
// Render the scene using the D3D10 device.									//
// As this skeleton is in DirectX10, this is the one we are using.			//
//**************************************************************************//

void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	//Variables to hold the view and projection matrices
    D3DXMATRIX matView;
    D3DXMATRIX matProj;

	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	// This is Generic Code used to setup the view and give it a 'blueish' background

    float ClearColor[4] = { 0.176f, 0.196f, 0.667f, 0.0f };
    ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
    pd3dDevice->ClearRenderTargetView( pRTV, ClearColor );

    // Clear the depth stencil
    ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
    pd3dDevice->ClearDepthStencilView( pDSV, D3D10_CLEAR_DEPTH, 1.0, 0 );

    // If the settings dialog is being shown, then render it instead of rendering the app's scene
    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.OnRender( fElapsedTime );
        return;
    }

	
	//****************************************************************************///
	// Get the projection & view matrix, which are now hidden in the camera class.//
	//****************************************************************************//
    
    matProj = *g_Camera.GetProjMatrix();
    matView = *g_Camera.GetViewMatrix();
	g_p_TEffect->g_p_fTimeInShader->SetFloat(             ( float )fTime );

	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	// Render all of my models
	
	//Setup the View Projection matrix, and create a simple identity matrix
	D3DXMATRIX vp = matView*matProj;
	D3DXMATRIX ident;
	D3DXMatrixIdentity(&ident);

	//Render the sky and the tiger
	g_NewTiger->Render(vp,ident);
	g_NewSkyBox->Render(vp, ident);

	//Render all of the tiles
	for(std::vector<TObject2D *>::const_iterator it = tiles.begin(); it != tiles.end(); it++){
		(*it)->Render(vp,ident);
	}

	//Render all of the balls
	for(std::vector<TBall *>::const_iterator it = balls.begin(); it != balls.end(); it++){
		(*it)->Render(vp,ident);
	}


	////////////////////////////////////////////////////////////////////////////////
	// Update the Follow Cam
	////////////////////////////////////////////////////////////////////////////////

	//**************************************************************//
	//Put the camera on the thing.   We have to create a new viewer //
	// matrix.  Here the viewer matris is hidden in the camera.  In //
	// fact the camera class Microsoft give us can follow objects	//
	// anyway, if somebody can find out how!						//
	//**************************************************************//

	D3DXVECTOR3 viewerPos;
	D3DXVECTOR3 lookAtThis;
	D3DXVECTOR3 up         ( 0.0f, 1.0f, 0.0f );
	D3DXVECTOR3 newUp;

	//Set the viewer's position to the position of the thing.

	viewerPos.x = g_NewTiger->position->m_x;   viewerPos.y = g_NewTiger->position->m_y;
	viewerPos.z = g_NewTiger->position->m_z;

	//*****************************************************************//
	// Create a new vector for the direction for the virwer to look in.//
	// We do this by getting the object's initial direction vector,    //
	// and multiplying it by the thing's combined x, y, z rotation     //
	// matrices.   Up is still up; I'll leave that to you!             //
	//*****************************************************************//

	g_NewTiger->setupWorldMatrices();

	D3DXVECTOR3 newDir, lookAtPoint;
	D3DXVec3TransformCoord(&newDir, &g_NewTiger->position->initVecDir, &g_NewTiger->getRotationMatrix());

	// The viewer should now be looking at a point a little in front//
	// of the object.   I hope.   Nigel.                            //

	D3DXVec3Normalize(&newDir, &newDir);
			
	newDir *= -10;

	viewerPos += newDir;
	viewerPos.y += 3;
			
	lookAtPoint.x = g_NewTiger->position->m_x;
	lookAtPoint.y = g_NewTiger->position->m_y;
	lookAtPoint.z = g_NewTiger->position->m_z;

	g_Camera.SetViewParams(&viewerPos, &lookAtPoint);


   
	//**********************************************************************//
	// Update the status of the sounds.										//
	//**********************************************************************//

	sound1.updateStatus();
	sound2.updateStatus();
	sound3.updateStatus();
	sound1.pXACTWave->GetState(&sound1State);


	//****************************************************************************//
	// And finally, render the DXUT buttons and the thing which grandly calls	  //
	// itself a HUD (Head Up Display).											  //
	//****************************************************************************//

    DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" );
    RenderText();
    g_HUD.OnRender( fElapsedTime );
    g_SampleUI.OnRender( fElapsedTime );

    DXUT_EndPerfEvent();
}



//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D10ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10ReleasingSwapChain( void* pUserContext )
{
    g_DialogResourceManager.OnD3D10ReleasingSwapChain();
}


//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D10CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10DestroyDevice( void* pUserContext )
{
    g_DialogResourceManager.OnD3D10DestroyDevice();
    g_SettingsDlg.OnD3D10DestroyDevice();
    SAFE_RELEASE( g_pFont10 );
    //    SAFE_RELEASE( g_p_Effect );
    SAFE_RELEASE( g_pVertexLayout );
    SAFE_RELEASE( g_pSprite10 );
    SAFE_DELETE( g_pTxtHelper );
	SAFE_DELETE(g_NewTiger);
	SAFE_DELETE(g_NewSkyBox);
	SAFE_DELETE(g_LWing);
	SAFE_DELETE(g_RWing);
	SAFE_DELETE(g_p_TEffect);
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    if( pDeviceSettings->ver == DXUT_D3D9_DEVICE )
    {
        IDirect3D9* pD3D = DXUTGetD3D9Object();
        D3DCAPS9 Caps;
        pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal, pDeviceSettings->d3d9.DeviceType, &Caps );

        // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
        // then switch to SWVP.
        if( ( Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
            Caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
        {
            pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }

        // Debugging vertex shaders requires either REF or software vertex processing 
        // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
        if( pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF )
        {
            pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
            pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
            pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }
#endif
#ifdef DEBUG_PS
        pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif
    }

    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( ( DXUT_D3D9_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF ) ||
            ( DXUT_D3D10_DEVICE == pDeviceSettings->ver &&
              pDeviceSettings->d3d10.DriverType == D3D10_DRIVER_TYPE_REFERENCE ) )
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
    }

    return true;
}


//**************************************************************************//
// Handle updates to the scene.  This is called regardless of which D3D API //
// is used.																	//
//**************************************************************************//

void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    // Update the camera's position based on user input 
    g_Camera.FrameMove( fElapsedTime );
	UpdateControllerState(fElapsedTime);

	for(std::vector<TBall *>::const_iterator it = balls.begin(); it != balls.end(); it++){
		(*it)->update(fElapsedTime);
	}

	//**********************************************************************//
	// Process key presses.													//
	//**********************************************************************//

	fElapsedTime *= 2;
	
	if (g_b_LeftArrowDown)  g_NewTiger->position->RotateByRad(0,-fElapsedTime,0);	//Rotate about y in a frame rate independent
	if (g_b_RightArrowDown) g_NewTiger->position->RotateByRad(0,fElapsedTime,0); //way.  Case is not used as it is possible 	
	if (g_b_UpArrowDown)    g_NewTiger->position->RotateByRad(fElapsedTime,0,0);	//that several keys could be down at once.
	if (g_b_DownArrowDown)  g_NewTiger->position->RotateByRad(-fElapsedTime,0,0);
	if (g_b_W)			g_NewTiger->position->MoveForward(fElapsedTime*2);

	if (g_b_Space){
			TBall *ball = new TBall(g_p_d3dDevice,g_p_TEffect,g_p_TEffect->g_p_TechniqueRenderScene, g_MeshProducer->ProduceTiger());
			ball->position->MoveTo(g_NewTiger->position->m_x,g_NewTiger->position->m_y,g_NewTiger->position->m_z);
			balls.push_back(ball);
	}

	if(g_NewTiger->position->m_y < 0.55){
		g_Rotating = true;
		g_Moving = true;
	}
	if(g_Moving){
		g_NewTiger->position->MoveBy(0,0.05,0);
		xyz pos = g_NewTiger->position->GetPositionXYZ();
		if(pos.y > 0.8) g_Moving = false;
	}
	if(g_Rotating){
		g_NewTiger->position->RotateByDeg(1,0,0);
		xyz pos = g_NewTiger->position->GetRotationXYZ();
		if(pos.x > 0) g_Rotating = false;
	}
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{
    // Pass messages to dialog resource manager calls so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass messages to settings dialog if its active
    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass all remaining windows messages to camera so it can respond to user input
    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}


//****************************************************************************//
// This doesn't give us the lovely remapable keys that the old common		  //
// framework classes give us.  And you need to look in the header file		  //
// "WinUser.h" to see what the keys mean.  Right click on, say, VK_F1, to open//
// the header file.															  //
//																			  //
// The "bKeyDown" parameter tells us wheather the key was pressed or released.//
//****************************************************************************//

void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
switch( nChar )
    {
        
		//******************************************************************//
		// Nigel code to rotate the tiger.									//
		//******************************************************************//

		case VK_LEFT:  g_b_LeftArrowDown  = bKeyDown; break;
		case VK_RIGHT: g_b_RightArrowDown = bKeyDown; break;
		case VK_UP:    g_b_UpArrowDown    = bKeyDown; break;
		case VK_DOWN:  g_b_DownArrowDown  = bKeyDown; break;
		case VK_SPACE:  
			g_b_Space		  = bKeyDown; 
			sound1.play();
			break;
		case 'W':	   g_b_W			  = bKeyDown; break;

    }

/*	switch (nChar) 
	{
		case '1': 
			hr = sound1.play(); 
			//sound1.stop(XACT_FLAG_STOP_RELEASE);
			break;
		case '2': 
			sound2.play();
			//sound2.stop(XACT_FLAG_STOP_RELEASE); 
			break;
		case '3': 
			sound3.play(); 
			//sound3.stop(XACT_FLAG_STOP_RELEASE); 
			break;

		case 'S': 
			hr = sound1.stop(XACT_FLAG_STOP_RELEASE); 
			break;


	} */

}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN:
            DXUTToggleFullScreen(); break;
        case IDC_TOGGLEREF:
            DXUTToggleREF(); break;
        case IDC_TOGGLEWARP:
            DXUTToggleWARP(); break;
        case IDC_CHANGEDEVICE:
            g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;
    }
}



//****************************************************************************//
// Polls the Xbox controllers (if present) and uptates their state into a	  //
// global array g_Controllers.												  //
//****************************************************************************//

void UpdateControllerState(float frameTime)
{
    DWORD dwResult;
    for( DWORD i = 0; i < MAX_CONTROLLERS; i++ )
    {
        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState( i, &g_Controllers[i].state );

        if( dwResult == ERROR_SUCCESS )
            g_Controllers[i].connected = true;
        else
            g_Controllers[i].connected = false;
    }


	//**********************************************************************//
	// We only use controller[0].	Look in the state.GamePad variable to   //
	// see what is available.												//
	//**********************************************************************//

	if (g_Controllers[0].connected)
	{
		float ly = g_Controllers[0].state.Gamepad.sThumbLX;	//Left Joypad 
		float lx = g_Controllers[0].state.Gamepad.sThumbLY; //Left Joypad 

		float ry = g_Controllers[0].state.Gamepad.sThumbRX;	//Left Joypad 
		float rx = g_Controllers[0].state.Gamepad.sThumbRY; //Left Joypad

		//**********************************************************************//
		// Joypads have a dead zone.  To prevent noise, zero the reading if the //
		// answer is within the dead zone.										//
		//**********************************************************************//

		if ((ry < INPUT_DEADZONE) && (ry > -INPUT_DEADZONE )) ry = 0;
		if ((rx < INPUT_DEADZONE) && (rx > -INPUT_DEADZONE )) rx = 0;

		if ((ly < INPUT_DEADZONE) && (ly > -INPUT_DEADZONE )) ly = 0;
		if ((lx < INPUT_DEADZONE) && (lx > -INPUT_DEADZONE )) lx = 0;

		//**********************************************************************//
		// Scale the result.  Full scale is + / - 32767.  Multiply by 90  for a //
		// max ov 90 degrees and convert to radians.							//
		//**********************************************************************//

		g_NewTiger->position->RotateByRad(0, D3DXToRadian(( ry / (12767.0))), 0);
		g_NewTiger->position->RotateByRad( D3DXToRadian((rx / (12767.0))),0,0);

		if(lx > 0) g_NewTiger->position->MoveForward(frameTime*4);
	}
}
