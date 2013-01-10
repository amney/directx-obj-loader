//**************************************************************************//
// Crude Sound Manager class, heavily based on Microsoft's sample code.		//
//**************************************************************************//

//**************************************************************************//
// This code is copyright of Dr Nigel Barlow, lecturer in computing,		//
// University of Plymouth, UK.  email: nigel@soc.plymouth.ac.uk.			//
//																			//
// You may use, modify and distribute this (rather cack-handed in places)	//
// code subject to the following conditions:								//
//																			//
//	1:	You may not use it, or sell it, or use it in any adapted form for	//
//		financial gain, without my written premission.						//
//																			//
//	2:	You must not remove the copyright messages.							//
//																			//
//	3:	You should correct at least 10% of the typig abd spekking errirs.   //
//**************************************************************************//

#include "DXUT.h"
#include "NigSoundManager.h"

//**************************************************************************//
// Constructors and destructors.											//
//**************************************************************************//

NigSoundManager::NigSoundManager()
{
}


NigSoundManager::~NigSoundManager(){
}



//**************************************************************************//
// Initialize XACT engine.													//
//**************************************************************************//

HRESULT NigSoundManager::init()
{
{
    // Init COM and create XACT engine
    HRESULT hr;
    hr = CoInitializeEx( NULL, COINIT_MULTITHREADED );
    if( SUCCEEDED( hr ) )
    {
        // Switch to auditioning mode based on command line.  Change if desired
        bool bAuditionMode = false;
		bool bDebugMode = false;

        DWORD dwCreationFlags = 0;
        if( bAuditionMode ) dwCreationFlags |= XACT_FLAG_API_AUDITION_MODE;
        if( bDebugMode ) dwCreationFlags |= XACT_FLAG_API_DEBUG_MODE;

        hr = XACT3CreateEngine( dwCreationFlags, &pXACTEngine );
    }
    if( FAILED( hr ) || pXACTEngine == NULL )
        return E_FAIL;

    XACT_RENDERER_DETAILS rendererDetails = {0};
    if( FAILED( hr = ( pXACTEngine )->GetRendererDetails( 0, &rendererDetails ) ) )
        return hr;

    XACT_RUNTIME_PARAMETERS xrParams = {0};
    xrParams.fnNotificationCallback = NULL;
    xrParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
    xrParams.pRendererID = rendererDetails.rendererID;

    if( FAILED( hr = ( pXACTEngine )->Initialize( &xrParams ) ) )
        return hr;

    return S_OK;
}


}
