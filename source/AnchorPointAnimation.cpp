//*********************************************************************//
//	File:		AnchorPointAnimation.cpp
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	AnchorPointAnimation class runs an animation of 
//				variable-size frames based on the elapsed time.
//
//				The anchor-point is the relative offset between
//				the entity's position to the top-left corner
//				of the frame.
//*********************************************************************//

#include "AnchorPointAnimation.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Utilities.h"


//*********************************************************************//
// Initialize
//	- hardcode the animation
void AnchorPointAnimation::Initialize( void )
{
	// Load the image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
					L"resource/graphics/SGD_Anim_Explosion.png" );

	m_vFrames.resize( 10 );		// 10 frames: 0->9

	m_vFrames[ 0 ] = { {  0,   0,    60,  50}, {30, 25}, 0.1f };	// source, position, duration
	m_vFrames[ 1 ] = { { 60,   0,   120,  50}, {30, 25}, 0.1f };
	m_vFrames[ 2 ] = { {120,   0,   180,  50}, {30, 25}, 0.1f };
	m_vFrames[ 3 ] = { {180,   0,   240,  50}, {30, 25}, 0.1f };
	m_vFrames[ 4 ] = { {240,   0,   300,  50}, {30, 25}, 0.1f };
	m_vFrames[ 5 ] = { {300,   0,   360,  50}, {30, 25}, 0.1f };
	m_vFrames[ 6 ] = { {  0,  50,    60, 100}, {30, 25}, 0.1f };
	m_vFrames[ 7 ] = { { 60,  50,   120, 100}, {30, 25}, 0.1f };
	m_vFrames[ 8 ] = { {120,  50,   180, 100}, {30, 25}, 0.1f };
	m_vFrames[ 9 ] = { {180,  50,   240, 100}, {30, 25}, 0.1f };


	m_nCurrFrame	= 0;
	
	m_fTimeWaited	= 0.0f;
	m_fSpeed		= 1.0f;
	
	m_bIsPlaying		= false;
	m_bIsLooping		= false;
	m_bIsFinished		= false;
}

//*********************************************************************//
// Terminate
//	- unload the resources
void AnchorPointAnimation::Terminate( void )
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture( m_hImage );
}


//*********************************************************************//
// Update
//	- run the animation timer
void AnchorPointAnimation::Update( float elapsedTime )
{
	// Is the animation paused?
	if( m_bIsPlaying == false )
		return;


	// Increase the timer
	m_fTimeWaited += elapsedTime * m_fSpeed;

	// Is it time to move to the next frame?
	if( m_fTimeWaited >= m_vFrames[ m_nCurrFrame ].fDuration )
	{
		m_fTimeWaited = 0.0f;
		++m_nCurrFrame;


		// Has it reached the end?
		if( m_nCurrFrame == m_vFrames.size() )
		{
			// Should the animation loop from the beginning?
			if( m_bIsLooping == true )
				m_nCurrFrame = 0;
			else 
			{
				// Stop on the last valid frame
				--m_nCurrFrame;
				m_bIsPlaying = false;
				m_bIsFinished = true;
			}
		}
	}
}


//*********************************************************************//
// Render
//	- draw the current frame offset from the given position
void AnchorPointAnimation::Render( SGD::Point position, bool flipped,
								   float scale, SGD::Color color ) const
{
	// Validate the image
	SGD_ASSERT( m_hImage != SGD::INVALID_HANDLE,
		"AnchorPointAnimation::Render - animation was not initialized" );
	
	// Check the parameters
	if( scale <= 0.0f
		|| color.alpha == 0 )
		return;


	// Flip an image:
	//		(>'')>
	//	     |
	//   <(''<)
	float scaleX = scale;

	if( flipped == true )
		scaleX = -scaleX;


	// Retrieve the source rect for the current frame
	SGD::Rectangle frame = m_vFrames[ m_nCurrFrame ].rFrame;
	SGD::Point anchor = m_vFrames[ m_nCurrFrame ].ptAnchor;


	// Draw the current frame, offset from the position by
	// the anchor amount (to get the top-left corner)
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(
		m_hImage, 
		{ position.x - (anchor.x * scaleX), 
		  position.y - (anchor.y * scale) }, 
		frame, 
		0.0f, {},
		color, {scaleX, scale} );
}


//*********************************************************************//
// GetRect
//	- return the frame rect at the given position
SGD::Rectangle	AnchorPointAnimation::GetRect( SGD::Point position, bool flipped,
						float scale ) const
{
	// Retrieve the source rect for the current frame
	SGD::Rectangle	frame	= m_vFrames[ m_nCurrFrame ].rFrame;
	SGD::Point		anchor	= m_vFrames[ m_nCurrFrame ].ptAnchor;

	// Is it flipped?
	if( flipped == true )
	{
		SGD::Rectangle result = { };
		result.right	= position.x + (anchor.x * scale);
		result.top		= position.y - (anchor.y * scale);
		result.left		= result.right - (frame.ComputeWidth()  * scale);
		result.bottom	= result.top   + (frame.ComputeHeight() * scale);
		return result;
	}
	else
	{
		SGD::Rectangle result = { };
		result.left		= position.x - (anchor.x * scale);
		result.top		= position.y - (anchor.y * scale);
		result.right	= result.left + (frame.ComputeWidth()  * scale);
		result.bottom	= result.top  + (frame.ComputeHeight() * scale);
		return result;
	}
}


//*********************************************************************//
// Restart
//	- start the animation over from frame 0
void AnchorPointAnimation::Restart( bool looping, float speed )
{
	// Store the parameters
	m_bIsLooping	= looping;
	m_fSpeed	= speed;

	// Reset animation
	m_nCurrFrame	= 0;
	m_fTimeWaited	= 0.0f;
	m_bIsPlaying		= true;
	m_bIsFinished		= false;
}
