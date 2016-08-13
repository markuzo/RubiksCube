/*********************************************************************************/
/* Title:		AppSettings.h																								         */
/* Author:	Markus Schlafli																							         */
/* Date:		29-Jul-2013																								           */
/*********************************************************************************/

#pragma once

/*
====================================================================================
	OS enum
	
		Specifies the operating system.
====================================================================================
*/
enum OS {
	WIN		= 0,
	LINUX = 1
};

/*
====================================================================================
	Renderer enum
	
		Holds the different types of graphics APIs that are available to the 
		application.
====================================================================================
*/
enum Renderer {
	OPENGL	= 0,
	DIRECTX = 1
};

/*
====================================================================================
	AppSettings static class
	
		Holds the application's settings.
====================================================================================
*/
class AppSettings {

public:
	static const char * Title() { return "Rubik's Cube"; }
	static OS OperatingSystem();
	static Renderer CurrentRenderer();

	static void SetRenderer(Renderer renderer) { _currentRenderer = renderer; }
	
private:
	AppSettings() {};
	static Renderer _currentRenderer;

protected:

};