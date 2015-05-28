//*********************************************************************//
//	File:		AnchorPointAnimation.h
//	Author:		Eva-Lotta Wahlberg
//	Course:		SGD 1505
//	Purpose:	AnchorPointAnimation class runs an animation of 
//				variable-size frames based on the elapsed time.
//
//				The anchor-point is the relative offset between
//				the entity's position to the top-left corner
//				of the frame.
//*********************************************************************//

#pragma once

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"
#include <vector>


//*********************************************************************//
// AnchorPointAnimation class
//	- runs animation using an image of variable-size frames
//	- stores a relative offset from the top-left corner of the frame rect 
//	  to the render position
//	- image MUST have a size power-of-2 (e.g. 64, 128, 256, 512)
class AnchorPointAnimation
{
public:
	//*****************************************************************//
	// Default Constructor & Destructor
	AnchorPointAnimation( void )	= default;
	~AnchorPointAnimation( void )	= default;
	

	//*****************************************************************//
	// Initialize & Terminate
	void	Initialize	( void );	// SHOULD have a config file
	void	Terminate	( void );


	//*****************************************************************//
	// Animation Controls:
	void	Update		( float elapsedTime );
	void	Render		( SGD::Point position, bool flipped = false, float scale = 1.0f, SGD::Color color = { } ) const;
	
	SGD::Rectangle GetRect( SGD::Point position, bool flipped = false, float scale = 1.0f ) const;

	void	Restart		( bool looping = false, float speed = 1.0f );
	void	Pause		( bool pause = true )	{	m_bIsPlaying = !pause;	}
	

	//*****************************************************************//
	// Accessors:
	bool	IsPlaying	( void ) const	{	return m_bIsPlaying;	}
	bool	IsFinished	( void ) const	{	return m_bIsFinished;	}

private:
	//*****************************************************************//
	// image
	SGD::HTexture			m_hImage		= SGD::INVALID_HANDLE;

	// frame data
	struct Frame
	{
		SGD::Rectangle		rFrame;		// source rectangle
		SGD::Point			ptAnchor;	// relative position within source
		float				fDuration;	// time to wait on this frame
	};

	std::vector< Frame >	m_vFrames;

	// animation data
	int						m_nCurrFrame	= 0;

	float					m_fTimeWaited	= 0.0f;
	float					m_fSpeed		= 1.0f;		// multiplier: 2.0 - twice as fast

	bool					m_bIsPlaying	= false;
	bool					m_bIsLooping	= false;
	bool					m_bIsFinished	= false;

};
