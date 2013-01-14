#include "DXUT.h"
#include "Nig.h"
#include "TEffect.h"

TEffect::TEffect( ID3D10Device *device, LPCTSTR fileName )
{
	this->device = device;

	HRESULT hr;
	ID3D10Blob *errors;

	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	// first, see if the effect file exists.
	WCHAR path[MAX_PATH];
	hr =  DXUTFindDXSDKMediaFileCch( path, MAX_PATH, fileName );
	V(hr);


	hr = D3DX10CreateEffectFromFile( path, NULL, NULL, "fx_4_0", 
		dwShaderFlags, 0, device, NULL,
		NULL, &g_p_Effect, &errors, NULL );



	//**********************************************************************//
	// This code is simply to extract any errors that occur when compiling  //
	// the ".fx" fine and displaying them in a message box so that we can   //
	// see them.															//
	//**********************************************************************//

	if (FAILED(hr))
	{
		WCHAR errorWStr[500];
		char  *errorCharStr;
		char  errorCharStrFull[400];

		if (errors == NULL) V(hr);		//Probably file not found.

		errorCharStr = (char *) errors->GetBufferPointer();

		strcpy_s(errorCharStrFull, "Error compiling FX file\n\n");
		strcat_s(errorCharStrFull, errorCharStr);

		charStrToWideChar(errorWStr, errorCharStrFull);
		ShowMessage(errorWStr);
	}



	g_lightDirection = D3DXVECTOR4(0, 1, -1, 0);				//w value unused.
	g_lightDiffuseColour = D3DXVECTOR4(1, 1, 1, 1);			//w value unused.
	g_materialDiffuseColour = D3DXVECTOR4(1, 1, 1, 1);		//w value unused.



	//**********************************************************************//
	// Obtain the technique and other shader variables.						//
	//**********************************************************************//

    g_p_TechniqueRenderScene = g_p_Effect->GetTechniqueByName( "RenderScene" );
	g_p_TechniqueRenderWing  = g_p_Effect->GetTechniqueByName( "RenderWing" );
	g_p_TechniqueRenderSkyBox  = g_p_Effect->GetTechniqueByName( "RenderSkyBox" );

    g_p_txDiffuseInShader  = g_p_Effect->GetVariableByName( "g_MeshTexture" )->AsShaderResource();
    
	g_p_MatWorldViewProjInShader = g_p_Effect->GetVariableByName( "g_mWorldViewProjection" )->AsMatrix();
    g_p_MatWorldInShader         = g_p_Effect->GetVariableByName( "g_mWorld" )->AsMatrix();
    g_p_fTimeInShader            = g_p_Effect->GetVariableByName( "g_fTime" )->AsScalar();
    g_p_fWingAngleInShader       = g_p_Effect->GetVariableByName( "g_fWingAngle" )->AsScalar();
	
	g_p_LightDiffuseColourInShader     = g_p_Effect->GetVariableByName ("g_LightDiffuse")->AsVector();
	g_p_LightDirectionInShader		   = g_p_Effect->GetVariableByName ("g_LightDir")->AsVector();
	g_p_MaterialDiffuseColourInShader  = g_p_Effect->GetVariableByName ("g_MaterialDiffuseColor")->AsVector();

	g_p_LightDirectionInShader->SetFloatVector(      (float *) &g_lightDirection);
	g_p_LightDiffuseColourInShader->SetFloatVector(  (float *) &g_lightDiffuseColour);
	g_p_MaterialDiffuseColourInShader->SetFloatVector((float *) &g_materialDiffuseColour);


	const D3D10_INPUT_ELEMENT_DESC layout[] =
	{
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};
  	   
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

    D3D10_PASS_DESC PassDesc;
    g_p_TechniqueRenderScene->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	g_p_TechniqueRenderWing->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	g_p_TechniqueRenderSkyBox->GetPassByIndex( 0 )->GetDesc( &PassDesc );

    device->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature,
                                             PassDesc.IAInputSignatureSize, &g_pVertexLayout );

    // Set the input layout
    device->IASetInputLayout( g_pVertexLayout );
}


TEffect::~TEffect(void)
{
	SAFE_RELEASE(g_pVertexLayout);
	SAFE_RELEASE(g_p_Effect);
}


//**************************************************************************//
// Convert an old chrtacter (char *) string to a WCHAR * string.  There must//
// be something built into Visual Studio to do this for me, but I can't		//
// find it - Nigel.															//
//**************************************************************************//

void TEffect::charStrToWideChar(WCHAR *dest, char *source)
{
	int length = strlen(source);
	for (int i = 0; i <= length; i++)
		dest[i] = (WCHAR) source[i];
}
