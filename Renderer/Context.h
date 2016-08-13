/*********************************************************************************/
/* Title:		Context.h																										         */
/* Author:	Markus Schlafli																							         */
/* Date:		29-Jul-2013																								           */
/*********************************************************************************/

#ifndef CONTEXT_H
#define CONTEXT_H

#include <windows.h>
#include <glm/glm.hpp>

/*
====================================================================================
	Context abstract class
	
		Holds the rendering context. 
====================================================================================
*/
class RENDERER Context {

public:
	virtual void resizeWindow( int width, int height) = 0;
	virtual void render() = 0;
	virtual void rotateView( int dx, int dy ) = 0;
	virtual void rotateView(glm::mat4 mat) = 0;
	virtual void rotateCube(char key) = 0;
	virtual void loadShaders() = 0;

	virtual bool initialized() const { return _initialized; }
	virtual void setupScene()=0;

private:
	// not implemented
	Context( const Context& rhs );
	Context& operator=( const Context& rhs );

protected:
	explicit Context( HWND hwnd ) : _hwnd(hwnd), _initialized(false) {  };
	virtual ~Context() { };

	int _windowHeight;	
	int _windowWidth;

	HWND _hwnd;
	bool _initialized;

};

#endif // CONTEXT_H