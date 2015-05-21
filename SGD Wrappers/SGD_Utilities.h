/***********************************************************************\
|																		|
|	File:			SGD_Utilities.h 									|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-12-31											|
|																		|
|	Purpose:		To provide debugging utilities						|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_UTILITIES_H
#define SGD_UTILITIES_H


//*********************************************************************//
// DEBUG MACROS:
//	- SGD_ASSERT	- sets a breakpoint when the expression is false
//	- SGD_PRINT		- prints a message to the Output window
#if defined( DEBUG ) || defined( _DEBUG )
	#define SGD_ASSERT( expression, message )	SGD::Assert( expression, message )
	#define SGD_PRINT( message )				SGD::Print( message )
#else 
	#define SGD_ASSERT( expression, message )	(void)0
	#define SGD_PRINT( message )				(void)0
#endif


namespace SGD
{	
	//*****************************************************************//
	// Alert
	//	- displays a message box and prints to the Output window
	void Alert( const char* message );
	void Alert( const wchar_t* message );

	//*****************************************************************//
	// Assert
	//	- displays a message box and triggers a breakpoint
	//	  only if the expression is false
	void Assert( bool expression, const char* message );
	void Assert( bool expression, const wchar_t* message );
	
	//*****************************************************************//
	// Print
	//	- prints a message to the Output window
	void Print( const char* message );
	void Print( const wchar_t* message );

}	// namespace SGD

#endif //SGD_UTILITIES_H
