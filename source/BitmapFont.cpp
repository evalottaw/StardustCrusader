//*********************************************************************//
//	File:		BitmapFont.cpp
//	Author:		
//	Course:		
//	Purpose:	BitmapFont class draws text using an image of
//				fixed-size glyphs
//*********************************************************************//

#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Utilities.h"

#include <cstring>
#include <cctype>


//*********************************************************************//
// Initialize
//	- load resources & set the data members
//	- should load info from a text file
//	- hardcoded to handle the SGD_Font_Glow.png
void BitmapFont::Initialize( void )
{
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
		L"resource/graphics/ELW_MenuFont.png", SGD::Color{ 0, 0, 0 });

	m_nNumCols			= 10;
	m_nCharWidth		= 32;
	m_nCharHeight		= 32;
		
	m_cFirstChar		= ' ';		// space
	m_bOnlyUppercase	= true;
}

//*********************************************************************//
// Terminate
//	- unload the image
void BitmapFont::Terminate( void )
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture( m_hImage );
}


//*********************************************************************//
// Draw
//	- render the array of characters, one-at-a-time,
//	  using the Cell Algorithm to calculate the source rect
//	  of the glyph within the image
void BitmapFont::Draw( const char* output, SGD::Point position, float scale, SGD::Color color ) const
{
	// Validate the image
	SGD_ASSERT( m_hImage != SGD::INVALID_HANDLE,
		"BitmapFont::Draw - font was not initialized" );
	
	// Validate the string
	SGD_ASSERT( output != nullptr,
		"BitmapFont::Draw - string CANNOT be null!" );

	// Check the parameters
	if( output[ 0 ] == '\0'			// empty string
		|| scale <= 0.0f			// no size or inverted
		|| color.alpha == 0 )		// transparent
		return;


	// Store the starting x
	float startX = position.x;


	// Loop through the string, until hitting null terminator
	for( unsigned int i = 0; output[ i ] != '\0'; i++ )
	{
		char ch = output[ i ];

		// Handle the whitespace
		if( ch == ' ' )
		{
			// Move to the next position on screen
			position.x += m_nCharWidth * scale;
			continue;
		}
		else if( ch == '\n' )
		{
			// Drop down a line & reset to the starting x position
			position.x = startX;
			position.y += m_nCharHeight * scale;
			continue;
		}
		else if( ch == '\t' )
		{
			// Calculate the number of pixels on this line
			float pixels = position.x - startX;

			// Calculate the numbers of characters on this line
			int chars = (int)( pixels / (m_nCharWidth * scale) );

			// Calculate the number of spaces to add
			// for a 4-space alignment
			int spaces = 4 - (chars % 4);

			
			// Move to the next position on screen
			position.x += m_nCharWidth * scale * spaces;
			continue;
		}


		// Do we need to convert to uppercase?
		if( m_bOnlyUppercase == true )
			ch = toupper( ch );

		// Calculate the tile ID that matches the ASCII character
		// (characters MUST be in ASCII order!)
		int ID = ch - m_cFirstChar;

		// Calculate the source rect of the tile
		// using the Cell Algorithm
		SGD::Rectangle cell = { };

		cell.left = (float)( (ID % m_nNumCols) * m_nCharWidth  );
		cell.top  = (float)( (ID / m_nNumCols) * m_nCharHeight );

		cell.right  = cell.left + m_nCharWidth;
		cell.bottom = cell.top  + m_nCharHeight;


		// Draw the character section
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_hImage,
			position,
			cell,
			0.0f, {},
			color,
			{ scale, scale } );

		
		// Move to the next position on screen
		position.x += m_nCharWidth * scale;
	}
}

//*********************************************************************//
// Draw
//	- render the array of characters, one-at-a-time,
//	  using the Cell Algorithm to calculate the source rect
//	  of the glyph within the image
void BitmapFont::Draw( const wchar_t* output, SGD::Point position, float scale, SGD::Color color ) const
{
	// Validate the image
	SGD_ASSERT( m_hImage != SGD::INVALID_HANDLE,
		"BitmapFont::Draw - font was not initialized" );
	
	// Validate the string
	SGD_ASSERT( output != nullptr,
		"BitmapFont::Draw - string CANNOT be null!" );

	// Check the parameters
	if( output[ 0 ] == L'\0'		// empty string
		|| scale <= 0.0f			// no size or inverted
		|| color.alpha == 0 )		// transparent
		return;


	// Store the starting x
	float startX = position.x;


	// Loop through the string, until hitting null terminator
	for( unsigned int i = 0; output[ i ] != '\0'; i++ )
	{
		// Get the current character (narrowed to ASCII)
		char ch = (char)output[ i ];

		// Handle the whitespace
		if( ch == ' ' )
		{
			// Move to the next position on screen
			position.x += m_nCharWidth * scale;
			continue;
		}
		else if( ch == '\n' )
		{
			// Drop down a line & reset to the starting x position
			position.x = startX;
			position.y += m_nCharHeight * scale;
			continue;
		}
		else if( ch == '\t' )
		{
			// Calculate the number of pixels on this line
			float pixels = position.x - startX;

			// Calculate the numbers of characters on this line
			int chars = (int)( pixels / (m_nCharWidth * scale) );

			// Calculate the number of spaces to add
			// for a 4-space alignment
			int spaces = 4 - (chars % 4);

			
			// Move to the next position on screen
			position.x += m_nCharWidth * scale * spaces;
			continue;
		}


		// Do we need to convert to uppercase?
		if( m_bOnlyUppercase == true )
			ch = toupper( ch );
		
		// Calculate the tile ID that matches the ASCII character
		// (characters MUST be in ASCII order!)
		int ID = ch - m_cFirstChar;

		// Calculate the source rect of the tile
		// using the Cell Algorithm
		SGD::Rectangle cell = { };

		cell.left = (float)( (ID % m_nNumCols) * m_nCharWidth  );
		cell.top  = (float)( (ID / m_nNumCols) * m_nCharHeight );

		cell.right  = cell.left + m_nCharWidth;
		cell.bottom = cell.top  + m_nCharHeight;


		// Draw the character section
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_hImage,
			position,
			cell,
			0.0f, {},
			color,
			{ scale, scale } );

		
		// Move to the next position on screen
		position.x += m_nCharWidth * scale;
	}
}
