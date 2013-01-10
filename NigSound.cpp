//**************************************************************************//
// Crude Sound to go with the Sound Manager class, heavily based on			//
// Microsoft's sample code.													//
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
#include "NigSound.h"
#include "SDKmisc.h"



//**************************************************************************//
// Constructors and destructors.											//
//**************************************************************************//

NigSound::NigSound()
{
	soundDurationInSeconds = 2;
	samplingRate		   = 44100;
	numAudioChannels       = 2;
	bitsPerSample		   = 16;

	pXACTWave			   = NULL;
	pWaveBuffer            = NULL;
	hStreamingFile         = NULL;
	dwState                = 0;
}

NigSound::NigSound(NigSoundManager *manager)
{
	soundDurationInSeconds = 2;
	samplingRate		   = 44100;
	numAudioChannels       = 2;
	bitsPerSample			= 16;
	pXACTWave			   = NULL;
	pWaveBuffer            = NULL;
	hStreamingFile         = NULL;
	dwState                = 0;
	pSoundManager		   = manager;
}


NigSound::~NigSound()
{
}


//**************************************************************************//
// The (easy??) entry point for you.  Create a sound from the file specified//
// with the given duration is seconds.										//
//																			//
// I recommend streaming sounds, and the (optional) streaming parameter		//
// defaults to true.														//
//**************************************************************************//

HRESULT NigSound::createSoundFromFile(WCHAR *fileName, int durationInSeconds, 
									  bool streaming)
{
	HRESULT hr;
	isStreaming = streaming;
	wcscpy(soundFileName, fileName);
	soundDurationInSeconds = durationInSeconds;

	if (isStreaming)
	{
		hr = LoadWaveFileStreaming(pSoundManager->pXACTEngine, fileName, durationInSeconds, 
		samplingRate, numAudioChannels, bitsPerSample, &pXACTWave, &hStreamingFile);

		if (hr != S_OK) return hr;
	}
	else
	{
		hr = LoadWaveFileInMemory(pSoundManager->pXACTEngine, fileName, durationInSeconds,
			samplingRate, numAudioChannels, bitsPerSample, &pXACTWave, &pWaveBuffer);
	
		if (hr != S_OK) return hr;
	}
	
	return hr;
}



//**************************************************************************//
// Play the sound.															//
//**************************************************************************//

HRESULT NigSound::play()
{
	timeStarted = timeGetTime();
	return pXACTWave->Play();
}

HRESULT NigSound::stop(DWORD flags)
{
	return pXACTWave->Stop(flags);

}



//**************************************************************************//
// Check the status of the sound, wheather it is ready, wheather it has		//
// expired, that sort of thing.												//
//**************************************************************************//

void NigSound::updateStatus()
{
	//**********************************************************************//
	// The preparation of the sound is non-blocking; it doesn't sit and		//
	// wait.  This is a bit of a waste of a non-blocking finction;			//
	// Call DoWork until the wave is fully prepared.						//
	//**********************************************************************//

	DWORD state;
	pXACTWave->GetState(&state);
	
	if (state == XACT_STATE_STOPPED) 
	{
		resetSound();
		return;
	}

	elapsedTime = (timeGetTime() - timeStarted)/1000;
	
	if (elapsedTime >= soundDurationInSeconds) 
	{
		if (state == XACT_STATE_STOPPED || state == XACT_STATE_PLAYING)	
		{
			stop(XACT_FLAG_STOP_RELEASE);
			resetSound();
		}
	}

	
	if (state != XACT_STATE_PREPARED )
		pSoundManager->pXACTEngine->DoWork();
}





//**************************************************************************//
// Can this be true?  We have to re-create the sound in order to play it	//
// again?																	//
//**************************************************************************//

HRESULT NigSound::resetSound()
{
	HRESULT hr;

	if (isStreaming)
	{
		CloseHandle(hStreamingFile);

		hr = LoadWaveFileStreaming(pSoundManager->pXACTEngine, soundFileName, soundDurationInSeconds, 
		samplingRate, numAudioChannels, bitsPerSample, &pXACTWave, &hStreamingFile);

		pSoundManager->pXACTEngine->DoWork();
		if (hr != S_OK) return hr;
	}
	else
	{
		hr = LoadWaveFileInMemory(pSoundManager->pXACTEngine, soundFileName, soundDurationInSeconds,
			samplingRate, numAudioChannels, bitsPerSample, &pXACTWave, &pWaveBuffer);
	
		if (hr != S_OK) return hr;
	}
}




//**************************************************************************//
// Load PCM wave data into a in-memory IXACT3Wave*.							//
//**************************************************************************//

