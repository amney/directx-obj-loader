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
#include <xact3.h>
#include "NigSoundManager.h"


#ifndef NigSoundH   //Guards, as usual.
#define NigSoundH



class NigSound
{
	//**********************************************************************//
	// Public instance variables.  OO purists will want to make these		//
	// private and write the setThis() and getThat() methods.				//
	//**********************************************************************//

	public:
		int soundDurationInSeconds;
		int samplingRate;
		int numAudioChannels;
		int bitsPerSample;
		DWORD timeStarted;
		int elapsedTime;
		bool isStreaming;
   		WCHAR soundFileName[50];

		IXACT3Wave* pXACTWave;
		BYTE* pWaveBuffer;
		HANDLE hStreamingFile;
		DWORD dwState;

		NigSoundManager *pSoundManager;


	//**********************************************************************//
	// Public methods.														//
	//**********************************************************************//

   public:
	   NigSound();	//Constructor.
	   NigSound(NigSoundManager *manager);
	   ~NigSound();	//Destructor.


		//**********************************************************************//
		// The (easy??) entry point for you.  Create a sound from the file		//
		// specified with the given duration is seconds.						//
		//																		//
		// I recommend streaming sounds, and the (optional) streaming parameter	//
		// defaults to true.													//
		//**********************************************************************//

		HRESULT createSoundFromFile(WCHAR *fileName, int durationInSeconds, 
		                           bool streaming = true);
		//**********************************************************************//
		// Play the sound.														//
		//**********************************************************************//

		HRESULT play();

		HRESULT stop(DWORD flags);

		void updateStatus();

		HRESULT resetSound();
	   
		HRESULT LoadWaveFileInMemory( IXACT3Engine* pXACTEngine, WCHAR* strFile, DWORD dwSeconds,
                              DWORD dwSampleRate, DWORD dwChannels, DWORD dwBits,
                              IXACT3Wave** ppXACTWave, BYTE** ppWaveBuffer );

		HRESULT LoadWaveFileStreaming( IXACT3Engine* pXACTEngine, WCHAR* strFile, DWORD dwSeconds,
                               DWORD dwSampleRate, DWORD dwChannels, DWORD dwBits,
                               IXACT3Wave** ppXACTWave, HANDLE* phStreamingFile );

};    //Must be a ";" here in C++ - weird, eh?   Nigel


#endif	//End of guard
