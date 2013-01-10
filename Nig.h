//**************************************************************************//
// Quirky header file ©Nigel Barlow email: nigel@soc.plymouth.ac.uk.		//
//**************************************************************************//

#ifndef NigH     //These are termed "guards", can you see what they do?
#define NigH	 

#include <windows.h>
#include <stdio.h>


#define ShowMessage(msg) MessageBox(0, msg, L"Oops", MB_OK)

#define random(n) ((float)rand()/(float)RAND_MAX*(float)n)




#endif		//End guard.

