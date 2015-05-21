//*********************************************************************//
//	File:		BitmapFont.h
//	Author:		
//	Course:		
//	Purpose:	BitmapFont class draws text using an image of
//				fixed-size glyphs
//*********************************************************************//

#pragma once

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"


//*********************************************************************//
// BitmapFont class
//	- displays text using an image of fixed-size glyphs
//	- RESTRICTIONS:
//		- IMAGE SIZE MUST BE A POWER-OF-2!!! (16, 32, 64, 128, etc)
//		- glyphs in the image must have a fixed-size
//		- glyphs MUST be in ASCII order (with empty space reserved for missing glyphs)
//		- can skip over glyphs at the beginning
//		- can skip over glyphs at the end
class BitmapFont
{
public:
	//*****************************************************************//
	// Default Constructor & Destructor
	BitmapFont( void )	= default;
	~BitmapFont( void )	= default;
	

	//*****************************************************************//
	// Initialize & Terminate
	void Initialize( void );	// write one Initialize method for each font
	void Terminate ( void );
	

	//*****************************************************************//
	// Draw
	void Draw( const char* output, SGD::Point position, float scale = 1.0f, SGD::Color color = { } ) const;
	void Draw( const wchar_t* output, SGD::Point position, float scale = 1.0f, SGD::Color color = { } ) const;

private:
	//*****************************************************************//
	// image
	SGD::HTexture	m_hImage			= SGD::INVALID_HANDLE;

	// cell algorithm
	int				m_nNumCols			= 0;
	int				m_nCharWidth		= 0;
	int				m_nCharHeight		= 0;

	// font config
	char			m_cFirstChar		= 0;
	bool			m_bOnlyUppercase	= false;

};