HRESULT NigSound::LoadWaveFileInMemory( IXACT3Engine* pXACTEngine, WCHAR* strFile, DWORD dwSeconds,
                              DWORD dwSampleRate, DWORD dwChannels, DWORD dwBits,
                              IXACT3Wave** ppXACTWave, BYTE** ppWaveBuffer )
{
    HRESULT hr;

    WCHAR strFilePath[MAX_PATH];
    if( FAILED( hr = DXUTFindDXSDKMediaFileCch( strFilePath, MAX_PATH, strFile ) ) )
        return hr;

    // Open PCM audio data in file
    HANDLE hFile = CreateFile( strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0 );
    if( hFile == INVALID_HANDLE_VALUE )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

    DWORD dwBlockAlign = dwChannels * dwBits / 8;
    DWORD dwTotalNumBytes = dwSeconds * dwSampleRate * dwBlockAlign;

    // Initialize WAVEBANKENTRY struct
    WAVEBANKENTRY entry;
    entry.Format.wFormatTag = WAVEBANKMINIFORMAT_TAG_PCM;
    entry.Format.wBitsPerSample = ( dwBits == 16 ) ? WAVEBANKMINIFORMAT_BITDEPTH_16 : WAVEBANKMINIFORMAT_BITDEPTH_8;
    entry.Format.nChannels = dwChannels;
    entry.Format.wBlockAlign = dwChannels * ( dwBits / 8 );
    entry.Format.nSamplesPerSec = dwSampleRate;
    entry.Duration = dwSeconds * dwSampleRate;
    entry.LoopRegion.dwStartSample = 0;
    entry.LoopRegion.dwTotalSamples = 0;
    entry.PlayRegion.dwOffset = 0;
    entry.PlayRegion.dwLength = dwTotalNumBytes;
    entry.dwFlags = 0;

    BYTE* pXACTWaveBuffer = new BYTE[dwTotalNumBytes];
    DWORD dwBytesRead = 0;

    // Read PCM audio data from a file
    ReadFile( hFile, pXACTWaveBuffer, 44, &dwBytesRead, NULL );  // Skip wave format chunk
    ReadFile( hFile, pXACTWaveBuffer, dwTotalNumBytes, &dwBytesRead, NULL );
    if( dwBytesRead >= dwTotalNumBytes )
    {
        SAFE_DELETE_ARRAY( pXACTWaveBuffer );
        CloseHandle( hFile );
        return E_FAIL;
    }
    CloseHandle( hFile );

    // Create an in-memory IXACTWave interface using wave file data
    if( FAILED( hr = pXACTEngine->PrepareInMemoryWave( XACT_FLAG_UNITS_MS, entry, NULL,
                                                       pXACTWaveBuffer, 0, 0, ppXACTWave ) ) )
    {
        SAFE_DELETE_ARRAY( pXACTWaveBuffer );
        return hr;
    }

    return S_OK;
}





//**************************************************************************//
// Load PCM wave data into a streaming IXACT3Wave*							//
//**************************************************************************//

HRESULT NigSound::LoadWaveFileStreaming( IXACT3Engine* pXACTEngine, WCHAR* strFile, DWORD dwSeconds,
                               DWORD dwSampleRate, DWORD dwChannels, DWORD dwBits,
                               IXACT3Wave** ppXACTWave, HANDLE* phStreamingFile )
{
    HRESULT hr;

    WCHAR strFilePath[MAX_PATH];
    if( FAILED( hr = DXUTFindDXSDKMediaFileCch( strFilePath, MAX_PATH, strFile ) ) )
        return hr;

    // Open PCM audio data in file
    HANDLE hFile = CreateFile( strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0 );
    if( hFile == INVALID_HANDLE_VALUE )
        return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

    DWORD dwBlockAlign = dwChannels * dwBits / 8;
    DWORD dwTotalNumBytes = dwSeconds * dwSampleRate * dwBlockAlign;

    // Initialize WAVEBANKENTRY struct
    WAVEBANKENTRY entry;
    entry.Format.wFormatTag = WAVEBANKMINIFORMAT_TAG_PCM;
    entry.Format.wBitsPerSample = ( dwBits == 16 ) ? WAVEBANKMINIFORMAT_BITDEPTH_16 : WAVEBANKMINIFORMAT_BITDEPTH_8;
    entry.Format.nChannels = dwChannels;
    entry.Format.wBlockAlign = dwChannels * ( dwBits / 8 );
    entry.Format.nSamplesPerSec = dwSampleRate;
    entry.Duration = dwSeconds * dwSampleRate;
    entry.LoopRegion.dwStartSample = 0;
    entry.LoopRegion.dwTotalSamples = 0;
    entry.PlayRegion.dwOffset = 0;
    entry.PlayRegion.dwLength = dwTotalNumBytes;
    entry.dwFlags = 0;

    // Load PCM into an streaming IXACTWave
    *phStreamingFile = hFile;

    XACT_STREAMING_PARAMETERS streamingParams;
    streamingParams.file = hFile;
    streamingParams.flags = 0;
    streamingParams.offset = 44;
    streamingParams.packetSize = ( WORD )dwBits;

    // Create an streaming IXACTWave interface using wave file data
    if( FAILED( hr = pXACTEngine->PrepareStreamingWave( XACT_FLAG_UNITS_MS, entry, streamingParams,
                                                        4096, NULL, 0, 0, ppXACTWave ) ) )
    {
        return hr;
    }

    return S_OK;
}
