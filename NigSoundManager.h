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
#include <xact3.h>


#ifndef NigSoundManagerH   //Guards, as usual.
#define NigSoundManagerH



class NigSoundManager
{

	//**********************************************************************//
	// Public instance variables.  OO purists will want to make these		//
	// private and write the setThis() and getThat() methods.				//
	//**********************************************************************//

	public:
		IXACT3Engine* pXACTEngine;


	//**********************************************************************//
	// Public methods.														//
	//**********************************************************************//

   public:
	   NigSoundManager();	//Constructor.
	   ~NigSoundManager();	//Destructor.

	   HRESULT init();

};    //Must be a ";" here in C++ - weird, eh?   Nigel


#endif	//End of guard
