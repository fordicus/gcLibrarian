#pragma once
#include <math.h>
#include <sstream>


//==================================================================================
// Personal Defines:		GENERIC
//==================================================================================
#define DELAY_5ms				5
#define DELAY_10ms				10
#define DELAY_50ms				50
#define DELAY_100ms				100
#define DELAY_150ms				150
#define DELAY_200ms				200
#define DELAY_250ms				250
#define DELAY_500ms				500
#define DELAY_1000ms			1000
#define kindleDELAY				500
#define LibrarianDELAY			3000
#define drawTimer				100
#define maxThreads				8
#define maxDetectors			2

#define gcLibrarian				L"gcLibrarian"
#define testPath				"C:\\_test\\"
#define consoleSweep			"\r\n \r\n \r\n \r\n "
#define notify_ONE				"Allow single Process."
#define notify_END				"the End."


//==================================================================================
// Personal Defines:		GENERIC
//==================================================================================
#define	WM_TRAY_NOTIFICATION	WM_APP + 1


//==================================================================================
// Error Suppression
//==================================================================================
#define NOMINMAX									 //inhibit definition @Windows.h

#pragma once
	typedef unsigned int uint;

#ifndef TRUE
	#define TRUE	true
	#define FALSE	false
#endif


//==================================================================================
// Extensions
//==================================================================================
enum gcEXT : int
{
	DspData	= 0,	//".gcDSP"
	iniTxT	= 1,	//".gctINI"
	iniBin	= 2		//".gcbINI"
};


//==================================================================================
// Macros
//==================================================================================
#define gcQueryStackSize	( array )	( sizeof( array )/sizeof( *array ) )
#define gcShowDesktopPath	()			MessageBox(to_cstring(gcQueryDesktopPath()), gcLibrarian, 0)


//==================================================================================
// Constants
//==================================================================================
// Common sampling rate for sound cards on IBM/PC
const double SAMPLE_RATE	= 11025.0;

const double PI				= (4.0) * atan(1.0);
const double TWO_PI			= (8.0) * atan(1.0);


//==================================================================================
//																		End of Line
//==================================================================================