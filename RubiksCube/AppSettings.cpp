/*********************************************************************************/
/* Title:		AppSettings.cpp																							         */
/* Author:	Markus Schlafli																							         */
/* Date:		29-Jul-2013																								           */
/*********************************************************************************/

#include "stdafx.h"

#include "AppSettings.h"

Renderer AppSettings::_currentRenderer = Renderer::OPENGL; 

OS
AppSettings::OperatingSystem() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
	return OS::WIN;
#else
	return OS::LINUX
#endif

}

Renderer
AppSettings::CurrentRenderer() {
	return _currentRenderer;
}